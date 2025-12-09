# Migrating from ESP32 to ESP32-C3

This guide helps you migrate your ESPTeamsPresence setup from a standard ESP32 to an ESP32-C3 board.

## Why Migrate?

### Advantages of ESP32-C3
- **Lower Cost**: ~$2-3 vs ~$5 for ESP32
- **Modern Architecture**: RISC-V instead of proprietary Xtensa
- **More RAM**: 400KB vs 320KB
- **Better BLE**: BLE 5.0 support
- **USB Serial**: Native USB support on some boards

### Considerations
- **Single Core**: May affect very performance-critical applications (not an issue for this project)
- **Fewer GPIO**: 22 vs 34 pins
- **Less Bluetooth**: No Bluetooth Classic (BLE only)

## Hardware Migration

### What You Need
1. **ESP32-C3 Board** (e.g., ESP32-C3-DevKitM-1)
2. **Same WS2812B LED strip** (no change needed)
3. **Same 5V power supply** (no change needed)
4. **USB-C or Micro-USB cable** (depends on board)

### Wiring Changes

The wiring is **identical** if you use the default GPIO 13:

```
ESP32-C3      WS2812B LEDs
--------      ------------
GPIO 13  -->  DIN
   5V    -->  VCC
  GND    -->  GND
```

#### If You Used Different GPIO on ESP32

ESP32 GPIO → Suggested ESP32-C3 GPIO:
```
GPIO 26  →  GPIO 10
GPIO 27  →  GPIO 4
GPIO 32  →  GPIO 5
GPIO 33  →  GPIO 6
```

**Note**: ESP32-C3 doesn't have GPIOs above 21.

## Software Migration

### Step 1: Update platformio.ini

#### Before (ESP32):
```ini
[platformio]
default_envs = esp32doit-devkit-v1
```

#### After (ESP32-C3):
```ini
[platformio]
default_envs = esp32-c3-devkitm-1
```

### Step 2: Rebuild

```bash
# Clean previous build
pio run -t clean

# Build for ESP32-C3
pio run -e esp32-c3-devkitm-1
```

### Step 3: Upload

```bash
# Upload firmware
pio run -e esp32-c3-devkitm-1 -t upload
```

**Tip**: If upload fails, hold BOOT button during power-on.

## Configuration Migration

### Automatic Configuration Transfer

Your existing configuration **will NOT transfer** automatically. You need to:

1. **Note your current settings:**
   - Client ID
   - Tenant ID
   - Polling interval
   - Number of LEDs

2. **On first boot**, the ESP32-C3 will create its own AP:
   - SSID: `ESPTeamsPresence`
   - Password: `presence`

3. **Connect and configure:**
   - Join the AP with your phone/computer
   - Navigate to `http://192.168.4.1`
   - Enter your WiFi credentials
   - Enter your Client ID and Tenant ID
   - Set polling interval and LED count

4. **Re-authenticate:**
   - Start device login flow
   - Enter the code at microsoft.com/devicelogin
   - Your presence will start syncing

### Manual Configuration Backup (Advanced)

If you want to backup settings from ESP32:

1. **Before removing ESP32:**
   ```bash
   # Read SPIFFS
   pio run -e esp32doit-devkit-v1 -t uploadfs
   ```

2. **Save these values:**
   - In web UI: note Client ID, Tenant, etc.
   - Or use `/api/settings` endpoint to get JSON

3. **After ESP32-C3 is running:**
   - Enter values manually via web UI

## GPIO Pin Mapping

If you used custom GPIO pins, update `platformio.ini`:

### Before (ESP32):
```ini
build_flags=
    -DDATAPIN=26
    -DNUMLEDS=16
```

### After (ESP32-C3):
```ini
[env:esp32-c3-devkitm-1]
build_flags=
    ${env.build_flags}
    -DESP32C3
    -DDATAPIN=10
    -DNUMLEDS=16
```

### Safe GPIO Pins on ESP32-C3

✅ **Safe for General Use**:
- GPIO 0, 1, 3, 4, 5, 6, 7, 10
- GPIO 18, 19, 20, 21

⚠️ **Use with Caution**:
- GPIO 2, 8, 9 (strapping pins - avoid during boot)

❌ **Avoid**:
- GPIO 11-17 (SPI flash pins)

## Functional Differences

