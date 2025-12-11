# ESP8266 Teams Presence Light

[![License: MPL 2.0](https://img.shields.io/badge/License-MPL%202.0-brightgreen.svg)](https://opensource.org/licenses/MPL-2.0)
![](https://github.com/toblum/ESPTeamsPresence/workflows/BuildAndRelease/badge.svg)
![](https://img.shields.io/github/v/release/toblum/ESPTeamsPresence)

## Project Status: Paused
Unfortunately, I am no longer able to actively support or update this project. The primary reason for this is that I no longer have access to a dedicated M365 development tenant/sandbox, compounded by a general lack of time.

Please note that the longevity of the built-in app registration is uncertain, and you may need to create your own.

I will keep this repository online for anyone interested in its current state, but please understand that I can no longer provide active support. I may revisit this project in the future if circumstances change.

---

**A standalone Microsoft Teams presence light based on ESP8266 and RGB neopixel LEDs.**

> **Note:** This fork has been updated to support **ESP8266** microcontrollers (like NodeMCU) and uses the standard **Adafruit NeoPixel** library instead of WS2812FX. See [ESP8266_MIGRATION.md](ESP8266_MIGRATION.md) for details.

This project allows you to build a standalone device that visualizes your presence status from Microsoft Teams with colored LEDs. It's really easy to build and quite cheap.

See this video for a short overview:  

[![OverView](https://img.youtube.com/vi/MHl5En8YuxQ/0.jpg)](https://www.youtube.com/watch?v=MHl5En8YuxQ)

Some technical details:  
This projects implements the device login flow to authenticate against Microsoft Azure AD and to get a access token. Using this token, the device can call the Microsoft Graph API to get presence information for the authenticated user. The token is automatically refreshed so that it can run standalone for some time.

Everything is implemented in C code for Arduino-style microcontrollers and runs directly on the cheap and powerful WiFi-connected ESP32 board. [Getting the hardware](https://toblum.github.io/ESPTeamsPresence/#/buy) is usually no problem.

[![Build and Setup](https://img.youtube.com/vi/DH3zN3nLk9w/0.jpg)](https://www.youtube.com/watch?v=DH3zN3nLk9w)

The build and setup procedure is extremely easy and well documented. The device consists of only two active parts and three wires and is powered via Micro-USB. It also features a cool retro-style web UI for configuration.

[![UI](https://img.youtube.com/vi/3qcatKaqbU4/0.jpg)](https://www.youtube.com/watch?v=3qcatKaqbU4)


## Libraries used
This project uses the following libraries from different authors:
- [IotWebConf](https://github.com/prampec/IotWebConf) by prampec
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson) by bblanchon
- [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) by Adafruit
- [Docute](https://github.com/egoist/docute) by egoist
- [NES.css](https://github.com/nostalgic-css/NES.css/) by nostalgic-css

Thanks to all the authors.


## Possible enhancements
- [ ] Make color / animation configurable via web UI.
- [ ] Make LED brightness configurable.
- [ ] Make LED PIN configurable.
- [ ] Allow manual setting of the status via UI / API.


## Licence
All code is licensed under the [MPLv2 License](https://github.com/toblum/ESPTeamsPresence/blob/master/LICENSE).
