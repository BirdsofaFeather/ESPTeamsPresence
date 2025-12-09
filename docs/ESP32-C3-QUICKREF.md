# ESP32-C3 Quick Reference

## Quick Start

### Build Commands
```bash
# Build for ESP32-C3
pio run -e esp32-c3-devkitm-1

# Upload to ESP32-C3
pio run -e esp32-c3-devkitm-1 -t upload

# Monitor serial output
pio device monitor -e esp32-c3-devkitm-1
```

### Default Configuration
- **Board**: ESP32-C3-DevKitM-1
- **Data Pin**: GPIO 13
- **LED Count**: 16
- **Baud Rate**: 115200

## Wiring

```
ESP32-C3      WS2812B LEDs
--------      ------------
GPIO 13  -->  DIN
   5V    -->  VCC
  GND    -->  GND
```

## Common GPIO Pins

| Purpose | Recommended GPIO |
|---------|------------------|
| LEDs    | 2, 3, 4, 5, 10   |
| Buttons | 6, 7, 18, 19     |
| I2C SDA | 8                |
| I2C SCL | 9                |

**Avoid**: GPIO 11-17 (flash), GPIO 2/8/9 (strapping)

## Custom Build Flags

Edit `platformio.ini`:
```ini
[env:esp32-c3-devkitm-1]
build_flags=
    ${env.build_flags}
    -DESP32C3
    -DDATAPIN=10      # Change LED data pin
    -DNUMLEDS=24      # Change LED count
```

## Troubleshooting

### Won't Upload
1. Hold BOOT button during power-on
2. Release after device shows in port list
3. Try different USB cable/port

### LED Not Working
- Check GPIO pin (not 11-17)
- Verify 5V power supply
- Check LED strip data direction
- Test with fewer LEDs first

### WiFi Issues
- Ensure correct WiFi credentials
- Check antenna connection
- Move away from interference
- Use 2.4GHz network (not 5GHz)

### Memory Issues
- ESP32-C3 has 400KB RAM
- Reduce polling interval if needed
- Decrease LED count if memory errors occur

## Comparison: ESP32 vs ESP32-C3

| Feature        | ESP32          | ESP32-C3       |
|----------------|----------------|----------------|
| Architecture   | Xtensa         | RISC-V         |
| Cores          | 2              | 1              |
| Clock          | 240 MHz        | 160 MHz        |
| RAM            | 320 KB         | 400 KB         |
| Flash          | 4 MB           | 4 MB           |
| WiFi           | 2.4 GHz        | 2.4 GHz        |
| Bluetooth      | Classic + BLE  | BLE 5.0        |
| GPIO           | 34             | 22             |
| Price          | ~$5            | ~$2-3          |

## Performance Notes

### ESP32-C3 is sufficient for:
✅ WiFi connectivity  
✅ LED animations  
✅ Web server  
✅ API polling  
✅ Token management

### ESP32-C3 limitations:
⚠️ Single core (no core pinning)  
⚠️ Fewer GPIO pins  
⚠️ Slightly slower clock

## LED Effects Available

All standard effects work on ESP32-C3:
- Static colors (Available, Busy, etc.)
- Breathing animations (InACall)
- Scanning (InAMeeting)
- Color wipes (Presenting)

## Power Consumption

Typical consumption:
- **Idle**: ~80mA @ 5V
- **LEDs On (16x)**: ~300-960mA @ 5V (depends on brightness)
- **WiFi Active**: +100-150mA

**Tip**: Use external 5V power supply for >8 LEDs

## Web Interface

Access at: `http://espteamspresence.local` or device IP

Features:
- Configure WiFi
- Set Client ID and Tenant
- Start device login
- View memory usage (shows 400KB for ESP32-C3)
- Adjust polling interval
- Set LED count

## Default Credentials

### AP Mode
- **SSID**: ESPTeamsPresence
- **Password**: presence

Appears when:
- First boot
- WiFi not configured
- Can't connect to WiFi

## Status Indicators (LED Colors)

| Color           | Meaning              |
|-----------------|----------------------|
| White Theater   | AP Mode              |
| Blue Theater    | Connecting WiFi      |
| Green Theater   | WiFi Connected       |
| Purple Theater  | Device Login Started |
| Red Theater     | Token Refresh        |
| Presence Colors | Normal Operation     |

## Serial Monitor Commands

```bash
# View debug output
pio device monitor

# Specific baud rate
pio device monitor -b 115200

# Exit monitor
Ctrl+C
```

## Clean Build

If build fails:
```bash
pio run -e esp32-c3-devkitm-1 -t clean
pio pkg update
pio run -e esp32-c3-devkitm-1
```

## Documentation Links

- [Full ESP32-C3 Guide](ESP32-C3.md)
- [Changes Summary](ESP32-C3-CHANGES.md)
- [Main Project README](../README.md)

## Support

- This is a community port for ESP32-C3
- Original project: [toblum/ESPTeamsPresence](https://github.com/toblum/ESPTeamsPresence)
- Issues: Use GitHub Issues

---

**Last Updated**: 2025-12-08  
**Tested**: ESP32-C3-DevKitM-1, Arduino framework 2.0+
