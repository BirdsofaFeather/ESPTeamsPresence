# OLED Display Quick Start Guide

Get your OLED display working with ESPTeamsPresence in 5 easy steps!

## ⚡ Quick Setup (5 Minutes)

### Step 1: Gather Hardware
You need:
- ✅ Your ESP32 or ESP32-C3 board
- ✅ SSD1306 OLED Display (128x64, I2C)
  - Look for: "0.96 inch OLED" or "1.3 inch OLED"
- ✅ 4 jumper wires (Female-to-Female recommended)

### Step 2: Connect the Display

**For ESP32:**
```
OLED Display → ESP32
────────────────────
VCC → 3.3V
GND → GND
SDA → GPIO 21
SCL → GPIO 22
```

**For ESP32-C3:**
```
OLED Display → ESP32-C3
───────────────────────
VCC → 3.3V
GND → GND
SDA → GPIO 8
SCL → GPIO 9
```

💡 **Tip**: Most OLED modules have the pins labeled on the back!

### Step 3: Upload the Code

The code already includes OLED support! Just upload as normal:

**Using PlatformIO:**
```bash
pio run -t upload
```

**Using VS Code:**
- Click "Upload" button in PlatformIO toolbar

### Step 4: Watch the Display

After upload, you should see:
1. ✅ "ESPTeamsPresence Initializing..." - during boot
2. ✅ WiFi status - when connecting
3. ✅ Teams presence - after authentication

### Step 5: Enjoy!

That's it! Your display will now show:
- Current Teams status in LARGE text
- WiFi network name
- System messages

---

## 🔧 Troubleshooting

### Display is blank?

**Check 1: Power**
- Is the display getting power? Some modules have a power LED
- Try connecting VCC to 5V instead of 3.3V (if your module supports it)

**Check 2: Wiring**
- Double-check all 4 connections
- Make sure SDA/SCL aren't swapped

**Check 3: Serial Monitor**
Open serial monitor (115200 baud) and look for:
```
OLED Display initialized
```

If you see "SSD1306 allocation failed", there's a wiring or hardware issue.

**Check 4: I2C Address**
Some displays use address 0x3D instead of 0x3C.

To fix: Edit `main.cpp` and change:
```cpp
#define SCREEN_ADDRESS 0x3C
```
to:
```cpp
#define SCREEN_ADDRESS 0x3D
```

---

## 📊 What You'll See

### Normal Operation
```
Teams Presence
──────────────
  Available
Status: Free

WiFi: YourNetwork
```

### During Calls
```
Teams Presence
──────────────
  In Call
Status: On Phone

WiFi: YourNetwork
```

### All Status Messages
The display shows these Teams statuses:
- ✅ Available (Green LED)
- 🟡 Away (Yellow LED)
- 🟣 Busy (Purple LED)
- 🔴 In Call (Red LED)
- 🔴 In Meeting (Red LED)
- 🟠 Be Right Back (Orange LED)
- 🔴 Do Not Disturb (Pink LED)
- 🔴 Presenting (Red LED)
- ⚫ Offline (LEDs off)

---

## 🎨 Customization (Optional)

### Change I2C Pins

To use different GPIO pins, edit `initDisplay()` in `main.cpp`:

```cpp
void initDisplay() {
  // Change these numbers to your desired pins
  Wire.begin(YOUR_SDA_PIN, YOUR_SCL_PIN);
  // ... rest of function
}
```

### Disable Display

To turn off the display without removing hardware:

Comment out this line in `setup()`:
```cpp
// initDisplay();  // Display disabled
```

---

## 📚 More Information

- [Full OLED Documentation](OLED-DISPLAY.md) - Complete feature list
- [Wiring Diagrams](OLED-WIRING.md) - Detailed connection info  
- [Screen Examples](OLED-SCREENS.md) - All display states
- [Integration Summary](OLED-INTEGRATION-SUMMARY.md) - Technical details

---

## ❓ Common Questions

**Q: Do I need to modify the code?**
A: No! OLED support is built-in. Just connect and upload.

**Q: Can I use a different size OLED?**
A: The code is optimized for 128x64. For 128x32, you'll need to adjust the layout.

**Q: Will this slow down my device?**
A: No, the display updates are very fast and don't affect polling.

**Q: Can I use I2C for other things too?**
A: Yes! I2C is a bus, so you can add more I2C devices on the same pins.

**Q: My display shows garbage/random pixels?**
A: Check your connections and verify you have the correct display resolution (128x64).

**Q: Does this work with ESP8266?**
A: This code is for ESP32/ESP32-C3. For ESP8266, you'll need to adjust the pin assignments.

---

## 🎯 Pro Tips

1. **Use short wires** - Keep I2C wires under 20cm for best reliability
2. **Secure connections** - Use a breadboard or soldering for permanent installs
3. **Test first** - Verify display works before final assembly
4. **Add a case** - 3D print or buy a case to protect your display
5. **Monitor serial output** - Helps debug if anything goes wrong

---

## ✅ Success Checklist

Before asking for help, verify:
- [ ] Display is getting power (3.3V or 5V)
- [ ] All 4 wires are connected firmly
- [ ] Correct pins for your ESP32 variant
- [ ] Code uploaded successfully
- [ ] Serial monitor shows "OLED Display initialized"
- [ ] I2C address is correct (0x3C or 0x3D)

---

## 🎉 You're Done!

Enjoy your new Teams Presence display! 

For issues or questions:
- Check [Troubleshooting Guide](OLED-DISPLAY.md#troubleshooting)
- Review [Wiring Diagrams](OLED-WIRING.md)
- Open an issue on GitHub
