# ESP8266 Pin Reference for ESPTeamsPresence

## NodeMCU Pin Mapping

The ESP8266 uses GPIO pin numbers, but the NodeMCU board labels them differently:

| NodeMCU Label | GPIO Number | Default Use in ESPTeamsPresence |
|--------------|-------------|--------------------------------|
| D0           | GPIO 16     | -                              |
| D1           | GPIO 5      | -                              |
| D2           | GPIO 4      | -                              |
| D3           | GPIO 0      | -                              |
| **D4**       | **GPIO 2**  | **LED Data Pin (Default)**     |
| D5           | GPIO 14     | -                              |
| D6           | GPIO 12     | -                              |
| D7           | GPIO 13     | -                              |
| D8           | GPIO 15     | -                              |

## Wiring Diagram (ASCII)

```
ESP8266 NodeMCU          LED Strip (WS2812B/NeoPixel)
┌──────────────┐         ┌────────────┐
│              │         │            │
│   D4 (GPIO2) ├────────►│ DIN        │
│              │         │            │
│     GND      ├────────►│ GND        │
│              │         │            │
│              │         │ +5V        │
└──────────────┘         └─────┬──────┘
                               │
                               │
                         ┌─────▼──────┐
                         │ 5V Power   │
                         │ Supply     │
                         │ (2-5A)     │
                         └────────────┘
```

## Important Notes

### Power Requirements
- **Small setups (1-10 LEDs)**: Can power from ESP8266's 3.3V pin, but 5V is recommended
- **Medium setups (11-50 LEDs)**: Use external 5V power supply
- **Large setups (50+ LEDs)**: MUST use external 5V power supply (2-5A recommended)

### Grounding
- **Always connect GND** between ESP8266 and LED strip
- Connect GND of external power supply to ESP8266 GND

### Signal Level
- WS2812B LEDs expect 5V data signal
- ESP8266 outputs 3.3V
- Usually works fine for short distances (<1 meter)
- For longer runs, consider using a level shifter (3.3V → 5V)

### Data Pin Selection
The default is GPIO 2 (D4), but you can use almost any GPIO pin:

**Good choices:**
- GPIO 2 (D4) - Default, has built-in LED on NodeMCU
- GPIO 4 (D2)
- GPIO 5 (D1)
- GPIO 12 (D6)
- GPIO 13 (D7)
- GPIO 14 (D5)

**Avoid:**
- GPIO 0 (D3) - Used for boot mode
- GPIO 15 (D8) - Must be LOW at boot
- GPIO 16 (D0) - Different behavior, limited PWM
- GPIO 1 (TX) - Used for serial communication
- GPIO 3 (RX) - Used for serial communication

## Changing the Data Pin

### Via Build Flags (platformio.ini)
Edit the `platformio.ini` file:
```ini
build_flags=
    -DDATAPIN=4    ; Change to desired GPIO number
    -DNUMLEDS=16
```

### Examples

**Use GPIO 4 (D2):**
```ini
build_flags=
    -DDATAPIN=4
    -DNUMLEDS=16
```

**Use GPIO 13 (D7):**
```ini
build_flags=
    -DDATAPIN=13
    -DNUMLEDS=16
```

## Testing LEDs

After uploading the firmware, you should see:
1. **White theater chase** - When in AP mode (not configured)
2. **Blue theater chase** - When connecting to WiFi
3. **Green theater chase** - When WiFi connected successfully

If you don't see any lights:
1. Check wiring (especially data pin and GND)
2. Verify power supply to LEDs
3. Check that LED strip is WS2812B compatible
4. Try a different GPIO pin

## Capacitor Recommendation

To prevent power spikes when turning on LEDs:
- Add a **1000µF capacitor** between +5V and GND near the LED strip
- Add a **100-500Ω resistor** between ESP8266 data pin and LED strip DIN

```
ESP8266 D4 ──[470Ω]── LED Strip DIN

+5V ──┬── LED Strip +5V
      │
      ├── [1000µF Cap] ── GND
      │
GND ──┴── LED Strip GND
```

## Typical Setup Example

**Hardware:**
- 1x NodeMCU ESP8266
- 1x WS2812B LED Ring (16 LEDs)
- 1x 5V 2A power supply
- 1x 470Ω resistor
- 1x 1000µF capacitor
- Jumper wires

**Connections:**
1. NodeMCU D4 → 470Ω resistor → LED DIN
2. NodeMCU GND → LED GND
3. Power supply +5V → LED +5V
4. Power supply GND → LED GND AND NodeMCU GND
5. 1000µF capacitor between +5V and GND near LEDs

**Software:**
- Use default settings (GPIO 2, 16 LEDs)
- Or modify platformio.ini if using different pin/LED count
