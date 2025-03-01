# **A-GPS and NEO-M8N GPS Module for IoT Pet Collar: Detailed Guide**

## **1. Introduction to A-GPS and Project Overview**

### **Project Brief**

We are designing a **smart pet collar** that ensures pet safety by tracking its location. The collar:

1. **Creates a Geofence:** Uses WiFi range (mobile hotspot) to define a safe zone.
2. **Activates GPS When Out of Geofence:** The GPS remains off within the geofence to **save battery**.
3. **Uses A-GPS for Fast Fix:** Reduces GPS lock time to provide real-time tracking.
4. **Bluetooth Setup for Seamless Connectivity:**
   - Allows SSID and password input over Bluetooth.
   - Connects to WiFi automatically.
   - Disconnects Bluetooth after successful WiFi connection.
5. **Sends Live GPS Data:** If the pet is outside the geofence, the device transmits its latitude and longitude every second.

### **Benefits of A-GPS in Our Project**

- **Faster Time-To-First-Fix (TTFF):** Ensures quick GPS lock when the pet leaves the geofence.
- **Battery Efficiency:** GPS remains off within geofence, only activating when needed.
- **Seamless User Experience:** Bluetooth setup eliminates the need for hardcoded SSID/password.

---

## **2. Required Components**

### **Hardware**

- **NEO-M8N GPS Module** (for location tracking)
- **ESP8266/ESP32** (for WiFi & Bluetooth connectivity)
- **HC-05/HC-06 Bluetooth Module** (for seamless WiFi setup)
- **Battery (LiPo 3.7V)** (for portability)
- **Power Management Circuit** (to optimize battery usage)
- **USB-to-UART Adapter** (for initial setup & debugging)

### **Software**

- **u-center** (for configuring NEO-M8N) - [Download](https://www.u-blox.com/en/product/u-center)
- **Arduino IDE** (for programming ESP) - [Download](https://www.arduino.cc/en/software)
- **AssistNow Service** (for fetching A-GPS data) - [AssistNow](https://www.u-blox.com/en/assistnow)

---

## **3. System Implementation**

### **Step 1: Configuring Bluetooth for SSID & Password Setup**

Instead of hardcoding SSID and password, we use **Bluetooth (HC-05/HC-06)** to receive credentials from a mobile app.

#### **Wiring ESP8266 with HC-05 Bluetooth Module**

| **HC-05 Pin** | **ESP8266 Pin**                        |
| ------------- | -------------------------------------- |
| VCC           | 3.3V (Use Logic Level Converter if 5V) |
| GND           | GND                                    |
| TX            | RX (D7 on ESP8266)                     |
| RX            | TX (D8 on ESP8266)                     |

#### **Code to Receive SSID & Password via Bluetooth**

```cpp
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

SoftwareSerial BTSerial(14, 12); // RX, TX for Bluetooth
char ssid[30], password[30];

void setup() {
    Serial.begin(115200);
    BTSerial.begin(9600);
    Serial.println("Waiting for SSID & Password...");
    
    int index = 0;
    while (BTSerial.available() == 0);
    while (BTSerial.available()) {
        ssid[index] = BTSerial.read();
        index++;
    }
    ssid[index] = '\0';
    
    delay(1000);
    index = 0;
    while (BTSerial.available() == 0);
    while (BTSerial.available()) {
        password[index] = BTSerial.read();
        index++;
    }
    password[index] = '\0';
    
    Serial.print("Received SSID: ");
    Serial.println(ssid);
    Serial.print("Received Password: ");
    Serial.println(password);
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    BTSerial.end(); // Disconnect Bluetooth after setup
}

void loop() {}
```

### **Step 2: Detecting Geofence Violation**

- **Check WiFi Signal Strength:** If ESP8266 loses connection to the mobile hotspot, assume the pet has moved out of range.
- **Turn on GPS Only When Necessary:**
  ```cpp
  if (WiFi.status() != WL_CONNECTED) {
      digitalWrite(GPS_POWER, HIGH); // Turn GPS ON
  } else {
      digitalWrite(GPS_POWER, LOW); // Turn GPS OFF
  }
  ```

### **Step 3: Fetch A-GPS Data for Fast Fix**

#### **Code to Fetch A-GPS Data via WiFi**

```cpp
#include <ESP8266HTTPClient.h>

const char* agps_url = "https://online-live1.services.u-blox.com/GetOnlineData?token=YOUR_UBLOX_TOKEN";

void fetchAGPSData() {
    HTTPClient http;
    http.begin(agps_url);
    int httpCode = http.GET();
    
    if (httpCode == 200) {
        String agpsData = http.getString();
        gpsSerial.write(agpsData.c_str(), agpsData.length());
    }
    http.end();
}
```

### **Step 4: Send GPS Data When Pet is Lost**

If the pet is outside the geofence, send GPS coordinates via MQTT/HTTP.

```cpp
void sendLocation(float lat, float lon) {
    HTTPClient http;
    String url = "http://yourserver.com/update_location?lat=" + String(lat) + "&lon=" + String(lon);
    http.begin(url);
    http.GET();
    http.end();
}
```

---

## **4. Power Management for Longevity**

### **Battery Optimization Techniques**

1. **Deep Sleep Mode:**
   - ESP8266 enters sleep mode when inside the geofence.
   ```cpp
   ESP.deepSleep(600e6); // Sleep for 10 minutes
   ```
2. **Low Power GPS Mode:**
   - Configure NEO-M8N to operate in power-saving mode using u-center.


---

## **6. References**

- **u-blox AssistNow Service:** [AssistNow](https://www.u-blox.com/en/assistnow)
- **Arduino IDE:** [Download](https://www.arduino.cc/en/software)
- **NEO-M8N Datasheet:** [Download](https://www.u-blox.com/sites/default/files/NEO-M8N-FW3_DataSheet_%28UBX-15031086%29.pdf)

---

## **7. Conclusion**

This guide provides a complete setup for a **smart pet collar** using **A-GPS, Bluetooth-based WiFi setup, and geofencing** to efficiently track pets. The combination of **battery management, real-time updates, and low-power operation** ensures the best performance. 🚀


