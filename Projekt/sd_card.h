#ifndef SD_CARD_H
#define SD_CARD_H

#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include "bme_module.h"
#include "rtc_module.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WebServer.h>

void SDCardSetup();
void writeFile(fs::FS &fs, const char * path, const char * message);
void appendFile(fs::FS &fs, const char * path, const char * message);
void saveMeasurement(fs::FS &fs, BME_Module & bme, RTC_Module & rtc);
char * prepareLineCSV(BME_Module & bme, RTC_Module & rtc);
int readCsvFileAsJSON(fs::FS &fs, const char* path, WebServer & server, int limit, int offset = 0);
int listDir(fs::FS &fs, const char * dirname, WebServer & server);

#endif
