# 3D Printable Enclosures for ESP8266 Teams Presence

## Overview

Two enclosure designs for the ESP8266 Teams Presence device with 48mm LED ring:

1. **2-Piece Design** - Base + translucent diffuser lid
2. **1-Piece Design** - Single body with integrated dome + optional bottom cover

Both designs feature:
- Light diffusion for ambient glow
- Proper ventilation
- Easy assembly
- USB port access
- Professional appearance

---

## Hardware Compatibility

### ESP8266 Board
- Dimensions: 25.5mm x 34.6mm x 1.36in (height)
- Compatible with: NodeMCU, Wemos D1 Mini, generic ESP8266
- Power: USB-C connection

### LED Ring
- Outer diameter: 48mm
- Inner diameter: 27mm  
- Thickness: ~2mm
- Compatible with WS2812B ring lights

---

## Design 1: 2-Piece Enclosure

![2-Piece Design](../artifacts/enclosure_2piece_design.png)

### Features
- **Base**: Houses ESP8266 with mounting posts
- **Lid**: Translucent diffuser with LED ring recess
- **Assembly**: Snap-fit connection
- **Dimensions**: 60mm diameter x 58mm total height

### Components

#### Base (bottom piece)
- Height: 40mm
- Internal cavity for ESP8266
- 4x M3 screw post holes for secure mounting
- USB-C port cutout (9mm x 3.5mm)
- Wire routing channels
- Snap-fit rim at top

#### Lid (top piece)  
- Height: 18mm
- Frosted/translucent for light diffusion
- 48mm recess for LED ring
- Domed top for aesthetics
- Ventilation slots
- Snap-fit groove

### Printing Instructions

**Base:**
```
Material: PLA or PETG
Layer height: 0.2mm
Infill: 20%
Supports: None needed
Orientation: Print upright (as modeled)
Color: Any opaque color (black, white, gray recommended)
```

**Lid:**
```
Material: Natural/Clear PLA or translucent PETG
Layer height: 0.15-0.2mm  
Infill: 15% (for light diffusion)
Supports: None needed
Orientation: Print upside down (snap-fit side up)
Color: Clear, white, or natural for best diffusion
Print settings: Reduce print speed for better clarity
```

### Assembly

1. **Install ESP8266**:
   - Insert board into base cavity
   - Optionally secure with M3 screws through posts
   - Route wires to LED area

2. **Mount LED Ring**:
   - Place LED ring in lid recess
   - Connect wires from ESP8266
   - Ensure data wire reaches pad on ring

3. **Close Enclosure**:
   - Align lid with base
   - Press down until snap-fit clicks
   - Should be secure but removable

---

## Design 2: 1-Piece Enclosure

### Features
- **Single Print**: Entire enclosure in one piece
- **Internal Platforms**: Separate shelves for ESP8266 and LED
- **Integrated Dome**: Built-in light diffuser
- **Bottom Cover**: Optional snap-on plate
- **Dimensions**: 60mm diameter x 55mm height

### Components

#### Main Body
- Total height: 55mm
- Open bottom for access
- ESP8266 platform at 15mm height
- LED ring platform at 35mm height
- Integrated translucent dome (top 15mm)
- USB-C port cutout
- Wire routing channels
- Ventilation slots

#### Bottom Cover (optional)
- Height: 3mm
- Snap-fit attachment
- "Teams" labelarea
- Rubber feet mounting points

### Printing Instructions

**Main Enclosure:**
```
Material: PLA or PETG
Layer height: 0.2mm
Infill: 20% (solid walls, 10% dome for transparency)
Supports: Required for internal platforms
Orientation: Print upright
Special: Use variable infill - 20% for walls, 10% for dome
Color: Base in any color, consider translucent filament for dome
```

**Dome Section (top 15mm):**
For best light diffusion, use one of these techniques:
- Clear/natural filament with low infill (10%)
- White translucent PLA
- Print slower at lower temp for clarity
- OR: Print separately and glue on

**Bottom Cover:**
```
Material: Match main body
Layer height: 0.2mm
Infill: 20%
Supports: None
Orientation: Print upside down (flat side down)
```

### Assembly

1. **Remove Supports**:
   - Carefully remove support material from platforms
   - Clean up any stringing

2. **Install ESP8266**:
   - Slide board onto lower platform
   - Secure with screws if desired
   - Route wires up to LED platform

3. **Mount LED Ring**:
   - Place ring on upper platform
   - Use retaining clips or glue lightly
   - Connect wires to ESP8266

4. **Attach Cover** (optional):
   - Snap bottom cover onto enclosure
   - Press firmly until clicks

---

## Light Diffusion Techniques

### Option 1: Translucent Filament
- Use natural/clear PLA or PETG
- Print lid/dome at 10-15% infill
- Slower print speed for clarity

### Option 2: Post-Processing
1. Print in white PLA at 100% infill
2. Sand with 400-800 grit sandpaper
3. Optional: apply clear matte spray

### Option 3: Frosted Acrylic Insert
1. Print with larger LED cutout
2. Cut frosted acrylic disk (48mm)
3. Insert between LED and lid

---

## Bill of Materials (BOM)

### For Either Design

**Electronics:**
- 1x ESP8266 board (NodeMCU or similar)
- 1x WS2812B 48mm LED ring
- 1x USB-C cable (for power and programming)
- 3x Jumper wires (3-4 inches)

