"""
TRINETRA Target Detection & Coordinate Processing
"""

import math
from config import MAX_RADAR_DISTANCE, TARGET_THRESHOLD

class TargetDetector:
    def __init__(self, center_x, center_y, radius):
        self.cx = center_x
        self.cy = center_y
        self.radius = radius
        self.max_distance = MAX_RADAR_DISTANCE
        self.target_threshold = TARGET_THRESHOLD

    def update_geometry(self, center_x, center_y, radius):
        self.cx = center_x
        self.cy = center_y
        self.radius = radius

    def polar_to_screen(self, angle_deg, distance_cm):
        """
        Converts polar radar coordinates (0° to 180°, 0 to 250 cm)
        into screen pixel coordinates on the radar semi-circle.
        Angle 0° is to the right, 90° is dead ahead, 180° is to the left.
        """
        clamped_dist = max(0, min(distance_cm, self.max_distance))
        r = (clamped_dist / self.max_distance) * self.radius

        # Convert to radians with 90° as top center
        rad = math.radians(180 - angle_deg)
        x = self.cx + r * math.cos(rad)
        y = self.cy - r * math.sin(rad)
        return int(x), int(y)

    def is_target_valid(self, distance_cm):
        """Checks if distance satisfies detection threshold (2 cm to 150 cm)."""
        return 2 <= distance_cm <= self.target_threshold
