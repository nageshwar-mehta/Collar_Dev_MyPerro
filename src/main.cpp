#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <WiFi.h>
// using namespace std; ///removed to reduce memory usage

// Pin Definitions
#define POWER_BUTTON_PIN 0
#define LED_PIN 2
#define LTE_POWER_PIN 4
#define LTE_UART_RX 16
#define LTE_UART_TX 17

// BLE UUIDs
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CREDENTIALS_UUID    "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// System States
enum SystemState {
  STATE_INIT,
  STATE_BLE_ON,
  STATE_LTE_ON,
  STATE_GET_IMEI,
  STATE_WAIT_CONNECTION,
  STATE_GET_CREDENTIALS,
  STATE_CONNECT_WIFI,
  STATE_CONNECTED,
  STATE_ERROR
};

// Global Variables
SystemState currentState = STATE_INIT;
BLEServer *pServer = nullptr;
BLEService *pService = nullptr;
BLECharacteristic *pCredentialsChar = nullptr;
bool bleConnected = false;
bool credentialsReceived = false;
String imei = "";
String ssid = "";
String password = "";
unsigned long buttonPressTime = 0;
bool longPressDetected = false;
unsigned long lastBlinkTime = 0;
bool ledState = false;
HardwareSerial lteSerial(1);  // UART1 for LTE module

// BLE Server Callbacks
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        bleConnected = true;
    }

    void onDisconnect(BLEServer* pServer) {
        bleConnected = false;
        pServer->startAdvertising();  // Restart advertising when disconnected
    }
};

// BLE Characteristic Callbacks
class CredentialsCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        if (value.length() > 0) {
            String data = String(value.c_str());
            int separatorIndex = data.indexOf(':');
            if (separatorIndex != -1) {
                ssid = data.substring(0, separatorIndex);
                password = data.substring(separatorIndex + 1);
                credentialsReceived = true;
            }
        }
    }
};

void setup() {
    Serial.begin(115200);
    
    // Initialize GPIOs
    pinMode(POWER_BUTTON_PIN, INPUT_PULLUP); ///Used Input Pullup to avoid floating state
    pinMode(LED_PIN, OUTPUT);
    pinMode(LTE_POWER_PIN, OUTPUT);
    
    // Initialize LTE UART
    lteSerial.begin(115200, SERIAL_8N1, LTE_UART_RX, LTE_UART_TX);
    
    // Initial LED state
    digitalWrite(LED_PIN, LOW);
}

// Power on the LTE module
void powerOnLTE() {
    // Power sequence for Quectel module
    digitalWrite(LTE_POWER_PIN, LOW);
    delay(1000);
    digitalWrite(LTE_POWER_PIN, HIGH);
    delay(100);
}

// Send AT command to LTE module
String sendATCommand(const char* command, unsigned long timeout = 2000) {
    lteSerial.println(command);
    String response = "";
    unsigned long startTime = millis();
    
    while (millis() - startTime < timeout) {
        while (lteSerial.available()) {
            char c = lteSerial.read();
            response += c;
        }
    }
    return response;
}

// Connect to WiFi using received credentials
bool connectToWiFi() {
    WiFi.begin(ssid.c_str(), password.c_str());
    
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - startTime > 120000) { // 2 min timeout
            return false;
        }
        delay(500);
    }
    return true;
}

// Initialize BLE service and characteristics
void initBLE() {
    // Initialize BLE device
    BLEDevice::init("SmartCollar");///sets the device name to "SmartCollar" that will be visible in BLE scans
    
/*
    BLEDevice (class)
  └── createServer() → returns BLEServer*
                           ↓
                  pServer (pointer to BLEServer)
                           ↓
                pServer->setCallbacks(...)
                           ↓
             Registers MyServerCallbacks object
                           ↓
        onConnect() / onDisconnect() get triggered
        
        */

    // Create BLE server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    
    // Create BLE service
    pService = pServer->createService(SERVICE_UUID);
    
    // Create credentials characteristic
    pCredentialsChar = pService->createCharacteristic(
        CREDENTIALS_UUID,
        BLECharacteristic::PROPERTY_WRITE
    );
    pCredentialsChar->setCallbacks(new CredentialsCallbacks());
    
    // Start the service
    pService->start();
    
    // Start advertising
    BLEAdvertising* pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
}

