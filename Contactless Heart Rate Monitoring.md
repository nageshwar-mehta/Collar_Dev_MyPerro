

# 🐾 Dog Breed-Specific Recommendations for Contactless Heart Rate Monitoring

---

## 🎯 Objective

Match the most effective **contactless heart rate sensing technique** to various **dog breed types** based on their fur, behavior, and anatomical features.

---

## 🐕 Breed Categorization (Based on Coat Type)

| Coat Category                  | Examples                                     | Characteristics                       |
| ------------------------------ | -------------------------------------------- | ------------------------------------- |
| **Short-Haired / Thin-Coated** | Beagle, Boxer, Doberman, Dalmatian           | Low fur density, more skin exposure   |
| **Medium-Haired**              | Labrador, Golden Retriever, Cocker Spaniel   | Moderate fur, partially visible skin  |
| **Long-Haired / Dense-Coated** | Husky, German Shepherd, Shih Tzu, Pomeranian | Thick, double coat; fur obscures skin |
| **Hairless / Very Sparse Fur** | Xoloitzcuintli, Chinese Crested              | High visibility of skin               |

---

## 📊 Recommended Sensor Technologies by Breed Type

### 🟢 **1. Short-Haired Breeds**

**Ideal Techniques:**

* ✅ rPPG (remote camera + light)
* ✅ Thermal Imaging (target belly, snout, ears)

**Why:** Visible skin allows both optical and thermal imaging to perform well.

**Example:**

* Beagle + rPPG via camera under light at 30–40 cm.
* Boxer + MLX90640 thermal camera targeting the belly.

---

### 🟡 **2. Medium-Haired Breeds**

**Ideal Techniques:**

* ⚠️ Thermal Imaging (only snout/ears)
* ✅ mmWave Radar
* ⚠️ rPPG with difficulty (limited visible skin)

**Why:** Some skin may be visible, but fur disrupts optical/thermal significantly. mmWave is more reliable.

**Example:**

* Labrador: mmWave radar aimed at chest while resting.
* Golden Retriever: Thermal camera on ear region + backup radar.

---

### 🔴 **3. Long-Haired Breeds**

**Ideal Techniques:**

* ❌ rPPG and Thermal = Not reliable
* ✅ mmWave Radar
* ⚠️ Acoustic Doppler (only with close contact)

**Why:** Dense fur prevents visible/infrared light from reaching skin. mmWave radio frequencies **penetrate fur** best.

**Example:**

* Siberian Husky: Use Seeed 60GHz radar at \~30–80 cm distance.
* German Shepherd: TI IWR6843 radar at side of kennel for passive monitoring.

---

### 🟢 **4. Hairless Breeds**

**Ideal Techniques:**

* ✅ rPPG (camera-based)
* ✅ Thermal IR
* ✅ ECG (if allowed)

**Why:** All non-contact and contact techniques work well due to maximum skin exposure.

**Example:**

* Chinese Crested: FLIR Lepton module aimed at chest.
* Xolo: Raspberry Pi HQ Camera for rPPG from a distance.

---

## 🧠 Summary Table

| Breed Type    | Radar  | Thermal IR  | rPPG (camera) | Notes                       |
| ------------- | ------ | ----------- | ------------- | --------------------------- |
| Short-Haired  | ✅ Good | ✅ Good      | ✅ Good        | Good lighting improves rPPG |
| Medium-Haired | ✅ Best | ⚠️ Limited  | ⚠️ Limited    | Radar is best compromise    |
| Long-Haired   | ✅ Best | ❌ Poor      | ❌ Very poor   | Avoid optical/IR methods    |
| Hairless      | ✅ Good | ✅ Excellent | ✅ Excellent   | All methods are valid       |

---

## 🐶 Specific Breed + Sensor Pairing Examples

| Breed           | Suggested Tech    | Sensor Module                      |
| --------------- | ----------------- | ---------------------------------- |
| Labrador        | mmWave Radar      | TI IWR6843 or Seeed 60GHz          |
| Beagle          | rPPG / Thermal    | Logitech C920 or MLX90640          |
| Shih Tzu        | mmWave Radar Only | Seeed 60GHz or IWR1443             |
| German Shepherd | mmWave Radar      | IWR6843 with FFT signal processing |
| Chinese Crested | Thermal / rPPG    | FLIR Lepton or Pi HQ Camera        |
| Pomeranian      | mmWave Radar      | Seeed mmWave                       |

---

## 🏁 Final Recommendation

If your monitoring system will be used across **multiple breeds** or in **unknown conditions**:

> 💡 **Standardize on mmWave radar** for maximum reliability across fur types, environments, and sizes.

* Works indoors, day/night.
* Requires signal filtering (FFT + band-pass).
* Can detect both **heart rate and respiration**.

---


