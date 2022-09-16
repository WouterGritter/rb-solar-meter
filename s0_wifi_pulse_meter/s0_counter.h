#ifndef S0_COUNTER_H
#define S0_COUNTER_H

#include <Arduino.h>
#include "constants.h"

void s0_counter_setup();
void s0_counter_loop();

unsigned long getLastPulseDuration();
unsigned long getPulseCount();
unsigned long getAliveMs();
double getCalculatedWattage();

#endif