// Turn off BLE and free resources
void deinitBLE() {
    if (pServer) {
        BLEAdvertising* pAdvertising = pServer->getAdvertising();
        pAdvertising->stop();
    }
    BLEDevice::deinit();
    pServer = nullptr;
    pService = nullptr;
    pCredentialsChar = nullptr;
}

// Main state machine processing
void processStateMachine() {
    switch (currentState) {
        case STATE_INIT:
            if (longPressDetected) {
                Serial.println("Starting sequence...");
                currentState = STATE_BLE_ON;
                longPressDetected = false;
            }
            break;

        case STATE_BLE_ON:
            Serial.println("Turning BLE ON");
            initBLE();
            currentState = STATE_LTE_ON;
            break;

        case STATE_LTE_ON:
            Serial.println("Turning LTE ON");
            powerOnLTE();
            delay(3000); // Allow time for module to initialize
            currentState = STATE_GET_IMEI;
            break;

        case STATE_GET_IMEI:
            Serial.println("Getting IMEI");
            imei = sendATCommand("AT+CGSN");
            imei.trim();
            Serial.println("IMEI: " + imei);
            
            if (imei.length() >= 15) {
                // TODO: Implement actual verification with backend
                currentState = STATE_WAIT_CONNECTION;
            } else {
                Serial.println("Invalid IMEI");
                currentState = STATE_ERROR;
            }
            break;

        case STATE_WAIT_CONNECTION:
            if (bleConnected) {
                Serial.println("BLE Connected");
                digitalWrite(LED_PIN, HIGH); // Solid while processing
                currentState = STATE_GET_CREDENTIALS;
            }
            break;

        case STATE_GET_CREDENTIALS:
            if (credentialsReceived) {
                Serial.println("Credentials received");
                Serial.println("SSID: " + ssid);
                currentState = STATE_CONNECT_WIFI;
            }
            break;

        case STATE_CONNECT_WIFI:
            Serial.println("Connecting to WiFi...");
            if (connectToWiFi()) {
                Serial.println("WiFi connected!");
                currentState = STATE_CONNECTED;
            } else {
                Serial.println("WiFi connection failed");
                currentState = STATE_ERROR;
            }
            break;

        case STATE_CONNECTED:
            Serial.println("Connection sequence complete");
            deinitBLE();  // Turn off BLE
            
            // Turn off LTE
            sendATCommand("AT+QPOWD", 5000);
            Serial.println("LTE powered down");
            
            // Solid green LED
            digitalWrite(LED_PIN, HIGH);
            // Stay in this state
            break;

        case STATE_ERROR:
            Serial.println("Error state");
            // Rapid blinking to indicate error
            if (millis() - lastBlinkTime > 200) {
                ledState = !ledState;
                digitalWrite(LED_PIN, ledState);
                lastBlinkTime = millis();
            }
            break;
    }
}

void loop() {
    // Long press detection (2 seconds)
    if (digitalRead(POWER_BUTTON_PIN) == LOW) {
        if (buttonPressTime == 0) {
            buttonPressTime = millis();
        } else if (millis() - buttonPressTime > 2000 && !longPressDetected) {
            longPressDetected = true;
            Serial.println("Long press detected");
        }
    } else {
        buttonPressTime = 0;
    }

    // LED blinking during BLE advertising
    if (currentState == STATE_WAIT_CONNECTION && !bleConnected) {
        if (millis() - lastBlinkTime > 500) {
            ledState = !ledState;
            digitalWrite(LED_PIN, ledState);
            lastBlinkTime = millis();
        }
    }

    processStateMachine();
    delay(10);
}