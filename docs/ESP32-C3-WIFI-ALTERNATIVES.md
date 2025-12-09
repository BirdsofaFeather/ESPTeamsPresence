# ESP32-C3 WiFi AP Mode - Alternative Configurations

If the ESP32-C3 still doesn't broadcast the AP after the WiFi mode fix, try these alternatives:

## Option 1: Force AP Mode on ESP32-C3 (Recommended for First Boot)

**Edit `src/main.cpp` around line 671:**

```cpp
  iotWebConf.setWifiConnectionCallback(&onWifiConnected);
  iotWebConf.setConfigSavedCallback(&onConfigSaved);
  iotWebConf.setupUpdateServer(&httpUpdater);
  
  #ifdef ESP32C3
  // Force AP mode on ESP32-C3 for initial setup
  // iotWebConf.skipApStartup();  // Disabled for ESP32-C3
  #else
  iotWebConf.skipApStartup();
  #endif
  
  // ESP32-C3 WiFi initialization - must be done before iotWebConf.init()
  #ifdef ESP32C3
  DBG_PRINTLN(F("ESP32-C3: Initializing WiFi mode..."));
  WiFi.mode(WIFI_AP_STA);  // Enable both AP and Station modes
  delay(100);  // Give WiFi time to initialize
  #endif
  
  iotWebConf.init();
```

## Option 2: Increase AP Timeout

**Edit `src/main.cpp`:**

```cpp
  #ifdef ESP32C3
  // ESP32-C3: Stay in AP mode longer
  iotWebConf.setApTimeoutMs(300000);  // 5 minutes in AP mode
  #endif
  
  iotWebConf.init();
```

## Option 3: Completely Disable skipApStartup for ESP32-C3

**Replace line 671 with:**

```cpp
  #ifndef ESP32C3
  iotWebConf.skipApStartup();  // Only skip AP on regular ESP32
  #endif
```

## Option 4: Force WiFi Reset on Boot

**Add to setup() before WiFi initialization:**

```cpp
  #ifdef ESP32C3
  // Force WiFi reset on ESP32-C3
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  delay(100);
  #endif
```

## Option 5: Minimal Test Configuration

**Create a test version of setup():**

```cpp
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n=== ESP32-C3 WiFi Test ===");
  
  #ifdef ESP32C3
  Serial.println("1. Resetting WiFi...");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  delay(500);
  
  Serial.println("2. Setting AP_STA mode...");
  WiFi.mode(WIFI_AP_STA);
  delay(500);
  
  Serial.println("3. Starting AP...");
  WiFi.softAP("ESPTeamsPresence", "presence");
  delay(1000);
  
  Serial.print("4. AP IP: ");
  Serial.println(WiFi.softAPIP());
  
  Serial.print("5. AP MAC: ");
  Serial.println(WiFi.softAPmacAddress());
  
  Serial.println("=== AP Should be broadcasting now ===");
  #endif
  
  // Rest of setup...
}
```

## Option 6: Check for Saved Config

**Add diagnostic code:**

```cpp
  #ifdef ESP32C3
  EEPROM.begin(512);
  bool hasConfig = (EEPROM.read(0) == 'I' && 
                    EEPROM.read(1) == 'o' &&
                    EEPROM.read(2) == 't' && 
                    EEPROM.read(3) == 'C');
  
  Serial.print("Has saved config: ");
  Serial.println(hasConfig ? "YES" : "NO");
  
  if (hasConfig) {
    Serial.println("WARNING: Saved config found. AP may not start.");
    Serial.println("To clear: Hold button or use erase command");
  }
  #endif
```

## Complete Recommended Fix for main.cpp

Replace the WiFi initialization section (lines 668-680) with:

```cpp
  iotWebConf.setWifiConnectionCallback(&onWifiConnected);
  iotWebConf.setConfigSavedCallback(&onConfigSaved);
  iotWebConf.setupUpdateServer(&httpUpdater);
  
  // ESP32-C3 specific configuration
  #ifdef ESP32C3
  DBG_PRINTLN(F("ESP32-C3 detected - applying WiFi fixes..."));
  
  // Don't skip AP startup on ESP32-C3 (at least on first boot)
  // Comment this line if you want AP to always start:
  // iotWebConf.skipApStartup();
  
  // Set longer AP timeout for ESP32-C3
  iotWebConf.setApTimeoutMs(300000);  // 5 minutes
  
  // Initialize WiFi explicitly
  WiFi.mode(WIFI_OFF);
  delay(100);
  WiFi.mode(WIFI_AP_STA);
  delay(100);
  
  DBG_PRINTLN(F("ESP32-C3 WiFi initialized in AP_STA mode"));
  #else
  // Standard ESP32 behavior
  iotWebConf.skipApStartup();
  #endif
  
  iotWebConf.init();
```

## Testing Each Fix

After applying each fix:

1. **Erase flash** (important!):
   ```bash
   pio run -e esp32-c3-devkitm-1 -t erase
   ```

2. **Upload new firmware**:
   ```bash
   pio run -e esp32-c3-devkitm-1 -t upload
   ```

3. **Monitor serial**:
   ```bash
   pio device monitor
   ```

4. **Look for AP**:
   - SSID: `ESPTeamsPresence`
   - Password: `presence`
   - Should appear within 10-30 seconds

## Most Likely Solution

Based on ESP32-C3 behavior, **Option 3** (disabling skipApStartup for ESP32-C3) combined with the WiFi.mode() initialization is most likely to work.

## If Nothing Works

1. **Test bare WiFi AP** - Use Option 5 minimal test
2. **Check antenna** - Some boards have faulty antennas
3. **Try different board** - Could be hardware issue
4. **Check power** - Insufficient power can prevent WiFi
5. **Update ESP32 platform** - Try `pio pkg update`

---

**Quick Test Command:**

```bash
# Complete reset and upload
pio run -e esp32-c3-devkitm-1 -t erase && \
pio run -e esp32-c3-devkitm-1 -t upload && \
pio device monitor
```
