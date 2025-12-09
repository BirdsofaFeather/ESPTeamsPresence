# OLED Display Integration

The ESPTeamsPresence now supports displaying the Teams Presence status on an SSD1306 OLED display (128x64 pixels).

## Hardware Requirements

- **OLED Display**: SSD1306-based OLED display (128x64 pixels)
  - Common models: 0.96" or 1.3" I2C OLED displays
  - I2C interface (SDA/SCL)
  - Usually operates at 3.3V or 5V

## Wiring

### ESP32 (Standard)
- **VCC** → 3.3V or 5V (depending on your OLED module)
- **GND** → GND
- **SDA** → GPIO 21
- **SCL** → GPIO 22

### ESP32-C3
- **VCC** → 3.3V or 5V (depending on your OLED module)
- **GND** → GND
- **SDA** → GPIO 8
- **SCL** → GPIO 9

## Display Features

The OLED display shows:

1. **Teams Presence Status** - Large text showing current activity:
   - Available
   - Busy
   - In Call / Meeting
   - Away
   - Be Right Back
   - Do Not Disturb (DND)
   - Presenting
   - Offline
   - Unknown

2. **Status Details** - Additional context below the main status

3. **WiFi Information** - Current WiFi SSID at the bottom of the screen

4. **System Messages** - During initialization and state changes:
   - AP Mode notification
   - WiFi connecting status
   - WiFi connected with IP address
   - Device login prompts
   - Token refresh notifications

## Display States

### Initialization
Shows "ESPTeamsPresence" and "Initializing..." message

### AP Mode
When the device is in Access Point mode:
```
ESPTeamsPresence
─────────────────
AP Mode
Connect to WiFi:
ESPTeamsPresence
```

### WiFi Connecting
```
ESPTeamsPresence
─────────────────
Connecting...
Joining WiFi
```

### WiFi Connected
```
ESPTeamsPresence
─────────────────
WiFi Connected
[SSID Name]
[IP Address]
```

### Teams Presence
```
Teams Presence
─────────────────
  Available
Status: Free

WiFi: [SSID]
```

## Customization

### Change I2C Address
If your OLED display uses a different I2C address (default is 0x3C), modify the `SCREEN_ADDRESS` definition in `main.cpp`:

```cpp
#define SCREEN_ADDRESS 0x3D // Change to your display's address
```

### Change I2C Pins
To use different GPIO pins for I2C, modify the `initDisplay()` function in `main.cpp`:

```cpp
// For ESP32
Wire.begin(SDA_PIN, SCL_PIN); // Replace with your desired pins

// For ESP32-C3
Wire.begin(SDA_PIN, SCL_PIN); // Replace with your desired pins
```

### Display Size
If you have a different size OLED (e.g., 128x32), update these definitions:

```cpp
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32  // Change to match your display
```

## Troubleshooting

### Display Not Working
1. **Check wiring** - Ensure SDA/SCL are connected to the correct GPIO pins
2. **Check I2C address** - Use an I2C scanner sketch to verify the address (usually 0x3C or 0x3D)
3. **Check power** - Ensure the display is receiving appropriate voltage (3.3V or 5V)
4. **Check serial monitor** - Look for "OLED Display initialized" message

### Display Shows Garbled Text
1. Verify the display resolution matches the code (128x64)
2. Check for loose connections
3. Try a different I2C address

### No Changes After Code Update
1. Clean build and re-upload: `pio run -t clean && pio run -t upload`
2. Monitor serial output for any error messages
3. Verify the libraries are installed correctly

## Dependencies

The OLED display feature uses these libraries (automatically installed via PlatformIO):
- `Adafruit SSD1306` - Display driver
- `Adafruit GFX Library` - Graphics library

## Performance

The OLED display updates occur:
- During state changes (WiFi, authentication, etc.)
- When Teams presence status is refreshed
- Minimal impact on polling performance

## Optional: Disable OLED Display

If you want to disable the OLED display without removing the code, you can comment out the initialization in `setup()`:

```cpp
// initDisplay();  // Comment out to disable OLED
```

The device will continue to function normally without the display.
