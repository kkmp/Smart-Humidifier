#ifndef BME_MODULE_H
#define BME_MODULE_H

#include <Arduino.h>
#include <Adafruit_BME280.h>
#include <ArduinoJson.h>

class BME_Module {
private:
    Adafruit_BME280 bme;
    BME_Module();
    BME_Module(const BME_Module&);
    ~BME_Module();

    float temperatureBME;
    float pressureBME;
    float humidityBME;

public:
    static BME_Module & instance();
    void BMESetup();
    void takeMeasurement();
    float getTemperatureBME();
    float getPressureBME();
    float getHumidityBME();
    String getMeasurementJSON();
};

#endif
