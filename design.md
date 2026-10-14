# TRINETRA — User Interface & Interaction Design Document

**Project:** TRINETRA (Smart Surveillance and Reconnaissance Ground Vehicle)  
**Document:** `design.md`  
**Status:** [IMPLEMENTED] Final UI/UX Specifications for Dual Interface Architecture

---

## 1. UI Architecture Overview

To accommodate distinct operator use cases, TRINETRA provides two specialized interfaces served directly by the ESP32:
1. **Laptop Tactical Radar HUD (`/radar`):** Optimized for laptops/desktops, focusing exclusively on full-screen radar visualization, target telemetry, and perimeter tracking without remote control clutter.
2. **Phone Remote Control (`/remote`):** Optimized for handheld smartphones, providing a virtual joystick, tactile action buttons, integrated camera preview with expand/minimize behavior, and critical telemetry.
3. **Command Portal Gateway (`/`):** A minimalist mission select landing page allowing operators to choose between the Laptop Radar or Phone Remote interfaces.

---

## 2. Interface 1: Laptop Tactical Radar HUD (`/radar`)

### 2.1 Aesthetic & Visual Identity
- **Theme:** Military reconnaissance terminal / Tactical CRT radar.
- **Palette:** Deep obsidian black (`#010804`), Neon green phosphor (`#00ff88`), Alert Crimson (`#ff3333`), Amber Warning (`#ff9900`).
- **Typography:** `'Share Tech Mono', monospace`.

### 2.2 Layout Breakdown
```
+-----------------------------------------------------------------------------------+
|  TRINETRA // TACTICAL RADAR          [RADAR: ACTIVE]  [UNLOCKED]  [COMMS: OK]     |
+----------------------------------------------------+------------------------------+
|                                                    |  [ TARGET ACQUISITION ]      |
|                                                    |  +------------------------+  |
|                                                    |  |  SCANNING / LOCKED     |  |
|                                                    |  +------------------------+  |
|                 TACTICAL RADAR                     |  Target Azimuth: 74°         |
|               2D CANVAS POLAR DISPLAY              |  Target Range: 120 cm        |
|                                                    |                              |
|           - 0° to 180° Polar Arc                   |  [ BEAM TELEMETRY ]          |
|           - Range Rings: 50, 100, 150, 200, 250cm  |  Current Beam: 74°           |
|           - Radial Angle Rays (every 30°)          |  Measured Dist: 120 cm       |
|           - Glowing Sweeper Beam Line              |  Aiming Servo: 74°           |
|           - Pulsing Red Target Crosshair Blip      |                              |
|                                                    |  [ VEHICLE SYSTEMS ]         |
|                                                    |  Front Obstacle: 45 cm       |
|                                                    |  Drive State: STOPPED        |
|                                                    |  Payload Relay: SAFE / OFF   |
|                                                    |  Nano Link: ONLINE           |
|                                                    |  Uno Link: ONLINE            |
+----------------------------------------------------+------------------------------+
|  TRINETRA RECONNAISSANCE GROUND VEHICLE                    [OPEN PHONE REMOTE ->] |
+-----------------------------------------------------------------------------------+
```

### 2.3 Radar Canvas Mechanics
- **Grid Rendering:** Smooth HTML5 2D Canvas with radial coordinate mapping.
- **Sweeper Line:** Real-time azimuth rotation tracking `data.angle` with a blurred phosphorescent trail.
- **Target Visualization:** Appears when `data.target == true` or `data.locked == true`:
  - 14px red crosshair + 7px core dot positioned at polar coordinate $(r, \theta)$:
    $$x = c_x + \left(\frac{\text{dist}}{\text{maxDist}}\right) \cdot R \cdot \cos\left(\frac{180 - \theta}{180}\pi\right)$$
    $$y = c_y - \left(\frac{\text{dist}}{\text{maxDist}}\right) \cdot R \cdot \sin\left(\frac{180 - \theta}{180}\pi\right)$$

---

## 3. Interface 2: Phone Remote Control (`/remote`)

