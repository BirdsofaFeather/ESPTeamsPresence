/**
 * ESPTeamsPresence -- A standalone Microsoft Teams presence light
 *   based on ESP32 and RGB neopixel LEDs.
 *   https://github.com/toblum/ESPTeamsPresence
 *
 * Copyright (C) 2020 Tobias Blum <make@tobiasblum.de>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "FS.h"
#include "SPIFFS.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <IotWebConf.h>
#include <NeoPixelBus.h>
#include <WiFiClientSecure.h>
#include <Wire.h>

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

// OLED Display Configuration
// Special 0.42-inch OLED that uses 128x64 buffer with 72x40 visible area
// Visible area offset calculated from position test: EFGH at (40,24), IJKL at
// (52,36)
#define SCREEN_WIDTH 128    // Buffer width (128x64 internally)
#define SCREEN_HEIGHT 64    // Buffer height
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // I2C address (try 0x3D if 0x3C doesn't work)
#define OLED_SDA 5          // GPIO5 for SDA (as shown on your display module)
#define OLED_SCL 6          // GPIO6 for SCL (as shown on your display module)
// Display offset for this special 0.42" variant
// Calculated from test: IJKL at X=52 centered, EFGH at Y=24 on top
#define DISPLAY_OFFSET_X 28 // Column offset (X=52 centered = offset ~28)
#define DISPLAY_OFFSET_Y 24 // Row offset (Y=24 at top = offset 24)
// Visible area dimensions (actual screen size)
#define VISIBLE_WIDTH 72  // Actual visible width
#define VISIBLE_HEIGHT 40 // Actual visible height

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
    "BHQwcjA3oDWgM4YxaHR0cDovL2NybTMuZGlnaWNlcnQuY29tL0RpZ2lDZXJ0R2xv\n"
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
    "Oi8vY3rlMy5kaWdpY2VydC5jb20vRGlnaUNlcnRHbG9iYWxSb290RzIuY3JsMB0G\n"
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
    "Client-ID (Generic ID: d4fd1dc6-aa98-41f3-9b1a-b6a49358269e)", "clientId",
    paramClientIdValue, STRING_LEN, "text",
    "e.g. d4fd1dc6-aa98-41f3-9b1a-b6a49358269e",
    "d4fd1dc6-aa98-41f3-9b1a-b6a49358269e");
IotWebConfParameter paramTenant = IotWebConfParameter(
    "Tenant hostname / ID", "tenantId", paramTenantValue, STRING_LEN, "text",
    "e.g. contoso.onmicrosoft.com", "common");
IotWebConfParameter paramPollInterval = IotWebConfParameter(
    "Presence polling interval (sec) (default: 30)", "pollInterval",
    paramPollIntervalValue, INTEGER_LEN, "number", "5..300",
    DEFAULT_POLLING_PRESENCE_INTERVAL, "min='5' max='300' step='5'");
IotWebConfParameter paramNumLeds = IotWebConfParameter(
    "Number of LEDs (default: 16)", "numLeds", paramNumLedsValue, INTEGER_LEN,
    "number", "1..500", "16", "min='1' max='500' step='1'");
IotWebConfParameter paramDataPin = IotWebConfParameter(
    "LED Data Pin (GPIO, default: 3)", "dataPin", paramDataPinValue,
    INTEGER_LEN, "number", "0..21", "3", "min='0' max='21' step='1'");
byte lastIotWebConfState;

// HTTP client
WiFiClientSecure client;

// NeoPixelBus - Uses RMT for ESP32/ESP32-C3 compatibility
// ESP32-C3 uses RMT channel 1, ESP32 uses RMT channel 0
#ifdef ESP32C3
typedef NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod> NeoPixelBusType;
#else
typedef NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> NeoPixelBusType;
#endif

NeoPixelBusType *strip = nullptr; // Will be initialized in setup()
int numberLeds = NUMLEDS;

// OLED Display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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

  File contextFile = SPIFFS.open(CONTEXT_FILE, FILE_WRITE);
  size_t bytesWritten = serializeJsonPretty(contextDoc, contextFile);
  contextFile.close();
  DBG_PRINT(F("saveContext() - Success: "));
  DBG_PRINTLN(bytesWritten);
  // DBG_PRINTLN(contextDoc.as<String>());
}

boolean loadContext() {
  File file = SPIFFS.open(CONTEXT_FILE);
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

// Remove context information file in SPIFFS
void removeContext() {
  SPIFFS.remove(CONTEXT_FILE);
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

/**
 * OLED Display Functions
 */
void initDisplay() {
  Wire.begin(OLED_SDA, OLED_SCL);

  DBG_PRINT(F("Initializing OLED with offset X="));
  DBG_PRINT(DISPLAY_OFFSET_X);
  DBG_PRINT(F(", Y="));
  DBG_PRINTLN(DISPLAY_OFFSET_Y);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    DBG_PRINTLN(F("SSD1306 allocation failed"));
    return;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y);
  display.println(F("Teams"));
  display.setCursor(DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y + 10);
  display.println(F("Ready!"));
  display.display();

  DBG_PRINTLN(F("OLED Display initialized"));
}

