# ESPTeamsPresence - Build and Upload Guide

## Quick Start Commands

### For ESP32-C3

```bash
# Build, upload, and monitor in one command
pio run -e esp32-c3-devkitm-1 -t upload && pio device monitor -b 115200
```

### For Standard ESP32

```bash
# Build, upload, and monitor in one command
pio run -e esp32doit-devkit-v1 -t upload && pio device monitor -b 115200
```

---

## Detailed Step-by-Step Instructions

### Step 1: Navigate to Project Directory

```bash
cd /Users/seyha.soun/Documents/GitHub/ESPTeamsPresence
```

### Step 2: Choose Your Board

Identify which ESP32 board you have:
- **ESP32-C3-DevKitM-1** (or similar ESP32-C3 board)
- **ESP32 DevKit v1** (standard dual-core ESP32)

### Step 3: Build the Firmware

#### For ESP32-C3:
```bash
pio run -e esp32-c3-devkitm-1
```

#### For ESP32:
```bash
pio run -e esp32doit-devkit-v1
```

**Expected Output:**
- Downloading dependencies (first time only)
- Compiling source files
- Linking firmware
- `SUCCESS` message

### Step 4: Connect Your Board

1. Connect ESP32/ESP32-C3 to your computer via USB
2. Wait for USB serial port to be detected
3. Check port (usually `/dev/cu.usbserial-*` or `/dev/cu.SLAB_USBtoUART`)

**Tip:** List available ports:
```bash
pio device list
```

### Step 5: Upload Firmware

#### For ESP32-C3:
```bash
pio run -e esp32-c3-devkitm-1 -t upload
```

**If upload fails:**
1. Hold the **BOOT** button on the ESP32-C3
2. Press the **RESET** button (while holding BOOT)
3. Release RESET, then release BOOT
4. Try upload command again

#### For ESP32:
```bash
pio run -e esp32doit-devkit-v1 -t upload
```

**Expected Output:**
- Connecting to board
- Erasing flash
- Writing firmware
- Verifying
- `SUCCESS` message

### Step 6: Monitor Serial Output

```bash
pio device monitor -b 115200
```

**To exit monitor:** Press `Ctrl+C`

---

## Expected Serial Output

### Successful Boot Sequence:

```
Build:Feb  7 2021
rst:0x1 (POWERON),boot:0xc (SPI_FAST_FLASH_BOOT)
[...]
entry 0x403cc710

setup() Starting up...
NeoPixelBus initialized: 16 LEDs on GPIO 13
ESP32-C3: AP mode enabled for initial configuration
ESP32-C3: Initializing WiFi mode...
iotWebConf: AP mode started
AP SSID: ESPTeamsPresence
AP Password: presence
setup() complete - NeoPixelBus ready
```

### Success Indicators:

- ✅ `rst:0x1` (POWERON) - Normal boot, not a crash
- ✅ "NeoPixelBus initialized" - LEDs working
- ✅ "AP mode started" - WiFi ready
- ✅ "setup() complete" - Initialization successful

---

## Additional Commands

### Clean Build (if having issues)

```bash
# Clean previous build files
pio run -e esp32-c3-devkitm-1 -t clean

# Rebuild from scratch
pio run -e esp32-c3-devkitm-1
```

### Update Dependencies

```bash
# Update all libraries and platform
pio pkg update
```

### Erase Flash Completely

```bash
# ESP32-C3
pio run -e esp32-c3-devkitm-1 -t erase

# ESP32
pio run -e esp32doit-devkit-v1 -t erase
```

### Build Without Uploading

```bash
# Just compile, don't upload
pio run -e esp32-c3-devkitm-1
```

### Upload Without Building

```bash
# Upload previously built firmware
pio run -e esp32-c3-devkitm-1 -t upload --upload-port /dev/cu.usbserial-*
```

---

## Troubleshooting

### Build Errors

**Problem:** Compilation errors

**Solution:**
```bash
# Update platform and libraries
pio pkg update

# Clean and rebuild
pio run -e esp32-c3-devkitm-1 -t clean
pio run -e esp32-c3-devkitm-1
```

### Upload Fails

**Problem:** Can't connect to board

**Solutions:**

1. **Check USB cable** - Try a different cable (must support data)
2. **Check port** - Verify with `pio device list`
3. **Press BOOT button** - Hold during upload
4. **Try different USB port** - Some ports provide more power
5. **Install drivers** - ESP32-C3 may need CH340/CP2102 drivers

**Manual port selection:**
```bash
pio run -e esp32-c3-devkitm-1 -t upload --upload-port /dev/cu.usbserial-14320
```

### Boot Loops (rst:0x8)

**Problem:** Device keeps resetting

**Solution:**
- This was fixed in the code updates (NeoPixelBus migration)
- If still happening, check GPIO pin configuration
- Ensure LEDs are not drawing too much power

### No Serial Output

**Problem:** Monitor shows nothing

**Solutions:**

1. **Check baud rate:**
   ```bash
   pio device monitor -b 115200
   ```

2. **Press RESET button** on ESP32

