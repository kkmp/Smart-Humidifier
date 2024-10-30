#include "rtc_module.h"

RTC_Module::RTC_Module() {}

RTC_Module::RTC_Module(const RTC_Module&) {}

RTC_Module::~RTC_Module() {}

RTC_Module & RTC_Module::instance() {
    static RTC_Module instance_;
    return instance_;
}

void RTC_Module::RTCSetup() {
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  Serial.println("RTC initialized");
}

char * RTC_Module::getCurrent()
{
    DateTime date = rtc.now();
    String formattedDate = "";

    for (auto i : {date.hour(), date.minute(), date.second()})
    {
        if (i <= 9)
        {
            formattedDate += "0";
        }
        formattedDate += (String)i + ":";
    }

    Serial.println(formattedDate.substring(0, formattedDate.length() - 1));
}

char * RTC_Module::getCurrentDate()
{
    DateTime date = rtc.now();

    static char bufferDate[12];
    sprintf(bufferDate,"%04u-%02u-%02u", date.year(), date.month(), date.day());

    return bufferDate;
}

char * RTC_Module::getCurrentTime()
{
    DateTime date = rtc.now();

    static char bufferTime[10];
    sprintf(bufferTime,"%02u:%02u:%02u", date.hour(), date.minute(), date.second());
    
    return bufferTime;
}
