# TRINETRA — Complete System Architecture Specification

**Project:** TRINETRA (Smart Surveillance and Reconnaissance Ground Vehicle)  
**Document:** `architecture.md`  
**Status:** [IMPLEMENTED] Full System Technical Specification Post-Integration

---

## 1. Hardware Architecture

TRINETRA employs a distributed, heterogeneous triple-microcontroller architecture designed to isolate low-latency sensor sweeping and motor driving from high-level Wi-Fi networking and web graphics.

```
+-----------------------------------------------------------------------------------+
|                                 TRINETRA HARDWARE                                 |
+-----------------------------------------------------------------------------------+

     +-----------------------+                    +-----------------------+
     |     ARDUINO NANO      |                    |      ARDUINO UNO      |
     |   (Radar Subsystem)   |                    | (Locomotion & Safety) |
     +-----------+-----------+                    +-----------+-----------+
                 |                                            |
        D9 (PWM) | D7(Trig)/D8(Echo)                 A0(Trig) | Motor Shield V1
                 |                                   A1(Echo) | (Pins 3-12)
                 v                                            v
         +-------+-------+                           +--------+--------+
         | SG90 Servo    |                           | HC-SR04 Sensor  | (Front Obstacle)
         | HC-SR04 Sensor| (Radar Scanner)           +--------+--------+
         +---------------+                           | 4x DC Motors    | (M1, M2, M3, M4)
                 ^                                   +-----------------+
                 | UART (9600 baud)                           ^
                 | D0(RX)/D1(TX)                              | UART (9600 baud)
                 |                                            | D0(RX)/D1(TX)
                 v                                            v
     +-----------+--------------------------------------------+-----------+
     |                                 ESP32                              |
     |                     (Master Controller & SoftAP)                   |
     |                                                                    |
     |  GPIO 16(RX2) / GPIO 17(TX2) <---> Nano                            |
     |  GPIO 4(RX1)  / GPIO 5(TX1)  <---> UNO                             |
     |  GPIO 13 (PWM)               <---> Secondary / Aiming Servo        |
     |  GPIO 27 (OUTPUT)            <---> Auxiliary Relay Module (Active HIGH)|
     |  Wi-Fi 802.11 b/g/n (2.4GHz) <---> Laptop Radar (/radar)           |
     |                              <---> Phone Remote (/remote)          |
     +--------------------------------------------------------------------+
```

### 1.1 Microcontroller Breakdown & Hardware Peripherals

| Component | Controller | Pin / Interface | Direction | Purpose | Power Rail | Implementation Status |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **ESP32 NodeMCU** | ESP32 | Onboard SoC | Processing / Wi-Fi | Master coordinator, SoftAP, Web Server, Target Locking, Relay | 5V VIN (`[INFERRED]`) | `[IMPLEMENTED]` |
| **Arduino Nano** | Nano | ATmega328P | Processing / Sensor | Radar angle sweeping & distance timing | 5V VCC (`[INFERRED]`) | `[IMPLEMENTED]` |
| **Arduino UNO** | UNO | ATmega328P | Processing / Motor | Motor driving & obstacle distance timing | 5V VCC / Motor Ext (`[INFERRED]`) | `[IMPLEMENTED]` |
| **Radar Servo** | Nano | D9 (PWM) | OUTPUT | Sweeps ultrasonic sensor across 0°–180° | 5V Rail (`[INFERRED]`) | `[IMPLEMENTED]` |
| **Radar Ultrasonic** | Nano | D7 (TRIG), D8 (ECHO) | D7: OUT, D8: IN | Radar distance echo measurement (up to 249 cm) | 5V Rail (`[INFERRED]`) | `[IMPLEMENTED]` |
| **Secondary Aiming Servo**| ESP32 | GPIO 13 (PWM) | OUTPUT | Continuous patrol oscillation / Target Heading Lock | 5V Rail (`[INFERRED]`) | `[IMPLEMENTED]` |
| **Front Ultrasonic** | UNO | A0 (TRIG), A1 (ECHO) | A0: OUT, A1: IN | Front collision safety distance measurement | 5V Rail (`[INFERRED]`) | `[IMPLEMENTED]` |
| **Adafruit Motor Shield**| UNO | Digital 3–12 | OUTPUT | Dual L293D H-bridge driver shield | External battery terminal (`[INFERRED]`) | `[IMPLEMENTED]` |
| **4x DC Motors** | UNO | M1, M2, M3, M4 | OUTPUT | 4-wheel drive / tank skid steer locomotion | Motor Shield power | `[IMPLEMENTED]` |
| **Auxiliary Relay** | ESP32 | GPIO 27 | OUTPUT | Safe low-voltage test load trigger (Active HIGH) | 5V VCC (`[INFERRED]`) | `[IMPLEMENTED]` |
| **Drone Camera** | External | Wi-Fi / RTSP / HTTP | Network Stream | Video surveillance feed to Phone Remote | Drone internal | `[PARTIALLY IMPLEMENTED]` (UI & Client Ready) |
| **Audio Hardware** | ESP32 | GPIO 26 / 14 | OUTPUT | Horn & introduction sounds | `[UNKNOWN]` | `[NOT POSSIBLE WITH CURRENT HARDWARE]` |