// Optimized for special 0.42" OLED with offset (13, 14)
// Uses 128x64 buffer but only 72x40 is visible
void updateDisplay(const String &status, const String &activity) {
  // Clear entire buffer
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // All text must be offset by (DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y)
  // to appear in the visible 72x40 window

  // Line 1: Title (0-7 in visible area = 14-21 in buffer)
  display.setCursor(DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y);
  display.println(F("Teams"));

  // Line 2-3: Status text (10-27 in visible area = 24-41 in buffer)
  int statusY = DISPLAY_OFFSET_Y + 10;
  display.setCursor(DISPLAY_OFFSET_X, statusY);

  // Display status - use large text for short words
  if (activity.equals("Available")) {
    display.setTextSize(2);
    display.println(F("Free"));
  } else if (activity.equals("Busy")) {
    display.setTextSize(2);
    display.println(F("Busy"));
  } else if (activity.equals("InACall") ||
             activity.equals("InAConferenceCall")) {
    display.setTextSize(2);
    display.println(F("Call"));
  } else if (activity.equals("InAMeeting")) {
    display.setTextSize(1);
    display.println(F("Meeting"));
  } else if (activity.equals("Away")) {
    display.setTextSize(2);
    display.println(F("Away"));
  } else if (activity.equals("BeRightBack")) {
    display.setTextSize(1);
    display.println(F("BRB"));
  } else if (activity.equals("DoNotDisturb") ||
             activity.equals("UrgentInterruptionsOnly")) {
    display.setTextSize(2);
    display.println(F("DND"));
  } else if (activity.equals("Presenting")) {
    display.setTextSize(1);
    display.println(F("Present"));
  } else if (activity.equals("Offline") || activity.equals("OffWork") ||
             activity.equals("OutOfOffice")) {
    display.setTextSize(1);
    display.println(F("Offline"));
  } else {
    display.setTextSize(1);
    display.println(F("Unknown"));
  }

  // Line 4: WiFi status at bottom (32-39 in visible area = 46-53 in buffer)
  display.setTextSize(1);
  display.setCursor(DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y + 32);
  if (WiFi.status() == WL_CONNECTED) {
    // Display first 12 chars of SSID to fit on screen
    String ssid = WiFi.SSID();
    if (ssid.length() > 12) {
      ssid = ssid.substring(0, 12);
    }
    display.print(ssid);
  } else {
    display.print(F("No WiFi"));
  }

  display.display();
}

// Simplified message display for special 0.42" screen with offset
void displayMessage(const String &line1, const String &line2 = "",
                    const String &line3 = "") {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Line 1 (0-7 in visible = 14-21 in buffer)
  display.setCursor(DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y);
  display.println(F("Teams"));

  // Line 2 (10-17 in visible = 24-31 in buffer)
  display.setCursor(DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y + 10);
  if (line1.length() > 12) {
    display.println(line1.substring(0, 12));
  } else {
    display.println(line1);
  }

  // Line 3 (20-27 in visible = 34-41 in buffer) - if provided
  if (line2.length() > 0) {
    display.setCursor(DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y + 20);
    if (line2.length() > 12) {
      display.println(line2.substring(0, 12));
    } else {
      display.println(line2);
    }
  }

  // Line 4 (30-37 in visible = 44-51 in buffer) - if provided
  if (line3.length() > 0) {
    display.setCursor(DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y + 30);
    if (line3.length() > 12) {
      display.println(line3.substring(0, 12));
    } else {
      display.println(line3);
    }
  }

  display.display();
}

// NeoPixelBus Color definitions
RgbColor colorRed(255, 0, 0);
RgbColor colorGreen(0, 255, 0);
RgbColor colorBlue(0, 0, 255);
RgbColor colorWhite(255, 255, 255);
RgbColor colorYellow(255, 255, 0);
RgbColor colorOrange(255, 165, 0);
RgbColor colorPurple(128, 0, 128);
RgbColor colorPink(255, 192, 203);
RgbColor colorBlack(0, 0, 0);

// Set all LEDs to a solid color
void setColor(RgbColor color) {
  if (strip == nullptr)
    return;

  for (int i = 0; i < numberLeds; i++) {
    strip->SetPixelColor(i, color);
  }
  strip->Show();
}

