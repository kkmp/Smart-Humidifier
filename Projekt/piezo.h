#ifndef PIEZO_H
#define PIEZO_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "buzzer_module.h"
#include "bme_module.h"

#define PIEZO_PIN 27
#define PIEZO_DEFAULT_TIME 250

#define RED 32
#define GREEN 33
#define BLUE 25

#define MIN_HUMIDITY 50

class Piezo_Module {
private:
    bool isOn;
    bool isAutomode;
    bool isMoistureOn;
    int minHumidity;
    Piezo_Module();
    Piezo_Module(const Piezo_Module&);
    ~Piezo_Module();
    void turnMoistureOff(int delayTime = PIEZO_DEFAULT_TIME);
    void turnMoistureOn(int delayTime = PIEZO_DEFAULT_TIME);

public:
    static Piezo_Module & instance();
    void piezoSetup();
    void piezoOn(int delayTime = PIEZO_DEFAULT_TIME);
    void piezoOff(int delayTime = PIEZO_DEFAULT_TIME);
    bool getIsOn();
    bool getIsAutomode();
    int getMinHumidity();
    String getIsOnJSON();
    String getIsAutomodeJSON();
    void setPower(bool power);
    void setMode(bool mode);
    void setParameter(int minHumidity);
    String getParameter();
    void piezoRed();
    void piezoGreen();
    void piezoBlue();
    void piezoControl(BME_Module & bme);
};

#endif