---

## 2. Software Architecture

### 2.1 Active Production Files
- **`ESP32/Main/Main.ino`:** Active master firmware. Manages SoftAP, WebServer serving `/radar`, `/remote`, `/`, and `/data`, dual hardware UART pipelines, non-blocking aiming servo sweep, and safe relay control.
- **`Nano/Radar/Radar.ino`:** Active radar firmware. Manages SG90 0°–180° sweep, HC-SR04 pulse timing, start/stop commands, and target lock holding.
- **`UNO/Tank/Tank.ino`:** Active drive firmware. Manages 4 DC motors via Adafruit Motor Shield V1, 10 cm autonomous forward collision stop, 1000 ms comms loss watchdog, and periodic distance telemetry.

---

## 3. Communication Architecture & Protocols

```
+-------------+                     +-------------+                     +-------------+
| ARDUINO NANO|                     |    ESP32    |                     | ARDUINO UNO |
+------+------+                     +------+------+                     +------+------+
       |                                   |                                   |
       |  RADAR,<angle>,<dist>\n (9600bd)  |                                   |
       |---------------------------------->|                                   |
       |                                   |                                   |
       |  LOCK:<angle>\n / UNLOCK\n        |                                   |
       |<----------------------------------|                                   |
       |                                   |                                   |
       |  START\n / STOP\n                 |                                   |
       |<----------------------------------|                                   |
       |                                   |                                   |
       |                                   |  'F'|'B'|'L'|'R'|'S'\n (9600bd)   |
       |                                   |---------------------------------->|
       |                                   |                                   |
       |                                   |  DIST:<dist>\n (9600bd)           |
       |                                   |<----------------------------------|
       |                                   |                                   |
```

### 3.1 Serial Line Specifications

| Bus | Pins | Baud Rate | Direction | Format | Description |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **ESP32 ↔ Nano** | ESP32 GPIO 16 (RX), 17 (TX)<br>Nano D1 (TX), D0 (RX) | 9600 baud | Bidirectional | ASCII newline-terminated | Telemetry: `RADAR,<deg>,<cm>`<br>Commands: `LOCK:<deg>`, `UNLOCK`, `START`, `STOP` |
| **ESP32 ↔ UNO** | ESP32 GPIO 4 (RX), 5 (TX)<br>UNO D1 (TX), D0 (RX) | 9600 baud | Bidirectional | ASCII newline-terminated | Commands: `F`, `B`, `L`, `R`, `S`<br>Telemetry: `DIST:<cm>`, `OBSTACLE_STOP`, `COMM_TIMEOUT_STOP` |
| **ESP32 USB Debug**| ESP32 GPIO 3 (RX0), 1 (TX0) | 115200 baud | To Host PC | Plain text | Boot logs, route debugging, command traces |

### 3.2 Web HTTP API Endpoints

