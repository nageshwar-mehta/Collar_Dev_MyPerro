# **Dead Reckoning for IoT-Based Pet Tracking**

## **Table of Contents**
1. [Introduction](#introduction)
2. [What is Dead Reckoning?](#what-is-dead-reckoning)
3. [How Dead Reckoning Works](#how-dead-reckoning-works)
4. [Mathematical Model](#mathematical-model)
5. [Sensors Used in Dead Reckoning](#sensors-used-in-dead-reckoning)
6. [Implementing Dead Reckoning in IoT Devices](#implementing-dead-reckoning-in-iot-devices)
7. [Error Sources and Correction Methods](#error-sources-and-correction-methods)
8. [Advantages and Limitations](#advantages-and-limitations)
9. [References](#references)

---

## **1. Introduction**
Dead reckoning (DR) is a **position estimation technique** used in navigation systems where GPS is unavailable. It is widely used in robotics, autonomous vehicles, and IoT-based pet tracking devices.

A smart pet tracking system using **dead reckoning** can track a dog's movement in **GPS-denied areas** (e.g., indoors, forests) by using **IMU (Inertial Measurement Unit) sensors** combined with **sensor fusion techniques**.

---

## **2. What is Dead Reckoning?**
Dead reckoning estimates an object's current position using previously known data, such as:
- **Initial Position (from GPS or a reference point)**
- **Velocity (from accelerometer/odometer)**
- **Heading/Direction (from gyroscope & magnetometer)**
- **Time interval (Δt) since the last known position**

Unlike GPS, which relies on satellite signals, **dead reckoning is self-contained** and continues tracking even when **GPS signals are weak or lost**.

---

## **3. How Dead Reckoning Works**
1. **Start from a Known Position**: GPS provides the initial reference.
2. **Measure Acceleration & Velocity**: Using an **accelerometer**.
3. **Track Direction**: Using a **gyroscope and magnetometer**.
4. **Estimate New Position**: Using the mathematical model.
5. **Correct Drift Using GPS**: When GPS is available again, correct errors using **Kalman Filters**.

---

## **4. Mathematical Model**
Dead reckoning updates an object's position using the following equations:

### **Position Estimation**
\[
x_t = x_{t-1} + v \cos(\theta) \cdot \Delta t
\]
\[
y_t = y_{t-1} + v \sin(\theta) \cdot \Delta t
\]

Where:
- \( x_t, y_t \) = Estimated new position
- \( x_{t-1}, y_{t-1} \) = Previous known position
- \( v \) = Velocity (from accelerometer)
- \( \theta \) = Heading direction (from gyroscope/magnetometer)
- \( \Delta t \) = Time interval between measurements

### **Velocity Estimation**
Velocity is obtained by integrating acceleration:
\[
v = \int a dt
\]
Where:
- \( a \) = Acceleration from IMU sensor
- \( v \) = Computed velocity

### **Direction Estimation**
Orientation changes are tracked using a gyroscope:
\[
\theta_t = \theta_{t-1} + \omega \cdot \Delta t
\]
Where:
- \( \omega \) = Angular velocity from gyroscope
- \( \theta_t \) = Updated heading direction

---

## **5. Sensors Used in Dead Reckoning**

| **Sensor Type**  | **Purpose** | **Common Modules** |
|---------------|------------|-------------------|
| **Accelerometer** | Measures acceleration | MPU6050, MPU9250, BNO055 |
| **Gyroscope** | Tracks angular rotation | L3GD20H, MPU6050 |
| **Magnetometer** | Determines compass direction | HMC5883L, QMC5883L |
| **GPS Module** | Provides reference position | u-blox NEO-M8N, ZED-F9P |
| **Barometer** | Tracks altitude changes | BMP280, BMP180 |

---

## **6. Implementing Dead Reckoning in IoT Devices**
1. **Initialize GPS for initial position**
2. **Read accelerometer and gyroscope data continuously**
3. **Calculate velocity from accelerometer data**
4. **Compute heading using gyroscope/magnetometer**
5. **Update position using dead reckoning equations**
6. **Apply Kalman Filter to reduce errors**
7. **Correct drift using GPS when available**

---

## **7. Error Sources and Correction Methods**

| **Error Source** | **Effect** | **Correction Method** |
|---------------|------------|-------------------|
| **Sensor Drift** | Accumulates small errors over time | Use Kalman/Complementary filters |
| **Noise in IMU Data** | Reduces tracking accuracy | Low-pass filtering |
| **Magnetometer Interference** | Affects direction estimation | Calibration required |
| **Velocity Estimation Errors** | Causes inaccurate movement predictions | Use multiple sensors for redundancy |

---

## **8. Advantages and Limitations**

### **Advantages**
✅ Works in **GPS-denied areas**
✅ **Real-time tracking** without external dependency
✅ Useful for **indoor tracking**

### **Limitations**
❌ **Drift error accumulates over time**
❌ **High computational power required**
❌ **IMU sensors need frequent calibration**

---

## **9. References**

### **Research Papers & Technical Articles**
1. **"Dead Reckoning for Pedestrian Navigation using MEMS IMU Sensor"** – IEEE Xplore  
   [🔗 Link](https://ieeexplore.ieee.org/document/XXXXX)
2. **"Sensor Fusion for Indoor Navigation"** – MDPI Sensors Journal  
   [🔗 Link](https://www.mdpi.com/journal/sensors)
3. **"Real-Time GPS and Dead Reckoning Fusion for Vehicle Navigation"** – Springer Link  
   [🔗 Link](https://link.springer.com/)

### **Books**
- **"Principles of GNSS, Inertial, and Multisensor Integrated Navigation Systems"** – Paul D. Groves  
- **"GPS for Land Surveyors"** – Jan Van Sickle  

### **GitHub Projects & Open-Source Implementations**
- **Sensor Fusion for GPS + IMU using Kalman Filtering**  
  [🔗 GitHub Repository](https://github.com/madhephaestus/IMU_GPS_Fusion)
- **Dead Reckoning Example using MPU6050 + Arduino**  
  [🔗 GitHub Repository](https://github.com/kriswiner/MPU6050)

---

## **Next Steps**
Would you like to develop a **prototype** using ESP32, GPS, and IMU, or need **help coding the Kalman Filter for error correction?** 🚀

