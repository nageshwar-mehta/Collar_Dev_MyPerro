# **Guide: Connecting u-blox NEO-M8L with ESP8266 & Integrating with ESP32**

## **1. Overview**

This guide provides step-by-step instructions to connect the **u-blox NEO-M8L GPS module** to an **ESP8266**, then transmit the GPS & IMU data to an **ESP32** for processing.

## **2. Components Required**

- **u-blox NEO-M8L** GNSS Module
- **ESP8266** (e.g., NodeMCU)
- **ESP32**
- **Logic Level Shifter (3.3V↔5V)** (if needed)
- **Jumper Wires**

---

## **3. Wiring Connections**

Since both ESP8266 and ESP32 operate at **3.3V logic**, you must ensure the NEO-M8L module is also **3.3V compatible** (some variants use 5V and require a logic level shifter).

### **Wiring: NEO-M8L ↔ ESP8266**

| NEO-M8L Pin              | ESP8266 Pin          |
| ------------------------ | -------------------- |
| **VCC** (3.3V/5V)        | **3.3V**             |
| **GND**                  | **GND**              |
| **TX (GPS Output)**      | **RX (D7 / GPIO13)** |
| **RX (Input, Optional)** | **TX (D8 / GPIO15)** |

### **Wiring: ESP8266 ↔ ESP32 (Serial Communication)**

| ESP8266 Pin | ESP32 Pin           |
| ----------- | ------------------- |
| TX (D8)     | RX (GPIO16 / UART2) |
| RX (D7)     | TX (GPIO17 / UART2) |
| GND         | GND                 |

---

## **4. Configuring ESP8266 to Read GPS Data**

ESP8266 reads GPS data from **u-blox NEO-M8L** and transmits it to ESP32.

### **Arduino Code for ESP8266** (Line-by-Line Explanation)

```cpp
#include <SoftwareSerial.h> // Include library for software serial communication
SoftwareSerial gpsSerial(13, 15); // Define RX=D7 (GPIO13) and TX=D8 (GPIO15)

void setup() {
  Serial.begin(115200); // Start serial communication with PC
  gpsSerial.begin(9600); // Start GPS serial communication
}

void loop() {
  while (gpsSerial.available()) { // Check if GPS data is available
    char c = gpsSerial.read(); // Read a character from GPS module
    Serial.write(c); // Forward GPS data to Serial Monitor
  }
}
```

---

## **5. Configuring ESP32 to Receive GPS Data**

ESP32 receives GPS data from ESP8266 over Serial and processes it.

### **Arduino Code for ESP32** (Line-by-Line Explanation)

```cpp
#include <HardwareSerial.h> // Include library for hardware serial communication
HardwareSerial gpsSerial(2); // Use UART2 (RX=16, TX=17)

void setup() {
  Serial.begin(115200); // Start serial communication with PC
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17); // Configure UART2 for GPS
}

void loop() {
  while (gpsSerial.available()) { // Check if GPS data is available
    char c = gpsSerial.read(); // Read a character from GPS module
    Serial.write(c); // Display GPS data on Serial Monitor
  }
}
```

---

## **6. Sending GPS Data from ESP8266 to ESP32 via Wi-Fi (MQTT & HTTP)**

### **Using MQTT for Communication**
ESP8266 publishes GPS data to an MQTT broker, and ESP32 subscribes to receive it.

#### **ESP8266 (MQTT Publisher)** (Line-by-Line Explanation)
```python
import paho.mqtt.client as mqtt  # Import MQTT client library
import serial  # Import serial library

broker = "192.168.1.100"  # Change to your broker's IP
port = 1883  # Default MQTT port
client = mqtt.Client()  # Create MQTT client instance
client.connect(broker, port)  # Connect to MQTT broker

gps = serial.Serial("COM5", 9600, timeout=1)  # Open serial port for GPS

while True:
    line = gps.readline().decode("utf-8", errors="ignore").strip()  # Read GPS data
    if line.startswith("$GNGGA"):  # Check for valid NMEA sentence
        client.publish("gps/data", line)  # Publish GPS data to MQTT topic
```

#### **ESP32 (MQTT Subscriber)** (Line-by-Line Explanation)
```python
import paho.mqtt.client as mqtt  # Import MQTT client library

def on_message(client, userdata, message):
    print("Received GPS Data:", message.payload.decode())  # Print received data

client = mqtt.Client()  # Create MQTT client instance
client.connect("192.168.1.100", 1883)  # Connect to broker
client.subscribe("gps/data")  # Subscribe to GPS data topic
client.on_message = on_message  # Assign message handler
client.loop_forever()  # Keep running
```

---

### **Using HTTP for Communication**
ESP8266 runs a web server, and ESP32 or a mobile app requests GPS data.

#### **ESP8266 (Web Server)** (Line-by-Line Explanation)
```python
from flask import Flask  # Import Flask for web server
import serial  # Import serial library

app = Flask(__name__)  # Create Flask app
gps = serial.Serial("COM5", 9600, timeout=1)  # Open serial port for GPS

@app.route("/gps")  # Define endpoint

def get_gps():
    return gps.readline().decode("utf-8", errors="ignore").strip()  # Return GPS data

app.run(host='0.0.0.0', port=5000)  # Start web server
```

#### **ESP32 or Mobile (HTTP Client)** (Line-by-Line Explanation)
```python
import requests  # Import requests library

while True:
    response = requests.get("http://192.168.1.100:5000/gps")  # Fetch GPS data
    print("GPS Data:", response.text)  # Display GPS data
```

---

## **7. Implementing PainlessMesh for Mesh Networking**
PainlessMesh enables multiple ESP devices to communicate without a central router.

### **ESP8266 (Mesh Node)** (Line-by-Line Explanation)
```cpp
#include <painlessMesh.h>  // Include PainlessMesh library
#define MESH_PREFIX "GPSMesh"  // Set mesh network name
#define MESH_PASSWORD "password123"  // Set mesh password
#define MESH_PORT 5555  // Set mesh communication port

Scheduler userScheduler;  // Create scheduler
painlessMesh mesh;  // Create mesh network instance

void setup() {
  Serial.begin(115200);  // Start serial communication
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);  // Initialize mesh
}

void loop() {
  mesh.update();  // Update mesh network
}
```


