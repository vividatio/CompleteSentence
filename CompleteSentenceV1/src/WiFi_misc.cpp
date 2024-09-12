#include "WiFi_misc.hpp"

#include <LittleFS.h>


/* WebSocket holder */
AsyncWebSocket* WebSocketP;
/* Socket outside the class, because got callback troubles */



/* AccessPoint WLAN for initalization */
const char *AP_ssid     = "WLAN VON DER UHR";
const char *AP_password = "08154711";



String sliderValueRed = "255";
String sliderValueGreen = "163";
String sliderValueBlue = "1";


int    SliderRed = 255;
int    SliderGreen = 163;
int    SliderBlue = 1;


// Replaces placeholder with button section in your web page
String processor(const String& var){
  if (var == "SLIDERVALUE_RED"){
    return sliderValueRed;
  }
  if (var == "SLIDERVALUE_GREEN"){
    return sliderValueGreen;
  }
  if (var == "SLIDERVALUE_BLUE"){
    return sliderValueBlue;
  }


  return String();
}


CWIFI::CWIFI() {
    // nix
 }

CWIFI::~CWIFI() {
    //nix
}

CWIFI::CWIFI(const char * ssid, const char * passwd) {
    
    m_erroro_code = init(ssid, passwd);
}


/* Falls eine Webseite nicht gefunden wird */
void CWIFI::notFound(AsyncWebServerRequest *request) {
    AsyncWebServerResponse* response = request->beginResponse(LittleFS, "/404.html", "text/html");
      response->setCode(404);
      request->send(response);
}

/*  
    1. Voreingestellt wird ein WLAN, dass ich kenne. die Verbindung sollte funktionieren 
    2. Sollte das Connect nach eine gewissen Zeit nicht zu stande kommen wird ein ssid und pswd vorgegeben, und damit ein AccessPoint erstellt 
    3. Ober einen Webserver kann dann die neue WLAN Verbindung eingegeben und persistent gespeichert werden
    4. beim restart wird das WLAN aus dem Pers Speicher geladen und ein Connect versuchet. 
    5. Erfolgreich? dann weiter mit dem NTP kram Nicht Erfolgreich: Dann zurueck zu 2. 
*/

int CWIFI::init(const char * ssid, const char * passwd) {

    bool StartAsSoftAP = false;

    /* regular StartUp */
    WiFi.begin(ssid, passwd);

    Serial.print("connecting WIFI like defined ");

    int LoopCounter = 50; 

    while ( WiFi.status() != WL_CONNECTED ) {
        delay ( 500 );
        LoopCounter--;
        Serial.print ( "." );
        if (LoopCounter <= 0) {
              StartAsSoftAP = true;
            Serial.println("Conection failed: starting AccessPoint - Mode");
            break;
        }
    }

    if (StartAsSoftAP) {
        /* WIFI wird AccessPoint */
        WiFi.softAPConfig(IPAddress(192,168,100,1), IPAddress(192,168,100,255), IPAddress(255,255,255,0) );
        WiFi.softAP(AP_ssid, AP_password);

        IPAddress myIP = WiFi.softAPIP();
        String mySSID = WiFi.softAPSSID();
        Serial.print("IP Adresse des Access Points: ");
        Serial.println(myIP);
    } else {
        Serial.println("Connected like defined ");
        /* Wenn externes WLAN connected */
        // Lokale IP-Adresse im Seriellen Monitor ausgeben und Server starten
        this->print();
    }

    /* set reconnection true */
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);

    /* ********************************** now initializing of the WebServer ******************************** */
    WebServerP = new AsyncWebServer(80);

    /* the MainPage HTML-File*/
    WebServerP->on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/index.html");
    });

    /* sending css by request from LittleFS */
    WebServerP->on("/assets/css/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/assets/css/style.css", "text/css");
    });

    /* sending Javascript by from LittleFS */
    WebServerP->on("/assets/js/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/assets/js/script.js", "application/javascript");
    });

   /* my nice WordClock Image */
    WebServerP->on("/assets/images/favicon-96.png", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/assets/images/favicon-96.png", "image/png");

    });

    /* ********************************** and now the WebSocket ******************************** */
    WebSocketP = new AsyncWebSocket("/ws");


    /* set Callback for WebSocket event handling */
    WebSocketP->onEvent(onWebsocketsEvent); 
    /* Tell the Server about WebSocket */
    WebServerP->addHandler( WebSocketP ); 

    WebServerP->begin();

    return ERR_NO_ERROR;
}


/* ********************************************************************************************************************** */
/* ************************************************** SebSocket Handling ************************************************ */
/* ********************************************************************************************************************** */

void onWebsocketsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len){

    if ( type == WS_EVT_CONNECT ){
#ifdef DEBUG
         Serial.println("Client Verbinung wurde hergestellt!");
#endif
        client->text("user:welcome");
    }


    if ( type == WS_EVT_DISCONNECT ){
#ifdef DEBUG
        Serial.println("Client Verbindung wurde beendet!");
#endif  
    }

    if ( type == WS_EVT_DATA ){
#ifdef DEBUG
        Serial.println("Websocket-Nachricht empfangen");
#endif
        handleWebSocketMessage ( client, arg, data, len );
    }
}


/* this Message is a broadcast to all clients */
void notifyClients( String msg){ 

    WebSocketP->textAll(msg); 

}

/*  client->text( "Diese Nachricht geht nur an den Empfänger zurück!" ); */


void handleWebSocketMessage(AsyncWebSocketClient *client, void *arg, uint8_t *data, size_t len) {

#ifdef DEBUG
    Serial.println("handleWebSocketMessage();");
#endif

    AwsFrameInfo *info = ( AwsFrameInfo * )arg;

    if ( (info->final) && (info->index == 0) && (info->len == len) && (info->opcode == WS_TEXT) ){
        /* Enthält den Inhalt der Nachricht vom Client */
        String message = String( (char *) data );
    
        if ( message.indexOf("connection:new") >= 0 ){
#ifdef DEBUG
            Serial.println("Ein neuer Client hat sich verbunden");
#endif
            notifyClients("user:new");
        }
    }
}



int CWIFI::print(){
    m_erroro_code = ERR_NO_ERROR;

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("SSID      : ");
    Serial.println(WiFi.SSID());
    Serial.print("Channel   : ");
    Serial.println(WiFi.channel());
    Serial.print("RSSI      : ");
    Serial.println(WiFi.RSSI());

    return m_erroro_code;
}
