# Configurable GPIO Data Pin Feature

## Overview
The ESP8266 Teams Presence project now supports **configurable GPIO data pin** selection via the web configuration interface. You no longer need to edit code or recompile to change which GPIO pin drives your LED strip!

## What Changed

### Previous Behavior
- Data pin was hardcoded via build flags in `platformio.ini`
- Required recompilation to change the pin
- Default was GPIO 2 (D4 on NodeMCU)

### New Behavior
- **Data pin is now configurable via web interface**
- Can be changed at runtime without recompilation
- Default is still GPIO 2, but can be set to any valid GPIO (0-16)
- NeoPixel strip is automatically reinitialized when pin changes

## How to Use

### Via Web Interface

1. **Connect to the device**:
   - Access the web UI at `http://espteamspresence.local` or the device's IP address

2. **Go to Configuration**:
   - Click on "configuration page" link

3. **Set LED Data Pin**:
   - Find the field labeled **"LED Data Pin GPIO (default: 2)"**
   - Enter your desired GPIO pin number (0-16)
   - Click "Apply"

4. **Automatic Reinitialization**:
   - The device will automatically reinitialize the LED strip on the new pin
   - LEDs should start working immediately on the new pin

### Via Build Flags (Optional)

You can still set a default in `platformio.ini`:
```ini
build_flags=
    -DDATAPIN=4    ; Default to GPIO 4 instead of 2
    -DNUMLEDS=16
```

## Technical Details

### Code Changes

#### 1. **New Configuration Parameter** (`main.cpp`)
```cpp
char paramDataPinValue[INTEGER_LEN];
IotWebConfParameter paramDataPin = IotWebConfParameter(
    "LED Data Pin GPIO (default: 2)", "dataPin", paramDataPinValue, INTEGER_LEN,
    "number", "0..16", "2", "min='0' max='16' step='1'");
```

#### 2. **Dynamic NeoPixel Strip** (`main.cpp`)
Changed from static object to pointer:
```cpp
// Old:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLEDS, DATAPIN, NEO_GRB + NEO_KHZ800);

// New:
Adafruit_NeoPixel *strip = nullptr;
int dataPin;
```

#### 3. **Initialization in setup()** (`main.cpp`)
```cpp
dataPin = atoi(paramDataPinValue);
if (dataPin < 0 || dataPin > 16) {
  dataPin = DATAPIN;  // Use default if invalid
}

strip = new Adafruit_NeoPixel(numberLeds, dataPin, NEO_GRB + NEO_KHZ800);
strip->begin();
strip->show();
```

#### 4. **Runtime Reconfiguration** (`request_handler.h`)
The `onConfigSaved()` function now:
- Validates the new data pin
- Deletes the old NeoPixel strip object
- Creates a new one with updated pin
- Automatically reinitializes the LEDs

```cpp
void onConfigSaved() {
  int newDataPin = atoi(paramDataPinValue);
  
  // Validate
  if (newDataPin < 0 || newDataPin > 16) {
    newDataPin = dataPin;  // Keep current if invalid
  }
  
  // Recreate strip if pin changed
  if (!strip || newDataPin != dataPin) {
    if (strip) delete strip;
    dataPin = newDataPin;
    strip = new Adafruit_NeoPixel(numberLeds, dataPin, NEO_GRB + NEO_KHZ800);
    strip->begin();
    strip->show();
  }
}
```

## Supported GPIO Pins

### Valid Pins (ESP8266)
- GPIO 0 (D3) - ⚠️ Caution: affects boot mode
- GPIO 2 (D4) - ✅ **Default** - Has built-in LED
- GPIO 4 (D2) - ✅ Good choice
- GPIO 5 (D1) - ✅ Good choice
- GPIO 12 (D6) - ✅ Good choice
- GPIO 13 (D7) - ✅ Good choice
- GPIO 14 (D5) - ✅ Good choice
- GPIO 15 (D8) - ⚠️ Caution: must be LOW at boot
- GPIO 16 (D0) - ⚠️ Caution: different behavior

