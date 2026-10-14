# TRINETRA — Project Rules & Governance

**Project:** TRINETRA (Smart Surveillance and Reconnaissance Ground Vehicle)  
**Document:** `rules.md`  
**Status:** [IMPLEMENTED] Operational & Development Safety Standards

---

## 1. Core Hardware & Safety Rules

1. **Safety Test Load for Relay/FIRE Function:**
   - The relay on ESP32 GPIO 27 must **ONLY** be connected to a safe, low-voltage test load (such as a bench LED, low-voltage indicator lamp, or multimeter).
   - Under **NO** circumstances should any high-voltage, explosive, pyrotechnic, weapon-like, or hazardous physical load be attached, activated, or tested.
   - The relay defaults to `LOW` (OFF) on boot. Any vehicle `STOP ('S')` or communication watchdog event forces immediate relay shutoff (`relayOff()`).
2. **Forward Collision Prevention Priority:**
   - Forward obstacle detection on the Arduino UNO (`SAFE_DISTANCE = 10 cm`) has hard priority over remote drive commands.
   - If an obstacle is detected in front, the vehicle must halt immediately and refuse further forward thrust.
   - Reverse (`'B'`) and pivot maneuvers (`'L'`, `'R'`) remain permitted for evasive clearing.
3. **Communication Loss Failsafe (Watchdog):**
   - Arduino UNO implements a 1000 ms communication watchdog. If locomotion is active and no command packet is received within 1000 ms, the UNO automatically halts all motors.
   - ESP32 implements a 1500 ms client watchdog. If the web client drops connection while the vehicle is driving, ESP32 transmits `'S'` to UNO and turns the relay OFF.
4. **Mechanical Servo Limits:**
   - SG90 Radar servo (Nano D9) and Secondary Aiming servo (ESP32 GPIO 13) are standard positional servos constrained strictly between `0°` and `180°`.
   - Never command angles outside $[0, 180]$ to avoid physical gear stripping or motor burnout.

---

## 2. Architecture & Modular Separation

1. **Separation of Concerns:**
   - **ESP32:** Master controller, Wi-Fi SoftAP, WebServer host (dual interface `/radar` & `/remote`), target lock coordinator, secondary aiming servo driver, relay controller.
   - **Arduino Nano:** Dedicated radar scanner (SG90 servo sweep & HC-SR04 pulse timing, start/stop, lock holding).
   - **Arduino UNO:** Locomotion execution (Adafruit Motor Shield V1) & forward obstacle collision watchdog.
2. **Dedicated User Interfaces:**
   - Laptop client: Uses `/radar` for pure tactical radar display, target acquisition tracking, and situational awareness.
   - Phone client: Uses `/remote` for handheld vehicle driving (virtual joystick), radar start/stop, target lock, camera view, and relay testing.
   - Portal gateway: `/` directs operators cleanly to either dedicated interface.

---

## 3. Communication & Protocol Invariants

1. **Preserve Wire Protocols:**
   - Nano UART: `RADAR,<angle>,<dist>\n` stream, `LOCK:<angle>\n`, `UNLOCK\n`, `START\n`, `STOP\n` at 9600 baud.
   - UNO UART: Single character commands `F\n`, `B\n`, `L\n`, `R\n`, `S\n`, distance telemetry `DIST:<dist>\n` at 9600 baud.
   - Primary ESP32 UART (`GPIO 1/3` at 115200 baud) is strictly reserved for developer debugging and monitoring.
2. **No Fake Camera Streams:**
   - Drone camera feed integration uses standard browser stream embedding (`<img>` for MJPEG/HTTP or configurable stream URL).
   - If no live camera URL is configured or video is offline, the UI displays an explicit `AWAITING STREAM URL` / `OFFLINE` status overlay. Never fabricate video feeds.

---

## 4. Coding & Maintenance Rules

1. **Source of Truth Hierarchy:**
   $$\text{LATEST MODEL SPECIFICATION} > \text{OLD INTENDED DESIGN} > \text{OLD IMPLEMENTATION}$$
2. **Update Documentation Immediately:**
   - Any hardware pin change, endpoint adjustment, or protocol update must be documented simultaneously in `architecture.md`, `memory.md`, and `PRD.md`.
