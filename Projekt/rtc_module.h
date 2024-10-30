#ifndef RTC_MODULE_H
#define RTC_MODULE_H

#include <Arduino.h>
#include <RTClib.h>

class RTC_Module {
private:
    RTC_DS3231 rtc;
    RTC_Module();
    RTC_Module(const RTC_Module&);
    ~RTC_Module();

public:
    static RTC_Module & instance();
    void RTCSetup();
    char *getCurrent(); //być może do usunięcia
    char *getCurrentDate();
    char *getCurrentTime();
};

#endif
