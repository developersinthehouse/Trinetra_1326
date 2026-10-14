"""
TRINETRA Desktop Tactical Radar HUD
Standalone Pygame visualization and target tracking application for laptops.
"""

import sys
import math
import time
import pygame
from config import (
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    FPS,
    COLOR_BG,
    COLOR_PANEL_BG,
    COLOR_BORDER,
    COLOR_GRID,
    COLOR_GRID_TEXT,
    COLOR_BEAM,
    COLOR_TRAIL,
    COLOR_TARGET,
    COLOR_TEXT_TITLE,
    COLOR_TEXT_VAL,
    COLOR_TEXT_MUTED,
    COLOR_WARN,
    COLOR_DANGER,
    MAX_RADAR_DISTANCE,
)
from network import NetworkClient
from detector import TargetDetector

def main():
    pygame.init()
    pygame.display.set_caption("TRINETRA // TACTICAL RADAR HUD")
    screen = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
    clock = pygame.time.Clock()

    font_title = pygame.font.SysFont("consolas", 20, bold=True)
    font_bold = pygame.font.SysFont("consolas", 14, bold=True)
    font_normal = pygame.font.SysFont("consolas", 12)
    font_small = pygame.font.SysFont("consolas", 11)

    # Initialize network client
    client = NetworkClient()
    client.start()

    # Radar geometry
    radar_cx = 340
    radar_cy = 480
    radar_radius = 290
    detector = TargetDetector(radar_cx, radar_cy, radar_radius)

    # Sweep trail persistence buffer
    trail_surface = pygame.Surface((WINDOW_WIDTH, WINDOW_HEIGHT), pygame.SRCALPHA)

    running = True
    pulse_phase = 0.0

    while running:
        dt = clock.tick(FPS) / 1000.0
        pulse_phase = (pulse_phase + dt * 4.0) % (2.0 * math.pi)

        # Handle keyboard & window events
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                if event.key in (pygame.K_ESCAPE, pygame.K_q):
                    running = False
                elif event.key == pygame.K_l:
                    client.send_lock()
                elif event.key == pygame.K_u:
                    client.send_unlock()
                elif event.key == pygame.K_s:
                    data, _ = client.get_telemetry()
                    if data.get("radarRunning", True):
                        client.send_radar_stop()
                    else:
                        client.send_radar_start()

        # Fetch telemetry snapshot
        data, is_connected = client.get_telemetry()
        angle = data.get("angle", 90)
        distance = data.get("distance", 249)
        is_locked = data.get("locked", False)
        lock_angle = data.get("lockAngle", 90)
        is_target = data.get("target", False)
        target_angle = data.get("targetAngle", 90)
        target_distance = data.get("targetDistance", 0)
        aiming_angle = data.get("aimingAngle", 90)
        front_dist = data.get("front", 999)
        movement = data.get("movement", "STOP")
        relay_state = data.get("relay", False)
        radar_running = data.get("radarRunning", True)

        # -------------------------------------------------
        # Render Background
        # -------------------------------------------------
        screen.fill(COLOR_BG)

        # Slowly fade trail surface for phosphor persistence
        trail_surface.fill((0, 0, 0, 18), special_flags=pygame.BLEND_RGBA_SUBTRACT)

        # Draw Header Bar
        pygame.draw.rect(screen, (0, 30, 15), (0, 0, WINDOW_WIDTH, 48))
        pygame.draw.line(screen, COLOR_BEAM, (0, 48), (WINDOW_WIDTH, 48), 1)

        title_surf = font_title.render("TRINETRA // TACTICAL RADAR HUD", True, COLOR_TEXT_TITLE)
        screen.blit(title_surf, (20, 12))

        # Header Badges
        comms_color = COLOR_BEAM if is_connected else COLOR_DANGER
        comms_text = "COMMS: ONLINE" if is_connected else "COMMS: OFFLINE"
        comms_surf = font_bold.render(comms_text, True, comms_color)
        screen.blit(comms_surf, (WINDOW_WIDTH - 380, 16))

        radar_text = "RADAR: ACTIVE" if radar_running else "RADAR: STOPPED"
        radar_color = COLOR_BEAM if radar_running else COLOR_WARN
        r_surf = font_bold.render(radar_text, True, radar_color)
        screen.blit(r_surf, (WINDOW_WIDTH - 200, 16))

        # -------------------------------------------------
        # Render Radar Display
        # -------------------------------------------------
        radar_box = pygame.Rect(16, 60, 650, 620)
        pygame.draw.rect(screen, COLOR_PANEL_BG, radar_box, border_radius=6)
        pygame.draw.rect(screen, COLOR_GRID, radar_box, 1, border_radius=6)

        # Outer Radar Arc
        pygame.draw.arc(screen, COLOR_GRID, (radar_cx - radar_radius, radar_cy - radar_radius, radar_radius * 2, radar_radius * 2), 0, math.pi, 2)
        pygame.draw.line(screen, COLOR_GRID, (radar_cx - radar_radius, radar_cy), (radar_cx + radar_radius, radar_cy), 2)

        # Range Rings (50cm, 100cm, 150cm, 200cm, 250cm)
        ranges = [50, 100, 150, 200, 250]
        for r_cm in ranges:
            r_px = int((r_cm / MAX_RADAR_DISTANCE) * radar_radius)
            ring_rect = pygame.Rect(radar_cx - r_px, radar_cy - r_px, r_px * 2, r_px * 2)
            pygame.draw.arc(screen, (0, 120, 60), ring_rect, 0, math.pi, 1)

            # Range label
            lbl = font_small.render(f"{r_cm}cm", True, COLOR_GRID_TEXT)
            screen.blit(lbl, (radar_cx + 8, radar_cy - r_px - 8))

        # Radial Degree Lines (every 30°)
        for deg in range(0, 181, 30):
            rad = math.radians(180 - deg)
            x_end = radar_cx + radar_radius * math.cos(rad)
            y_end = radar_cy - radar_radius * math.sin(rad)
            pygame.draw.line(screen, (0, 80, 40), (radar_cx, radar_cy), (int(x_end), int(y_end)), 1)

            tx = radar_cx + (radar_radius + 18) * math.cos(rad)
            ty = radar_cy - (radar_radius + 18) * math.sin(rad)
            lbl = font_small.render(f"{deg}°", True, COLOR_GRID_TEXT)
            screen.blit(lbl, (int(tx) - 10, int(ty) - 8))

        # Draw Trail Beam
        beam_rad = math.radians(180 - angle)
        beam_x = radar_cx + radar_radius * math.cos(beam_rad)
        beam_y = radar_cy - radar_radius * math.sin(beam_rad)
        pygame.draw.line(trail_surface, (0, 255, 136, 160), (radar_cx, radar_cy), (int(beam_x), int(beam_y)), 2)

        screen.blit(trail_surface, (0, 0))

        # Primary Active Sweeper Line
        pygame.draw.line(screen, COLOR_BEAM, (radar_cx, radar_cy), (int(beam_x), int(beam_y)), 2)

        # -------------------------------------------------
        # Render Target Blip
        # -------------------------------------------------
        active_target_angle = lock_angle if is_locked else target_angle
        active_target_dist = distance if is_locked else target_distance

        if (is_locked or (is_target and distance <= 150)) and active_target_dist <= 250:
            tx, ty = detector.polar_to_screen(active_target_angle, active_target_dist)

            pulse_radius = 12 + int(math.sin(pulse_phase) * 4)
            pygame.draw.circle(screen, COLOR_TARGET, (tx, ty), 6)
            pygame.draw.circle(screen, COLOR_TARGET, (tx, ty), pulse_radius, 1)

            # Target Crosshair
            pygame.draw.line(screen, COLOR_TARGET, (tx - 16, ty), (tx + 16, ty), 1)
            pygame.draw.line(screen, COLOR_TARGET, (tx, ty - 16), (tx, ty + 16), 1)

            t_text = font_small.render(f"TGT [{active_target_angle}°, {active_target_dist}cm]", True, COLOR_TARGET)
            screen.blit(t_text, (tx + 12, ty - 16))

        # -------------------------------------------------
        # Render Telemetry Sidebar
        # -------------------------------------------------
        panel_x = 680
        panel_w = 250

        # Card 1: Target Acquisition
        card1 = pygame.Rect(panel_x, 60, panel_w, 140)
        pygame.draw.rect(screen, COLOR_PANEL_BG, card1, border_radius=6)
        pygame.draw.rect(screen, COLOR_BORDER, card1, 1, border_radius=6)

        c1_title = font_bold.render("TARGET ACQUISITION", True, COLOR_TEXT_TITLE)
        screen.blit(c1_title, (panel_x + 10, 70))

        if is_locked:
            b_text = f"LOCKED @ {lock_angle}°"
            b_col = COLOR_DANGER
        elif is_target:
            b_text = f"ACQUIRED @ {target_angle}°"
            b_col = COLOR_WARN
        else:
            b_text = "SCANNING SECTOR"
            b_col = COLOR_BEAM

        banner_rect = pygame.Rect(panel_x + 10, 95, panel_w - 20, 28)
        pygame.draw.rect(screen, (20, 20, 20), banner_rect, border_radius=4)
        pygame.draw.rect(screen, b_col, banner_rect, 1, border_radius=4)
        b_surf = font_bold.render(b_text, True, b_col)
        screen.blit(b_surf, (panel_x + 22, 102))

        screen.blit(font_normal.render("TARGET AZ:", True, COLOR_TEXT_MUTED), (panel_x + 10, 135))
        screen.blit(font_bold.render(f"{active_target_angle}°" if (is_target or is_locked) else "--", True, COLOR_TEXT_VAL), (panel_x + 130, 135))

        screen.blit(font_normal.render("TARGET RNG:", True, COLOR_TEXT_MUTED), (panel_x + 10, 155))
        screen.blit(font_bold.render(f"{active_target_dist} cm" if (is_target or is_locked) else "--", True, COLOR_TEXT_VAL), (panel_x + 130, 155))

        screen.blit(font_normal.render("LOCK THRESH:", True, COLOR_TEXT_MUTED), (panel_x + 10, 175))
        screen.blit(font_bold.render("≤ 150 cm", True, COLOR_TEXT_VAL), (panel_x + 130, 175))

        # Card 2: Beam Telemetry
        card2 = pygame.Rect(panel_x, 215, panel_w, 130)
        pygame.draw.rect(screen, COLOR_PANEL_BG, card2, border_radius=6)
        pygame.draw.rect(screen, COLOR_BORDER, card2, 1, border_radius=6)

        c2_title = font_bold.render("BEAM TELEMETRY", True, COLOR_TEXT_TITLE)
        screen.blit(c2_title, (panel_x + 10, 225))

        screen.blit(font_normal.render("RADAR AZIMUTH:", True, COLOR_TEXT_MUTED), (panel_x + 10, 252))
        screen.blit(font_bold.render(f"{angle}°", True, COLOR_TEXT_VAL), (panel_x + 140, 252))

        screen.blit(font_normal.render("MEASURED RNG:", True, COLOR_TEXT_MUTED), (panel_x + 10, 275))
        dist_str = f"{distance} cm" if distance <= 249 else "--"
        screen.blit(font_bold.render(dist_str, True, COLOR_TEXT_VAL), (panel_x + 140, 275))

        screen.blit(font_normal.render("AIMING SERVO:", True, COLOR_TEXT_MUTED), (panel_x + 10, 298))
        screen.blit(font_bold.render(f"{aiming_angle}°", True, COLOR_TEXT_VAL), (panel_x + 140, 298))

        screen.blit(font_normal.render("SWEEP MODE:", True, COLOR_TEXT_MUTED), (panel_x + 10, 321))
        mode_str = "LOCKED" if is_locked else ("SWEEPING" if radar_running else "STOPPED")
        screen.blit(font_bold.render(mode_str, True, COLOR_WARN if is_locked else COLOR_BEAM), (panel_x + 140, 321))

        # Card 3: Vehicle Diagnostics
        card3 = pygame.Rect(panel_x, 360, panel_w, 150)
        pygame.draw.rect(screen, COLOR_PANEL_BG, card3, border_radius=6)
        pygame.draw.rect(screen, COLOR_BORDER, card3, 1, border_radius=6)

        c3_title = font_bold.render("VEHICLE DIAGNOSTICS", True, COLOR_TEXT_TITLE)
        screen.blit(c3_title, (panel_x + 10, 370))

        front_str = f"{front_dist} cm" if front_dist < 999 else "--"
        front_col = COLOR_DANGER if (front_dist <= 10) else COLOR_TEXT_VAL
        screen.blit(font_normal.render("FRONT COLLISION:", True, COLOR_TEXT_MUTED), (panel_x + 10, 397))
        screen.blit(font_bold.render(front_str, True, front_col), (panel_x + 140, 397))

        screen.blit(font_normal.render("DRIVE VECTOR:", True, COLOR_TEXT_MUTED), (panel_x + 10, 420))
        screen.blit(font_bold.render(movement, True, COLOR_TEXT_VAL), (panel_x + 140, 420))

        screen.blit(font_normal.render("PAYLOAD RELAY:", True, COLOR_TEXT_MUTED), (panel_x + 10, 443))
        relay_str = "TRIGGERED" if relay_state else "SAFE / OFF"
        relay_col = COLOR_WARN if relay_state else COLOR_TEXT_VAL
        screen.blit(font_bold.render(relay_str, True, relay_col), (panel_x + 140, 443))

        screen.blit(font_normal.render("NANO STATUS:", True, COLOR_TEXT_MUTED), (panel_x + 10, 466))
        screen.blit(font_bold.render("OK" if data.get("nanoConnected", False) else "LOST", True, COLOR_BEAM if data.get("nanoConnected", False) else COLOR_DANGER), (panel_x + 140, 466))

        screen.blit(font_normal.render("UNO STATUS:", True, COLOR_TEXT_MUTED), (panel_x + 10, 489))
        screen.blit(font_bold.render("OK" if data.get("unoConnected", False) else "LOST", True, COLOR_BEAM if data.get("unoConnected", False) else COLOR_DANGER), (panel_x + 140, 489))

        # Card 4: Keyboard Hotkeys Guide
        card4 = pygame.Rect(panel_x, 525, panel_w, 155)
        pygame.draw.rect(screen, COLOR_PANEL_BG, card4, border_radius=6)
        pygame.draw.rect(screen, COLOR_BORDER, card4, 1, border_radius=6)

        c4_title = font_bold.render("TACTICAL HOTKEYS", True, COLOR_TEXT_TITLE)
        screen.blit(c4_title, (panel_x + 10, 535))

        screen.blit(font_small.render("[L]      : LOCK TARGET HEADING", True, COLOR_TEXT_VAL), (panel_x + 10, 560))
        screen.blit(font_small.render("[U]      : UNLOCK / RESUME SWEEP", True, COLOR_TEXT_VAL), (panel_x + 10, 582))
        screen.blit(font_small.render("[S]      : TOGGLE RADAR START/STOP", True, COLOR_TEXT_VAL), (panel_x + 10, 604))
        screen.blit(font_small.render("[Q/ESC]  : QUIT RADAR HUD", True, COLOR_TEXT_VAL), (panel_x + 10, 626))
        screen.blit(font_small.render("AUTO-FAILSAFE COMM WATCHDOG ACTIVE", True, COLOR_TEXT_MUTED), (panel_x + 10, 650))

        pygame.display.flip()

    client.stop()
    pygame.quit()
    sys.exit(0)

if __name__ == "__main__":
    main()
