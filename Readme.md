# 🍱 Smart Food Delivery Bag with Sensors (Zomato Gold+ Concept)

## 🚀 Project Vision

This project aims to design a **smart food delivery system** powered by **sensor-enabled delivery bags** that monitor the **safety, integrity, and tampering** of food orders during transit. This concept was inspired by the gap in premium food delivery experiences for Zomato Gold members and beyond.

> ✅ Premium food deserves premium protection.

---

## 🧠 Problem Statement

Even for premium users (e.g., Zomato Gold), food can arrive:
- Spilled (especially gravies)
- Cold or shaken
- Tampered with or missing items

There is currently **no visibility or accountability** during the delivery journey. The customer receives food in poor condition with **no way to verify what happened** in transit.

---

## 💡 Solution Overview

A smart delivery bag equipped with:
- **Item Detection via Load Cells**
- **Strap Lock Monitoring with Hall Effect Sensor**
- **Tamper/Motion Detection via Gyroscope (MPU6050)**
- **Mobile App Integration for Real-time Alerts**

Delivery cannot begin unless:
1. All items are verified and strapped.
2. No tampering has occurred after restaurant pickup.
3. Customer receives a notification summary upon delivery.

---

## 🧩 Features

- 📦 **Strap-Based Item Confirmation**: Detects the number of items using load sensors and ensures all items are strapped before the ride begins.
- 🔐 **Tamper Alerts**: Notifies the customer if the strap was opened after pickup.
- 🧭 **Motion Analysis**: Detects if food boxes were shaken, flipped, or handled roughly.
- 📱 **Real-Time App Sync**: Sensor data is pushed to a companion mobile app that alerts users of issues.
- 🔋 **Rechargeable Hardware**: Battery-powered ESP32-based controller for easy reusability.

---

## 🛠️ Tech Stack

### 🧑‍🔧 Hardware
- ESP32 microcontroller
- Load cell + HX711 amplifier
- Hall effect sensor (for strap lock detection)
- MPU6050 gyroscope/accelerometer
- Li-ion rechargeable battery pack
- Insulated food bag (custom design)

### 📱 Software
- **Mobile App**: Flutter (Cross-platform)
- **Backend**: Firebase (Auth, Firestore, FCM Notifications)
- **Hardware Firmware**: Arduino/PlatformIO (C++ with BLE/Wi-Fi)

---

## 🗂️ Folder Structure (Planned)

```bash
/
├── hardware/
│   ├── circuit-diagrams/
│   ├── firmware/
├── mobile-app/
│   ├── lib/
│   ├── assets/
├── backend/
│   ├── firebase-config/
├── docs/
│   ├── patent-draft.pdf
│   ├── architecture.md
├── README.md
└── LICENSE
```