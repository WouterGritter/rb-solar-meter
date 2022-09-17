#include "s0_counter.h"

unsigned long firstPulseTime = 0;
unsigned long lastPulseTime = 0;
unsigned long lastPulseDuration = 0;
unsigned long pulseCount = 0;

ICACHE_RAM_ATTR void s0_trigger();

void s0_counter_setup() {
  pinMode(S0_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(S0_PIN), s0_trigger, FALLING);
}

void s0_counter_loop() {
}

unsigned long getLastPulseDuration() {
  return lastPulseDuration;
}

unsigned long getPulseCount() {
  return pulseCount;
}

double getWattHours() {
  return pulseCount * WH_PER_PULSE;
}

double getCalculatedWattage() {
  if (pulseCount < 2) {
    return 0.0;
  }

  unsigned long sinceLastPulse = millis() - lastPulseTime;
  if (sinceLastPulse > lastPulseDuration) {
    // The wattage is going to be lower. Calculate the current wattage on
    // the assumption that the pulse will occur now.
    return 3600000.0 / sinceLastPulse * WH_PER_PULSE;
  } else {
    return 3600000.0 / lastPulseDuration * WH_PER_PULSE;
  }
}

ICACHE_RAM_ATTR void s0_trigger() {
  unsigned long now = millis();

  if (now - lastPulseTime < MIN_PULSE_DELAY) {
    // Ignore pulse
    return;
  }

  pulseCount++;

  if (firstPulseTime != 0) {
    lastPulseDuration = now - lastPulseTime;
  } else {
    firstPulseTime = now;
  }

  lastPulseTime = now;
}
