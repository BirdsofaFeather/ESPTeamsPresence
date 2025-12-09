# OLED Display Screen Examples

This document shows what the OLED display will look like at different stages of operation.

## Display Specifications
- **Resolution**: 128x64 pixels
- **Colors**: Monochrome (white text on black background)
- **Size**: Typically 0.96" or 1.3" diagonal

---

## Screen 1: Initialization (Startup)

```
┌────────────────────┐
│ESPTeamsPresence    │
│Initializing...     │
│                    │
│                    │
│                    │
│                    │
│                    │
└────────────────────┘
```

**When shown**: During device boot-up and initialization

---

## Screen 2: AP Mode (Configuration Mode)

```
┌────────────────────┐
│ESPTeamsPresence    │
│────────────────────│
│AP Mode             │
│                    │
│Connect to WiFi:    │
│ESPTeamsPresence    │
│                    │
└────────────────────┘
```

**When shown**: When device is in Access Point mode for initial setup

---

## Screen 3: WiFi Connecting

```
┌────────────────────┐
│ESPTeamsPresence    │
│────────────────────│
│Connecting...       │
│                    │
│Joining WiFi        │
│                    │
│                    │
└────────────────────┘
```

**When shown**: While connecting to configured WiFi network

---

## Screen 4: WiFi Connected

```
┌────────────────────┐
│ESPTeamsPresence    │
│────────────────────│
│WiFi Connected      │
│                    │
│MyHomeNetwork       │
│192.168.1.42        │
│                    │
└────────────────────┘
```

**When shown**: After successful WiFi connection
- Line 3: "WiFi Connected" message
- Line 5: WiFi SSID name
- Line 6: Assigned IP address

---

## Screen 5: Device Login Prompt

```
┌────────────────────┐
│ESPTeamsPresence    │
│────────────────────│
│Device Login        │
│                    │
│Use web portal      │
│to authenticate     │
│                    │
└────────────────────┘
```

**When shown**: During Microsoft device login flow

---

## Screen 6: Token Refresh

```
┌────────────────────┐
│ESPTeamsPresence    │
│────────────────────│
│Refreshing          │
│                    │
│Token refresh...    │
│                    │
│                    │
└────────────────────┘
```

**When shown**: When refreshing authentication token

---

## Screen 7: Teams Status - Available

```
┌────────────────────┐
│Teams Presence      │
│────────────────────│
│                    │
│  Available         │
│                    │
│Status: Free        │
│                    │
│WiFi: MyHomeNetwork │
└────────────────────┘
```

**When shown**: When Teams status is "Available"
- Line 1: Title (small text)
- Line 2: Horizontal divider line
- Line 4: Status in LARGE text
- Line 6: Descriptive subtitle
- Line 8: WiFi network name

---

## Screen 8: Teams Status - Busy

```
┌────────────────────┐
│Teams Presence      │
│────────────────────│
│                    │
│  Busy              │
│                    │
│Status: Working     │
│                    │
│WiFi: MyHomeNetwork │
└────────────────────┘
```

**When shown**: When Teams status is "Busy"

---

## Screen 9: Teams Status - In Call

```
┌────────────────────┐
│Teams Presence      │
│────────────────────│
│                    │
│  In Call           │
│                    │
│Status: On Phone    │
│                    │
│WiFi: MyHomeNetwork │
└────────────────────┘
```

**When shown**: When in a Teams call or conference

---

## Screen 10: Teams Status - In Meeting

```
┌────────────────────┐
│Teams Presence      │
│────────────────────│
│                    │
│  Meeting           │
│                    │
│Status: In Meeting  │
│                    │
│WiFi: MyHomeNetwork │
└────────────────────┘
```

**When shown**: When in a Teams meeting

---

## Screen 11: Teams Status - Away

```
┌────────────────────┐
│Teams Presence      │
│────────────────────│
│                    │
│  Away              │
│                    │
│Status: Inactive    │
│                    │
│WiFi: MyHomeNetwork │
└────────────────────┘
```

**When shown**: When Teams status is "Away"

---

## Screen 12: Teams Status - Be Right Back

```
┌────────────────────┐
│Teams Presence      │
│────────────────────│
│                    │
│  Be Right          │
│  Back              │
│Status: BRB         │
│                    │
│WiFi: MyHomeNetwork │
└────────────────────┘
```

**When shown**: When Teams status is "Be Right Back"
- Note: This status uses two lines due to longer text

---

## Screen 13: Teams Status - Do Not Disturb

```
┌────────────────────┐
│Teams Presence      │
│────────────────────│
│                    │
│  DND               │
│                    │
│Do Not Disturb      │
│                    │
│WiFi: MyHomeNetwork │
└────────────────────┘
```

**When shown**: When in "Do Not Disturb" or "Urgent Interruptions Only" mode

---

## Screen 14: Teams Status - Presenting

```
┌────────────────────┐
│Teams Presence      │
│────────────────────│
│                    │
│  Present           │
│                    │
│Status: Presenting  │
│                    │
│WiFi: MyHomeNetwork │
└────────────────────┘
```

**When shown**: When presenting in Teams

---

## Screen 15: Teams Status - Offline

```
┌────────────────────┐
│Teams Presence      │
│────────────────────│
│                    │
│  Offline           │
│                    │
│Status: Not Online  │
│                    │
│WiFi: MyHomeNetwork │
└────────────────────┘
```

**When shown**: When offline, off work, or out of office

---

## Text Size Reference

The display uses different text sizes for visual hierarchy:

- **Title** (Line 1): Small text (6x8 pixels per character)
- **Status** (Large centered): Large text (12x16 pixels per character) 
- **Subtitle/Info**: Small text (6x8 pixels per character)
- **WiFi Info**: Small text (6x8 pixels per character)

## Layout Breakdown

```
┌────────────────────┐  ←─ Pixel 0 (Top)
│Teams Presence      │  ←─ Line 1: Title (pixels 0-10)
│────────────────────│  ←─ Line 2: Divider (pixel 10)
│                    │  
│  [LARGE STATUS]    │  ←─ Lines 3-4: Main status (pixels 15-31)
│                    │  
│Status: [Detail]    │  ←─ Line 6: Status detail (pixels 35-43)
│                    │  
│WiFi: [SSID]        │  ←─ Line 8: WiFi info (pixels 56-63)
└────────────────────┘  ←─ Pixel 63 (Bottom)
     128 pixels wide
```

## Color Mapping

Since the OLED is monochrome:
- **White pixels**: Text and graphics
- **Black pixels**: Background

On actual hardware, this appears as:
- **Bright blue/white**: Active pixels (depending on OLED type)
- **Black**: Inactive pixels

## Display Refresh Rate

The display updates:
- **Immediately**: On state changes (WiFi, login, etc.)
- **Every polling interval**: When Teams presence is refreshed (default: 30 seconds)
- **No flicker**: Display is only redrawn when content changes

## Physical Appearance Note

The actual OLED display will have:
- Crisp, bright white text on pure black background
- Possible slight blue tint (common for blue OLED modules)
- High contrast ratio
- Wide viewing angle
- No backlight bleed (pixels are self-illuminating)
