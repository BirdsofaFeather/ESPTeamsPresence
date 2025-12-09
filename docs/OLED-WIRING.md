# OLED Display Wiring Diagram

## ESP32 Standard (e.g., ESP32 DevKit, ESP32 DOIT)

```
┌─────────────────────┐              ┌──────────────┐
│      ESP32          │              │ SSD1306 OLED │
│                     │              │  (128x64)    │
│                     │              │              │
│       3.3V   ●──────┼──────────────┼──● VCC       │
│                     │              │              │
│       GND    ●──────┼──────────────┼──● GND       │
│                     │              │              │
│       GPIO21 ●──────┼──────────────┼──● SDA       │
│       (SDA)         │              │              │
│                     │              │              │
│       GPIO22 ●──────┼──────────────┼──● SCL       │
│       (SCL)         │              │              │
│                     │              │              │
└─────────────────────┘              └──────────────┘

```

## ESP32-C3 (e.g., ESP32-C3 DevKitM-1)

```
┌─────────────────────┐              ┌──────────────┐
│     ESP32-C3        │              │ SSD1306 OLED │
│                     │              │  (128x64)    │
│                     │              │              │
│       3.3V   ●──────┼──────────────┼──● VCC       │
│                     │              │              │
│       GND    ●──────┼──────────────┼──● GND       │
│                     │              │              │
│       GPIO8  ●──────┼──────────────┼──● SDA       │
│       (SDA)         │              │              │
│                     │              │              │
│       GPIO9  ●──────┼──────────────┼──● SCL       │
│       (SCL)         │              │              │
│                     │              │              │
└─────────────────────┘              └──────────────┘

```

## Notes

1. **Power Supply**: Most SSD1306 OLED modules can work with both 3.3V and 5V. Check your module's specifications.
   - If your module has a voltage regulator onboard, you can connect VCC to either 3.3V or 5V
   - If in doubt, use 3.3V to be safe

2. **I2C Pull-up Resistors**: Most OLED modules have built-in pull-up resistors (typically 10kΩ). You usually don't need external pull-ups.

3. **I2C Address**: The default I2C address is `0x3C`. Some modules use `0x3D`. If the display doesn't work:
   - Use an I2C scanner sketch to detect the address
   - Update `SCREEN_ADDRESS` in `main.cpp` if needed

4. **Wire Length**: Keep I2C wires as short as possible (ideally under 20cm) to avoid signal integrity issues.

## Fritzing-Style Connection Table

### ESP32 Standard
| ESP32 Pin | OLED Pin | Description |
|-----------|----------|-------------|
| 3.3V      | VCC      | Power (3.3V or 5V depending on module) |
| GND       | GND      | Ground |
| GPIO21    | SDA      | I2C Data Line |
| GPIO22    | SCL      | I2C Clock Line |

### ESP32-C3
| ESP32-C3 Pin | OLED Pin | Description |
|--------------|----------|-------------|
| 3.3V         | VCC      | Power (3.3V or 5V depending on module) |
| GND          | GND      | Ground |
| GPIO8        | SDA      | I2C Data Line |
| GPIO9        | SCL      | I2C Clock Line |

## Breadboard Setup Example

If using a breadboard, the typical layout would be:

```
                    OLED Display (Top View)
                    ┌──────────────┐
                    │   ▄▄▄▄▄▄▄▄   │
                    │   █      █   │
                    │   █ 128  █   │
                    │   █  x   █   │
                    │   █  64  █   │
                    │   █      █   │
                    │   ▀▀▀▀▀▀▀▀   │
                    └──────────────┘
                     │  │  │  │
                     │  │  │  └──── SCL (Yellow wire)
                     │  │  └─────── SDA (Green wire)
                     │  └────────── GND (Black wire)
                     └───────────── VCC (Red wire)
                            │
                     [Breadboard]
                            │
                        ESP32/C3
```

## Common OLED Module Types

Most compatible OLED displays:
- 0.96" 128x64 I2C OLED (most common)
- 1.3" 128x64 I2C OLED (larger display)

Both typically use the SSD1306 driver chip and work with this code.

## Troubleshooting Wiring

If the display doesn't work:

1. ✅ Check all 4 connections are secure
2. ✅ Verify correct GPIO pins (21/22 for ESP32, 8/9 for ESP32-C3)
3. ✅ Ensure proper power connection (3.3V recommended)
4. ✅ Try swapping SDA/SCL if you mixed them up
5. ✅ Check serial monitor for "OLED Display initialized" message
6. ✅ Use multimeter to verify voltage on OLED VCC pin