### 3.1 Handheld Ergonomics
- Viewport constrained (`user-scalable=no, maximum-scale=1.0`) for mobile touchscreens.
- Large interactive tap targets ($\ge 44\text{px}$) with haptic-like active scale animations.

### 3.2 Layout Breakdown
```
+------------------------------------------+
|  TRINETRA // REMOTE   [ONLINE]  [CLEAR]  |
+------------------------------------------+
|  [ CAMERA PREVIEW CONTAINER ]            |
|  +------------------------------------+  |
|  |  (Small inline floating box)       |  |
|  |  [FEED: AWAITING STREAM URL]       |  |
|  |  1-Tap: Expand / 2-Tap: Minimize   |  |
|  +------------------------------------+  |
+------------------------------------------+
|  [RADAR: 74°] [DIST: 120cm] [FRONT: 45cm]|
+------------------------------------------+
|                                          |
|            VIRTUAL JOYSTICK              |
|        (Drag to Drive / Center=Stop)     |
|                                          |
+------------------------------------------+
|  [RADAR START]         [RADAR STOP]      |
|  [LOCK TARGET]         [FREE UNLOCK]     |
|  [CAMERA (TOGGLE)]     [SET CAM URL]     |
|  [======== FIRE RELAY (TEST) ========]   |
|  [------------- RELAY OFF -----------]   |
+------------------------------------------+
|  TRINETRA CONTROLLER   [LAPTOP RADAR ->] |
+------------------------------------------+
```

### 3.3 Virtual Touch Joystick Mechanics
- **Outer Well:** 150px circular track.
- **Inner Handle:** 55px movable puck.
- **Max Deflection:** 45px radius from center.
- **Deadzone:** 12px from center.
- **Sector Mapping:**
  - Forward (`'F'`): $-135^\circ \le \theta < -45^\circ$
  - Right (`'R'`): $-45^\circ \le \theta < 45^\circ$
  - Backward (`'B'`): $45^\circ \le \theta < 135^\circ$
  - Left (`'L'`): Outside the above sectors
- **Release-to-Stop:** Releasing touch or mouse immediately centers handle and sends `/move?cmd=S`.

### 3.4 Camera Integration State Machine
The camera view incorporates an interactive multi-tier display hierarchy:

```
[CAMERA OFF] (Hidden)
     │
     │ Click [CAMERA] Button
     ▼
[SMALL PREVIEW] (Inline 140px card above joystick)
     │
     ├── Single-Tap Preview ───────► [FULLSCREEN EXPANDED] (100vw x 100vh overlay)
     │                                      │
     │                                      │ Double-Tap Preview
     │                                      ▼
     ◄────────────────────────────── [SMALL PREVIEW]
     │
     │ Click [CAMERA] Button
     ▼
[CAMERA OFF] (Hidden)
```

- **Configurable Video Stream:** Clicking `[SET CAM URL]` opens a prompt allowing the operator to input their live drone stream URL (e.g. `http://192.168.4.X:81/stream`), stored persistently in browser `localStorage`.
- **Explicit Offline Fallback:** When no URL is present or stream fails, the UI displays a clean tactical overlay: `DRONE CAMERA FEED: AWAITING STREAM URL` without fabricating synthetic video.

---

## 4. Subsystem Feedback & Status Indicators

- **Target Lock:**
  - Unlocked: Green badge `UNLOCKED / CLEAR`, text `SCANNING SECTOR`.
  - Target Detected: Amber badge `TARGET ACQUIRED`, banner `TARGET DETECTED @ X°`.
  - Target Locked: Glowing red badge `LOCKED @ X°`, banner `TARGET LOCKED @ X°`.
- **Radar State:**
  - Running: Green badge `RADAR: ACTIVE`.
  - Stopped: Amber badge `RADAR: STOPPED`.
- **Vehicle Movement:**
  - Live readout of current drive vector: `STOP`, `FORWARD`, `BACKWARD`, `LEFT`, `RIGHT`.
- **Relay / Fire:**
  - De-energized: Charcoal badge `SAFE / OFF`.
  - Energized: Amber badge `TRIGGERED (ON)` (Tested only with low-voltage load).
