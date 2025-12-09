# ESPTeamsPresence Build & Upload Guide

Complete command-line reference for building and uploading the ESPTeamsPresence firmware.

## Prerequisites

- PlatformIO installed (comes with VS Code PlatformIO extension)
- ESP32-C3 connected via USB

## Quick Reference

```bash
# Navigate to project directory
cd /Users/seyha.soun/Documents/GitHub/ESPTeamsPresence

# Build firmware
~/.platformio/penv/bin/platformio run

# Upload firmware
~/.platformio/penv/bin/platformio run -t upload

# Open Serial Monitor
~/.platformio/penv/bin/platformio device monitor -b 115200 -p /dev/cu.usbmodem2101

# Build + Upload + Monitor (one command)
~/.platformio/penv/bin/platformio run -t upload && ~/.platformio/penv/bin/platformio device monitor -b 115200
```

---

## Detailed Commands

### 1. Building the Firmware

**Build for default environment** (currently: `esp32-c3-devkitm-1-nocertcheck`):
```bash
~/.platformio/penv/bin/platformio run
```

**Build for specific environment**:
```bash
# ESP32-C3 with certificate checking disabled (RECOMMENDED)
~/.platformio/penv/bin/platformio run -e esp32-c3-devkitm-1-nocertcheck

# ESP32-C3 with certificate checking enabled
~/.platformio/penv/bin/platformio run -e esp32-c3-devkitm-1

# Standard ESP32
~/.platformio/penv/bin/platformio run -e esp32doit-devkit-v1
```

**Clean build** (removes previous build artifacts):
```bash
~/.platformio/penv/bin/platformio run -t clean
~/.platformio/penv/bin/platformio run
```

**Verbose build** (shows detailed compilation):
```bash
~/.platformio/penv/bin/platformio run -v
```

---

### 2. Uploading Firmware

**Auto-detect port and upload**:
```bash
~/.platformio/penv/bin/platformio run -t upload
```

**Upload to specific port**:
```bash
~/.platformio/penv/bin/platformio run -t upload --upload-port /dev/cu.usbmodem2101
```

**Upload specific environment**:
```bash
~/.platformio/penv/bin/platformio run -e esp32-c3-devkitm-1-nocertcheck -t upload
```

---

### 3. Serial Monitor

**Open Serial Monitor** (auto-detect port):
```bash
~/.platformio/penv/bin/platformio device monitor -b 115200
```

**Monitor specific port** (ESP32-C3):
```bash
~/.platformio/penv/bin/platformio device monitor -b 115200 -p /dev/cu.usbmodem2101
```

**Monitor with filters** (colorize output):
```bash
~/.platformio/penv/bin/platformio device monitor -b 115200 -p /dev/cu.usbmodem2101 --filter colorize
```

**Monitor with ESP32 exception decoder**:
```bash
~/.platformio/penv/bin/platformio device monitor -b 115200 -p /dev/cu.usbmodem2101 --filter esp32_exception_decoder
```

**Exit Serial Monitor**: Press `Ctrl+C`

---

### 4. Finding Your USB Port

**List all connected devices**:
```bash
~/.platformio/penv/bin/platformio device list
```

**On macOS, find USB serial ports**:
```bash
ls /dev/cu.*
```

Common ESP32 port names:
- `/dev/cu.usbmodem*` - ESP32-C3 (USB Serial/JTAG)
- `/dev/cu.usbserial-*` - ESP32 with CP2102/CH340 chip
- `/dev/cu.SLAB_USBtoUART` - ESP32 with Silicon Labs chip

---

### 5. Combined Workflows

**Build, Upload, and Monitor** (recommended for development):
```bash
~/.platformio/penv/bin/platformio run -t upload && \
~/.platformio/penv/bin/platformio device monitor -b 115200 -p /dev/cu.usbmodem2101
```

**Clean, Build, Upload, Monitor**:
```bash
~/.platformio/penv/bin/platformio run -t clean && \
~/.platformio/penv/bin/platformio run -t upload && \
~/.platformio/penv/bin/platformio device monitor -b 115200 -p /dev/cu.usbmodem2101
```

**Upload and immediately monitor**:
```bash
~/.platformio/penv/bin/platformio run -t upload; ~/.platformio/penv/bin/platformio device monitor -b 115200 -p /dev/cu.usbmodem2101
```

---

### 6. Troubleshooting

**Port is busy error**:
```bash
# Close any open Serial Monitor first, then:
~/.platformio/penv/bin/platformio run -t upload
```

**Can't find port**:
```bash
# Check connected devices
~/.platformio/penv/bin/platformio device list

# OR on macOS
ls -la /dev/cu.* | grep usb
```

**Upload fails**:
```bash
# Try holding BOOT button while uploading (standard ESP32)
# ESP32-C3 usually doesn't need this

# Or manually enter bootloader mode:
# 1. Hold BOOT button
# 2. Press RESET button
# 3. Release RESET
# 4. Release BOOT
# 5. Run upload command
```

**Serial Monitor shows nothing**:
```bash
# Press RESET button on ESP32 while monitor is open
# OR disconnect and reconnect USB
```

---

### 7. Project Information

**Show project info**:
```bash
~/.platformio/penv/bin/platformio project config
```

**Show library dependencies**:
```bash
~/.platformio/penv/bin/platformio lib list
```

**Show installed platforms**:
```bash
~/.platformio/penv/bin/platformio platform list
```

**Update all libraries and platforms**:
```bash
~/.platformio/penv/bin/platformio lib update
~/.platformio/penv/bin/platformio platform update
```

---

## Environment Configurations

Available build environments in this project:

