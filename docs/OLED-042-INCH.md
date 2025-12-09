# 0.42" OLED Display Setup Guide

## Your Display Module

Based on your hardware image, you have a **0.42-inch OLED display** integrated on the board.

### Display Specifications
- **Size**: 0.42 inches diagonal
- **Resolution**: 72x40 pixels
- **Driver**: SSD1306 (compatible)
- **Interface**: I2C
- **Colors**: Monochrome (white on black)

### Pin Configuration (From Your Image)

```
Display Pin → GPIO Pin
─────────────────────
SCL → GPIO6
SDA → GPIO5
```

The code has been updated to use these pins automatically!

## Wiring

Your display appears to be **already connected on the board** based on the image. 

If you need to confirm connections:
- **GPIO5** = SDA (Data line)
- **GPIO6** = SCL (Clock line)
- **GND** = Ground
- **VCC** = Power (3.3V)

## What You'll See

Due to the smaller 72x40 pixel display, the layout is simplified:

### Example: Available Status
```
┌────────────┐
│Teams       │  ← Title
│            │
│  Free      │  ← Status (large text when possible)
│            │
│YourNetwork │  ← WiFi SSID (truncated to 12 chars)
└────────────┘
```

### Example: In Call
```
┌────────────┐
│Teams       │
│            │
│  Call      │  ← Status abbreviated to fit
│            │
│YourNetwork │
└────────────┘
```

### All Status Displays

| Teams Status | Display Shows |
|--------------|---------------|
| Available | "Free" (size 2 text) |
| Busy | "Busy" (size 2 text) |
| InACall | "Call" (size 2 text) |
| InAMeeting | "Meeting" (size 1 text) |
| Away | "Away" (size 2 text) |
| BeRightBack | "BRB" (size 1 text) |
| DoNotDisturb | "DND" (size 2 text) |
| Presenting | "Present" (size 1 text) |
| Offline | "Offline" (size 1 text) |

## Testing

1. **Upload the code** to your ESP32-C3 board
2. **Open Serial Monitor** at 115200 baud
3. **Look for this message**:
   ```
   Initializing OLED on SDA=5, SCL=6
   OLED Display initialized
   ```

4. **Check the display** - you should see:
   ```
   Teams
   Init...
   ```

## Troubleshooting

### Display shows nothing?

1. **Check I2C address**
   - Most displays use `0x3C`
   - Some use `0x3D`
   
   If `0x3C` doesn't work, change this line in `main.cpp`:
   ```cpp
   #define SCREEN_ADDRESS 0x3D  // Change from 0x3C to 0x3D
   ```

2. **Check Serial Monitor output**
   - If you see "SSD1306 allocation failed", the display isn't responding
   - Verify I2C connections
   - Try power cycling the board

3. **Verify resolution**
   - The code is set for 72x40
   - If your display is different, update:
   ```cpp
   #define SCREEN_WIDTH 72
   #define SCREEN_HEIGHT 40
   ```

### Display shows garbled text?

- This usually means wrong resolution
- Verify your display is actually 72x40 pixels
- Some 0.42" displays might be different resolutions

### WiFi name is cut off?

- This is normal for small displays
- Only first 12 characters are shown
- The full connection works, just limited display space

## Display Layout Details

Due to the very small 72x40 pixel screen:

```
Pixel Rows:
0-7:   Title line "Teams"
8-9:   Blank spacing
10-23: Main status (large text if fits)
24-31: Blank spacing  
32-39: WiFi SSID (small text, max 12 chars)
```

## Configuration Summary

The code has been configured with:
```cpp
#define SCREEN_WIDTH 72
#define SCREEN_HEIGHT 40
#define OLED_SDA 5
#define OLED_SCL 6
#define SCREEN_ADDRESS 0x3C
```

## Serial Monitor Debug Output

When the display initializes, you'll see:
```
Initializing OLED on SDA=5, SCL=6
OLED Display initialized
```

When Teams status updates:
```
--> Availability: Available, Activity: Available

Teams
Free
YourNetwork
```

## Next Steps

1. ✅ Code is updated for your display
2. ✅ Build successful (compiled with no errors)
3. 📤 Upload to your ESP32-C3 board
4. 👁️ Watch the display come to life!

## Advanced: Customize Text

If you want to change what's displayed, edit these functions in `main.cpp`:

- `updateDisplay()` - Changes Teams status display
- `displayMessage()` - Changes system message display
- `initDisplay()` - Changes startup message

## Need Help?

If the display still doesn't work after trying these steps:

1. Share the **Serial Monitor output** (especially initialization messages)
2. Confirm the **I2C address** using an I2C scanner sketch
3. Verify the **display resolution** from the manufacturer specs
4. Check if display has a **separate power switch** or enable pin

---

**Note**: Your integrated display module is perfect for this project! The smaller size means simpler text, but you get a nice compact Teams presence indicator.
