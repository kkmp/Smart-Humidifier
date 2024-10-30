#include "piezo.h"
#include "sd_card.h"
#include "rtc_module.h"
#include "bme_module.h"
#include "buzzer_module.h"
#include <WiFi.h>
#include <WebServer.h>

#define BUTTON 35
#define LED_ERROR 26

RTC_Module & rtc = RTC_Module::instance();
BME_Module & bme = BME_Module::instance();
Piezo_Module & piezo = Piezo_Module::instance();

WebServer server(80);

unsigned long previousMillis = 0;
const long interval = 60000;

const char* ssid = "Le réseau de Charles";
const char* password = "intmain()";

void setup() {
  Serial.begin(115200);

  pinMode(LED_ERROR, OUTPUT);
  pinMode(BUTTON, INPUT);
  delay(1000);

  digitalWrite(LED_ERROR, HIGH);

  piezo.piezoSetup();
  SDCardSetup();
  rtc.RTCSetup();
  bme.BMESetup();
  buzzerSetup();

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    i++;
    if (i % 2 == 0)
    {
      Serial.print(".");
    } else {
      Serial.print("+");
    }
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  server.on("/data", HTTP_GET, handleGetRequest);
  server.on("/listFiles", HTTP_GET, handleGetFiles);
  server.on("/getMeasurement", HTTP_GET, handleGetMeasurement);
  server.on("/piezoToggle", HTTP_GET, handlePiezoToggle);
  server.on("/isOn", HTTP_GET, handleGetIsOn);
  server.on("/piezoMode", HTTP_GET, handleMode);
  server.on("/isAutomode", HTTP_GET, handleGetIsAutomode);
  server.on("/setParameters", HTTP_GET, handleSetParameters);
  server.on("/getParameters", HTTP_GET, handleGetParameters);

  server.begin();
  Serial.println("HTTP server started");

  digitalWrite(LED_ERROR, LOW);
  piezo.piezoRed();
}

void handleGetRequest() {
  if (server.hasArg("page")) {
    if (!server.hasArg("date")) {
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(400, "text/plain", "No date parameter");
      server.client().stop();
      return;
    }

    int page = server.arg("page").toInt();

    if (page <= 0) {
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(400, "text/plain", "Invaild page parameter");
      server.client().stop();
      return;
    }

    int limit = 100;
    int offset = (page - 1) * limit;

    String date = "/" + server.arg("date") + ".csv";

    char *dateChar = new char[date.length() + 1];
    strcpy(dateChar, date.c_str());

    Serial.println("Selected date:");
    Serial.println(date);

    int status = readCsvFileAsJSON(SD, dateChar, server, limit, offset);//18

    Serial.print("Status: ");
    Serial.println(status);

    if (status == -1) {
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(500, "text/plain", "Error occurred");
      server.client().stop();
      return;
    }
  }
  else {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(400, "text/plain", "No page parameter");
    server.client().stop();
  }
}

void handleGetFiles() {
  int status = listDir(SD, "/", server);

  if (status == -1) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(500, "text/plain", "Error occurred");
    server.client().stop();
    return;
  }
}

void handleGetMeasurement() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", bme.getMeasurementJSON());
  server.client().stop();
}

void handlePiezoToggle() {
  if (server.hasArg("power")) {
    bool power = server.arg("power").toInt();
    piezo.setPower(power);
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", piezo.getIsOnJSON());
    server.client().stop();
  } else {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(400, "text/plain", "No power parameter");
    server.client().stop();
  }
}

void handleGetIsOn() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", piezo.getIsOnJSON());
  server.client().stop();
}

void handleMode() {
  if (server.hasArg("mode")) {
    bool mode = server.arg("mode").toInt();
    piezo.setMode(mode);
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", piezo.getIsAutomodeJSON());
    server.client().stop();
  } else {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(400, "text/plain", "No mode parameter");
    server.client().stop();
  }
}

void handleGetIsAutomode() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", piezo.getIsAutomodeJSON());
  server.client().stop();
}

void handleSetParameters() {
  if (server.hasArg("minHumidity")) {
    int minHumidity = server.arg("minHumidity").toInt();

    piezo.setParameter(minHumidity);
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", piezo.getParameter());
    server.client().stop();
  } else {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(400, "text/plain", "Missing humidity parameter");
    server.client().stop();
  }
}

void handleGetParameters() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", piezo.getParameter());
  server.client().stop();
}

void loop() {
  server.handleClient();
  
  unsigned long currentMillis = millis();
  
  if (digitalRead(BUTTON)) {
    piezo.setPower(!piezo.getIsOn());
    delay(500);
  }

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    saveMeasurement(SD, bme, rtc);
    piezo.piezoControl(bme);
  }
}