### What Works Exactly the Same
✅ WiFi connectivity  
✅ Device login flow  
✅ Token management  
✅ Presence polling  
✅ LED animations  
✅ Web interface  
✅ All presence states

### What's Different
⚠️ **Memory Display**: Shows 400KB instead of 320KB (this is correct)  
⚠️ **Performance**: Similar for this application  
⚠️ **Boot Time**: Slightly faster on ESP32-C3

## Troubleshooting Migration

### LEDs Don't Work After Migration

1. **Check GPIO Pin**:
   - Ensure using valid ESP32-C3 GPIO
   - Not GPIO 11-17
   - Update build flags if changed

2. **Verify Wiring**:
   - Same as before: DIN → GPIO, VCC → 5V, GND → GND
   - Check for loose connections

3. **Check Power**:
   - LED strips need sufficient current
   - Use external 5V supply for >8 LEDs

### Can't Upload Firmware

1. **Hold BOOT button** during power-on
2. **Different USB Driver** may be needed
3. **Try different USB cable/port**
4. **Update USB drivers** for ESP32-C3

### WiFi Connection Issues

1. **ESP32-C3 only supports 2.4GHz** (same as ESP32)
2. **Check WiFi credentials** (need to re-enter)
3. **Signal strength** - ESP32-C3 has good WiFi performance
4. **Channel** - try different router channels

### Device Login Not Working

1. **Re-enter Client ID and Tenant** (not transferred)
2. **Check certificate validation**:
   - Use `esp32-c3-devkitm-1` (with certs)
   - Or `esp32-c3-devkitm-1-nocertcheck` (without)
3. **Ensure correct time** (needed for SSL/TLS)

## Performance Comparison

### Before (ESP32)
- Dual-core 240 MHz
- WiFi + Presence polling: ~10-15% CPU
- LED updates: Core 0
- Main logic: Core 1

### After (ESP32-C3)
- Single-core 160 MHz
- WiFi + Presence polling: ~15-20% CPU
- LED updates: Same core as main logic
- **Result**: No noticeable difference for this application

## Cost Savings

Example prices (as of Dec 2024):

| Board | Typical Price |
|-------|---------------|
| ESP32 DevKit v1 | $5-8 |
| ESP32-C3-DevKitM-1 | $2-4 |
| **Savings** | **$3-4** |

For multiple devices, savings add up quickly!

## Rollback to ESP32

If you need to go back to ESP32:

```bash
# Switch environment
pio run -e esp32doit-devkit-v1

# Upload to ESP32
pio run -e esp32doit-devkit-v1 -t upload
```

Configuration needs to be re-entered (not transferred between boards).

## Migration Checklist

### Pre-Migration
- [ ] Note all current settings (Client ID, Tenant, etc.)
- [ ] Test LED strip works
- [ ] Verify 5V power supply adequate
- [ ] Check GPIO pins if custom

### During Migration
- [ ] Physically swap boards
- [ ] Connect LED strip to same/equivalent GPIO
- [ ] Power up ESP32-C3
- [ ] Join AP and configure WiFi

### Post-Migration
- [ ] Verify WiFi connection
- [ ] Enter Client ID and Tenant
- [ ] Complete device login
- [ ] Test presence polling
- [ ] Verify LED animations
- [ ] Check web UI memory display (should show 400KB)

### Optional
- [ ] Update documentation with new GPIO pin
- [ ] Label board with configuration
- [ ] Keep ESP32 as backup

## Tips

1. **Test First**: Try ESP32-C3 on breadboard before permanent installation
2. **Keep Notes**: Document your GPIO pin choices
3. **Label Wires**: Makes troubleshooting easier
4. **External Power**: Use for >8 LEDs regardless of board
5. **Backup Config**: Screenshot or save settings before migration

## Need Help?

1. Check [ESP32-C3 Guide](docs/ESP32-C3.md)
2. See [Quick Reference](docs/ESP32-C3-QUICKREF.md)
3. Review [Changes Summary](docs/ESP32-C3-CHANGES.md)
4. Open GitHub issue if stuck

## Final Notes

The migration is **straightforward** and the functionality is **identical**. The main challenge is re-entering configuration, not the code or hardware.

**Migration Time**: ~15-30 minutes (including device login)

---

**Last Updated**: 2025-12-08  
**Tested Migration Path**: ESP32 DevKit v1 → ESP32-C3-DevKitM-1
