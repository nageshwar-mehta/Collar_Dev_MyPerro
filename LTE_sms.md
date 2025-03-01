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

## **7. Suggestions for Optimization**

### **1. Use MQTT Instead of SMS for Real-Time Data (If Internet is Available)**
Instead of SMS, using **MQTT (Message Queuing Telemetry Transport)** over WiFi or LTE can drastically reduce operational costs. MQTT uses minimal bandwidth and works well for low-power IoT devices.
- **Pros:** Lower cost, real-time updates, works with cloud services.
- **Cons:** Needs an internet connection.
- **Implementation:** Use MQTT brokers like **Mosquitto** or cloud-based options like **AWS IoT, Adafruit IO, or HiveMQ**.

### **2. Optimize GPS Wake-Up & Data Transmission**
- Instead of sending GPS data **every second**, send updates only when a significant movement is detected (>50 meters change).
- Use **motion sensors (like MPU6050 or ADXL345)** to detect movement and activate GPS only when needed.

### **3. Power Optimization Techniques**
- **ESP8266 Deep Sleep Mode:** Save battery by sleeping when not in use.
- **Lower GPS Update Rate:** Instead of **1-second updates**, use **30-second or movement-based updates**.
- **Use External Battery with Efficient Charging Circuit:** A **3.7V LiPo battery with a TP4056 charging module** improves longevity.

### **4. Use a Cheaper LTE Alternative**
The **A7672S LTE module** is robust but may be costly. Consider using **SIM800L (2G)** or **SIM7600 (4G, but cheaper than A7672S)**.

### **5. Reduce SMS Costs Using Bulk SMS APIs**
Instead of direct LTE-based SMS, using **bulk SMS APIs** reduces cost:
| **Provider** | **Cost per SMS** |
|-------------|-----------------|
| Fast2SMS   | ₹0.2 - ₹0.8      |
| Twilio     | ₹0.5 - ₹1.2      |
| Textlocal  | ₹0.3 - ₹1.0      |

**Implementation:**
Use **WiFi** when available and only use **LTE SMS when out of range**.

🚀 **Final Recommendation:** Use a hybrid approach: **WiFi + MQTT when available, LTE + SMS as fallback** to keep costs low while maintaining reliability.

