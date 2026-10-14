# TRINETRA — User Manual & Deployment Guide
**Smart Surveillance and Reconnaissance Ground Vehicle**

---

## 1. System Overview

TRINETRA is an unmanned, semi-autonomous ground reconnaissance vehicle controlled via Wi-Fi. It integrates three microcontrollers:
- **ESP32 NodeMCU:** Master controller, Wi-Fi SoftAP host, dual web applications server, secondary aiming servo driver, and relay actuator.
- **Arduino Nano:** Dedicated radar controller sweeping a 180° SG90 servo and HC-SR04 ultrasonic sensor.
- **Arduino UNO:** Drive and locomotion controller driving 4 DC gear motors via Adafruit Motor Shield V1 with autonomous front collision safety.

---

## 2. Firmware Flashing Guide

Connect each microcontroller to your computer via USB one at a time and upload its respective sketch using Arduino IDE:

### 2.1 Arduino Nano (Radar Subsystem)
- **File:** `Nano/Radar/Radar.ino`
- **Board:** Arduino Nano
- **Processor:** ATmega328P (or ATmega328P Old Bootloader)
- **Port:** Select Nano COM port
- **Required Libraries:** `Servo` (built-in)

### 2.2 Arduino UNO (Locomotion & Safety)
- **File:** `UNO/Tank/Tank.ino`
- **Board:** Arduino Uno
- **Port:** Select UNO COM port
- **Required Libraries:** `AFMotor` (Adafruit Motor Shield V1 library)

### 2.3 ESP32 NodeMCU (Master Controller & Web HUD)
- **File:** `ESP32/Main/Main.ino`
- **Board:** ESP32 Dev Module (or NodeMCU-32S)
- **Port:** Select ESP32 COM port
- **Required Libraries:** `WiFi`, `WebServer`, `ESP32Servo`

---

## 3. Physical Bench Setup & Power

1. **Power Isolation:**
   - Power the logic rail (ESP32 VIN, Nano 5V, UNO VIN, Servos, HC-SR04 sensors) from a regulated 5V power supply.
   - Power the Adafruit Motor Shield from a separate 6V–12V DC motor battery pack via the shield's external terminal block. Remove the power jumper (`PWR`) on the shield to prevent motor noise from resetting the Arduino UNO.
   - **Common Ground:** Connect the GND rails of all power supplies together.
2. **Relay Safety Setup:**
   - Connect **ONLY** a benign low-voltage test load (such as a 5V LED with a 220Ω resistor or a digital multimeter in DC volts mode) across the relay COM and NO contacts.
   - **NEVER** connect high-voltage, explosive, pyrotechnic, or hazardous loads.

---

## 4. Connecting & Controlling TRINETRA

### 4.1 Wi-Fi Connection
1. Power up the vehicle.
2. Open your laptop or mobile phone Wi-Fi settings.
3. Connect to network:
   - **SSID:** `TRINETRA`
   - **Password:** `12345678`
   - **Default Gateway IP:** `192.168.4.1`

---

### 4.2 Laptop Tactical Radar HUD

Open Google Chrome or any modern browser on your laptop and navigate to:
```
http://192.168.4.1/radar
```

**Features:**
- **Full-Screen Tactical Polar Radar:** 0° to 180° sweep with concentric range rings at 50 cm, 100 cm, 150 cm, 200 cm, and 250 cm.
- **Dynamic Target Beacon:** Glowing red crosshair displayed when an object is detected within 150 cm.
- **Beam & Aiming Telemetry:** Shows current radar beam angle, measured range, aiming servo heading, and drive state.
- **Nano & UNO Health Badges:** Real-time link diagnostics.

**Alternative Standalone Python Radar App:**
If preferred, launch the standalone desktop GUI from your laptop terminal:
```bash
cd D:\TRINETRA\Python_Radar
python radar.py
```
*Keyboard Shortcuts:*
- `[L]`: Lock target heading
- `[U]`: Unlock and resume sweep
- `[S]`: Toggle Radar Start / Stop
- `[Q]` / `[ESC]`: Exit application

---

### 4.3 Phone Remote Control

Open your mobile browser (Safari, Chrome, etc.) and navigate to:
```
http://192.168.4.1/remote
```

**Controls:**
1. **Virtual Joystick:**
   - Touch and drag the glowing puck in any direction to drive the vehicle (Forward, Backward, Pivot Left, Pivot Right).
   - Release the joystick to center it and automatically halt the vehicle (`STOP`).
2. **Tactical Action Buttons:**
   - `[RADAR START]` / `[RADAR STOP]`: Pauses or resumes radar scanning.
   - `[LOCK TARGET]`: Freezes radar on detected target and holds secondary aiming servo heading.
   - `[FREE UNLOCK]`: Releases lock and resumes normal sweeping.
   - `[CAMERA]`: Toggles the drone camera preview card.
     - Tap preview once: Expands to full screen.
     - Double-tap preview: Minimizes back to small card.
     - Tap `[CAMERA]` button again: Closes preview.
   - `[SET CAM URL]`: Prompts you to enter your live drone stream URL (e.g., `http://192.168.4.X:81/stream`), stored in your phone browser.
   - `[FIRE RELAY (TEST)]`: Activates relay (Active-HIGH) on GPIO 27.
   - `[RELAY OFF]`: Deactivates relay.
   - *Failsafe:* Any vehicle `STOP` command automatically disarms the relay.

---

## 5. Autonomous Safety Features

1. **Forward Obstacle Safety Stop:**
   - If an obstacle is detected within 10 cm in front while driving forward, the Arduino UNO immediately cuts motor power, sets drive state to `STOP`, and disarms the relay.
   - Reverse and turning maneuvers remain enabled to navigate around the obstacle.
2. **Communication Watchdogs:**
   - **UNO Watchdog (1000 ms):** If drive commands cease for over 1000 ms, the UNO automatically shuts off all motors.
   - **ESP32 Watchdog (1500 ms):** If client Wi-Fi connectivity drops for over 1500 ms while moving, the ESP32 issues an immediate stop command and disarms the relay.
