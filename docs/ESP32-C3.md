# ESP32-C3 Support

This document describes ESP32-C3 support in ESPTeamsPresence.

## About ESP32-C3

The ESP32-C3 is a cost-effective, RISC-V based microcontroller from Espressif. Key differences from the standard ESP32:

- **Architecture**: RISC-V 32-bit single-core (vs. Xtensa dual-core)
- **Clock Speed**: Up to 160 MHz
- **RAM**: 400 KB SRAM (vs. 320 KB on ESP32)
- **WiFi**: 2.4 GHz 802.11 b/g/n
- **GPIO**: 22 programmable GPIOs
- **RMT**: Different RMT peripheral implementation

## Supported Boards

The following ESP32-C3 boards are preconfigured:

- **esp32-c3-devkitm-1** - ESP32-C3-DevKitM-1 development board
- Other ESP32-C3 boards should also work with minor modifications

## Building for ESP32-C3

### Option 1: With Certificate Checking (Recommended)

```bash
pio run -e esp32-c3-devkitm-1
```

### Option 2: Without Certificate Checking

```bash
pio run -e esp32-c3-devkitm-1-nocertcheck
```

**Note**: Disabling certificate checking is less secure and should only be used for testing or if you're having certificate issues.

## Uploading to ESP32-C3

```bash
# Upload with certificate checking
pio run -e esp32-c3-devkitm-1 -t upload

# Upload without certificate checking
pio run -e esp32-c3-devkitm-1-nocertcheck -t upload
```

## GPIO Configuration

The default GPIO pin for the NeoPixel data line is **GPIO 13**, which can be changed in `platformio.ini`:

```ini
build_flags=
    ${env.build_flags}
    -DESP32C3
    -DDATAPIN=13  # Change this to your desired GPIO
    -DNUMLEDS=16  # Change this to match your LED count
```

### Recommended GPIO Pins

Safe GPIO pins for ESP32-C3:
- GPIO 0-10 (avoid GPIO 2, 8, 9 during boot)
- GPIO 18-21

**Avoid**: GPIO 11-17 (SPI flash pins), GPIO 2/8/9 (used during boot)

## Hardware Differences Handled

The code automatically handles these ESP32-C3 specific differences:

1. **Single-Core Architecture**: Task creation uses `xTaskCreate()` instead of `xTaskCreatePinnedToCore()`
2. **RMT Clock Source**: ESP32-C3 uses a 40MHz APB clock (vs. 80MHz on ESP32)
3. **Memory Display**: Shows correct RAM size (400KB) in the web interface

## Wiring Example

For a typical WS2812B LED strip:

```
ESP32-C3 DevKitM-1    WS2812B Strip
─────────────────    ──────────────
    GPIO 13      →   DIN (Data In)
    5V           →   VCC
    GND          →   GND
```

**Important**: For longer LED strips or higher LED counts, use an external 5V power supply and connect grounds together.

## Known Limitations

1. **Single Core**: The ESP32-C3 has only one core, so the NeoPixel task runs on the same core as the main application (vs. pinned to core 0 on dual-core ESP32)
2. **GPIO Count**: Fewer GPIO pins available compared to standard ESP32
3. **USB Serial**: Some ESP32-C3 boards use USB serial, which may require different drivers

## Troubleshooting

### Build Issues

If you encounter build errors:

```bash
# Clean the build
pio run -e esp32-c3-devkitm-1 -t clean

# Update platform
pio pkg update
```

### Upload Issues

If upload fails:
1. Press and hold the **BOOT** button while connecting USB
2. Release after the device is recognized
3. Try a different USB cable or port

### Certificate Issues

If you see SSL/TLS certificate errors:
1. Use the `nocertcheck` environment for testing
2. Check your system time is correct
3. Ensure you have a stable internet connection

## Performance Notes

The ESP32-C3 performs well for this application despite being single-core:
- WiFi connectivity is stable
- NeoPixel animations are smooth
- Presence polling works reliably

## Further Information

- [ESP32-C3 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-c3_datasheet_en.pdf)
- [ESP32-C3 Technical Reference Manual](https://www.espressif.com/sites/default/files/documentation/esp32-c3_technical_reference_manual_en.pdf)
- [PlatformIO ESP32 Platform](https://docs.platformio.org/en/latest/platforms/espressif32.html)
