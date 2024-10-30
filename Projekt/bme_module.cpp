#include "bme_module.h"

BME_Module::BME_Module() : temperatureBME(0), pressureBME(0), humidityBME(0) {}

BME_Module::BME_Module(const BME_Module&) {}

BME_Module::~BME_Module() {}

BME_Module & BME_Module::instance() {
  static BME_Module instance_;
  return instance_;
}

void BME_Module::BMESetup() {
  if (!bme.begin()) {
    Serial.println("Couldn't find BME");
    while (1);
  }
  Serial.println("BME initialized");
}

void BME_Module::takeMeasurement() {
  this->temperatureBME = bme.readTemperature();
  this->humidityBME = bme.readHumidity();
  this->pressureBME = bme.readPressure() / 100.0;
}

float BME_Module::getTemperatureBME() {
  return this->temperatureBME;
}

float BME_Module::getPressureBME() {
  return this->pressureBME;
}

float BME_Module::getHumidityBME() {
  return this->humidityBME;
}

String BME_Module::getMeasurementJSON() {
  this->takeMeasurement();

  DynamicJsonDocument jsonDoc(100);

  jsonDoc["Humidity"] = this->getHumidityBME();
  jsonDoc["Temperature"] = this->getTemperatureBME();
  jsonDoc["Pressure"] = this->getPressureBME();

  String measurement = "";

  serializeJson(jsonDoc, measurement);

  return measurement;
}
