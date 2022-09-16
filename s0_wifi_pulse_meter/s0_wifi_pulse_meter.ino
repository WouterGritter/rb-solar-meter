#include "s0_wifi.h"
#include "s0_counter.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, world!");

  s0_wifi_setup();
  s0_counter_setup();

  Serial.println("Setup complete");
}

void loop() {
  s0_wifi_loop();
  s0_counter_loop();
}
