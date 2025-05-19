
# 🐶 Contactless Heart Rate Monitoring in Dogs – Techniques & Recommendations

---

## 📌 Objective

To explore and evaluate **non-contact or contactless heart rate sensing technologies** for dogs, focusing on:

* Working principles
* Applicability to furry animals
* Distance-related accuracy
* Sensor performance under fur
* Availability & cost in India

---

## 🔬 Techniques & Their Working Principles

### 1. **Infrared Thermography (IRT)**

**🔍 Principle:** Detects changes in infrared radiation emitted from skin (esp. nasal/muzzle area).
**🐕 Applicability:** Tracks thermal pulsations from superficial vessels.
**🎯 Ideal For:** Short-haired dogs, exposed areas (ears, nose).
**⚠️ Fur Effect:** Thick fur blocks heat radiation → significant drop in accuracy.
**📏 Distance:** 0.5–1 meter optimal; more leads to dispersion and noise.
**✅ Verdict:** Supplementary tool, not standalone for HR.

---

### 2. **Doppler Ultrasound (Contactless variant)**

**🔍 Principle:** Measures frequency shift of sound waves reflected from blood flow.
**🐕 Applicability:** Effective on areas with less fur or shaved patches.
**⚠️ Fur Effect:** Strongly attenuates sound waves, especially at high frequency.
**📏 Distance:** Close-range (within 5–10 cm), beyond which attenuation rises.
**✅ Verdict:** Not practical for contactless use in furry dogs.

---

### 3. **mmWave Radar (e.g., 60GHz, 77GHz)**

**🔍 Principle:** Emits electromagnetic waves to detect micro-vibrations in body/chest.
**🐕 Applicability:** Can penetrate fur; detects both respiration and heart rate.
**🎯 Ideal For:** Furry breeds, all sizes, indoor environments.
**⚠️ Fur Effect:** Minimal – mmWave penetrates through hair but accuracy may reduce slightly.
**📏 Distance:** Works from 10 cm to \~1 m. Some modules claim up to 2 m.
**✅ Verdict:** ✅ **Most promising** for furry animals.

---

### 4. **Camera-Based Photoplethysmography (Imaging PPG, iPPG)**

**🔍 Principle:** Detects color/intensity changes in skin due to blood flow (RGB/NIR camera).
**🐕 Applicability:** Works only on exposed skin (belly, inner thigh, shaved areas).
**⚠️ Fur Effect:** Severe – completely blocks signal.
**📏 Distance:** 0.5–1 m with high-res camera and good lighting.
**✅ Verdict:** Only useful for hairless areas.

---

### 5. **Ballistocardiography (BCG)** – *Non-contact via Pressure/Load Sensors or Radar*

**🔍 Principle:** Detects micro-body movements from heartbeat using radar or vibration sensors.
**🐕 Applicability:** Under-bed or platform-based sensing.
**⚠️ Fur Effect:** Not applicable, as signal from body mass movement.
**📏 Distance:** Limited to close contact (e.g., dog resting on platform).
**✅ Verdict:** Good for sleeping/resting states.

---

## 🧪 Comparative Summary Table

| Technique    | Fur Resistance | Contactless | Optimal Distance | Use Case (Dog)         | Verdict                |
| ------------ | -------------- | ----------- | ---------------- | ---------------------- | ---------------------- |
| IRT          | ❌ Poor         | ✅ Yes       | 0.5 – 1 m        | Nose, ears             | Supplement only        |
| Doppler      | ❌ Poor         | ⚠️ Limited  | ≤ 10 cm          | Shaved/sparse fur      | Not ideal              |
| mmWave Radar | ✅ Good         | ✅ Yes       | 10 cm – 1 m      | Chest/torso (thru fur) | ✅ Best option          |
| Camera PPG   | ❌ Very Poor    | ✅ Yes       | 0.5 – 1 m        | Belly, inner thigh     | Limited, unreliable    |
| BCG (radar)  | ✅ Good         | ✅ Semi      | Close/resting    | Sleeping dogs          | Use for resting vitals |

---

## 🏆 Recommendation: **mmWave Radar**

### 📣 Why mmWave Works Best:

* Can **penetrate fur** and clothing
* **Doesn’t require exposed skin**
* Works **without contact**, even at **\~1 m distance**
* Detects both **heart rate** and **respiration rate**
* Already used in **smart baby monitors** and **pet research** (e.g., TI's IWR6843)

---

## 📦 Recommended Modules – Available in India

### ✅ **1. IWR6843ISK-ODS (Texas Instruments)**

* **Type:** mmWave Radar
* **Price:** ₹9,500–₹12,000
* **Buy from:** [Robu.in](https://www.robu.in/), [TI Store India](https://www.ti.com/)
* **Features:** High accuracy, SDK available, connects via UART

---

### ✅ **2. Seeed Studio mmWave Human Static Presence Module (60GHz)**

* **Type:** Compact mmWave
* **Price:** ₹4,000–₹5,500
* **Buy from:** [Robu](https://robu.in), [Amazon.in](https://amazon.in)
* **Note:** Marketed for humans but works similarly on pets for breathing/HR

---

### ⚠️ Other Options

#### 🔸 **MLX90640 IRT Thermal Camera**

* **Price:** ₹6,000–₹9,000
* **Buy from:** [Robu](https://robu.in), Amazon
* **Note:** Use only on shaved or short-fur areas

#### 🔸 **Pi Camera + OpenCV for PPG**

* **Price:** ₹1,200–₹2,000
* **Buy from:** Amazon/Robu
* **Note:** High resolution and stable lighting required

---

## 🧪 Research & Veterinary Use-Cases

* **mmWave in Veterinary Research:**
  "Non-invasive heart rate and respiration monitoring in dogs using mmWave radar" – IEEE Access (2023)
* **IRT in Vet Practice:**
  "Infrared thermography in animal health: A review" – Vet World, 2020
* **Camera-PPG for Animals:**
  Limited success; used more in lab animals or where fur is shaved.

---

## 📌 Final Recommendation

> **For furry dogs in a real-world, non-contact monitoring scenario:**
> ✅ **Use mmWave radar sensors** – especially those operating at 60–77 GHz.
> ❌ Avoid PPG and IRT as primary methods due to fur interference.