3. **Check port:**
   ```bash
   pio device monitor --port /dev/cu.usbserial-*
   ```

### LEDs Don't Light Up

**Problem:** No LED output

**Check:**

1. **GPIO Pin** - Verify in web config (default: GPIO 13)
2. **Wiring** - DIN → GPIO, VCC → 5V, GND → GND
3. **LED Strip** - Test with another sketch
4. **Power** - External 5V for >8 LEDs
5. **Serial Output** - Look for "NeoPixelBus initialized" message

---

## Build Environments Available

| Environment | Platform | Board | Certificate Check |
|-------------|----------|-------|-------------------|
| `esp32-c3-devkitm-1` | ESP32-C3 | DevKitM-1 | ✅ Enabled |
| `esp32-c3-devkitm-1-nocertcheck` | ESP32-C3 | DevKitM-1 | ❌ Disabled |
| `esp32doit-devkit-v1` | ESP32 | DevKit v1 | ✅ Enabled |
| `esp32doit-devkit-v1-nocertcheck` | ESP32 | DevKit v1 | ❌ Disabled |
| `m5stack-core-esp32` | ESP32 | M5Stack | ✅ Enabled |

**Note:** Use `-nocertcheck` variants only for testing if having SSL/certificate issues.

---

## After Successful Upload

### Step 1: Connect to WiFi AP

1. Look for WiFi network: **ESPTeamsPresence**
2. Password: **presence**
3. Connect with your phone or computer

### Step 2: Configure via Web Interface

1. Open browser to: `http://192.168.4.1`
2. Configure:
   - WiFi SSID and password
   - Client ID (Microsoft app ID)
   - Tenant (your organization)
   - Polling interval (5-300 seconds)
   - Number of LEDs (1-500)
   - LED Data Pin (GPIO 0-21)

### Step 3: Start Device Login

1. Click "Start device login"
2. Copy the code shown
3. Go to: `https://microsoft.com/devicelogin`
4. Enter code and sign in

### Step 4: Verify Operation

1. Check serial monitor for:
   ```
   Polling presence info ...
   --> Availability: Available, Activity: Available
   ```

2. LEDs should change color based on presence:
   - **Green** - Available
   - **Red** - In call/meeting
   - **Yellow** - Away
   - **Purple** - Busy
   - **Pink** - Do not disturb

---

## Common Build/Upload Sequences

### First Time Setup:

```bash
cd /Users/seyha.soun/Documents/GitHub/ESPTeamsPresence
pio run -e esp32-c3-devkitm-1 -t upload
pio device monitor -b 115200
```

### Code Changes - Quick Upload:

```bash
# Already in project directory
pio run -e esp32-c3-devkitm-1 -t upload && pio device monitor -b 115200
```

### Complete Reset:

```bash
pio run -e esp32-c3-devkitm-1 -t erase
pio run -e esp32-c3-devkitm-1 -t upload
pio device monitor -b 115200
```

### Update Libraries:

```bash
pio pkg update
pio run -e esp32-c3-devkitm-1 -t clean
pio run -e esp32-c3-devkitm-1 -t upload
```

---

## Serial Monitor Tips

### While Monitoring:

- **Exit:** `Ctrl+C`
- **Clear screen:** `Cmd+K` (macOS Terminal)
- **Scroll up:** Use trackpad/mouse
- **Save output:** `pio device monitor > output.log`

### Useful Serial Commands:

```bash
# Monitor with timestamp
pio device monitor -b 115200 --echo

# Monitor specific port
pio device monitor --port /dev/cu.usbserial-14320 -b 115200

# Monitor and save to file
pio device monitor -b 115200 | tee serial_output.txt
```

---

## Environment Variable (Optional)

Add to `~/.zshrc` for convenience:

```bash
# PlatformIO alias
alias pioESP32C3="pio run -e esp32-c3-devkitm-1"
alias pioESP32="pio run -e esp32doit-devkit-v1"
```

Then use:
```bash
pioESP32C3 -t upload
```

---

## Success Checklist

After upload, verify:

- [ ] Serial output shows "setup() complete"
- [ ] No `rst:0x8` errors (boot loops)
- [ ] "NeoPixelBus initialized" message appears
- [ ] LEDs light up (white on boot)
- [ ] WiFi AP "ESPTeamsPresence" is broadcasting
- [ ] Can connect to web interface
- [ ] Serial shows configured GPIO pin

---

## Getting Help

If stuck:

1. **Check serial output** - Most informative
2. **Read error messages** - Usually point to the issue
3. **Try clean build** - Fixes most build issues
4. **Check wiring** - Common hardware issue
5. **Review documentation:**
   - [ESP32-C3 Guide](ESP32-C3.md)
   - [NeoPixelBus Migration](NEOPIXELBUS-MIGRATION.md)
   - [Troubleshooting](ESP32-C3-BOOTLOOP-FIX.md)

---

**Last Updated:** 2025-12-08  
**Platform:** ESP32 & ESP32-C3  
**Framework:** Arduino / PlatformIO  
**Library:** NeoPixelBus 2.7.9
