# Quick Print Guide

## 2-Piece Enclosure (Recommended)

### Print Settings
```
Base (Opaque):
├─ Material: Black/White PLA
├─ Layer: 0.2mm
├─ Infill: 20%
├─ Supports: ❌ None
├─ Time: ~2.5 hours
└─ Weight: ~30g

Lid (Translucent):
├─ Material: Clear/Natural PLA
├─ Layer: 0.15mm
├─ Infill: 10-15% ⭐
├─ Supports: ❌ None
├─ Time: ~1.5 hours
└─ Weight: ~15g
```

### Assembly Steps
1. Insert ESP8266 into base
2. Route wires to top
3. Place LED ring in lid recess
4. Connect wires  
5. Snap lid onto base

---

## 1-Piece Enclosure (Compact)

### Print Settings
```
Main Body:
├─ Material: Any PLA/PETG
├─ Layer: 0.2mm
├─ Infill: 20% (walls), 10% (dome)
├─ Supports: ✅ Yes (platforms)
├─ Time: ~3.5 hours
└─ Weight: ~40g

Bottom Cover (Optional):
├─ Material: Match body
├─ Layer: 0.2mm
├─ Infill: 20%
├─ Supports: ❌ None
├─ Time: ~30 min
└─ Weight: ~8g
```

### Assembly Steps
1. Remove supports carefully
2. Slide ESP8266 onto lower platform
3. Route wires up
4. Place LED ring on upper platform
5. Snap on bottom cover

---

## Tips for Best Results

### Light Diffusion ✨
- Use **natural/clear PLA** for translucent parts
- **10-15% infill** for even glow
- Print at **30-40mm/s** (slower = clearer)
- Layer height: **0.15mm** preferred

### Dimensional Accuracy 📏
- Calibrate your printer first
- Check with **test cube** (20x20x20mm)
- Measure LED ring with calipers
- Adjust OpenSCAD parameters if needed

### Snap-Fit Tuning 🔧
If too tight:
- Sand lightly with 220 grit
- Increase `snap_tolerance` to 0.4mm

If too loose:
- Decrease `snap_tolerance` to 0.2mm
- Add layer of tape

---

## Material Recommendations

### For Diffuser Parts
🥇 **Natural PLA** - Best diffusion, easy print
🥈 **Clear PETG** - More durable, harder to print
🥉 **White PLA** - Good diffusion, opaque

### For Base/Body
🥇 **Black PLA** - Professional look
🥈 **White PLA** - Clean appearance  
🥉 **Gray PETG** - More durable

---

## Common Issues & Fixes

| Problem | Solution |
|---------|----------|
| Lid won't snap on | Increase tolerance, sand edges |
| Light bleeding unevenly | Lower infill, print slower |
| ESP8266 won't fit | Remove all supports, file cavity |
| LED ring loose | Add glue dots or tighten clips |
| USB port blocked | Verify cutout position, file if needed |

---

## Customization Quick Ref

```scad
// In OpenSCAD, modify these:
outer_diameter = 60;        // Overall size
led_outer_diameter = 48;    // Your LED ring size
esp_width = 26;             // Your ESP board width
esp_length = 35;            // Your ESP board length
wall_thickness = 2.5;       // Structure strength
snap_tolerance = 0.3;       // Fit adjustment
```

---

## Print Time Estimates

| Design | Parts | Total Time | Total Weight |
|--------|-------|------------|--------------|
| 2-Piece | Base + Lid | ~4 hours | ~45g |
| 1-Piece | Main + Cover | ~4 hours | ~48g |

*Based on 50mm/s print speed, 0.2mm layers*

---

## Next Steps

1. ✅ Choose your design (2-piece recommended)
2. ✅ Download .scad files from `/enclosure/`
3. ✅ Open in OpenSCAD (free: openscad.org)
4. ✅ Export to STL (F6, then F7)
5. ✅ Slice in your slicer
6. ✅ Print!
7. ✅ Assemble and enjoy 🎉