**Hardware:**
- 4x M3 x 8mm screws (optional, for secure mounting)
- 4x Rubber feet (optional, for bottom)

**Filament Required:**

2-Piece:
- Base: ~30g PLA
- Lid: ~15g translucent PLA
- **Total: ~45g**

1-Piece:
- Main: ~40g PLA + translucent
- Cover: ~8g PLA
- **Total: ~48g**

---

## Customization Parameters

Both designs use OpenSCAD parametric models. Easily modify:

```scad
outer_diameter = 60;      // Change overall size
esp_width = 26;           // Adjust for your board
led_outer_diameter = 48;  // Match your LED ring
wall_thickness = 2.5;     // Structural strength
```

### Common Modifications

**Larger LED Rings** (e.g., 60mm):
```scad
led_outer_diameter = 60;
outer_diameter = 70;  // Adjust proportionally
```

**Different ESP8266 Boards**:
```scad
esp_width = 30;   // Wemos D1 Mini
esp_length = 34;  
```

**Taller for More Components**:
```scad
base_height = 50;  // Add 10mm
```

---

## Comparison: Which Design to Choose?

### 2-Piece Design
**Pros:**
✅ Easier to print (no supports)
✅ Better light diffusion (whole lid is translucent)
✅ Easier assembly/disassembly
✅ More professional appearance
✅ Better ventilation

**Cons:**
❌ Requires two prints
❌ Slightly taller
❌ Snap-fit may wear over time

**Best for:** Cleaner build, better aesthetics, frequent access

### 1-Piece Design
**Pros:**
✅ Single print (less hassle)
✅ More compact
✅ All-in-one construction
✅ Optional bottom cover

**Cons:**
❌ Requires support material
❌ Trickier dome diffusion
❌ Harder to access internals
❌ Support cleanup time

**Best for:** Quick build, permanent installation, desk toy

---

## Printing Tips

### General
1. **First Layer**: Critical - ensure good bed adhesion
2. **Temperature**: PLA 200-210°C, PETG 230-245°C
3. **Speed**: 50mm/s for structural parts, 30mm/s for translucent
4. **Cooling**: 100% fan for PLA, 50% for PETG

### For Translucent Parts
- Print slightly cooler (5-10°C below normal)
- Reduce speed to 30-40mm/s
- Use minimal cooling
- Consider printing at night (less vibration = clearer)

### For Snap-Fits
- Calibrate printer well (tight tolerances)
- Print a test piece first
- Adjust `snap_tolerance` parameter if too tight/loose
- Consider acetone vapor smoothing (ABS only)

---

## Troubleshooting

### Snap-Fit Too Tight
- Increase `snap_tolerance` by 0.1mm increments
- Lightly sand snap-fit areas
- Ensure no stringing/blobs

### Snap-Fit Too Loose
- Decrease `snap_tolerance`
- Add layer of tape to groove
- Use glue for permanent assembly

### Light Not Diffusing Well
- Reduce dome/lid infill to 10%
- Use natural/translucent filament
- Sand exterior for frosted effect
- Print thinner walls (1.5mm vs 2.5mm)

### LED Ring Doesn't Fit
- Check actual LED dimensions with calipers
- Adjust `led_outer_diameter` parameter
- Add 1-2mm clearance if tight
- File or sand recess if needed

### ESP8266 Doesn't Fit
- Verify board dimensions
- Adjust `esp_width` and `esp_length`
- Some boards have antennas - add clearance
- Remove support material completely

---

## Advanced Modifications

### Add Logo/Label
Use OpenSCAD's `text()` function on lid:
```scad
translate([0, 0, lid_height - 1])
    linear_extrude(height=1)
        text("TEAMS", size=5, halign="center");
```

### RGB Glow Through Base
Add thin sections in base walls:
```scad
// Windows for side glow
for(angle = [0:90:270]) {
    rotate([0, 0, angle])
        translate([outer_diameter/2 - 1, -5, 15])
            cube([1, 10, 10]);
}
```

### Mounting Bracket
Add VESA-style mounting holes:
```scad
translate([0, 0, -5])
    difference() {
        cube([80, 80, 5], center=true);
        // Standard VESA patterns
    }
```

---

## Files Included

```
enclosure/
├── 2_piece_enclosure.scad    # Parametric source (2-piece)
├── 1_piece_enclosure.scad    # Parametric source (1-piece)
├── README.md                  # This file
└── stl/                       # Pre-exported STL files
    ├── 2piece_base.stl
    ├── 2piece_lid.stl
    ├── 1piece_main.stl
    └── 1piece_cover.stl
```

---

## License

These enclosure designs are released under the same license as the main project (MPL 2.0). 

Feel free to:
- Modify for your needs
- Share your remixes
- Sell printed versions (consider attribution)

---

## Contributing

Improvements welcome! Consider:
- Alternative mounting solutions
- Desk/wall mount versions
- Multi-color designs
- Different size LED rings
- Cable management solutions

---

## Credits

Enclosure designs: 2024
ESP8266 Teams Presence: https://github.com/toblum/ESPTeamsPresence

---

## Gallery

*Add photos of your builds here!*

Share your prints:
- Tag with #ESPTeamsPresence
- Submit photos via GitHub Issues
- Show off different color combinations
