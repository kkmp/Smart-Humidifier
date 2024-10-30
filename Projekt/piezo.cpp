#include "piezo.h"

Piezo_Module::Piezo_Module() : isOn(false), isAutomode(true), isMoistureOn(false), minHumidity(30) {}

Piezo_Module::Piezo_Module(const Piezo_Module&) {}

Piezo_Module::~Piezo_Module() {}

Piezo_Module & Piezo_Module::instance() {
  static Piezo_Module instance_;
  return instance_;
}

void Piezo_Module::piezoRed() {
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);
}

void Piezo_Module::piezoGreen() {
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, HIGH);
}

void Piezo_Module::piezoBlue() {
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, LOW);
}

void Piezo_Module::piezoSetup() {
  pinMode(PIEZO_PIN, OUTPUT);
  digitalWrite(PIEZO_PIN, HIGH);
  this->isOn = false;
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);
}

void Piezo_Module::piezoOn(int delayTime) {
  digitalWrite(PIEZO_PIN, LOW);
  delay(delayTime);
  digitalWrite(PIEZO_PIN, HIGH);
  this->isOn = true;
  this->isMoistureOn = true;
  this->piezoGreen();
  makeSound();
}

void Piezo_Module::piezoOff(int delayTime) {
  if (this->isMoistureOn) {
    digitalWrite(PIEZO_PIN, LOW);
    delay(delayTime);
    digitalWrite(PIEZO_PIN, HIGH);
    delay(delayTime);
    digitalWrite(PIEZO_PIN, LOW);
    delay(delayTime);
    digitalWrite(PIEZO_PIN, HIGH);
    this->isMoistureOn = false;
  }
  this->isOn = false;
  this->piezoRed();
  makeSound();
}

bool Piezo_Module::getIsOn() {
  return this->isOn;
}

bool Piezo_Module::getIsAutomode() {
  return this->isAutomode;
}

int Piezo_Module::getMinHumidity() {
  return this->minHumidity;
}

String Piezo_Module::getIsOnJSON() {
  DynamicJsonDocument jsonDoc(20);
  jsonDoc["isOn"] = this->getIsOn();
  String isOn = "";
  serializeJson(jsonDoc, isOn);
  return isOn;
}

void Piezo_Module::setPower(bool power) {
  if (power) {
    this->isAutomode = true;
    this->minHumidity = MIN_HUMIDITY;
    this->piezoOn();
  } else {
    this->piezoOff();
  }
}

void Piezo_Module::setMode(bool mode) {
  this->minHumidity = MIN_HUMIDITY;
  if (mode) {
    this->isAutomode = true;
    this->piezoGreen();
  } else {
    this->isAutomode = false;
    this->piezoBlue();
  }
  makeSound();
}

String Piezo_Module::getIsAutomodeJSON() {
  DynamicJsonDocument jsonDoc(20);
  jsonDoc["isAutomode"] = this->getIsAutomode();
  String isAutomode = "";
  serializeJson(jsonDoc, isAutomode);
  return isAutomode;
}

void Piezo_Module::setParameter(int minHumidity) {
  this->minHumidity = minHumidity;
  makeSound();
}

String Piezo_Module::getParameter() {
  DynamicJsonDocument jsonDoc(20);
  jsonDoc["minHumidity"] = this->getMinHumidity();
  String parameter = "";
  serializeJson(jsonDoc, parameter);
  return parameter;
}

void Piezo_Module::turnMoistureOff(int delayTime) {
  digitalWrite(PIEZO_PIN, LOW);
  delay(delayTime);
  digitalWrite(PIEZO_PIN, HIGH);
  delay(delayTime);
  digitalWrite(PIEZO_PIN, LOW);
  delay(delayTime);
  digitalWrite(PIEZO_PIN, HIGH);
  this->isMoistureOn = false;
}

void Piezo_Module::turnMoistureOn(int delayTime) {
  digitalWrite(PIEZO_PIN, LOW);
  delay(delayTime);
  digitalWrite(PIEZO_PIN, HIGH);
  this->isMoistureOn = true;
}

void Piezo_Module::piezoControl(BME_Module & bme) {
  if (this->isOn) {
    bme.takeMeasurement();
    float * humidity = new float;
    *humidity = bme.getHumidityBME();

    if (!this->isMoistureOn and * humidity < this->minHumidity) {
      this->turnMoistureOn();
    } else if (this->isMoistureOn and * humidity > this->minHumidity) {
      this->turnMoistureOff();
    }

    delete humidity;
  }
}