### Avoid
- GPIO 1 (TX) - Serial communication
- GPIO 3 (RX) - Serial communication

## Web UI Display

The configured data pin is displayed in three locations:

1. **Current Settings Section** (main page):
   - Shows: "LED Data Pin (GPIO): 2" (or your configured value)

2. **Configuration Page** (IotWebConf):
   - Editable field with validation

3. **API Endpoint** (`/api/settings`):
   ```json
   {
     "client_id": "...",
     "tenant": "...",
     "poll_interval": "30",
     "num_leds": "16",
     "data_pin": "2"
   }
   ```

## Example Use Cases

### 1. Using Multiple ESP8266 Boards
Different ESP8266 boards may have different convenient pins:
- **NodeMCU**: GPIO 2 (D4) is convenient (has LED)
- **Wemos D1 Mini**: GPIO 5 (D1) might be better positioned
- **ESP-01**: GPIO 2 is one of few available

### 2. Hardware Conflicts
If your LED data pin conflicts with other hardware:
- Change the data pin via web UI
- No need to modify and recompile code

### 3. Testing Different Pins
Want to try different pins for best signal quality?
- Switch between pins easily via web interface
- Find the one that works best for your setup

## Validation

The code validates the data pin in two places:

1. **On Configuration Save**:
   - Checks if pin is 0-16
   - Reverts to current pin if invalid

2. **On Startup**:
   - Checks if pin is 0-16
   - Uses default (from DATAPIN build flag) if invalid

## Backward Compatibility

- ✅ Old configurations without `data_pin` will use default (GPIO 2)
- ✅ Build flags still work as defaults
- ✅ Existing behavior preserved if you don't change the setting

## Troubleshooting

### LEDs Not Working After Pin Change

1. **Check Wiring**: Make sure LED DIN is connected to new GPIO pin
2. **Check Serial Output**: Look for "Reinitializing NeoPixel strip..." message
3. **Verify Pin Number**: Ensure you entered the GPIO number, not D number
   - Example: For D4, enter **2** (not 4)
4. **Power Cycle**: Try restarting the ESP8266

### Pin Number Confusion

ESP8266 boards use different labeling:
- **GPIO Number**: Internal pin number (use this in configuration)
- **D Number**: NodeMCU silk screen label

| NodeMCU Label | GPIO Number | Use in Config |
|--------------|-------------|---------------|
| D0           | GPIO 16     | Enter "16"    |
| D1           | GPIO 5      | Enter "5"     |
| D2           | GPIO 4      | Enter "4"     |
| D3           | GPIO 0      | Enter "0"     |
| **D4**       | **GPIO 2**  | Enter "2"     |
| D5           | GPIO 14     | Enter "14"    |
| D6           | GPIO 12     | Enter "12"    |
| D7           | GPIO 13     | Enter "13"    |
| D8           | GPIO 15     | Enter "15"    |

## Future Enhancements

Potential improvements:
- [ ] Add pin validation feedback in web UI
- [ ] Show visual indicator when strip reinitializes
- [ ] Add "Test LED" button to validate new pin
- [ ] Support for multiple LED strips on different pins

## Related Documentation

- [ESP8266_PIN_REFERENCE.md](ESP8266_PIN_REFERENCE.md) - Detailed pin information
- [ESP8266_MIGRATION.md](ESP8266_MIGRATION.md) - Migration guide
- [README.md](README.md) - Main project documentation

## Summary

The configurable data pin feature makes the ESP8266 Teams Presence device much more flexible:
- ✅ No recompilation needed to change pins
- ✅ Easy testing of different GPIO pins
- ✅ Better compatibility with different ESP8266 boards
- ✅ Runtime reconfiguration with automatic strip reinitialization

This is especially useful when:
- Working with different ESP8266 hardware variants
- Resolving GPIO conflicts with other peripherals
- Testing signal quality on different pins
- Deploying to multiple devices with different wiring
