# ESP32-C3 Status - LED Functionality Disabled

## Current Status: WiFi Only Mode

Due to RMT peripheral compatibility issues with ESP32-C3, LED functionality has been **temporarily disabled** to get the core functionality working.

## What This Means

### ✅ Working (ESP32-C3):
- WiFi AP mode
- Web interface
- Device login flow
- Token management
- Presence polling
- All Microsoft Graph API features

### ❌ Not Working (ESP32-C3):
- NeoPixel/WS2812B LED display
- LED animations
- Visual presence indicators

### ✅ Fully Working (Standard ESP32):
- All features including LEDs

## Why LEDs Are Disabled

The ESP32-C3's RMT peripheral implementation differs from standard ESP32:
1. Different clock sources and timing
2. Potential library compatibility issues
3. Causing watchdog timer crashes during initialization

## What You'll See

### Serial Output (ESP32-C3):
```
setup() Starting up...
ESP32-C3 DEBUG: Step 1 - Serial working
ESP32-C3: LED initialization DISABLED for debugging
ESP32-C3 DEBUG: Step 2 - Skipped LEDs
ESP32-C3: AP mode enabled for initial configuration
ESP32-C3: Initializing WiFi mode...
[WiFi and IotWebConf messages]
ESP32-C3: Neopixel task NOT created (LEDs disabled)
setup() ready...
```

### What Still Works:
- **Web Interface**: http://espteamspresence.local or http://192.168.4.1
- **Presence Detection**: Polls Microsoft Graph API 
- **Configuration**: All settings through web UI
- **Token Management**: Automatic refresh

## Re-enabling LEDs for ESP32-C3

To re-enable LEDs once RMT issue is resolved, edit `src/main.cpp`:

### Change Line ~654:
```cpp
#ifndef ESP32C3
  // WS2812FX - DISABLED ON ESP32-C3 DUE TO RMT ISSUES
```

To:
```cpp
#if 1  // Enable LEDs for all boards
  // WS2812FX
```

### Change Line ~740:
```cpp
#ifndef ESP32C3
  xTaskCreatePinnedToCore(
```

To:
```cpp
#ifdef ESP32C3
  xTaskCreate(
    neopixelTask,
    "Neopixels",
    4096,
    NULL,
    1,
    &TaskNeopixel);
#else
  xTaskCreatePinnedToCore(
```

## Alternative: Use NeoPixelBus Library

For proper ESP32-C3 LED support, consider switching to NeoPixelBus:

### In `platformio.ini`:
```ini
lib_deps=
  IotWebConf@2.3.3
  ArduinoJson@6.21.0
  NeoPixelBus@2.7.0  # Better ESP32-C3 support
```

### Code Changes Required:
- Replace WS2812FX calls with NeoPixelBus
- Different animation handling
- Better ESP32-C3 RMT support

## Using the Device Without LEDs

### 1. Configure WiFi
- Connect to AP: `ESPTeamsPresence`
- Password: `presence`
- Navigate to http://192.168.4.1

### 2. Start Device Login
- Enter Client ID and Tenant
- Click "Start device login"  
- Follow authentication flow

### 3. Monitor Presence
- Check web interface for current status
- API endpoint: `/api/settings`

### 4. View Logs
- Serial monitor shows presence updates:
  ```
  Polling presence info ...
  --> Availability: Available, Activity: Available
  ```

## Testing Instructions

### Upload and Test:
```bash
# Upload
pio run -e esp32-c3-devkitm-1 -t upload

# Monitor
pio device monitor -b 115200
```

### Expected Result:
- ✅ No more boot loops
- ✅ Serial output appears
- ✅ WiFi AP broadcasts
- ✅ Web interface accessible
- ❌ LEDs don't light up (expected)

## Future Work

### Option 1: Fix WS2812FX for ESP32-C3
- Debug RMT driver compatibility
- May require library updates
- Test with latest ESP32 Arduino core

### Option 2: Switch to NeoPixelBus
- More maintained library
- Better ESP32-C3 support
- Requires code refactoring

### Option 3: Software LED Control
- Bit-bang GPIO (slower)
- No RMT peripheral needed
- May have timing issues

## Current Recommendation

**Use ESP32-C3 for WiFi/API functionality only**, without LEDs. If LEDs are required:
- Use standard ESP32 (dual-core)
- Or wait for LED support improvement
- Or help develop ESP32-C3 LED solution

## Summary

| Feature | ESP32 | ESP32-C3 |
|---------|-------|----------|
| WiFi | ✅ | ✅ |
| Web UI | ✅ | ✅ |
| Device Login | ✅ | ✅ |
| Presence API | ✅ | ✅ |
| LED Display | ✅ | ❌ (disabled) |
| Cost | ~$5 | ~$2 |

---

**Status**: ESP32-C3 WiFi functionality working, LEDs disabled pending RMT fix

**Last Updated**: 2025-12-08

**Known Issue**: RMT peripheral causing boot crashes on ESP32-C3
