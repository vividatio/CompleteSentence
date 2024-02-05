#ifndef CWIFI_MISC_HPP
#define CWIFI_MISC_HPP

#include "declarations.hpp"

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

/* fuer den Webserver */
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

class CWIFI {
private:
    int             m_erroro_code;

    AsyncWebServer* WebServerP;

    /* Parameter from Web-Server */
    
public:

    CWIFI();
    ~CWIFI();
   
    CWIFI(const char * ssid, const char * passwd);

    int init(const char * ssid, const char * passwd);

    int get_SliederRed();
    
    int get_SliederGreen();

    int get_SliederBlue();

    int print();
};

#endif