| Environment | Description | SSL Certs |
|-------------|-------------|-----------|
| `esp32-c3-devkitm-1-nocertcheck` | ESP32-C3 (RECOMMENDED) | Disabled |
| `esp32-c3-devkitm-1` | ESP32-C3 | Enabled |
| `esp32doit-devkit-v1` | Standard ESP32 | Enabled |
| `esp32doit-devkit-v1-nocertcheck` | Standard ESP32 | Disabled |

**Current default**: `esp32-c3-devkitm-1-nocertcheck` (set in `platformio.ini`)

---

## Your Specific Configuration

Based on your working setup:

```bash
# Project directory
cd /Users/seyha.soun/Documents/GitHub/ESPTeamsPresence

# Build (nocertcheck variant)
~/.platformio/penv/bin/platformio run

# Upload to ESP32-C3
~/.platformio/penv/bin/platformio run -t upload

# Monitor on correct port
~/.platformio/penv/bin/platformio device monitor -b 115200 -p /dev/cu.usbmodem2101
```

### Hardware Configuration
- **Board**: ESP32-C3 DevKitM-1
- **OLED**: GPIO5 (SDA), GPIO6 (SCL), Offset (28, 24)
- **LEDs**: GPIO3 (NeoPixel Data Pin)
- **USB Port**: `/dev/cu.usbmodem2101`

---

## Bash Aliases (Optional)

Add these to your `~/.zshrc` or `~/.bashrc` for quick access:

```bash
# PlatformIO shortcuts
alias pio='~/.platformio/penv/bin/platformio'
alias pio-build='~/.platformio/penv/bin/platformio run'
alias pio-upload='~/.platformio/penv/bin/platformio run -t upload'
alias pio-monitor='~/.platformio/penv/bin/platformio device monitor -b 115200 -p /dev/cu.usbmodem2101'
alias pio-all='~/.platformio/penv/bin/platformio run -t upload && ~/.platformio/penv/bin/platformio device monitor -b 115200 -p /dev/cu.usbmodem2101'

# ESPTeamsPresence specific
alias esp-build='cd /Users/seyha.soun/Documents/GitHub/ESPTeamsPresence && ~/.platformio/penv/bin/platformio run'
alias esp-upload='cd /Users/seyha.soun/Documents/GitHub/ESPTeamsPresence && ~/.platformio/penv/bin/platformio run -t upload'
alias esp-monitor='~/.platformio/penv/bin/platformio device monitor -b 115200 -p /dev/cu.usbmodem2101'
```

After adding aliases, reload your shell:
```bash
source ~/.zshrc  # or source ~/.bashrc
```

Then you can simply use:
```bash
esp-build     # Build firmware
esp-upload    # Upload firmware
esp-monitor   # Open serial monitor
pio-all       # Build + Upload + Monitor
```

---

## SSL Certificate Information

### Recommended: Use nocertcheck (Current Setup)

**Why nocertcheck is recommended for ESP32-C3:**
1. ESP32-C3 has known SSL certificate validation issues
2. Microsoft certificates change periodically, requiring code updates
3. Official repository uses this approach: [GitHub Commit dae37ca](https://github.com/toblum/ESPTeamsPresence/commit/dae37ca78c05f4df5840d6c0b36e2a6e6cf9015e)
4. Secure enough for local Teams presence display
5. Avoids maintenance burden of updating certificates

**How it works:**
- Build flag `-DDISABLECERTCHECK` is added
- Code calls `client->setInsecure()` instead of `client->setCACert()`
- SSL connection is still encrypted, just doesn't validate server certificates

### Alternative: Certificate Validation (Not Recommended for ESP32-C3)

If you need cert validation, use the standard environment:
```bash
~/.platformio/penv/bin/platformio run -e esp32-c3-devkitm-1 -t upload
```

**Limitations:**
- Certificates in code may expire
- ESP32-C3 SSL validation can be unreliable
- Requires manual certificate updates when Microsoft changes certs
- May cause connection failures

**Updating certificates** (if needed):
1. Get current certificates: https://projects.petrucci.ch/esp32/
2. Update `rootCACertificateGraph` and `rootCACertificateLogin` in `src/main.cpp`
3. Rebuild and upload

---

## Serial Monitor Tips

### Keyboard Shortcuts (in monitor)
- `Ctrl+C` - Exit monitor
- `Ctrl+T` - Open menu
- `Ctrl+T` then `Ctrl+H` - Help

### Useful Filter Options
```bash
# Colorize output (easier to read)
--filter colorize

# Show timestamp for each line
--filter time

# Decode ESP32 crash dumps
--filter esp32_exception_decoder

# Multiple filters
--filter colorize --filter time
```

### Example with filters:
```bash
~/.platformio/penv/bin/platformio device monitor -b 115200 -p /dev/cu.usbmodem2101 --filter colorize --filter time
```

---

## Build Output Interpretation

```
RAM:   [=         ]  13.7% (used 44748 bytes from 327680 bytes)
Flash: [========  ]  84.0% (used 1101396 bytes from 1310720 bytes)
```

- **RAM**: Dynamic memory usage (13.7% is excellent)
- **Flash**: Program storage (84% is high but acceptable)

**Warning levels:**
- RAM > 80% - May cause stability issues
- Flash > 95% - May fail to upload

---

## Quick Troubleshooting Reference

| Problem | Solution |
|---------|----------|
| Port busy | Close Serial Monitor before uploading |
| No serial output | Press RESET while monitor is open |
| Upload fails | Check USB cable, try different port, hold BOOT button |
| SSL errors | Use `-nocertcheck` environment |
| Compilation errors | Run `pio run -t clean` first |
| Library errors | Run `pio lib update` |

---

**Document Version**: 1.0  
**Last Updated**: 2025-12-08  
**Your Configuration**: ESP32-C3 with 0.42" OLED + NeoPixels
