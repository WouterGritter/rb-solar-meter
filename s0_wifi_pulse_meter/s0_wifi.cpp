#include "s0_wifi.h"
#include "s0_counter.h"

ESP8266WebServer server(80);

void handleRoot();

void s0_wifi_setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);

  Serial.println("Connecting to wifi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  
  Serial.print("Success! IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin(MDNS_NAME)) {
    Serial.print("MDNS responder started for ");
    Serial.print(MDNS_NAME);
    Serial.println(".local");
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
  
  snprintf(temp, 128, "{\"pulseCount\":%d,\"wattHours\":%f,\"lastPulseDurationMs\":%d,\"calculatedWattage\":%f}",
              getPulseCount(), getWattHours(), getLastPulseDuration(), getCalculatedWattage());

  server.send(200, "text/html", temp);
}
