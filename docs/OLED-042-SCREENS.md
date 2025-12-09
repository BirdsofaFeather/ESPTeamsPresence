# 0.42" OLED Display Screens (72x40 pixels)

Visual reference for what your small OLED will display.

## Screen Dimensions
```
┌────────────┐  ← 72 pixels wide
│            │
│  72 x 40   │  ← 40 pixels tall
│            │
└────────────┘
```

---

## 1. Startup Screen

```
┌────────────┐
│Teams       │  Line 1 (0-7)
│Init...     │  Line 2 (8-15)
│            │
│            │
│            │
└────────────┘
```

---

## 2. Teams Status Screens

### Available (Free)
```
┌────────────┐
│Teams       │  ← Small text
│            │
│  Free      │  ← LARGE text (2x size)
│            │
│MyNetwork   │  ← WiFi SSID
└────────────┘
```

### Busy
```
┌────────────┐
│Teams       │
│            │
│  Busy      │  ← LARGE text
│            │
│MyNetwork   │
└────────────┘
```

### In Call
```
┌────────────┐
│Teams       │
│            │
│  Call      │  ← LARGE text (abbreviated)
│            │
│MyNetwork   │
└────────────┘
```

### In Meeting
```
┌────────────┐
│Teams       │
│            │
│Meeting     │  ← Small text (word too long for large)
│            │
│MyNetwork   │
└────────────┘
```

### Away
```
┌────────────┐
│Teams       │
│            │
│  Away      │  ← LARGE text
│            │
│MyNetwork   │
└────────────┘
```

### Be Right Back
```
┌────────────┐
│Teams       │
│            │
│BRB         │  ← Abbreviated
│            │
│MyNetwork   │
└────────────┘
```

### Do Not Disturb
```
┌────────────┐
│Teams       │
│            │
│  DND       │  ← LARGE text (abbreviated)
│            │
│MyNetwork   │
└────────────┘
```

### Presenting
```
┌────────────┐
│Teams       │
│            │
│Present     │  ← Small text
│            │
│MyNetwork   │
└────────────┘
```

### Offline
```
┌────────────┐
│Teams       │
│            │
│Offline     │  ← Small text
│            │
│MyNetwork   │
└────────────┘
```

---

## 3. System Status Screens

### WiFi Connecting
```
┌────────────┐
│Teams       │
│Connecting..│
│Joining WiFi│
│            │
│            │
└────────────┘
```

### WiFi Connected
```
┌────────────┐
│Teams       │
│WiFi OK     │
│MyNetwork   │
│192.168.1.42│  ← IP (if fits)
│            │
└────────────┘
```

### AP Mode
```
┌────────────┐
│Teams       │
│AP Mode     │
│Connect to: │
│TeamsPres   │  ← Truncated SSID
│            │
└────────────┘
```

### Device Login
```
┌────────────┐
│Teams       │
│Device Login│
│Use portal  │
│            │
│            │
└────────────┘
```

### Token Refresh
```
┌────────────┐
│Teams       │
│Refreshing  │
│Token...    │
│            │
│            │
└────────────┘
```

---

## Layout Breakdown

```
Row 0-7:    ┌────────────┐
            │Teams       │  ← Title (always visible)
            ├────────────┤
Row 8-9:    │            │  ← Spacing
            ├────────────┤
Row 10-27:  │  STATUS    │  ← Main content
            │            │    (Large text: 12x16 pixels)
            │            │    (Small text: 6x8 pixels)
            ├────────────┤
Row 28-31:  │            │  ← Spacing
            ├────────────┤
Row 32-39:  │WiFi/Info   │  ← Bottom info line
            └────────────┘
```

## Text Sizing

**Size 1 (Small)**: 6x8 pixels per character
- Fits ~12 characters per line
- Used for: Title, WiFi info, longer status words

**Size 2 (Large)**: 12x16 pixels per character  
- Fits ~6 characters per line
- Used for: Short statuses (Free, Busy, Call, Away, DND)

## WiFi SSID Display

Due to limited space, WiFi names are truncated:

| Actual SSID | Displayed As |
|-------------|--------------|
| MyHomeNetwork | MyHomeNetwor |
| Office-Wifi-5G | Office-Wifi- |
| Home | Home |
| SuperLongNetworkName | SuperLongNet |

Maximum: **12 characters**

---

## Pixel-Perfect Layout

For developers wanting exact positioning:

```
Y-Position Guide (pixels from top):
0:  Start of "Teams" title
10: Start of main status text
32: Start of WiFi/info line

X-Position:
0:  Left edge (all text left-aligned)
72: Right edge

Available area:
- Width: 72 pixels
- Height: 40 pixels
- Usable text area: ~3-4 lines (depending on text size)
```

---

## Comparison to Larger Displays

### Your 0.42" Display (72x40)
```
┌────────────┐
│Teams       │
│  Free      │
│MyNetwork   │
└────────────┘
   Compact!
```

### Standard 0.96" Display (128x64)
```
┌──────────────────────┐
│Teams Presence        │
│──────────────────────│
│                      │
│    Available         │
│                      │
│Status: Free          │
│                      │
│WiFi: MyHomeNetwork   │
└──────────────────────┘
     Much more space!
```

---

## Tips for Small Display

1. **Keep it simple**: Less information = more readable
2. **Large text when possible**: Easier to read at a glance  
3. **Abbreviations**: DND, BRB, etc. save space
4. **Strategic placement**: Most important info largest/center

---

## What You'll Actually See

The physical display will show:
- ✨ **Bright white text** on black background
- 📏 **Very small** - about the size of your fingernail
- 👁️ **High contrast** - easy to read despite small size
- ⚡ **Instant updates** - no lag when status changes

---

**Note**: While small, this display is perfect for at-a-glance status checking. The large text modes (Free, Busy, Call, etc.) are very readable even from a distance!
