# ESP8266 Migration Summary

## Overview
The ESPTeamsPresence project has been successfully updated to support **ESP8266** microcontrollers and use the **standard Adafruit NeoPixel library** instead of the ESP32-specific WS2812FX library.

## Changes Made

### 1. platformio.ini
- **Changed platform**: From `espressif32` to `espressif8266`
- **Changed board**: From `esp32doit-devkit-v1` to `nodemcuv2` (ESP8266)
- **Changed default GPIO pin**: From pin 13 to pin 2 (D4 on NodeMCU)
- **Updated library dependencies**:
  - Removed: `WS2812FX@1.4.1`
  - Added: `Adafruit NeoPixel@^1.12.0`
- **Removed ESP32-specific environments**: Removed all ESP32 board configurations

### 2. src/main.cpp
Major changes to support ESP8266:

#### Library Includes
- Changed `#include <HTTPClient.h>` → `#include <ESP8266HTTPClient.h>`
- Changed `#include <ESPmDNS.h>` → `#include <ESP8266mDNS.h>`
- Changed `#include <WS2812FX.h>` → `#include <Adafruit_NeoPixel.h>`
- Removed ESP32-specific includes:
  - `#include "SPIFFS.h"` (now part of FS.h in ESP8266)
  - `#include "ESP32_RMT_Driver.h"`

#### LED Control Implementation
- **Replaced WS2812FX library** with Adafruit_NeoPixel
- **Removed ESP32 RMT driver** (not needed for ESP8266)
- **Implemented custom animation functions**:
  - `setColor()` - Set all pixels to a solid color
  - `setAnimation()` - Set animation mode and color
  - `updateAnimations()` - Update LED animations in the main loop
- **Supported animation modes**:
  - MODE_STATIC - Solid color
  - MODE_BREATH - Breathing effect
  - MODE_SCAN - Scanning effect
  - MODE_THEATER_CHASE - Theater chase effect
  - MODE_COLOR_WIPE - Color wipe effect

#### Multicore Support
- **Removed ESP32 multicore task implementation**:
  - Removed `neopixelTask()` function
  - Removed `TaskNeopixel` handle
  - Removed `xTaskCreatePinnedToCore()` calls
- ESP8266 is single-core, so LED updates are now called in the main loop

#### SPIFFS Changes
- Changed `SPIFFS.begin(true)` to `SPIFFS.begin()`
- Added explicit `SPIFFS.format()` call on mount failure
- ESP8266 handles SPIFFS differently than ESP32

### 3. src/request_handler.h
- Updated `onConfigSaved()` function to use Adafruit NeoPixel's `updateLength()` method

## Hardware Configuration

### Default Pin Assignments (ESP8266)
- **LED Data Pin**: GPIO 2 (D4 on NodeMCU)
- **Number of LEDs**: 16 (configurable via web interface)

### Recommended Hardware
- **Board**: NodeMCU ESP8266 (or any ESP8266 board)
- **LED Strip**: WS2812B/NeoPixel compatible LEDs
- **Power**: 5V power supply for LEDs (separate from ESP8266 if using >10 LEDs)

## Building and Uploading

### Prerequisites
Install PlatformIO if not already installed:
```bash
pip install platformio
```

### Build the Project
```bash
cd /Users/seyha.soun/Documents/GitHub/ESPTeamsPresence
platformio run
```

### Upload to ESP8266
```bash
platformio run --target upload
```

### Monitor Serial Output
```bash
platformio device monitor
```

## Configuration

The device creates a WiFi access point on first boot:
- **SSID**: ESPTeamsPresence
- **Password**: presence

Connect to this AP and configure:
1. WiFi credentials
2. Microsoft Teams Client ID
3. Microsoft Teams Tenant ID
4. Polling interval (seconds)
5. Number of LEDs
6. **LED Data Pin (GPIO)** ← NEW! Configurable via web interface

## LED Status Indicators

### System Status
- **White (Theater Chase)**: Access Point mode / Not configured
- **Blue (Theater Chase)**: Connecting to WiFi
- **Green (Theater Chase)**: WiFi connected
- **Purple (Theater Chase)**: Device login in progress
- **Red (Theater Chase)**: Refreshing token

### Teams Presence Status
- **Green (Solid)**: Available
- **Yellow (Solid)**: Away
- **Orange (Solid)**: Be Right Back
- **Purple (Solid)**: Busy
- **Pink (Solid)**: Do Not Disturb
- **Red (Breathing)**: In a Call
- **Red (Slow Breathing)**: In a Conference Call
- **White (Breathing)**: Inactive
- **Red (Scanning)**: In a Meeting
- **Black (Off)**: Offline/Off Work/Out of Office
- **Red (Color Wipe)**: Presenting

## Testing Checklist

- [ ] Build completes without errors
- [ ] Upload to ESP8266 succeeds
- [ ] Device creates AP on first boot
- [ ] Can configure WiFi via web interface
- [ ] Can configure Teams credentials
- [ ] LEDs light up and show animations
- [ ] Device login flow works
- [ ] Presence polling works
- [ ] LED colors change based on Teams status
- [ ] Configuration persists after reboot

## Known Limitations

### ESP8266 vs ESP32 Differences
1. **Single Core**: ESP8266 has only one core, so animations run in the main loop
2. **Less RAM**: ESP8266 has ~80KB usable RAM vs ESP32's ~320KB
3. **Slower CPU**: 80MHz vs ESP32's 240MHz
4. **No Hardware RMT**: Uses bit-banging for LED control (works fine but slightly less efficient)

## Troubleshooting

### LEDs not lighting up
- Check LED data pin connection (GPIO 2 / D4)
- Verify 5V power to LED strip
- Ensure ground is common between ESP8266 and LED strip

### WiFi connection issues
- Check WiFi credentials in configuration
- Ensure 2.4GHz WiFi (ESP8266 doesn't support 5GHz)

### Out of memory errors
- Reduce number of LEDs in configuration
- Reduce polling interval

## Migration from ESP32

If you were previously using this project on ESP32:
1. Back up your configuration (Client ID, Tenant ID)
2. Flash the new ESP8266 firmware
3. Re-enter your configuration via the web interface
4. Your authentication tokens will need to be re-acquired

## Future Enhancements

Potential improvements:
- Add support for additional LED effects
- Optimize animation performance
- Add OTA (Over-The-Air) update support
- Add support for other ESP8266 boards

## Credits

Original project: https://github.com/toblum/ESPTeamsPresence
ESP8266 migration and Adafruit NeoPixel integration: 2024
