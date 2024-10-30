#include "buzzer_module.h"

void buzzerSetup() {
    ledcAttachPin(BUZZZER_PIN, 0);
    ledcSetup(0, 1000, 1); 
    Serial.println("Buzzer initialized");
}

void makeSound(const int & timePeriod) {
    ledcWrite(0, 1);
    delay(timePeriod);
    ledcWrite(0, 0);
}
