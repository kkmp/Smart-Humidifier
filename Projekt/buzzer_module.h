#ifndef BUZZER_MODULE_H
#define BUZZER_MODULE_H

#define BUZZZER_PIN  12
#define BUZZER_TIME 100

#include <Arduino.h>

void buzzerSetup();
void makeSound(const int & timePeriod = BUZZER_TIME);

#endif
