"""
TRINETRA Python Radar Network Client
Background polling thread communicating with the ESP32 HTTP API.
"""

import threading
import time
import requests
from config import (
    DATA_ENDPOINT,
    LOCK_ENDPOINT,
    UNLOCK_ENDPOINT,
    START_RADAR_ENDPOINT,
    STOP_RADAR_ENDPOINT,
    POLL_INTERVAL_SEC,
)

class NetworkClient:
    def __init__(self):
        self._running = False
        self._thread = None
        self._lock = threading.Lock()
        self._connected = False
        self._last_data = {
            "angle": 90,
            "distance": 249,
            "radarRunning": True,
            "target": False,
            "targetAngle": 90,
            "targetDistance": 249,
            "locked": False,
            "lockAngle": 90,
            "aimingAngle": 90,
            "front": 999,
            "movement": "STOP",
            "relay": False,
            "nanoConnected": False,
            "unoConnected": False,
        }

    def start(self):
        """Starts the background telemetry polling thread."""
        if self._running:
            return
        self._running = True
        self._thread = threading.Thread(target=self._poll_loop, daemon=True)
        self._thread.start()

    def stop(self):
        """Stops the background telemetry thread."""
        self._running = False
        if self._thread:
            self._thread.join(timeout=1.0)

    def _poll_loop(self):
        session = requests.Session()
        session.trust_env = False

        while self._running:
            try:
                resp = session.get(DATA_ENDPOINT, timeout=0.8)
                if resp.status_code == 200:
                    data = resp.json()
                    with self._lock:
                        self._last_data.update(data)
                        self._connected = True
                else:
                    with self._lock:
                        self._connected = False
            except Exception:
                with self._lock:
                    self._connected = False

            time.sleep(POLL_INTERVAL_SEC)

    def get_telemetry(self):
        """Returns a snapshot of the latest telemetry and connection status."""
        with self._lock:
            return dict(self._last_data), self._connected

    def send_lock(self):
        """Sends target lock request to ESP32."""
        try:
            requests.get(LOCK_ENDPOINT, timeout=0.8)
        except Exception:
            pass

    def send_unlock(self):
        """Sends unlock request to ESP32."""
        try:
            requests.get(UNLOCK_ENDPOINT, timeout=0.8)
        except Exception:
            pass

    def send_radar_start(self):
        """Sends radar start command."""
        try:
            requests.get(START_RADAR_ENDPOINT, timeout=0.8)
        except Exception:
            pass

    def send_radar_stop(self):
        """Sends radar stop command."""
        try:
            requests.get(STOP_RADAR_ENDPOINT, timeout=0.8)
        except Exception:
            pass
