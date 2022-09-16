#include "s0_wifi.h"
#include "s0_counter.h"

ESP8266WebServer server(80);

void handleRoot();

void s0_wifi_setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);

  Serial.println("Connecting to wifi...");
  
  Serial.print("Success! IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("solar-energy-meter")) {
    Serial.println("MDNS responder started for solar-energy-meter.local");
  }

  server.on("/", handleRoot);

  server.begin();
}

void s0_wifi_loop() {
  server.handleClient();
  MDNS.update();
}

void handleRoot() {
  char temp[128];
  
  snprintf(temp, 128, "{\"aliveMs\":%d,\"pulseCount\":%d,\"lastPulseDurationMs\":%d,\"calculatedWattage\":%f}",
              getAliveMs(), getPulseCount(), getLastPulseDuration(), getCalculatedWattage());

  server.send(200, "text/html", temp);
}
