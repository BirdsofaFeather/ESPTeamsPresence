# ESP32-C3 Boot Loop Fix

## Changes Applied

The boot loop was caused by several ESP32-C3 specific issues:

### 1. **RMT Channel Conflict**
- **Issue**: ESP32-C3's RMT channel 0 may conflict with other peripherals
- **Fix**: Changed to use RMT_CHANNEL_1 for ESP32-C3
- Files modified:
  - `src/main.cpp` - customShow() function
  - `src/main.cpp` - setup() function

### 2. **Stack Overflow**
- **Issue**: 1000 bytes stack was too small for ESP32-C3's RMT operations
- **Fix**: Increased to 4096 bytes for both ESP32 and ESP32-C3
- File modified: `src/main.cpp` - neopixelTask creation

### 3. **WiFi Initialization Issues**
- Previously fixed: WiFi mode set to AP_STA before IotWebConf init

## Testing Instructions

### 1. Clean Build
```bash
# Erase flash completely
pio run -e esp32-c3-devkitm-1 -t erase

# Rebuild
pio run -e esp32-c3-devkitm-1

# Upload
pio run -e esp32-c3-devkitm-1 -t upload
```

### 2. Monitor Serial Output
```bash
pio device monitor -b 115200
```

### Expected Output

You should now see:
```
Build:Feb  7 2021
rst:0x1 (POWERON),boot:0xc (SPI_FAST_FLASH_BOOT)
[... boot info ...]
entry 0x403cc710

setup() Starting up...
ESP32-C3: Initializing RMT for LEDs...
ESP32-C3: AP mode enabled for initial configuration
ESP32-C3: Initializing WiFi mode...
IotWebConf: ... [configuration messages]
ESP32-C3: Neopixel task created
setup() ready...
```

### No More Boot Loops!

The watchdog timer should not trigger anymore.

## If Still Boot Looping

### Quick Test: Disable LEDs Temporarily

Edit `src/main.cpp` around line 650 and comment out RMT:

```cpp
  // WS2812FX
  ws2812fx.init();
  
  #ifdef ESP32C3
  // TEMPORARILY DISABLED FOR TESTING
  // DBG_PRINTLN(F("ESP32-C3: Initializing RMT for LEDs..."));
  // rmt_tx_int(RMT_CHANNEL_1, ws2812fx.getPin());
  Serial.println("ESP32-C3: LEDs disabled for testing");
  #else
  rmt_tx_int(RMT_CHANNEL_0, ws2812fx.getPin());
  #endif
  
  // ALSO COMMENT THIS:
  // ws2812fx.start();
  // setAnimation(0, FX_MODE_STATIC, WHITE);
```

This will let you test if WiFi works without LED issues.

### Check Power Supply

Boot loops can also be caused by:
- **Insufficient power** - Use good USB cable and port
- **Brownout** - Try powered USB hub or external 5V supply
- **LED strip drawing too much** - Disconnect LED strip temporarily

### Check GPIO Pin

If using custom GPIO pin, verify it's valid for ESP32-C3:
- **Safe**: GPIO 0-10, 18-21
- **Avoid**: GPIO 11-17 (flash), GPIO 2/8/9 (strapping)

## Detailed Debug

### Add Debug Print in setup()

Add after line 642:
```cpp
  Serial.begin(115200);
  delay(1000);  // Give serial time
  DBG_PRINTLN();
  DBG_PRINTLN(F("=== ESP32-C3 Debug ==="));
  DBG_PRINTLN(F("1. Serial started"));
```

Add before RMT init:
```cpp
  DBG_PRINTLN(F("2. WS2812FX init..."));
  ws2812fx.init();
  DBG_PRINTLN(F("3. WS2812FX init complete"));
  
  #ifdef ESP32C3
  DBG_PRINTLN(F("4. Starting RMT init..."));
  rmt_tx_int(RMT_CHANNEL_1, ws2812fx.getPin());
  DBG_PRINTLN(F("5. RMT init complete"));
  #else
  ...
```

This will show exactly where it crashes.

## Alternative: Use NeoPixelBus Library

If RMT continues to cause issues, consider using NeoPixelBus library which has better ESP32-C3 support:

```cpp
// In platformio.ini:
lib_deps =
  IotWebConf@2.3.3
  ArduinoJson@6.21.0
  NeoPixelBus@2.7.0  // Instead of WS2812FX
```

## Common Boot Reasons

| rst code | Meaning | Likely Cause |
|----------|---------|--------------|
| 0x1 | POWERON | Normal power on |
| 0x3 | SW_RESET | Software reset (ESP.restart()) |
| 0x8 | TG1WDT_SYS_RST | Watchdog timeout (CRASH) |
| 0xc | RTC_WDT_SYS_RST | RTC watchdog (CRASH) |

You were seeing `rst:0x8` which is a crash/hang causing watchdog.

## Success Indicators

Once fixed, you'll see:
1. ✅ `rst:0x1` (POWERON) instead of `rst:0x8`
2. ✅ "setup() Starting up..." message
3. ✅ "ESP32-C3: Neopixel task created"
4. ✅ WiFi AP broadcasts: `ESPTeamsPresence`
5. ✅ LEDs show white color (if connected)

---

**Current Status**: Applied RMT channel fix and increased stack size. This should resolve the boot loop.

**Next Step**: Upload new firmware and check serial output.
