#ifndef CWIFI_MISC_HPP
#define CWIFI_MISC_HPP

#include "declarations.hpp"

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

/* fuer den Webserver */
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "PersistentSave.hpp"

class CWIFI {
private:
    int             m_erroro_code;

   /* Parameter from Web-Server */
    AsyncWebServer* WebServerP;
    
public:

    CWIFI();
    ~CWIFI();
   
    CWIFI(const char * ssid, const char * passwd, CPersistentSave* LiFS);

    int init(const char * ssid, const char * passwd, CPersistentSave* LiFS);

    int print();

    /* error 404 callback fct*/
    void notFound(AsyncWebServerRequest *request);

};

/* Websocket functionalities not ion class, this caused compiler trouble */
    void notifyClients( String msg );                                                                   /* send msg to all Clients */
    void handleWebSocketMessage(AsyncWebSocketClient *client, void *arg, uint8_t *data, size_t len);    /* handle Msg from client */
    void onWebsocketsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, \
                           void *arg, uint8_t *data, size_t len);                                       /* */
    

#endif