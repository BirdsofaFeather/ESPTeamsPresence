/**
 * ESPTeamsPresence -- A standalone Microsoft Teams presence light
 *   based on ESP8266 and RGB neopixel LEDs.
 *   https://github.com/toblum/ESPTeamsPresence
 *
 * Copyright (C) 2020 Tobias Blum <make@tobiasblum.de>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <IotWebConf.h>
#include <LittleFS.h>
#include <WiFiClientSecure.h>

// Global settings
// #define NUMLEDS 16							//
// Number of LEDs on the strip (if not set via build flags) #define DATAPIN 26
// // GPIO pin used to drive the LED strip (20 == GPIO/D13) (if not set via
// build flags) #define DISABLECERTCHECK 1
// // Uncomment to disable https certificate checks (if not set via build flags)
// #define STATUS_PIN LED_BUILTIN				// User builtin
// LED for status (if not set via build flags)
#define DEFAULT_POLLING_PRESENCE_INTERVAL                                      \
  "30" // Default interval to poll for presence info (seconds)
#define DEFAULT_ERROR_RETRY_INTERVAL                                           \
  30 // Default interval to try again after errors
#define TOKEN_REFRESH_TIMEOUT                                                  \
  60 // Number of seconds until expiration before token gets refreshed
#define CONTEXT_FILE "/context.json" // Filename of the context file
#define VERSION "0.18.3"             // Version of the software

#define DBG_PRINT(x) Serial.print(x)
#define DBG_PRINTLN(x) Serial.println(x)

#ifndef DISABLECERTCHECK
// Tool to get certs: https://projects.petrucci.ch/esp32/

// certificate for https://graph.microsoft.com
// DigiCert Global Root CA, valid until Mon Sep 23 2030, size: 1761 bytes
const char *rootCACertificateGraph =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIE6DCCA9CgAwIBAgIQAnQuqhfKjiHHF7sf/P0MoDANBgkqhkiG9w0BAQsFADBh\n"
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
    "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n"
    "QTAeFw0yMDA5MjMwMDAwMDBaFw0zMDA5MjIyMzU5NTlaME0xCzAJBgNVBAYTAlVT\n"
    "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxJzAlBgNVBAMTHkRpZ2lDZXJ0IFNIQTIg\n"
    "U2VjdXJlIFNlcnZlciBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n"
    "ANyuWJBNwcQwFZA1W248ghX1LFy949v/cUP6ZCWA1O4Yok3wZtAKc24RmDYXZK83\n"
    "nf36QYSvx6+M/hpzTc8zl5CilodTgyu5pnVILR1WN3vaMTIa16yrBvSqXUu3R0bd\n"
    "KpPDkC55gIDvEwRqFDu1m5K+wgdlTvza/P96rtxcflUxDOg5B6TXvi/TC2rSsd9f\n"
    "/ld0Uzs1gN2ujkSYs58O09rg1/RrKatEp0tYhG2SS4HD2nOLEpdIkARFdRrdNzGX\n"
    "kujNVA075ME/OV4uuPNcfhCOhkEAjUVmR7ChZc6gqikJTvOX6+guqw9ypzAO+sf0\n"
    "/RR3w6RbKFfCs/mC/bdFWJsCAwEAAaOCAa4wggGqMB0GA1UdDgQWBBQPgGEcgjFh\n"
    "1S8o541GOLQs4cbZ4jAfBgNVHSMEGDAWgBQD3lA1VtFMu2bwo+IbG8OXsj3RVTAO\n"
    "BgNVHQ8BAf8EBAMCAYYwHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMBIG\n"
    "A1UdEwEB/wQIMAYBAf8CAQAwdgYIKwYBBQUHAQEEajBoMCQGCCsGAQUFBzABhhho\n"
    "dHRwOi8vb2NzcC5kaWdpY2VydC5jb20wQAYIKwYBBQUHMAKGNGh0dHA6Ly9jYWNl\n"
    "cnRzLmRpZ2ljZXJ0LmNvbS9EaWdpQ2VydEdsb2JhbFJvb3RDQS5jcnQwewYDVR0f\n"
    "BHQwcjA3oDWgM4YxaHR0cDovL2NybDMuZGlnaWNlcnQuY29tL0RpZ2lDZXJ0R2xv\n"
    "YmFsUm9vdENBLmNybDA3oDWgM4YxaHR0cDovL2NybDQuZGlnaWNlcnQuY29tL0Rp\n"
    "Z2lDZXJ0R2xvYmFsUm9vdENBLmNybDAwBgNVHSAEKTAnMAcGBWeBDAEBMAgGBmeB\n"
    "DAECATAIBgZngQwBAgIwCAYGZ4EMAQIDMA0GCSqGSIb3DQEBCwUAA4IBAQB3MR8I\n"
    "l9cSm2PSEWUIpvZlubj6kgPLoX7hyA2MPrQbkb4CCF6fWXF7Ef3gwOOPWdegUqHQ\n"
    "S1TSSJZI73fpKQbLQxCgLzwWji3+HlU87MOY7hgNI+gH9bMtxKtXc1r2G1O6+x/6\n"
    "vYzTUVEgR17vf5irF0LKhVyfIjc0RXbyQ14AniKDrN+v0ebHExfppGlkTIBn6rak\n"
    "f4994VH6npdn6mkus5CkHBXIrMtPKex6XF2firjUDLuU7tC8y7WlHgjPxEEDDb0G\n"
    "w6D0yDdVSvG/5XlCNatBmO/8EznDu1vr72N8gJzISUZwa6CCUD7QBLbKJcXBBVVf\n"
    "8nwvV9GvlW+sbXlr\n"
    "-----END CERTIFICATE-----\n"
    "";

// certificate for https://login.microsoftonline.com
//  1 s:C = US, O = Microsoft Corporation, CN = Microsoft Azure RSA TLS Issuing
//  CA 04
//    i:C = US, O = DigiCert Inc, OU = www.digicert.com, CN = DigiCert Global
//    Root G2
const char *rootCACertificateLogin =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIFrDCCBJSgAwIBAgIQCfluwpVVXyR0nq8eXc7UnTANBgkqhkiG9w0BAQwFADBh\n"
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
    "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH\n"
    "MjAeFw0yMzA2MDgwMDAwMDBaFw0yNjA4MjUyMzU5NTlaMF0xCzAJBgNVBAYTAlVT\n"
    "MR4wHAYDVQQKExVNaWNyb3NvZnQgQ29ycG9yYXRpb24xLjAsBgNVBAMTJU1pY3Jv\n"
    "c29mdCBBenVyZSBSU0EgVExTIElzc3VpbmcgQ0EgMDQwggIiMA0GCSqGSIb3DQEB\n"
    "AQUAA4ICDwAwggIKAoICAQDBeUy13eRZ/QC5bN7/IOGxodny7Xm2BFc88d3cca3y\n"
    "HyyVx1Y60+afY6DAo/2Ls1uzAfbDfMzAVWJazPH4tckaItDv//htEbbNJnAGvZPB\n"
    "4VqNviwDEmlAWT/MTAmzXfTgWXuUNgRlzZbjoFaPm+t6iJ6HdvDpWQAJbsBUZCga\n"
    "t257tM28JnAHUTWdiDBn+2z6EGh2DA6BCx04zHDKVSegLY8+5P80Lqze0d6i3T2J\n"
    "J7rfxCmxUXfCGOv9iQIUZfhv4vCb8hsm/JdNUMiomJhSPa0bi3rda/swuJHCH//d\n"
    "wz2AGzZRRGdj7Kna4t6ToxK17lAF3Q6Qp368C9cE6JLMj+3UbY3umWCPRA5/Dms4\n"
    "/wl3GvDEw7HpyKsvRNPpjDZyiFzZGC2HZmGMsrZMT3hxmyQwmz1O3eGYdO5EIq1S\n"
    "W/vT1yShZTSusqmICQo5gWWRZTwCENekSbVX9qRr77o0pjKtuBMZTGQTixwpT/rg\n"
    "Ul7Mr4M2nqK55Kovy/kUN1znfPdW/Fj9iCuvPKwKFdyt2RVgxJDvgIF/bNoRkRxh\n"
    "wVB6qRgs4EiTrNbRoZAHEFF5wRBf9gWn9HeoI66VtdMZvJRH+0/FDWB4/zwxS16n\n"
    "nADJaVPXh6JHJFYs9p0wZmvct3GNdWrOLRAG2yzbfFZS8fJcX1PYxXXo4By16yGW\n"
    "hQIDAQABo4IBYjCCAV4wEgYDVR0TAQH/BAgwBgEB/wIBADAdBgNVHQ4EFgQUO3DR\n"
    "U+l2JZ1gqMpmD8abrm9UFmowHwYDVR0jBBgwFoAUTiJUIBiV5uNu5g/6+rkS7QYX\n"
    "jzkwDgYDVR0PAQH/BAQDAgGGMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcD\n"
    "AjB2BggrBgEFBQcBAQRqMGgwJAYIKwYBBQUHMAGGGGh0dHA6Ly9vY3NwLmRpZ2lj\n"
    "ZXJ0LmNvbTBABggrBgEFBQcwAoY0aHR0cDovL2NhY2VydHMuZGlnaWNlcnQuY29t\n"
    "L0RpZ2lDZXJ0R2xvYmFsUm9vdEcyLmNydDBCBgNVHR8EOzA5MDegNaAzhjFodHRw\n"
    "Oi8vY3JsMy5kaWdpY2VydC5jb20vRGlnaUNlcnRHbG9iYWxSb290RzIuY3JsMB0G\n"
    "A1UdIAQWMBQwCAYGZ4EMAQIBMAgGBmeBDAECAjANBgkqhkiG9w0BAQwFAAOCAQEA\n"
    "o9sJvBNLQSJ1e7VaG3cSZHBz6zjS70A1gVO1pqsmX34BWDPz1TAlOyJiLlA+eUF4\n"
    "B2OWHd3F//dJJ/3TaCFunjBhZudv3busl7flz42K/BG/eOdlg0kiUf07PCYY5/FK\n"
    "YTIch51j1moFlBqbglwkdNIVae2tOu0OdX2JiA+bprYcGxa7eayLetvPiA77ynTc\n"
    "UNMKOqYB41FZHOXe5IXDI5t2RsDM9dMEZv4+cOb9G9qXcgDar1AzPHEt/39335zC\n"
    "HofQ0QuItCDCDzahWZci9Nn9hb/SvAtPWHZLkLBG6I0iwGxvMwcTTc9Jnb4Flysr\n"
    "mQlwKsS2MphOoI23Qq3cSA==\n"
    "-----END CERTIFICATE-----\n"
    "";
#endif

// IotWebConf
// -- Initial name of the Thing. Used e.g. as SSID of the own Access Point.
const char thingName[] = "ESPTeamsPresence";
// -- Initial password to connect to the Thing, when it creates an own Access
// Point.
const char wifiInitialApPassword[] = "presence";

DNSServer dnsServer;
WebServer server(80);

IotWebConf iotWebConf(thingName, &dnsServer, &server, wifiInitialApPassword);

// Add parameter
#define STRING_LEN 64
#define INTEGER_LEN 16
char paramClientIdValue[STRING_LEN];
char paramTenantValue[STRING_LEN];
char paramPollIntervalValue[INTEGER_LEN];
char paramNumLedsValue[INTEGER_LEN];
char paramDataPinValue[INTEGER_LEN];
IotWebConfSeparator separator = IotWebConfSeparator();
IotWebConfParameter paramClientId = IotWebConfParameter(
    "Client-ID (Generic ID: 3837bbf0-30fb-47ad-bce8-f460ba9880c3)", "clientId",
    paramClientIdValue, STRING_LEN, "text",
    "e.g. 3837bbf0-30fb-47ad-bce8-f460ba9880c3",
    "3837bbf0-30fb-47ad-bce8-f460ba9880c3");
IotWebConfParameter paramTenant =
    IotWebConfParameter("Tenant hostname / ID", "tenantId", paramTenantValue,
                        STRING_LEN, "text", "e.g. contoso.onmicrosoft.com");
IotWebConfParameter paramPollInterval = IotWebConfParameter(
    "Presence polling interval (sec) (default: 30)", "pollInterval",
    paramPollIntervalValue, INTEGER_LEN, "number", "5..300",
    DEFAULT_POLLING_PRESENCE_INTERVAL, "min='5' max='300' step='5'");
IotWebConfParameter paramNumLeds = IotWebConfParameter(
    "Number of LEDs (default: 16)", "numLeds", paramNumLedsValue, INTEGER_LEN,
    "number", "1..500", "16", "min='1' max='500' step='1'");
IotWebConfParameter paramDataPin = IotWebConfParameter(
    "LED Data Pin GPIO (default: 2)", "dataPin", paramDataPinValue, INTEGER_LEN,
    "number", "0..16", "2", "min='0' max='16' step='1'");
byte lastIotWebConfState;

// HTTP client
WiFiClientSecure client;

// Adafruit NeoPixel
Adafruit_NeoPixel *strip = nullptr;
int numberLeds;
int dataPin;

// Animation state variables
unsigned long lastUpdate = 0;
int animationMode = 0;
uint32_t animationColor = 0;
uint16_t animationSpeed = 3000;
int animationStep = 0;

// OTA update
HTTPUpdateServer httpUpdater;

// Global variables
String user_code = "";
String device_code = "";
uint8_t interval = 5;

String access_token = "";
String refresh_token = "";
String id_token = "";
unsigned int expires = 0;

String availability = "";
String activity = "";

// Statemachine
#define SMODEINITIAL 0               // Initial
#define SMODEWIFICONNECTING 1        // Wait for wifi connection
#define SMODEWIFICONNECTED 2         // Wifi connected
#define SMODEDEVICELOGINSTARTED 10   // Device login flow was started
#define SMODEDEVICELOGINFAILED 11    // Device login flow failed
#define SMODEAUTHREADY 20            // Authentication successful
#define SMODEPOLLPRESENCE 21         // Poll for presence
#define SMODEREFRESHTOKEN 22         // Access token needs refresh
#define SMODEPRESENCEREQUESTERROR 23 // Access token needs refresh
uint8_t state = SMODEINITIAL;
uint8_t laststate = SMODEINITIAL;
static unsigned long tsPolling = 0;
uint8_t retries = 0;

/**
 * Helper
 */