void setPresenceAnimation() {
  // Activity: Available, Away, BeRightBack, Busy, DoNotDisturb, InACall,
  // InAConferenceCall, Inactive, InAMeeting, Offline, OffWork, OutOfOffice,
  // PresenceUnknown, Presenting, UrgentInterruptionsOnly

  if (activity.equals("Available")) {
    setColor(colorGreen);
  } else if (activity.equals("Away")) {
    setColor(colorYellow);
  } else if (activity.equals("BeRightBack")) {
    setColor(colorOrange);
  } else if (activity.equals("Busy")) {
    setColor(colorRed);
  } else if (activity.equals("DoNotDisturb") ||
             activity.equals("UrgentInterruptionsOnly")) {
    setColor(colorPink);
  } else if (activity.equals("InACall") ||
             activity.equals("InAConferenceCall")) {
    setColor(
        colorRed); // Solid red for calls (breathing effect can be added later)
  } else if (activity.equals("Inactive")) {
    setColor(colorWhite);
  } else if (activity.equals("InAMeeting")) {
    setColor(colorRed); // Solid red for meetings
  } else if (activity.equals("Offline") || activity.equals("OffWork") ||
             activity.equals("OutOfOffice") ||
             activity.equals("PresenceUnknown")) {
    setColor(colorBlack); // Turn off LEDs
  } else if (activity.equals("Presenting")) {
    setColor(colorRed);
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
    updateDisplay(availability, activity);
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
      setColor(colorWhite);
      displayMessage("AP Mode", "Connect to WiFi:", thingName);
    }
    if (iotWebConfState == IOTWEBCONF_STATE_CONNECTING) {
      DBG_PRINTLN(F("WiFi connecting"));
      state = SMODEWIFICONNECTING;
    }
  }
  lastIotWebConfState = iotWebConfState;

  // Statemachine: Wifi connection start
  if (state == SMODEWIFICONNECTING && laststate != SMODEWIFICONNECTING) {
    setColor(colorBlue);
    displayMessage("Connecting...", "Joining WiFi");
  }

  // Statemachine: After wifi is connected
  if (state == SMODEWIFICONNECTED && laststate != SMODEWIFICONNECTED) {
    setColor(colorGreen);
    startMDNS();
    loadContext();
    // WiFi client
    DBG_PRINTLN(F("Wifi connected, waiting for requests ..."));
    displayMessage("WiFi Connected", WiFi.SSID(), WiFi.localIP().toString());
  }

  // Statemachine: Devicelogin started
  if (state == SMODEDEVICELOGINSTARTED) {
    if (laststate != SMODEDEVICELOGINSTARTED) {
      setColor(colorPurple);
      displayMessage("Device Login", "Use web portal", "to authenticate");
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
      setColor(colorRed);
      displayMessage("Refreshing", "Token refresh...");
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

// NeoPixelBus doesn't need a separate task or customShow function
// It handles everything internally with the RMT peripheral

/**
 * Main functions
 */
void setup() {
  Serial.begin(115200);
  delay(1000); // Give serial time to initialize
  DBG_PRINTLN();
  DBG_PRINTLN(F("setup() Starting up..."));
  Serial.println("ESP32-C3 DEBUG: Step 1 - Serial working");

  // Initialize OLED Display
  initDisplay();

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

// ESP32-C3: Don't skip AP startup - it needs AP mode for reliable first boot
#ifndef ESP32C3
  iotWebConf.skipApStartup();
#else
  DBG_PRINTLN(F("ESP32-C3: AP mode enabled for initial configuration"));
  iotWebConf.setApTimeoutMs(300000); // 5 minutes AP timeout for ESP32-C3
#endif

// ESP32-C3 WiFi initialization - must be done before iotWebConf.init()
#ifdef ESP32C3
  DBG_PRINTLN(F("ESP32-C3: Initializing WiFi mode..."));
  WiFi.mode(WIFI_AP_STA); // Enable both AP and Station modes
  delay(100);             // Give WiFi time to initialize
#endif

  iotWebConf.init();

  // Get configured LED parameters
  int configuredLeds = atoi(paramNumLedsValue);
  if (configuredLeds < 1) {
    configuredLeds = NUMLEDS; // Use default if not configured
  }

  int configuredDataPin = atoi(paramDataPinValue);
  if (configuredDataPin < 0 || configuredDataPin > 21) {
    configuredDataPin = DATAPIN; // Use default (13) if invalid
  }

  // Initialize or reconfigure NeoPixelBus with configured values
  if (strip == nullptr || configuredLeds != numberLeds) {
    if (strip != nullptr) {
      delete strip;
    }
    numberLeds = configuredLeds;
    strip = new NeoPixelBusType(numberLeds, configuredDataPin);
    strip->Begin();
    setColor(colorWhite);
    Serial.printf("NeoPixelBus initialized: %d LEDs on GPIO %d\n", numberLeds,
                  configuredDataPin);
  }

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
  DBG_PRINTLN(F("SPIFFS.begin() "));
  if (!SPIFFS.begin(true)) {
    DBG_PRINTLN("SPIFFS Mount Failed");
    return;
  }

  DBG_PRINTLN(F("setup() complete - NeoPixelBus ready"));
}

void loop() {
  // iotWebConf - doLoop should be called as frequently as possible.
  iotWebConf.doLoop();

  statemachine();
}
