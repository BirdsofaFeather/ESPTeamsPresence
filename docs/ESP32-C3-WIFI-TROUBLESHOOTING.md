# ESP32-C3 WiFi AP Not Broadcasting - Troubleshooting Guide

## Quick Diagnostics

### Step 1: Check Serial Output

Connect to serial monitor:
```bash
pio device monitor -b 115200
```

or

```bash
screen /dev/cu.usbserial-* 115200
```

### Step 2: Look for These Messages

**Expected on first boot or no config:**
```
setup() Starting up...
IotWebConf: AP mode started
AP SSID: ESPTeamsPresence
AP Password: presence
```

**If you see instead:**
```
WiFi connecting
```
This means it's trying to connect to saved WiFi instead of starting AP.

## Common Issues and Fixes

### Issue 1: ESP32-C3 Needs Explicit WiFi Mode

The ESP32-C3 might need explicit WiFi mode initialization. Try this fix:

**Add to `setup()` before `iotWebConf.init():`**

```cpp
// ESP32-C3 WiFi initialization
#ifdef ESP32C3
  WiFi.mode(WIFI_MODE_APSTA);  // Enable both AP and STA modes
  delay(100);
#endif
```

### Issue 2: LED_BUILTIN Not Defined

ESP32-C3 boards might not have LED_BUILTIN defined, causing initialization issues.

**Check serial output for:** `LED_BUILTIN not defined` errors

### Issue 3: RMT Channel Conflict

ESP32-C3 has fewer RMT channels. GPIO conflicts might prevent WiFi initialization.

### Issue 4: Saved WiFi Config

If you previously used this ESP32-C3 with different firmware, it might have saved WiFi credentials.

**Fix: Force AP Mode**

Temporarily comment out line 671:
```cpp
// iotWebConf.skipApStartup();  // TEMPORARILY DISABLED FOR TESTING
```

### Issue 5: EEPROM/NVS Issues

Corrupted config might prevent AP mode.

**Fix: Clear flash**
```bash
pio run -e esp32-c3-devkitm-1 -t erase
pio run -e esp32-c3-devkitm-1 -t upload
```

## Detailed Debug Steps

### 1. Enable Debug Output

Uncomment line 644 in main.cpp:
```cpp
Serial.setDebugOutput(true);
```

### 2. Add WiFi Debug

Add after line 648:
```cpp
#ifdef ESP32C3
  Serial.println("ESP32-C3 detected");
  Serial.print("WiFi Mode: ");
  Serial.println(WiFi.getMode());
#endif
```

### 3. Check GPIO Pin

Verify GPIO 13 (or your custom pin) is not conflicting:
```cpp
Serial.print("LED Data Pin: ");
Serial.println(ws2812fx.getPin());
```

## Quick Fix to Try First

Add this code to `setup()` right before `iotWebConf.init()`:

```cpp
  // ESP32-C3 WiFi setup
  #ifdef ESP32C3
  Serial.println("Initializing ESP32-C3 WiFi...");
  WiFi.mode(WIFI_AP_STA);
  delay(100);
  #endif
  
  iotWebConf.init();
```

## Alternative: Force AP Mode for Testing

Replace line 671 with:
```cpp
  // iotWebConf.skipApStartup();  // Comment this out
  iotWebConf.setApTimeoutMs(0);  // Never exit AP mode (for testing)
```

## Check Hardware

### Power Supply
- Ensure stable 5V supply
- Try different USB cable/port
- Some ESP32-C3 boards are picky about power

### Antenna
- Check built-in antenna connection
- Some boards have external antenna connectors

## Serial Output Analysis

Share the complete serial output from startup. Look for:

1. **"setup() Starting up..."** - Confirms code is running
2. **IotWebConf messages** - Configuration library status
3. **WiFi mode messages** - What mode WiFi is in
4. **Error messages** - Any failures

## Manual Config Reset

If nothing works, manually reset EEPROM:

Add to start of `setup()`:
```cpp
  #ifdef ESP32C3
  EEPROM.begin(512);
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  Serial.println("EEPROM cleared (ESP32-C3)");
  delay(1000);
  #endif
```

Then upload, let it run once, then remove this code and re-upload.

## Next Steps

1. **Get serial output** - This is the most important diagnostic info
2. **Try the quick fix** - Add WiFi.mode() before init
3. **Force AP mode** - Comment out skipApStartup()
4. **Erase flash** - Clear any saved config

---

**Most Likely Issue**: ESP32-C3 WiFi initialization sequence differs from ESP32. The quick fix should resolve it.