// Calculate token lifetime
int getTokenLifetime() { return (expires - millis()) / 1000; }

// Save context information to file in SPIFFS
void saveContext() {
  const size_t capacity = JSON_OBJECT_SIZE(3) + 5000;
  DynamicJsonDocument contextDoc(capacity);
  contextDoc["access_token"] = access_token.c_str();
  contextDoc["refresh_token"] = refresh_token.c_str();
  contextDoc["id_token"] = id_token.c_str();

  File contextFile = LittleFS.open(CONTEXT_FILE, "w");
  size_t bytesWritten = serializeJsonPretty(contextDoc, contextFile);
  contextFile.close();
  DBG_PRINT(F("saveContext() - Success: "));
  DBG_PRINTLN(bytesWritten);
  // DBG_PRINTLN(contextDoc.as<String>());
}

boolean loadContext() {
  File file = LittleFS.open(CONTEXT_FILE, "r");
  boolean success = false;

  if (!file) {
    DBG_PRINTLN(F("loadContext() - No file found"));
  } else {
    size_t size = file.size();
    if (size == 0) {
      DBG_PRINTLN(F("loadContext() - File empty"));
    } else {
      const int capacity = JSON_OBJECT_SIZE(3) + 10000;
      DynamicJsonDocument contextDoc(capacity);
      DeserializationError err = deserializeJson(contextDoc, file);

      if (err) {
        DBG_PRINT(F("loadContext() - deserializeJson() failed with code: "));
        DBG_PRINTLN(err.c_str());
      } else {
        int numSettings = 0;
        if (!contextDoc["access_token"].isNull()) {
          access_token = contextDoc["access_token"].as<String>();
          numSettings++;
        }
        if (!contextDoc["refresh_token"].isNull()) {
          refresh_token = contextDoc["refresh_token"].as<String>();
          numSettings++;
        }
        if (!contextDoc["id_token"].isNull()) {
          id_token = contextDoc["id_token"].as<String>();
          numSettings++;
        }
        if (numSettings == 3) {
          success = true;
          DBG_PRINTLN(F("loadContext() - Success"));
          if (strlen(paramClientIdValue) > 0 && strlen(paramTenantValue) > 0) {
            DBG_PRINTLN(F("loadContext() - Next: Refresh token."));
            state = SMODEREFRESHTOKEN;
          } else {
            DBG_PRINTLN(
                F("loadContext() - No client id or tenant setting found."));
          }
        } else {
          Serial.printf("loadContext() - ERROR Number of valid settings in "
                        "file: %d, should be 3.\n",
                        numSettings);
        }
        // DBG_PRINTLN(contextDoc.as<String>());
      }
    }
    file.close();
  }

  return success;
}

