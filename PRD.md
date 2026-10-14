# TRINETRA — Product Requirements Document (PRD)

**Project Name:** TRINETRA — Smart Surveillance and Reconnaissance Ground Vehicle  
**Document:** `PRD.md`  
**Phase:** Integration & Implementation Complete  
**Classification Standard:** `[IMPLEMENTED]`, `[PARTIALLY IMPLEMENTED]`, `[UNKNOWN]`, `[NOT POSSIBLE WITH CURRENT HARDWARE]`

---

## 1. Project Purpose & Scope

TRINETRA is an unmanned, semi-autonomous reconnaissance ground vehicle designed for surveillance, perimeter monitoring, target discovery and lock-on tracking, and remote tactical actuation.  
The system coordinates three microcontrollers (ESP32, Arduino Nano, Arduino UNO) to provide:
1. Long-range wireless control via an onboard Wi-Fi SoftAP (`TRINETRA`).
2. Dedicated dual web interfaces:
   - **Laptop Tactical Radar HUD** (`/radar`): Full-screen radar visualization, target tracking, and telemetry monitoring without control clutter.
   - **Phone Remote Control** (`/remote`): Handheld mobile controller with large virtual joystick, action buttons, camera preview, and telemetry.
   - **Command Portal Gateway** (`/`): Tactical landing page directing operators to either interface.
3. Autonomous forward obstacle avoidance with communication loss failsafe.
4. 180° scanning ultrasonic radar with software Start/Stop and Target Lock.
5. Secondary Aiming Servo ("chatter-patter" mechanism): smooth autonomous sweeping during patrol, holding target heading when locked.
6. Safe low-voltage relay output module (FIRE button) with vehicle stop disarm.
7. Drone camera integration framework with preview expand/minimize state machine.

---

## 2. Requirements & Implementation Matrix

### 2.1 Radar Scanning Subsystem
* **Status:** `[IMPLEMENTED]`
* **Capabilities:**
  - Arduino Nano sweeps SG90 servo between 0° and 180° in 2° increments with a 30 ms delay.
  - Measures distance using HC-SR04 ultrasonic sensor (Trigger D7, Echo D8) up to 249 cm.
  - Streams serial data: `RADAR,<angle>,<distance>\n` at 9600 baud.
  - Remote operator can trigger `START` and `STOP` from `/radar/start` and `/radar/stop`.
  - When stopped: Nano pauses sweep, stops ultrasonic pulses, and ESP32 clears stale target detections.

### 2.2 Target Detection & Target Lock Subsystem
* **Status:** `[IMPLEMENTED]`
* **Capabilities:**
  - **Detection Condition:** Distance $\ge 2\text{ cm}$ and $\le 150\text{ cm}$ (`TARGET_DISTANCE = 150`) flags `targetDetected = true`.
  - **Target Lock Command (`/lock`):**
    - Remembers `lockedAngle = targetAngle` and `lockedDistance = targetDistance`.
    - Commands Arduino Nano with `LOCK:<lockedAngle>\n` to freeze radar servo on target and continuously ping range every 100 ms.
    - Commands Secondary Aiming Servo on ESP32 GPIO 13 directly to `lockedAngle` and holds heading.
    - UI displays `TARGET LOCKED @ X°`.
  - **Target Unlock Command (`/unlock`):**
    - Sends `UNLOCK\n` to Nano, resuming continuous radar sweep.
    - Releases Secondary Aiming Servo back to normal continuous patrol oscillation.
    - UI updates to `UNLOCKED / SECTOR CLEAR`.

### 2.3 Secondary / Aiming Servo ("Chatter-Patter" Mechanism)
* **Status:** `[IMPLEMENTED]`
* **Hardware Confirmation:**
  - Hardware uses standard positional hobby servo (500–2400 μs pulse width) on ESP32 GPIO 13.
  - Constrained strictly to physical safe range `0°` to `180°` (true continuous 360° rotation is not physically supported by standard positional servos).
* **Behavior:**
  - Normal patrol state: Smooth autonomous back-and-forth angular oscillation across 0°–180° via non-blocking step timer (30 ms).
  - Target lock state: Halts oscillation, drives to `lockedAngle`, and holds position.
  - Target unlock state: Resumes normal oscillation from current position.

