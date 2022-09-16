#ifndef S0_WIFI_H
#define S0_WIFI_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "constants.h"

void s0_wifi_setup();
void s0_wifi_loop();

#endif
