# TRINETRA — Persistent Project Memory & Knowledge Base

**Project:** TRINETRA (Smart Surveillance and Reconnaissance Ground Vehicle)  
**Document:** `memory.md`  
**Status:** [CONFIRMED] Active System Memory Post-Implementation

---

## 1. Hardware Roles & Responsibilities

| Controller | Primary Role | Status |
| :--- | :--- | :--- |
| **ESP32 NodeMCU** | Master coordinator, Wi-Fi SoftAP, Web Server (dual UI at `/radar` & `/remote`), Target lock computation, Secondary Aiming Servo drive, Relay control | [CONFIRMED] |
| **Arduino Nano** | Radar subsystem: Sweeps radar servo (0°–180°), measures HC-SR04 distance, performs local lock hold, starts/stops sweep on command, streams radar packets to ESP32 | [CONFIRMED] |
| **Arduino UNO** | Locomotion & collision safety: Drives 4 DC motors via Adafruit Motor Shield V1, reads front ultrasonic sensor, enforces autonomous forward stop, runs comms watchdog | [CONFIRMED] |

---

## 2. Confirmed Pin Mappings

### 2.1 ESP32 (`ESP32/Main/Main.ino`)
- **GPIO 16 (RX2):** Connected to Arduino Nano Serial TX (`[CONFIRMED]`).
- **GPIO 17 (TX2):** Connected to Arduino Nano Serial RX (`[CONFIRMED]`).
- **GPIO 4 (RX1):** Connected to Arduino UNO Serial TX (`[CONFIRMED]`).
- **GPIO 5 (TX1):** Connected to Arduino UNO Serial RX (`[CONFIRMED]`).
- **GPIO 13 (PWM):** Secondary / Aiming Servo signal (`lockServo`) (`[CONFIRMED]`).
- **GPIO 27 (OUTPUT):** Relay trigger signal (`RELAY_PIN`), Active HIGH (`[CONFIRMED]`).
- **GPIO 1 (TX0) / GPIO 3 (RX0):** Primary USB Serial monitor / debugging at 115200 baud (`[CONFIRMED]`).

### 2.2 Arduino Nano (`Nano/Radar/Radar.ino`)
- **D7 (OUTPUT):** HC-SR04 Ultrasonic Trigger (`TRIG_PIN`) (`[CONFIRMED]`).
- **D8 (INPUT):** HC-SR04 Ultrasonic Echo (`ECHO_PIN`) (`[CONFIRMED]`).
- **D9 (PWM):** Radar SG90 Servo signal (`SERVO_PIN`) (`[CONFIRMED]`).
- **D0 (RX) / D1 (TX):** Hardware Serial to ESP32 at 9600 baud (`[CONFIRMED]`).

### 2.3 Arduino UNO (`UNO/Tank/Tank.ino`)
- **A0 (OUTPUT):** Front HC-SR04 Ultrasonic Trigger (`TRIG_PIN`) (`[CONFIRMED]`).
- **A1 (INPUT):** Front HC-SR04 Ultrasonic Echo (`ECHO_PIN`) (`[CONFIRMED]`).
- **D3–D12:** Adafruit Motor Shield V1 (Motor channels 1, 2, 3, 4) (`[CONFIRMED]`).
- **D0 (RX) / D1 (TX):** Hardware Serial to ESP32 at 9600 baud (`[CONFIRMED]`).

---

## 3. Serial Communication Protocols

### 3.1 ESP32 ↔ Arduino Nano (9600 baud, 8N1)
- **Nano -> ESP32:**
  - `RADAR,<angle>,<distance>\n`: Emitted during sweep and during lock. Angle: 0–180; distance: 2–249 cm (`[CONFIRMED]`).
  - `RADAR_LOCKED,<angle>\n`: Emitted immediately upon lock command (`[CONFIRMED]`).
  - `RADAR_UNLOCKED\n`: Emitted upon unlock command (`[CONFIRMED]`).
  - `RADAR_STOPPED\n`: Emitted when radar sweep is stopped (`[CONFIRMED]`).
  - `RADAR_STARTED\n`: Emitted when radar sweep resumes (`[CONFIRMED]`).
- **ESP32 -> Nano:**
  - `LOCK:<angle>\n`: Freezes radar servo at `<angle>` and pulses ultrasonic continuously (`[CONFIRMED]`).
  - `UNLOCK\n`: Resumes continuous sweeping (`[CONFIRMED]`).
  - `START\n`: Resumes radar sweep and ultrasonic pinging (`[CONFIRMED]`).
  - `STOP\n`: Pauses radar sweep and ultrasonic pinging (`[CONFIRMED]`).

