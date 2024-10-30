#include "sd_card.h"

void SDCardSetup() {
  if (!SD.begin()) {
    Serial.println("Couldn't find SD card");
    while (1);
  }
  Serial.println("SD card initialized");
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)){
      Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

int readCsvFileAsJSON(fs::FS &fs, const char* path, WebServer & server, int limit, int offset) {
  File file = fs.open(path);
    if(!file){
      Serial.println("Failed to open file for reading");
      return -1;
    }

  DynamicJsonDocument jsonDoc(60000);
  JsonArray jsonArray = jsonDoc.to<JsonArray>();

  int rowIndex = 0;
  int columnIndex = 0;

  String cellValue = "";
  int limit_reached = 0;
  while(file.available()) {
    char currentChar = file.read();


    if (currentChar == ';' || currentChar == '\n') {
      if (rowIndex != 0 and rowIndex > offset) {
        if (columnIndex == 0) {
          jsonArray[rowIndex - 1 - offset]["Time"] = cellValue;
        } else if (columnIndex == 1) {
          jsonArray[rowIndex - 1 - offset]["Humidity"] = cellValue.toFloat();
        }else if (columnIndex == 2) {
          jsonArray[rowIndex - 1 - offset]["Temperature"] = cellValue.toFloat();
        }else if (columnIndex == 3) {
          jsonArray[rowIndex - 1 - offset]["Pressure"] = cellValue.toFloat();
        }
      }

      if (currentChar == '\n') {
        rowIndex++;
        columnIndex = 0;
        if(rowIndex - offset > limit)
        {
          limit_reached = 1;
          break;
        }
        
      } else {
        columnIndex++;
      }
      cellValue = "";
    } else {
      cellValue += currentChar;
    }
  }

  file.close();
  
  String * jsonString = new String;
  serializeJson(jsonDoc, *jsonString);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", *jsonString);
  server.client().stop();
  
  Serial.println(ESP.getFreeHeap());
  jsonArray.clear(); 
  jsonDoc.clear();

  delete jsonString;
  
  return limit_reached;
}

void saveMeasurement(fs::FS &fs, BME_Module & bme, RTC_Module & rtc) {
  char * path = new char[16];
  
  sprintf(path, "/%s.csv", rtc.getCurrentDate());
  
  if (fs.exists(path)) {
    Serial.print("File ");
    Serial.print(path);
    Serial.println(" exists!");

    appendFile(fs, path, prepareLineCSV(bme, rtc));
  } else {
    Serial.print("File ");
    Serial.print(path);
    Serial.println(" does not exist.");
    writeFile(fs, path, "Time;Humidity;Temperature;Pressure;\n");
  }
  delete path;
}

char * prepareLineCSV(BME_Module & bme, RTC_Module & rtc) {
    bme.takeMeasurement();
    static char bufferLine[34];
    sprintf(bufferLine,"%s;%.2f;%.2f;%.2f;\n", rtc.getCurrentTime(), bme.getHumidityBME(), bme.getTemperatureBME(), bme.getPressureBME());
    return bufferLine;
}

int listDir(fs::FS &fs, const char * dirname, WebServer & server){
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return -1;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return -1;
  }

  DynamicJsonDocument jsonDoc(10000);
  JsonArray jsonArray = jsonDoc.to<JsonArray>();
  
  int * i = new int;
  *i = 0;
  
  File file = root.openNextFile();
  while(file){
      if (String(file.name()).startsWith("/2")) {
        String * fileName = new String;
        *fileName = file.name();
        Serial.println(*fileName);
        jsonArray[*i]["File"] = *fileName;
        (*i)++;
        delete fileName;
      }
      file = root.openNextFile();
  }

  String * fileNames = new String;
  *fileNames = "";
  serializeJson(jsonDoc, *fileNames);
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", *fileNames);
  server.client().stop();

  delete fileNames;
  delete i;
  return 1;
}
