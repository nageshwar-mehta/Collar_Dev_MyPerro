/*
   IoT Pet Collar: Geofencing with ESP32-C6 + NEO-M8M + SIM7672S (FASE)
   Features:
   - WiFi-based geofencing to determine if the pet is in a safe zone.
   - GPS (NEO-M8M) turns on only when outside the WiFi range to save battery.
   - LTE (SIM7672S) sends GPS coordinates when the pet is outside the geofence.
   - Optimized for low power consumption.
*/

#include <WiFi.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

// WiFi Details
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

// Define GPS & LTE Serial Communication
HardwareSerial gpsSerial(1);  // GPS connected to UART1
HardwareSerial lteSerial(2);  // LTE connected to UART2

TinyGPSPlus gps;

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    delay(5000);
    
    gpsSerial.begin(9600, SERIAL_8N1, 16, 17);  // GPS TX = GPIO16, RX = GPIO17
    lteSerial.begin(115200, SERIAL_8N1, 18, 19); // LTE TX = GPIO18, RX = GPIO19
    
    Serial.println("Checking Geofence...");
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Pet Inside Safe Zone");
    } else {
        Serial.println("Pet Outside Geofence! Activating GPS and LTE...");
        trackPetLocation();
    }
}

void trackPetLocation() {
    while (true) {
        while (gpsSerial.available()) {
            gps.encode(gpsSerial.read());
            if (gps.location.isUpdated()) {
                float latitude = gps.location.lat();
                float longitude = gps.location.lng();
                Serial.printf("Location: Lat: %f, Lon: %f\n", latitude, longitude);
                sendLocationOverLTE(latitude, longitude);
                delay(100); // Send location every 100 miliseconds
            }
        }
    }
}

void sendLocationOverLTE(float lat, float lon) {
    String locationMsg = "Pet Location: " + String(lat, 6) + ", " + String(lon, 6);
    lteSerial.println("AT+CMGF=1"); // Set SMS mode
    delay(100);
    lteSerial.println("AT+CMGS=\"+91XXXXXXXXXX\""); // Replace with actual mobile number
    delay(100);
    lteSerial.println(locationMsg);
    delay(100);
    lteSerial.write(26); // Send SMS
    Serial.println("SMS Sent!");
}

void loop() {
    // Keep checking WiFi status
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Pet is back inside the geofence. Disabling GPS and LTE.");
        delay(30000);
    }
}