### 2.4 Vehicle Movement & Locomotion Subsystem
* **Status:** `[IMPLEMENTED]`
* **Capabilities:**
  - Arduino UNO with Adafruit Motor Shield V1 controls 4 DC motors (`M1`–`M4`) at fixed speed 200/255.
  - Directional commands: Forward (`'F'`), Backward (`'B'`), Pivot Left (`'L'`), Pivot Right (`'R'`), Stop (`'S'`).
  - Mobile Remote features a virtual 360° touch joystick mapping drag angles to discrete movement commands.
  - Communication Watchdogs:
    - **UNO Watchdog:** If robot is driving and no serial command arrives within 1000 ms, UNO halts all motors automatically.
    - **ESP32 Watchdog:** If mobile client drops Wi-Fi connection for >1500 ms while driving, ESP32 transmits `'S'` to UNO and disarms relay.

### 2.5 Forward Obstacle Avoidance Subsystem
* **Status:** `[IMPLEMENTED]`
* **Capabilities:**
  - Arduino UNO reads dedicated HC-SR04 ultrasonic sensor on analog pins A0 (TRIG) and A1 (ECHO).
  - Safety threshold: 10 cm (`SAFE_DISTANCE = 10`).
  - Continuous loop safety check halts motors during forward drive if obstacle $\le 10\text{ cm}$, transitions internal command to `'S'`, and disarms relay.
  - UNO transmits `DIST:<distance>\n` every 100 ms over serial to ESP32.
  - ESP32 ingests telemetry and reflects `frontDistance` on both Laptop Radar HUD and Phone Remote in real time.

### 2.6 Relay / Auxiliary Weapon Trigger (Safe Low-Voltage Test Load)
* **Status:** `[IMPLEMENTED]`
* **Safety Mandate:** Connected strictly to a safe low-voltage test indicator (e.g. LED/multimeter). No pyrotechnic or high-voltage loads.
* **Capabilities:**
  - ESP32 GPIO 27 driven Active HIGH (`digitalWrite(RELAY_PIN, HIGH)` is ON, `LOW` is OFF).
  - Defaults to SAFE/OFF state at boot.
  - Remotely triggered via `[FIRE RELAY]` (`/fire/on` and `/relay/on`) and disarmed via `[RELAY OFF]` (`/fire/off` and `/relay/off`).
  - Failsafe Disarm: Any vehicle `STOP ('S')`, forward obstacle detection, or client watchdog timeout immediately turns relay OFF.

### 2.7 Drone Camera System
* **Status:** `[PARTIALLY IMPLEMENTED]`
* **Hardware & Network Reality:**
  - The vehicle hardware does not include an onboard ESP32-CAM or local video digitizer; the drone camera is an external unit (`[UNKNOWN]` stream URL/IP/format in repository).
  - ESP32 cannot transcode or bridge high-definition RTSP streams due to RAM/CPU constraints.
* **Implemented Capabilities:**
  - Phone Remote UI incorporates full Camera integration container and state machine:
    $$\text{CAMERA OFF} \xrightarrow{\text{Click [CAMERA]}} \text{SMALL PREVIEW} \xrightarrow{\text{Single-Tap}} \text{EXPANDED FULLSCREEN} \xrightarrow{\text{Double-Tap}} \text{SMALL PREVIEW} \xrightarrow{\text{Click [CAMERA]}} \text{CAMERA OFF}$$
  - Stream URL Configuration Modal: Operators can input the drone camera's live HTTP/MJPEG/Web stream URL (stored in browser local storage).
  - Explicit Offline State: When no stream is configured or camera is unreachable, UI displays `FEED STATUS: AWAITING STREAM URL` without fabricating video.

### 2.8 Audio Subsystem
* **Status:** `[NOT POSSIBLE WITH CURRENT HARDWARE]`
* **Reality:**
  - Active firmware contains no audio hardware drivers.
  - Backup stubs reference `ESP8266Audio` I2S libraries with dummy 1-byte arrays (`intro_mp3`, `horn_mp3`), but no physical I2S DAC (e.g., MAX98357A) or speaker module is confirmed wired to the chassis.
  - GPIO 27 was previously aliased to `AUDIO_DEMO_PIN`, which conflicted with the active relay pin. GPIO 27 is strictly reserved for the relay.

---

## 3. Dedicated User Interfaces Summary

| View | Route | Intended Platform | Primary Purpose | Features |
| :--- | :--- | :--- | :--- | :--- |
| **Command Portal** | `/` | All Devices | Mode Selection Gateway | Direct launch buttons to `/radar` and `/remote` |
| **Tactical Radar HUD**| `/radar` | Laptop / Monitor | Situational Awareness | Full-screen polar radar canvas, sweep line, distance rings, target crosshairs, telemetry cards |
| **Phone Remote** | `/remote` | Smartphone / Tablet | Handheld Vehicle Drive | Virtual joystick, radar start/stop, target lock/unlock, camera preview with expand/minimize, fire relay |
| **Telemetry API** | `/data` | Background JSON | Telemetry Synchronization | Real-time payload polling (100 ms) for all sensor states |