| Method | Endpoint | Description | Payload / Response |
| :--- | :--- | :--- | :--- |
| `GET` | `/` | Command Portal Gateway | HTML page linking to `/radar` and `/remote` |
| `GET` | `/radar` | Dedicated Laptop Tactical Radar Display | Full-screen polar radar canvas HUD |
| `GET` | `/remote` | Dedicated Phone Remote Control | Handheld mobile controller HUD |
| `GET` | `/data` | Real-time JSON Telemetry | Complete state JSON |
| `GET` | `/radar/start` | Commands Nano to start radar scanning | Plain text: `"RADAR STARTED"` |
| `GET` | `/radar/stop` | Commands Nano to stop radar scanning | Plain text: `"RADAR STOPPED"` |
| `GET` | `/lock` | Locks target on detected angle | Plain text: `"LOCK OK"` |
| `GET` | `/unlock` | Releases target lock | Plain text: `"UNLOCK OK"` |
| `GET` | `/move` | Drives vehicle (`?cmd=F\|B\|L\|R\|S`) | Plain text: `"MOVE OK"` |
| `GET` | `/stop` | Stops vehicle & disarms relay | Plain text: `"STOP OK"` |
| `GET` | `/fire/on` | Energizes relay (Safe Test Load) | Plain text: `"RELAY ON (SAFE TEST LOAD)"` |
| `GET` | `/fire/off`| Disarms relay | Plain text: `"RELAY OFF"` |
| `GET` | `/relay/on` | Alias for `/fire/on` | Plain text: `"RELAY ON (SAFE TEST LOAD)"` |
| `GET` | `/relay/off`| Alias for `/fire/off`| Plain text: `"RELAY OFF"` |

---

## 4. End-to-End System Flows

### 4.1 System Boot Flow
```
[POWER APPLIED]
   ├── Nano Boots -> Sweeps servo to 0° -> Emits "NANO_RADAR_READY" -> Enters 0°..180° sweep & streaming
   ├── UNO Boots  -> Releases all 4 motors -> Emits "UNO_MOVEMENT_READY" -> Enters command listen loop
   └── ESP32 Boots
         ├── Starts USB debug Serial @ 115200 baud
         ├── Initializes RadarSerial (UART2 @ 9600) on GPIO 16/17
         ├── Initializes UnoSerial (UART1 @ 9600) on GPIO 4/5
         ├── Attaches lockServo on GPIO 13, starts smooth oscillation
         ├── Configures GPIO 27 as OUTPUT, forces LOW (OFF)
         ├── Starts SoftAP ("TRINETRA" / "12345678") -> IP 192.168.4.1
         ├── Registers WebServer routes
         └── Enters main loop
```

### 4.2 Target Detection & Lock Flow
```
Nano sweeps servo 2° -> Measures HC-SR04 -> Sends "RADAR,<angle>,<dist>\n"
   ↓
ESP32 readRadar():
   ├── If distance <= 150 cm: sets targetDetected = true, targetAngle = angle, targetDistance = distance
   ↓
Operator clicks [LOCK TARGET] on Phone Remote or Web UI
   ↓
ESP32 /lock executes:
   ├── Sets targetLocked = true, lockedAngle = targetAngle
   ├── Commands lockServo.write(lockedAngle) on GPIO 13 (holds heading)
   └── Sends "LOCK:<lockedAngle>\n" to Nano
   ↓
Nano freezes servo at lockedAngle and streams range at locked heading every 100 ms
   ↓
Operator clicks [UNLOCK]:
   ├── ESP32 sets targetLocked = false, sends "UNLOCK\n" to Nano
   └── Aiming servo resumes smooth continuous oscillation
```

### 4.3 Locomotion & Collision Failsafe Flow
```
Operator drags virtual joystick on Phone Remote
   ↓
JS calculates angle -> sends GET /move?cmd=F
   ↓
ESP32 forward to UNO: sends 'F\n'
   ↓
UNO executeCommand('F'):
   ├── Checks obstacleAhead() (distance <= 10 cm)
   ├── If CLEAR: engages 4 motors FORWARD
   └── If OBSTACLE: stops motors, sets currentCommand = 'S', sends "OBSTACLE_STOP"
   ↓
Failsafe Watchdogs:
   ├── UNO Watchdog: If no command received for >1000 ms, halts motors
   └── ESP32 Watchdog: If phone client disconnects for >1500 ms, sends 'S' and disarms relay
```

