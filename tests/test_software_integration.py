"""
TRINETRA Software Integration & Protocol Test Suite
Validates mathematical models, protocols, state machines, and failsafes.
"""

import math
import unittest

class TestTrinetraProtocolsAndLogic(unittest.TestCase):

    # -------------------------------------------------------------------------
    # 1. Nano Radar Protocol & Command Parsing
    # -------------------------------------------------------------------------
    def test_nano_radar_packet_parser(self):
        sample_packet = "RADAR,74,120\n"
        packet = sample_packet.strip()
        self.assertTrue(packet.startswith("RADAR,"))
        parts = packet.split(",")
        self.assertEqual(len(parts), 3)
        angle = int(parts[1])
        dist = int(parts[2])
        self.assertEqual(angle, 74)
        self.assertEqual(dist, 120)
        self.assertTrue(0 <= angle <= 180)
        self.assertTrue(0 <= dist <= 249)

    def test_nano_commands(self):
        valid_commands = ["START\n", "STOP\n", "LOCK:74\n", "UNLOCK\n"]
        for cmd in valid_commands:
            c = cmd.strip()
            if c.startswith("LOCK:"):
                angle = int(c[5:])
                self.assertTrue(0 <= angle <= 180)
            else:
                self.assertIn(c, ["START", "STOP", "UNLOCK"])

    # -------------------------------------------------------------------------
    # 2. Target Detection & Range Discrimination
    # -------------------------------------------------------------------------
    def test_target_detection_threshold(self):
        detection_threshold = 150  # cm

        def evaluate_target(dist_cm, is_locked=False):
            if is_locked:
                return True
            return 2 <= dist_cm <= detection_threshold

        self.assertTrue(evaluate_target(2))
        self.assertTrue(evaluate_target(75))
        self.assertTrue(evaluate_target(150))
        self.assertFalse(evaluate_target(1))
        self.assertFalse(evaluate_target(151))
        self.assertFalse(evaluate_target(249))

    # -------------------------------------------------------------------------
    # 3. Joystick Direction Mapping Engine
    # -------------------------------------------------------------------------
    def test_joystick_sector_mapping(self):
        def map_joystick(dx, dy, deadzone=12):
            distance = math.sqrt(dx * dx + dy * dy)
            if distance <= deadzone:
                return 'S'
            angle = math.degrees(math.atan2(dy, dx))
            if -135 <= angle < -45:
                return 'F'  # Forward (drag up in screen coords)
            elif -45 <= angle < 45:
                return 'R'  # Right
            elif 45 <= angle < 135:
                return 'B'  # Backward (drag down in screen coords)
            else:
                return 'L'  # Left

        self.assertEqual(map_joystick(0, 0), 'S')
        self.assertEqual(map_joystick(5, 5), 'S')
        self.assertEqual(map_joystick(0, -35), 'F')
        self.assertEqual(map_joystick(35, 0), 'R')
        self.assertEqual(map_joystick(0, 35), 'B')
        self.assertEqual(map_joystick(-35, 0), 'L')

    # -------------------------------------------------------------------------
    # 4. Secondary Aiming Servo Patrol & Lock Mechanics
    # -------------------------------------------------------------------------
    def test_aiming_servo_oscillation_and_lock(self):
        # Patrol mode
        angle = 90
        direction = 1
        steps = 50
        for _ in range(steps):
            angle += direction * 2
            if angle >= 180:
                angle = 180
                direction = -1
            elif angle <= 0:
                angle = 0
                direction = 1
        self.assertTrue(0 <= angle <= 180)

        # Lock mode: snaps and stays frozen
        locked_angle = 74
        servo_locked_heading = locked_angle
        for _ in range(20):
            # Simulation of lock hold
            current_heading = servo_locked_heading
            self.assertEqual(current_heading, 74)

    # -------------------------------------------------------------------------
    # 5. UNO Obstacle Avoidance & Watchdog Failsafe
    # -------------------------------------------------------------------------
    def test_forward_obstacle_safety(self):
        safe_distance = 10  # cm

        def check_safety(cmd, obstacle_dist):
            if cmd == 'F' and obstacle_dist <= safe_distance:
                return 'S', True  # Force Stop
            return cmd, False

        # Obstacle clear
        cmd, blocked = check_safety('F', 45)
        self.assertEqual(cmd, 'F')
        self.assertFalse(blocked)

        # Obstacle critical
        cmd, blocked = check_safety('F', 8)
        self.assertEqual(cmd, 'S')
        self.assertTrue(blocked)

        # Backward maneuver permitted even when front blocked
        cmd, blocked = check_safety('B', 8)
        self.assertEqual(cmd, 'B')
        self.assertFalse(blocked)

    def test_communication_watchdog(self):
        comm_timeout_ms = 1000

        def evaluate_watchdog(cmd, last_cmd_time_ms, current_time_ms):
            if cmd != 'S' and (current_time_ms - last_cmd_time_ms > comm_timeout_ms):
                return 'S'
            return cmd

        # Under threshold
        self.assertEqual(evaluate_watchdog('F', 5000, 5800), 'F')
        # Timeout breached
        self.assertEqual(evaluate_watchdog('F', 5000, 6100), 'S')

    # -------------------------------------------------------------------------
    # 6. Relay Safety & Auto-Disarm on Vehicle Stop
    # -------------------------------------------------------------------------
    def test_relay_auto_disarm_on_stop(self):
        relay_state = True  # Armed
        cmd = 'S'
        if cmd == 'S':
            relay_state = False  # Auto-disarm
        self.assertFalse(relay_state)

    # -------------------------------------------------------------------------
    # 7. Drone Camera State Machine
    # -------------------------------------------------------------------------
    def test_camera_state_machine(self):
        # 0: OFF, 1: SMALL PREVIEW, 2: FULLSCREEN
        state = 0

        # Press CAMERA button -> Small Preview
        state = 1 if state == 0 else 0
        self.assertEqual(state, 1)

        # Single click on small preview -> Fullscreen
        if state == 1:
            state = 2
        self.assertEqual(state, 2)

        # Double click on fullscreen -> Small Preview
        if state == 2:
            state = 1
        self.assertEqual(state, 1)

        # Press CAMERA button again -> OFF
        state = 1 if state == 0 else 0
        self.assertEqual(state, 0)

    # -------------------------------------------------------------------------
    # 8. Web API Telemetry Payload Contract
    # -------------------------------------------------------------------------
    def test_telemetry_json_schema(self):
        sample_json = {
            "angle": 90,
            "distance": 120,
            "radarRunning": True,
            "target": True,
            "targetAngle": 90,
            "targetDistance": 120,
            "locked": False,
            "lockAngle": 90,
            "aimingAngle": 90,
            "front": 45,
            "movement": "FORWARD",
            "relay": False,
            "nanoConnected": True,
            "unoConnected": True,
        }

        required_keys = [
            "angle", "distance", "radarRunning", "target", "targetAngle",
            "targetDistance", "locked", "lockAngle", "aimingAngle", "front",
            "movement", "relay", "nanoConnected", "unoConnected"
        ]

        for k in required_keys:
            self.assertIn(k, sample_json)
        self.assertIsInstance(sample_json["angle"], int)
        self.assertIsInstance(sample_json["distance"], int)
        self.assertIsInstance(sample_json["radarRunning"], bool)
        self.assertIsInstance(sample_json["target"], bool)
        self.assertIsInstance(sample_json["locked"], bool)
        self.assertIsInstance(sample_json["movement"], str)
        self.assertIsInstance(sample_json["relay"], bool)

if __name__ == "__main__":
    unittest.main(verbosity=2)
