Here's a **line-by-line** breakdown of your code with **detailed explanations** for every part:

---

## **1. Including Required Libraries**
```cpp
#include <WiFi.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
```
- **`#include <WiFi.h>`** → Includes the **WiFi library**, which allows the ESP32 to connect to a WiFi network for geofencing.
- **`#include <TinyGPSPlus.h>`** → Includes the **TinyGPSPlus** library, which is used to decode GPS data from the **NEO-M8M** module.
- **`#include <HardwareSerial.h>`** → Includes the **HardwareSerial** library, which enables communication with **GPS and LTE modules** via UART.

---

## **2. Function to Enter Deep Sleep Mode**
```cpp
void enterDeepSleep() {
    Serial.println("Entering Deep Sleep Mode...");
    esp_sleep_enable_timer_wakeup(30000000); // Sleep for 30 seconds , taking value in micro secs
    esp_deep_sleep_start();
}
```
- **`void enterDeepSleep()`** → Function to put the ESP32 into **Deep Sleep mode**.
- **`Serial.println("Entering Deep Sleep Mode...");`** → Prints a message on the serial monitor.
- **`esp_sleep_enable_timer_wakeup(30000000);`** → Configures the ESP32 to **wake up after 30 seconds**. The value `30000000` is in **microseconds** (30 million µs = 30 sec).
- **`esp_deep_sleep_start();`** → Activates **Deep Sleep mode**, reducing power consumption.

---

## **3. Function to Send GPS Location via LTE (SIM7672S)**
```cpp
void sendLocationOverLTE(float lat, float lon) {
    String locationMsg = "Pet Location: " + String(lat, 6) + ", " + String(lon, 6);
    lteSerial.println("AT+CMGF=1"); // Set SMS mode
    delay(100);
    lteSerial.println("AT+CMGS=\"+919103553896\""); // Replace with actual mobile number
    delay(100);
    lteSerial.println(locationMsg);
    delay(100);
    lteSerial.write(26); // Send SMS
    Serial.println("SMS Sent!");
} 
```
- **`String locationMsg = "Pet Location: " + String(lat, 6) + ", " + String(lon, 6);`**  
  → Creates a **string** containing the **latitude and longitude** (6 decimal places).
- **`lteSerial.println("AT+CMGF=1");`**  
  → Sends an **AT command** to **set SMS mode**.
- **`lteSerial.println("AT+CMGS=\"+919103553896\"");`**  
  → Specifies the **recipient phone number** for the SMS.
- **`lteSerial.println(locationMsg);`**  
  → Sends the **GPS coordinates** via SMS.
- **`lteSerial.write(26);`**  
  → Sends the ASCII code **26 (CTRL+Z)** to **end the SMS**.
- **`Serial.println("SMS Sent!");`**  
  → Displays confirmation that the SMS was sent.

---

## **4. Function to Continuously Track Pet's Location**
```cpp
void trackPetLocation() {
    while (true) {
        while (gpsSerial.available()) {
            gps.encode(gpsSerial.read());
            if (gps.location.isUpdated()) {
                float latitude = gps.location.lat();
                float longitude = gps.location.lng();
                Serial.printf("Location: Lat: %f, Lon: %f\n", latitude, longitude);
                sendLocationOverLTE(latitude, longitude);
                delay(100); // Send location every 100 milliseconds
            }
        }
    }
}
```
- **`while (true) {`**  
  → An **infinite loop** to keep tracking the pet's location.
- **`while (gpsSerial.available()) {`**  
  → Checks if there is **new GPS data available** from the **NEO-M8M module**.
- **`gps.encode(gpsSerial.read());`**  
  → Reads and processes the **GPS data**.
- **`if (gps.location.isUpdated()) {`**  
  → Checks if **new location data** is available.
- **`float latitude = gps.location.lat();`**  
  → Retrieves the **latitude**.
- **`float longitude = gps.location.lng();`**  
  → Retrieves the **longitude**.
- **`Serial.printf("Location: Lat: %f, Lon: %f\n", latitude, longitude);`**  
  → Displays the **GPS coordinates** in the serial monitor.
- **`sendLocationOverLTE(latitude, longitude);`**  
  → Calls the function to **send the location via LTE**.
- **`delay(100);`**  
  → Sends location updates **every 100 milliseconds**.

---

## **5. WiFi Details (SSID & Password)**
```cpp
const char* ssid = "nagesh____";
const char* password = "rahul12345";
```
- **`const char* ssid = "nagesh____";`**  
  → Stores the **WiFi network name (SSID)**.
- **`const char* password = "rahul12345";`**  
  → Stores the **WiFi password**.

---

## **6. Defining Serial Communication for GPS & LTE Modules**
```cpp
HardwareSerial gpsSerial(1);  // GPS connected to UART1
HardwareSerial lteSerial(2);  // LTE connected to UART2
```
- **`HardwareSerial gpsSerial(1);`**  
  → Defines **UART1** for **GPS communication**.
- **`HardwareSerial lteSerial(2);`**  
  → Defines **UART2** for **LTE communication**.

---

## **7. ESP32 Setup Function**
```cpp
void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    delay(1000);
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
```
- **`Serial.begin(115200);`**  
  → Initializes the **serial monitor** at **115200 baud rate**.
- **`WiFi.begin(ssid, password);`**  
  → Connects the ESP32 to the **WiFi network**.
- **`delay(1000);`**  
  → Waits **1 second** to ensure a stable WiFi connection.
- **`gpsSerial.begin(9600, SERIAL_8N1, 16, 17);`**  
  → Initializes **UART1** at **9600 baud rate** for **GPS** (TX=16, RX=17).
- **`lteSerial.begin(115200, SERIAL_8N1, 18, 19);`**  
  → Initializes **UART2** at **115200 baud rate** for **LTE** (TX=18, RX=19).
- **`if (WiFi.status() == WL_CONNECTED) {`**  
  → Checks if the ESP32 **successfully connected to WiFi**.
- **`Serial.println("Pet Inside Safe Zone");`**  
  → If WiFi is connected, prints that the pet is **inside the geofence**.
- **`trackPetLocation();`**  
  → If WiFi is **not connected**, **GPS & LTE tracking** starts.

---

## **8. Main Loop Function**
```cpp
void loop() {
    if(WiFi.status()==WL_CONNECTED){
        serial.println("pet is back inside the geofence. Disabling GPS and LTE");
        enterDeepSleep(); 
    }
}
```
- **`if(WiFi.status()==WL_CONNECTED){`**  
  → Checks if the **pet has returned** inside the **WiFi geofence**.
- **`serial.println("pet is back inside the geofence. Disabling GPS and LTE");`**  
  → Displays a message that the pet has **returned home**.
- **`enterDeepSleep();`**  
  → Calls the **Deep Sleep function** to save battery.

---

## **Final Notes**
✅ The **ESP32 wakes up every 30 seconds** to check the **WiFi geofence**.  
✅ **GPS & LTE activate only when WiFi is lost** (pet is outside).  
✅ **SMS alerts with GPS coordinates are sent to the owner**.  

Would you like any modifications or further optimizations? 🚀