### 3.2 ESP32 ↔ Arduino UNO (9600 baud, 8N1)
- **ESP32 -> UNO:**
  - Single character command terminated with `\n`:
    - `'F'`: Move forward (`[CONFIRMED]`).
    - `'B'`: Move backward (`[CONFIRMED]`).
    - `'L'`: Pivot left (`[CONFIRMED]`).
    - `'R'`: Pivot right (`[CONFIRMED]`).
    - `'S'`: Stop all motors (`[CONFIRMED]`).
- **UNO -> ESP32:**
  - `DIST:<distance>\n`: Emitted every 100 ms with front ultrasonic distance (`[CONFIRMED]`).
  - `OBSTACLE_STOP\n`: Emitted when front obstacle $\le 10\text{ cm}$ halts vehicle (`[CONFIRMED]`).
  - `COMM_TIMEOUT_STOP\n`: Emitted when 1000 ms comm watchdog halts vehicle (`[CONFIRMED]`).

---

## 4. Web Endpoints & API Reference

| Endpoint | Method | Purpose | Response |
| :--- | :--- | :--- | :--- |
| `/` | `GET` | Command Portal Gateway | HTML page with links to `/radar` and `/remote` |
| `/radar` | `GET` | Dedicated Laptop Tactical Radar Display | Full-screen HUD with polar radar canvas and telemetry |
| `/remote` | `GET` | Dedicated Phone Remote Control | Mobile controller with virtual joystick, camera UI, buttons |
| `/data` | `GET` | Live JSON Telemetry Stream (100 ms polling) | JSON payload with all sensor, lock, and motor states |
| `/radar/start` | `GET` | Starts Nano radar scanning | `"RADAR STARTED"` |
| `/radar/stop` | `GET` | Stops Nano radar scanning | `"RADAR STOPPED"` |
| `/lock` | `GET` | Locks target on detected angle | `"LOCK OK"` |
| `/unlock` | `GET` | Releases lock and resumes sweep | `"UNLOCK OK"` |
| `/move?cmd=<c>`| `GET` | Sets driving direction (`F`,`B`,`L`,`R`,`S`) | `"MOVE OK"` |
| `/stop` | `GET` | Stops vehicle locomotion immediately | `"STOP OK"` |
| `/fire/on` | `GET` | Energizes relay (Safe Low-Voltage Test Load) | `"RELAY ON (SAFE TEST LOAD)"` |
| `/fire/off`| `GET` | Disarms relay | `"RELAY OFF"` |
| `/relay/on` | `GET` | Alias for `/fire/on` | `"RELAY ON (SAFE TEST LOAD)"` |
| `/relay/off`| `GET` | Alias for `/fire/off`| `"RELAY OFF"` |

---

## 5. Changelog (Integration Phase Modifications)

1. **`ESP32/Main/Main.ino`:**
   - Designed and embedded dedicated Laptop Radar HUD (`/radar`) and Phone Remote (`/remote`).
   - Integrated full Drone Camera UI state machine (Off -> Small Preview -> Fullscreen -> Small Preview -> Off) with configurable stream URL and explicit offline state.
   - Added bidirectional UNO distance telemetry ingest (`readUno()`) on `HardwareSerial(1)`.
   - Implemented Secondary Aiming Servo: smooth continuous oscillation (0°–180°) when free, solid target heading hold when locked.
   - Added client activity watchdog (1500 ms) auto-stopping vehicle on Wi-Fi drop.
   - Harmonized all WebServer routes (`/move?cmd=`, `/fire/*`, `/relay/*`, `/radar/*`).
2. **`Nano/Radar/Radar.ino`:**
   - Added serial `START` and `STOP` commands.
   - Added `radarRunning` check in main loop to pause sweep and ultrasonic pinging when stopped.
   - Maintained full backward compatibility with `LOCK:<angle>` and `UNLOCK`.
3. **`UNO/Tank/Tank.ino`:**
   - Added 1000 ms communication watchdog failsafe: halts motors if in motion and no command arrives.
   - Added periodic distance telemetry output (`DIST:<distance>`) every 100 ms to ESP32.
   - Preserved Adafruit Motor Shield V1 mapping and 10 cm autonomous forward collision stop.

---

## 6. Critical Technical Constraints (DO NOT ALTER CASUALLY)

1. **Relay Safety:**
   - GPIO 27 must remain Active HIGH, default to `LOW` on boot, and disarm on any vehicle stop.
   - Strictly attach to a safe low-voltage bench test load (LED/indicator).
2. **Servo Angular Limits:**
   - Hardware servos are standard hobby positional servos (500–2400 μs). Keep commands within $[0, 180]$ degrees.
3. **No Fake Camera Feed:**
   - Do not invent RTSP or HTTP URLs in code. Let the operator configure the actual stream URL via the UI modal.