---

## 5. Final Wiring Checklist

### 5.1 ESP32 Connections
| Pin | Component Connected | Direction | Notes |
| :--- | :--- | :--- | :--- |
| **GPIO 16 (RX2)** | Arduino Nano TX (D1) | INPUT | 3.3V logic (Nano 5V TX; verify voltage divider/shifter) |
| **GPIO 17 (TX2)** | Arduino Nano RX (D0) | OUTPUT | Direct 3.3V TX to Nano 5V RX |
| **GPIO 4 (RX1)** | Arduino UNO TX (D1) | INPUT | 3.3V logic (UNO 5V TX; verify voltage divider/shifter) |
| **GPIO 5 (TX1)** | Arduino UNO RX (D0) | OUTPUT | Direct 3.3V TX to UNO 5V RX |
| **GPIO 13** | Secondary Aiming Servo Signal | OUTPUT | 50 Hz PWM to SG90 / hobby servo |
| **GPIO 27** | Relay Module Signal (IN) | OUTPUT | Active HIGH. Connect ONLY to safe low-voltage test load |
| **VIN** | 5V Power Bus | POWER | 5V regulated input |
| **GND** | System Common Ground | GROUND | Common ground shared across all boards |

### 5.2 Arduino Nano Connections
| Pin | Component Connected | Direction | Notes |
| :--- | :--- | :--- | :--- |
| **D0 (RX)** | ESP32 TX2 (GPIO 17) | INPUT | 9600 baud serial receive |
| **D1 (TX)** | ESP32 RX2 (GPIO 16) | OUTPUT | 9600 baud serial transmit |
| **D7** | HC-SR04 (Radar) Trigger | OUTPUT | Ultrasonic 10 μs trigger pulse |
| **D8** | HC-SR04 (Radar) Echo | INPUT | Ultrasonic pulseIn echo timing |
| **D9** | SG90 Radar Servo Signal | OUTPUT | 50 Hz PWM positional sweep (0°–180°) |
| **5V / GND** | 5V Power Bus / Common Ground| POWER/GND | Shared power rails |

### 5.3 Arduino UNO & Motor Shield Connections
| Pin | Component Connected | Direction | Notes |
| :--- | :--- | :--- | :--- |
| **D0 (RX)** | ESP32 TX1 (GPIO 5) | INPUT | 9600 baud serial receive |
| **D1 (TX)** | ESP32 RX1 (GPIO 4) | OUTPUT | 9600 baud serial transmit |
| **A0** | HC-SR04 (Front) Trigger | OUTPUT | Ultrasonic 10 μs trigger pulse |
| **A1** | HC-SR04 (Front) Echo | INPUT | Ultrasonic pulseIn echo timing |
| **M1, M2** | Left DC Gear Motors | OUTPUT | Driven via Adafruit Motor Shield V1 channels 1 & 2 |
| **M3, M4** | Right DC Gear Motors | OUTPUT | Driven via Adafruit Motor Shield V1 channels 3 & 4 |
| **Shield EXT_PWR**| Motor Battery (6–12V) | POWER | Separate high-current motor battery pack |
| **UNO VIN/GND** | 5V Logic Bus / Common Ground| POWER/GND | Common logic ground with ESP32 |

### 5.4 Camera & Audio Connections
| Component | Status | Notes |
| :--- | :--- | :--- |
| **Drone Camera** | [PARTIALLY IMPLEMENTED] | External wireless unit. Feed URL configured dynamically in Phone Remote UI. |
| **Audio Hardware** | [UNKNOWN — VERIFY HARDWARE]| No physical audio hardware confirmed. Pin 27 strictly reserved for relay. |
| **Power Booster** | [UNKNOWN — VERIFY HARDWARE]| Verify battery regulator output voltages (5V logic, motor supply). |
