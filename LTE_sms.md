# **IoT Pet Collar: LTE-Based Tracking with ESP8266 & NEO-M8M GPS Module**

## **1. Introduction**

### **Project Update: LTE-Based Tracking**

We are designing a **smart pet collar** that tracks a pet’s location efficiently. Initially, we planned to use **WiFi geofencing with Bluetooth-based setup and A-GPS**. However, considering limitations in coverage, we are now exploring **LTE-based tracking using the A7672S LTE module**.

### **Updated Working Mechanism:**
1. **Geofence Detection** - ESP8266 checks if the device is within WiFi range.
2. **Trigger LTE Module (A7672S) if Outside Geofence** - If WiFi disconnects, LTE starts sending GPS data via **SMS** or **Cloud API**.
3. **Use NEO-M8M GPS Module** - To get real-time latitude and longitude.
4. **Power Efficiency** - ESP8266 remains in deep sleep mode inside the geofence.

---

## **2. Required Components**

### **Hardware**

- **NEO-M8M GPS Module** - Provides GPS coordinates.
- **ESP8266/ESP32** - Controls logic & connectivity.
- **A7672S LTE Module** - Sends SMS/data when the dog leaves the geofence.
- **LiPo Battery (3.7V)** - Powers the system.
- **Level Shifter (Logic Converter)** - For proper voltage conversion between modules.

### **Software & Libraries**

- **Arduino IDE** - [Download](https://www.arduino.cc/en/software)
- **TinyGPS++** (For GPS parsing) - Install from Arduino Library Manager
- **SoftwareSerial** (For serial communication with LTE module) - Included in Arduino IDE

---

## **3. Step-by-Step Guide to Connecting LTE & GPS with ESP8266**

### **Step 1: Wiring the Components**

#### **Wiring ESP8266 with NEO-M8M GPS Module**
| **NEO-M8M Pin** | **ESP8266 Pin** |
|-----------------|-----------------|
| VCC            | 3.3V             |
| GND            | GND              |
| TX             | RX (D7)          |
| RX             | TX (D8)          |

#### **Wiring ESP8266 with A7672S LTE Module**
| **A7672S Pin** | **ESP8266 Pin** |
|---------------|----------------|
| VCC (4.2V)   | External 4.2V Battery |
| GND          | GND           |
| TX           | RX (D5)       |
| RX           | TX (D6)       |

---

### **Step 2: Detecting Geofence Violation**

```cpp
#include <ESP8266WiFi.h>

const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    delay(5000);
    
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Geofence Breached! Activating LTE");
        digitalWrite(LTE_POWER, HIGH); // Turn LTE ON
    } else {
        Serial.println("Pet Inside Safe Zone");
        digitalWrite(LTE_POWER, LOW); // Keep LTE OFF
    }
}

void loop() {}
```

---

### **Step 3: Fetching GPS Data from NEO-M8M**

```cpp
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

SoftwareSerial gpsSerial(14, 12); // RX, TX for GPS
TinyGPSPlus gps;

void getGPSData() {
    while (gpsSerial.available()) {
        gps.encode(gpsSerial.read());
        if (gps.location.isUpdated()) {
            float latitude = gps.location.lat();
            float longitude = gps.location.lng();
            sendSMS(latitude, longitude);
        }
    }
}
```

---

### **Step 4: Sending GPS Coordinates via SMS Using A7672S LTE Module**

```cpp
#include <SoftwareSerial.h>

SoftwareSerial lteSerial(5, 4); // RX, TX for LTE Module

void sendSMS(float lat, float lon) {
    lteSerial.println("AT+CMGF=1"); // Set SMS mode
    delay(1000);
    lteSerial.println("AT+CMGS=\"+91XXXXXXXXXX\""); // Replace with mobile number
    delay(1000);
    String message = "Pet Location: " + String(lat, 6) + ", " + String(lon, 6);
    lteSerial.println(message);
    delay(1000);
    lteSerial.write(26); // Send SMS
    Serial.println("SMS Sent!");
}
```

---

## **4. Alternative: Using SMS API Instead of LTE Module**

Instead of using the **A7672S LTE module**, we can use an online **SMS API** that allows us to send messages using **WiFi** (which reduces power consumption significantly).

### **Popular SMS API Services:**

| **API Provider** | **Cost per SMS** |
|-----------------|-----------------|
| Twilio         | ₹0.5 - ₹1.2       |
| Fast2SMS       | ₹0.2 - ₹0.8       |
| Textlocal      | ₹0.3 - ₹1.0       |

### **Code for Sending SMS Using Twilio API**

```cpp
#include <ESP8266HTTPClient.h>

void sendAPISMS(float lat, float lon) {
    HTTPClient http;
    String url = "https://api.twilio.com/send?to=+91XXXXXXXXXX&msg=Pet Location: " + String(lat) + ", " + String(lon);
    http.begin(url);
    int httpCode = http.GET();
    http.end();
}
```

### **Pros of API-Based SMS Over LTE Module:**
✔️ **Lower Power Consumption**
✔️ **No Extra LTE Module Required**
✔️ **Easier Integration with Cloud Services**

### **Cons:**
❌ **Needs WiFi or Mobile Data Connection**
❌ **API Costs Per Message**

---

## **5. Power Optimization Strategies**

- **Use Deep Sleep Mode on ESP8266:**
  ```cpp
  ESP.deepSleep(300e6); // Sleep for 5 minutes
  ```
- **Optimize LTE Usage:**
  - Send SMS only once per minute to reduce battery drain.
  - Power off LTE module when inside the geofence.

---

## **6. Conclusion & Suggestions**

### **Final Implementation Choice**
1. **If power efficiency is the priority → Use SMS API with WiFi**
2. **If independent tracking is needed → Use LTE Module A7672S**

### **Additional Recommendations**
- **Use a Rechargeable LiPo Battery with Solar Charging (If Feasible).**
- **Consider a Lower Power Microcontroller like ESP32-S2 for Extended Battery Life.**
- **Implement a Cloud-Based Dashboard for Live Tracking.**

---

## **7. References & Further Reading**

- **A7672S Datasheet:** [Download](https://www.quectel.com/product/lte-a7672s)
- **NEO-M8M Datasheet:** [Download](https://www.u-blox.com/sites/default/files/NEO-M8M_DataSheet.pdf)
- **Twilio SMS API Docs:** [Twilio API](https://www.twilio.com/docs/sms/api)

🚀 **This guide provides an affordable and efficient solution for real-time pet tracking using LTE or SMS API!**

