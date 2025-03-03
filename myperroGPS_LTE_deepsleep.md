# IoT Pet Collar: ESP32-C6 + NEO-M8M + SIM7672S

## Overview
This project is a **smart pet collar** that uses **WiFi geofencing, GPS tracking, and LTE communication** to ensure pet safety. The system detects when a pet is outside the defined WiFi geofence and activates GPS tracking. It then sends the real-time location to a predefined mobile number via SMS using the **SIM7672S** LTE module. To optimize battery life, the ESP32-C6 uses **Deep Sleep mode** when not in active tracking mode.

## Features
- **WiFi Geofencing:** Determines if the pet is in a safe zone.
- **GPS Activation:** GPS (NEO-M8M) turns on only when the pet leaves the geofence.
- **LTE Communication:** Sends GPS coordinates via SMS to a registered mobile number.
- **Low Power Consumption:** Uses **ESP32 Deep Sleep mode** to extend battery life.

## Components Required
1. **ESP32-C6** (Microcontroller)
2. **NEO-M8M GPS Module**
3. **SIM7672S LTE Module**
4. **Battery Pack (Li-ion 3.7V)**
5. **Wires & Connectors**
6. **SIM Card (for LTE communication)**

## Circuit Connections
| **Component**  | **ESP32-C6 GPIO** |
|--------------|------------------|
| GPS TX       | GPIO16           |
| GPS RX       | GPIO17           |
| LTE TX       | GPIO18           |
| LTE RX       | GPIO19           |

## Step-by-Step Guide
### Step 1: Hardware Setup
1. **Connect GPS Module (NEO-M8M) to ESP32-C6**
   - GPS TX → ESP32 GPIO16
   - GPS RX → ESP32 GPIO17
2. **Connect LTE Module (SIM7672S) to ESP32-C6**
   - LTE TX → ESP32 GPIO18
   - LTE RX → ESP32 GPIO19
3. **Power ESP32-C6 using a Li-ion 3.7V battery.**
4. **Insert a valid SIM card into the LTE module.**

### Step 2: Flash the Code
1. **Download and install Arduino IDE.**
2. **Install necessary libraries:**
   - `TinyGPSPlus` for GPS data parsing.
   - `WiFi.h` for geofencing.
3. **Connect ESP32-C6 to your PC via USB.**
4. **Open the provided source code and update WiFi credentials.**
5. **Upload the code to ESP32-C6.**

### Step 3: Running the System
1. **Power on the ESP32-C6.**
2. **Monitor Serial Logs to verify WiFi connection.**
3. **If WiFi is available, ESP32 will enter Deep Sleep.**
4. **If WiFi is lost, GPS and LTE will activate.**
5. **SMS with GPS coordinates will be sent to the registered mobile number.**

## Code Explanation
### 1. WiFi Geofencing Check
```cpp
WiFi.begin(ssid, password);
if (WiFi.status() == WL_CONNECTED) {
    esp_deep_sleep_start();
}
```
- **WiFi.begin(ssid, password);** → Connects to the predefined WiFi network.
- **If connected, ESP32 enters Deep Sleep mode to save power.**
- **If disconnected, the device proceeds to GPS activation.**

### 2. GPS Location Tracking
```cpp
while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
    if (gps.location.isUpdated()) {
        float lat = gps.location.lat();
        float lon = gps.location.lng();
    }
}
```
- Reads incoming GPS data from the NEO-M8M module.
- Extracts **latitude and longitude** when a new update is received.
- Stores the coordinates for sending via SMS.

### 3. Sending GPS Data via LTE (SIM7672S)
```cpp
lteSerial.println("AT+CMGF=1"); // Set text mode for SMS
lteSerial.println("AT+CMGS=\"+91XXXXXXXXXX\""); // Set recipient
lteSerial.print("Dog lost! Location: ");
lteSerial.print(lat);
lteSerial.print(", ");
lteSerial.print(lon);
lteSerial.write(26); // End SMS (CTRL+Z ASCII code)
```
- **AT+CMGF=1** → Configures LTE module to **text mode**.
- **AT+CMGS=\"+91XXXXXXXXXX\"** → Specifies the recipient number.
- **lteSerial.write(26);** → Sends the SMS (ASCII code for CTRL+Z).

### 4. Deep Sleep Mode for Power Saving
```cpp
esp_sleep_enable_timer_wakeup(30000000);
esp_deep_sleep_start();
```
- **esp_sleep_enable_timer_wakeup(30000000);** → Configures ESP32 to wake up every **30 seconds** (unit: **microseconds**).
- **esp_deep_sleep_start();** → Puts ESP32 into **Deep Sleep mode** until the timer wakes it up.

## How to Use
1. **Flash the provided code** onto the ESP32-C6.
2. **Set your WiFi SSID and password** in the code.
3. **Insert a valid SIM card** into the LTE module.
4. **Turn on the device** and check serial monitor logs.
5. **Monitor SMS alerts** when the pet leaves the WiFi range.

## Expected Power Consumption
| **State** | **Power Consumption** |
|-----------|----------------------|
| **Active Mode** (WiFi, GPS, LTE) | ~200-300 mA |
| **Deep Sleep Mode** | ~10-20 µA |

## Potential Improvements
- Implement **Bluetooth configuration** for setting up WiFi SSID & password dynamically.
- Optimize GPS polling to reduce battery usage further.
- Add **motion detection sensors** to wake up ESP32 only when the pet moves.

## Troubleshooting
- If **GPS is not responding**, check the **baud rate (9600)** and wiring.
- If **LTE is not sending SMS**, ensure the **SIM card has an active plan**.
- If **ESP32 is not waking up**, verify the **Deep Sleep timer setup**.

## Conclusion
This IoT-based smart collar provides an **affordable and power-efficient solution** for pet tracking. It ensures **real-time monitoring** while **conserving battery life** through **Deep Sleep mode**.

---
### Notes
- `esp_sleep_enable_timer_wakeup(time_in_microseconds);` → Time is set in **microseconds (µs)**.
- 1 second = **1,000,000 µs**.
- In our case: `30000000 µs = 30 seconds`.