// Remove context information file in LittleFS
void removeContext() {
  LittleFS.remove(CONTEXT_FILE);
  DBG_PRINTLN(F("removeContext() - Success"));
}

void startMDNS() {
  DBG_PRINTLN("startMDNS()");
  // Set up mDNS responder
  if (!MDNS.begin(thingName)) {
    DBG_PRINTLN("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  // MDNS.addService("http", "tcp", 80);

  DBG_PRINT("mDNS responder started: ");
  DBG_PRINT(thingName);
  DBG_PRINTLN(".local");
}

#include "request_handler.h"
#include "spiffs_webserver.h"

// Color definitions (functions to handle dynamic strip object)
uint32_t getColorBlack() { return strip ? strip->Color(0, 0, 0) : 0; }
uint32_t getColorRed() { return strip ? strip->Color(255, 0, 0) : 0; }
uint32_t getColorGreen() { return strip ? strip->Color(0, 255, 0) : 0; }
uint32_t getColorBlue() { return strip ? strip->Color(0, 0, 255) : 0; }
uint32_t getColorWhite() { return strip ? strip->Color(255, 255, 255) : 0; }
uint32_t getColorYellow() { return strip ? strip->Color(255, 255, 0) : 0; }
uint32_t getColorOrange() { return strip ? strip->Color(255, 165, 0) : 0; }
uint32_t getColorPurple() { return strip ? strip->Color(128, 0, 128) : 0; }
uint32_t getColorPink() { return strip ? strip->Color(255, 192, 203) : 0; }

#define COLOR_BLACK getColorBlack()
#define COLOR_RED getColorRed()
#define COLOR_GREEN getColorGreen()
#define COLOR_BLUE getColorBlue()
#define COLOR_WHITE getColorWhite()
#define COLOR_YELLOW getColorYellow()
#define COLOR_ORANGE getColorOrange()
#define COLOR_PURPLE getColorPurple()
#define COLOR_PINK getColorPink()

// Animation modes
#define MODE_STATIC 0
#define MODE_BREATH 1
#define MODE_SCAN 2
#define MODE_THEATER_CHASE 3
#define MODE_COLOR_WIPE 4

// Neopixel control
void setColor(uint32_t color) {
  if (!strip)
    return;
  for (int i = 0; i < numberLeds; i++) {
    strip->setPixelColor(i, color);
  }
  strip->show();
}

void setAnimation(uint8_t segment, uint8_t mode, uint32_t color,
                  uint16_t speed = 3000, bool reverse = false) {
  Serial.printf("setAnimation: segment=%d, Mode: %d, Color: %lu, Speed: %d\n",
                segment, mode, color, speed);
  animationMode = mode;
  animationColor = color;
  animationSpeed = speed;
  animationStep = 0;
  lastUpdate = millis();

  if (mode == MODE_STATIC) {
    setColor(color);
  }
}

// Update animations
void updateAnimations() {
  unsigned long now = millis();
  if (now - lastUpdate < 50)
    return; // Update at ~20 fps
  lastUpdate = now;

  if (animationMode == MODE_STATIC) {
    return; // Already set
  }

  if (animationMode == MODE_BREATH) {
    // Breathing effect
    if (!strip)
      return;
    int breathValue = (exp(sin(millis() / 2000.0 * PI)) - 0.36787944) * 108.0;
    uint8_t r = ((animationColor >> 16) & 0xFF) * breathValue / 255;
    uint8_t g = ((animationColor >> 8) & 0xFF) * breathValue / 255;
    uint8_t b = (animationColor & 0xFF) * breathValue / 255;
    setColor(strip->Color(r, g, b));
  }

  if (animationMode == MODE_THEATER_CHASE) {
    // Theater chase effect
    if (!strip)
      return;
    for (int i = 0; i < numberLeds; i++) {
      if ((i + animationStep) % 3 == 0) {
        strip->setPixelColor(i, animationColor);
      } else {
        strip->setPixelColor(i, 0);
      }
    }
    strip->show();
    animationStep = (animationStep + 1) % 3;
  }

  if (animationMode == MODE_SCAN) {
    // Scanning effect
    if (!strip)
      return;
    for (int i = 0; i < numberLeds; i++) {
      strip->setPixelColor(i, 0);
    }
    strip->setPixelColor(animationStep % numberLeds, animationColor);
    strip->show();
    animationStep++;
  }

  if (animationMode == MODE_COLOR_WIPE) {
    // Color wipe effect
    if (!strip)
      return;
    for (int i = 0; i <= animationStep && i < numberLeds; i++) {
      strip->setPixelColor(i, animationColor);
    }
    strip->show();
    if (animationStep < numberLeds) {
      animationStep++;
    } else {
      animationStep = 0;
    }
  }
}

void setPresenceAnimation() {
  // Activity: Available, Away, BeRightBack, Busy, DoNotDisturb, InACall,
  // InAConferenceCall, Inactive, InAMeeting, Offline, OffWork, OutOfOffice,
  // PresenceUnknown, Presenting, UrgentInterruptionsOnly

  if (activity.equals("Available")) {
    setAnimation(0, MODE_STATIC, COLOR_GREEN);
  }
  if (activity.equals("Away")) {
    setAnimation(0, MODE_STATIC, COLOR_YELLOW);
  }
  if (activity.equals("BeRightBack")) {
    setAnimation(0, MODE_STATIC, COLOR_ORANGE);
  }
  if (activity.equals("Busy")) {
    setAnimation(0, MODE_STATIC, COLOR_PURPLE);
  }
  if (activity.equals("DoNotDisturb") ||
      activity.equals("UrgentInterruptionsOnly")) {
    setAnimation(0, MODE_STATIC, COLOR_PINK);
  }
  if (activity.equals("InACall")) {
    setAnimation(0, MODE_BREATH, COLOR_RED);
  }
  if (activity.equals("InAConferenceCall")) {
    setAnimation(0, MODE_BREATH, COLOR_RED, 9000);
  }
  if (activity.equals("Inactive")) {
    setAnimation(0, MODE_BREATH, COLOR_WHITE);
  }
  if (activity.equals("InAMeeting")) {
    setAnimation(0, MODE_SCAN, COLOR_RED);
  }
  if (activity.equals("Offline") || activity.equals("OffWork") ||
      activity.equals("OutOfOffice") || activity.equals("PresenceUnknown")) {
    setAnimation(0, MODE_STATIC, COLOR_BLACK);
  }
  if (activity.equals("Presenting")) {
    setAnimation(0, MODE_COLOR_WIPE, COLOR_RED);
  }
}

/**
 * Application logic
 */

// Handler: Wifi connected
void onWifiConnected() { state = SMODEWIFICONNECTED; }

// Poll for access token
void pollForToken() {
  String payload =
      "client_id=" + String(paramClientIdValue) +
      "&grant_type=urn:ietf:params:oauth:grant-type:device_code&device_code=" +
      device_code;
  Serial.printf("pollForToken()\n");

  // const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(7) + 530; //
  // Case 1: HTTP 400 error (not yet ready)
  const size_t capacity =
      JSON_OBJECT_SIZE(7) + 10000; // Case 2: Successful (bigger size of both
                                   // variants, so take that one as capacity)
  DynamicJsonDocument responseDoc(capacity);
  boolean res =
      requestJsonApi(responseDoc,
                     "https://login.microsoftonline.com/" +
                         String(paramTenantValue) + "/oauth2/v2.0/token",
                     payload, capacity);

  if (!res) {
    state = SMODEDEVICELOGINFAILED;
  } else if (responseDoc.containsKey("error")) {
    const char *_error = responseDoc["error"];
    const char *_error_description = responseDoc["error_description"];

    if (strcmp(_error, "authorization_pending") == 0) {
      Serial.printf("pollForToken() - Wating for authorization by user: %s\n\n",
                    _error_description);
    } else {
      Serial.printf("pollForToken() - Unexpected error: %s, %s\n\n", _error,
                    _error_description);
      state = SMODEDEVICELOGINFAILED;
    }
  } else {
    if (responseDoc.containsKey("access_token") &&
        responseDoc.containsKey("refresh_token") &&
        responseDoc.containsKey("id_token")) {
      // Save tokens and expiration
      access_token = responseDoc["access_token"].as<String>();
      refresh_token = responseDoc["refresh_token"].as<String>();
      id_token = responseDoc["id_token"].as<String>();
      unsigned int _expires_in = responseDoc["expires_in"].as<unsigned int>();
      expires = millis() +
                (_expires_in * 1000); // Calculate timestamp when token expires

      // Set state
      state = SMODEAUTHREADY;
    } else {
      Serial.printf("pollForToken() - Unknown response: %s\n",
                    responseDoc.as<const char *>());
    }
  }
}

// Get presence information
void pollPresence() {
  // See:
  // https://github.com/microsoftgraph/microsoft-graph-docs/blob/ananya/api-reference/beta/resources/presence.md
  const size_t capacity = 1024;
  DynamicJsonDocument responseDoc(capacity);
  boolean res = requestJsonApi(responseDoc,
                               "https://graph.microsoft.com/v1.0/me/presence",
                               "", capacity, "GET", true);

  if (!res) {
    state = SMODEPRESENCEREQUESTERROR;
    retries++;
  } else if (responseDoc.containsKey("error")) {
    const char *_error_code = responseDoc["error"]["code"];
    if (strcmp(_error_code, "InvalidAuthenticationToken")) {
      DBG_PRINTLN(F("pollPresence() - Refresh needed"));
      tsPolling = millis();
      state = SMODEREFRESHTOKEN;
    } else {
      Serial.printf("pollPresence() - Error: %s\n", _error_code);
      state = SMODEPRESENCEREQUESTERROR;
      retries++;
    }
  } else {
    // Store presence info
    availability = responseDoc["availability"].as<String>();
    activity = responseDoc["activity"].as<String>();
    retries = 0;

    setPresenceAnimation();
  }
}

// Refresh the access token
boolean refreshToken() {
  boolean success = false;
  // See:
  // https://docs.microsoft.com/de-de/azure/active-directory/develop/v1-protocols-oauth-code#refreshing-the-access-tokens
  String payload = "client_id=" + String(paramClientIdValue) +
                   "&grant_type=refresh_token&refresh_token=" + refresh_token;
  DBG_PRINTLN(F("refreshToken()"));

  const size_t capacity = JSON_OBJECT_SIZE(7) + 10000;
  DynamicJsonDocument responseDoc(capacity);
  boolean res =
      requestJsonApi(responseDoc,
                     "https://login.microsoftonline.com/" +
                         String(paramTenantValue) + "/oauth2/v2.0/token",
                     payload, capacity);

  // Replace tokens and expiration
  if (res && responseDoc.containsKey("access_token") &&
      responseDoc.containsKey("refresh_token")) {
    if (!responseDoc["access_token"].isNull()) {
      access_token = responseDoc["access_token"].as<String>();
      success = true;
    }
    if (!responseDoc["refresh_token"].isNull()) {
      refresh_token = responseDoc["refresh_token"].as<String>();
      success = true;
    }
    if (!responseDoc["id_token"].isNull()) {
      id_token = responseDoc["id_token"].as<String>();
    }
    if (!responseDoc["expires_in"].isNull()) {
      int _expires_in = responseDoc["expires_in"].as<unsigned int>();
      expires = millis() +
                (_expires_in * 1000); // Calculate timestamp when token expires
    }

    DBG_PRINTLN(F("refreshToken() - Success"));
    state = SMODEPOLLPRESENCE;
  } else {
    DBG_PRINTLN(F("refreshToken() - Error:"));
    Serial.println(responseDoc.as<String>());
    // Set retry after timeout
    tsPolling = millis() + (DEFAULT_ERROR_RETRY_INTERVAL * 1000);
  }
  return success;
}

// Implementation of a statemachine to handle the different application states
void statemachine() {

  // Statemachine: Check states of iotWebConf to detect AP mode and WiFi
  // Connection attepmt
  byte iotWebConfState = iotWebConf.getState();
  if (iotWebConfState != lastIotWebConfState) {
    if (iotWebConfState == IOTWEBCONF_STATE_NOT_CONFIGURED ||
        iotWebConfState == IOTWEBCONF_STATE_AP_MODE) {
      DBG_PRINTLN(F("Detected AP mode"));
      setAnimation(0, MODE_THEATER_CHASE, COLOR_WHITE);
    }
    if (iotWebConfState == IOTWEBCONF_STATE_CONNECTING) {
      DBG_PRINTLN(F("WiFi connecting"));
      state = SMODEWIFICONNECTING;
    }
  }
  lastIotWebConfState = iotWebConfState;

  // Statemachine: Wifi connection start
  if (state == SMODEWIFICONNECTING && laststate != SMODEWIFICONNECTING) {
    setAnimation(0, MODE_THEATER_CHASE, COLOR_BLUE);
  }

  // Statemachine: After wifi is connected
  if (state == SMODEWIFICONNECTED && laststate != SMODEWIFICONNECTED) {
    setAnimation(0, MODE_THEATER_CHASE, COLOR_GREEN);
    startMDNS();
    loadContext();
    // WiFi client
    DBG_PRINTLN(F("Wifi connected, waiting for requests ..."));
  }

  // Statemachine: Devicelogin started
  if (state == SMODEDEVICELOGINSTARTED) {
    if (laststate != SMODEDEVICELOGINSTARTED) {
      setAnimation(0, MODE_THEATER_CHASE, COLOR_PURPLE);
    }
    if (millis() >= tsPolling) {
      pollForToken();
      tsPolling = millis() + (interval * 1000);
    }
  }

  // Statemachine: Devicelogin failed
  if (state == SMODEDEVICELOGINFAILED) {
    DBG_PRINTLN(F("Device login failed"));
    state = SMODEWIFICONNECTED; // Return back to initial mode
  }

  // Statemachine: Auth is ready, start polling for presence immediately
  if (state == SMODEAUTHREADY) {
    saveContext();
    state = SMODEPOLLPRESENCE;
    tsPolling = millis();
  }

  // Statemachine: Poll for presence information, even if there was a error
  // before (handled below)
  if (state == SMODEPOLLPRESENCE) {
    if (millis() >= tsPolling) {
      DBG_PRINTLN(F("Polling presence info ..."));
      pollPresence();
      tsPolling = millis() + (atoi(paramPollIntervalValue) * 1000);
      Serial.printf("--> Availability: %s, Activity: %s\n\n",
                    availability.c_str(), activity.c_str());
    }

    if (getTokenLifetime() < TOKEN_REFRESH_TIMEOUT) {
      Serial.printf("Token needs refresh, valid for %d s.\n",
                    getTokenLifetime());
      state = SMODEREFRESHTOKEN;
    }
  }

  // Statemachine: Refresh token
  if (state == SMODEREFRESHTOKEN) {
    if (laststate != SMODEREFRESHTOKEN) {
      setAnimation(0, MODE_THEATER_CHASE, COLOR_RED);
    }
    if (millis() >= tsPolling) {
      boolean success = refreshToken();
      if (success) {
        saveContext();
      }
    }
  }

  // Statemachine: Polling presence failed
  if (state == SMODEPRESENCEREQUESTERROR) {
    if (laststate != SMODEPRESENCEREQUESTERROR) {
      retries = 0;
    }

    Serial.printf("Polling presence failed, retry #%d.\n", retries);
    if (retries >= 5) {
      // Try token refresh
      state = SMODEREFRESHTOKEN;
    } else {
      state = SMODEPOLLPRESENCE;
    }
  }

  // Update laststate
  if (laststate != state) {
    laststate = state;
    DBG_PRINTLN(F("============================================================"
                  "=========="));
  }
}

// ESP8266 doesn't require multicore tasks or RMT driver

/**
 * Main functions
 */
void setup() {
  Serial.begin(115200);
  DBG_PRINTLN();
  DBG_PRINTLN(F("setup() Starting up..."));
// Serial.setDebugOutput(true);
#ifdef DISABLECERTCHECK
  DBG_PRINTLN(F("WARNING: Checking of HTTPS certificates disabled."));
#endif

  // iotWebConf - Initializing the configuration.
#ifdef LED_BUILTIN
  iotWebConf.setStatusPin(LED_BUILTIN);
#endif
  iotWebConf.setWifiConnectionTimeoutMs(5000);
  iotWebConf.addParameter(&separator);
  iotWebConf.addParameter(&paramClientId);
  iotWebConf.addParameter(&paramTenant);
  iotWebConf.addParameter(&paramPollInterval);
  iotWebConf.addParameter(&paramNumLeds);
  iotWebConf.addParameter(&paramDataPin);
  // iotWebConf.setFormValidator(&formValidator);
  // iotWebConf.getApTimeoutParameter()->visible = true;
  // iotWebConf.getApTimeoutParameter()->defaultValue = "10";
  iotWebConf.setWifiConnectionCallback(&onWifiConnected);
  iotWebConf.setConfigSavedCallback(&onConfigSaved);
  iotWebConf.setupUpdateServer(&httpUpdater);
  iotWebConf.skipApStartup();
  iotWebConf.init();

  // Initialize LED strip with configured parameters
  dataPin = atoi(paramDataPinValue);
  if (dataPin < 0 || dataPin > 16) {
    DBG_PRINTLN(F("Data pin not valid, using default GPIO 2."));
    dataPin = DATAPIN;
  }

  numberLeds = atoi(paramNumLedsValue);
  if (numberLeds < 1) {
    DBG_PRINTLN(F("Number of LEDs not given, using 16."));
    numberLeds = NUMLEDS;
  }

  Serial.printf("Initializing NeoPixel strip: %d LEDs on GPIO %d\n", numberLeds,
                dataPin);
  strip = new Adafruit_NeoPixel(numberLeds, dataPin, NEO_GRB + NEO_KHZ800);
  strip->begin();
  strip->show(); // Initialize all pixels to 'off'
  setAnimation(0, MODE_STATIC, COLOR_WHITE);

  // HTTP server - Set up required URL handlers on the web server.
  server.on("/", HTTP_GET, handleRoot);
  server.on("/config", HTTP_GET, [] { iotWebConf.handleConfig(); });
  server.on("/config", HTTP_POST, [] { iotWebConf.handleConfig(); });
  server.on("/upload", HTTP_GET, [] { handleMinimalUpload(); });
  server.on("/api/startDevicelogin", HTTP_GET,
            [] { handleStartDevicelogin(); });
  server.on("/api/settings", HTTP_GET, [] { handleGetSettings(); });
  server.on("/api/clearSettings", HTTP_GET, [] { handleClearSettings(); });
  server.on("/fs/delete", HTTP_DELETE, handleFileDelete);
  server.on("/fs/list", HTTP_GET, handleFileList);
  server.on(
      "/fs/upload", HTTP_POST, []() { server.send(200, "text/plain", ""); },
      handleFileUpload);

  // server.onNotFound([](){ iotWebConf.handleNotFound(); });
  server.onNotFound([]() {
    iotWebConf.handleNotFound();
    if (!handleFileRead(server.uri())) {
      server.send(404, "text/plain", "FileNotFound");
    }
  });

  DBG_PRINTLN(F("setup() ready..."));

  // SPIFFS.begin() - Format if mount failed
  DBG_PRINTLN(F("LittleFS.begin() "));
  // Filesystem
  if (!LittleFS.begin()) {
    DBG_PRINTLN(F("LittleFS.begin() failed, formatting ..."));
    LittleFS.format();
    if (!LittleFS.begin()) {
      DBG_PRINTLN("SPIFFS Mount Failed after format");
      return;
    }
  }
}

void loop() {
  // iotWebConf - doLoop should be called as frequently as possible.
  iotWebConf.doLoop();

  statemachine();

  // Update LED animations
  updateAnimations();
}
