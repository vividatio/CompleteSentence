#include "WiFi_misc.hpp"



/* Global Variable to last intance of CWIFI. Very horrorble Hack */
CWIFI* Pseudo_WiFi = NULL;

CPersistentSave* l_LittleFS; 



/* WebSocket holder */
AsyncWebSocket* WebSocketP;
/* Socket outside the class, because got callback troubles */



/* AccessPoint WLAN for initalization */
const char *AP_ssid     = "WLAN VON DER UHR";
const char *AP_password = "08154711";


/* Values, set by Webfrontend*/
String SSID_from_Client;
String Password_from_Client;


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
    /* damit gebe ich nicht-member_methoden zugriff auf die aktuelle instanz. da es nur eine Instanz geben wird, ist das ok */
    Pseudo_WiFi = this; 
    
 }

CWIFI::~CWIFI() {
    Pseudo_WiFi = NULL; 
}

CWIFI::CWIFI(const char * ssid, const char * passwd, CPersistentSave* LiFS) {



    m_erroro_code = init(ssid, passwd, LiFS);

    /* damit gebe ich nicht-member_methoden zugriff auf die aktuelle instanz. da es nur eine Instanz geben wird, ist das ok */
    Pseudo_WiFi = this; 
    
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

int CWIFI::init(const char * ssid, const char * passwd, CPersistentSave* LiFS) {

    //INIT Varialble 
    SSID_from_Client = String(ssid);
    Password_from_Client = String(passwd);

    l_LittleFS = LiFS;


    bool StartAsSoftAP = false;


    /* Falls eine Connection etableirt ist: beenden. */
    if ( WiFi.isConnected() ) {
        WiFi.disconnect();
    }

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

        // SSID und Password in wifi_data.txt abspeichern 
        LiFS->set_WiFi_Data(SSID_from_Client, Password_from_Client);

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
    WebServerP->on("/assets/images/Vividatio.webp", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/assets/images/Vividatio.webp", "image/webp");

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
        Serial.println("Client Verbinung wurde hergestellt!");
// ToDo: wirft Fehler weil kein JSON        client->text("user:welcome");
    }


    if ( type == WS_EVT_DISCONNECT ){
        Serial.println("Client Verbindung wurde beendet!");
    }

    if ( type == WS_EVT_DATA ){
        Serial.println("Websocket-Nachricht empfangen");
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
// ToDo: wirft fehler Weil kein JSON            notifyClients("user:new");
        }

        /* call for actual ssid, password, r,g,b and h data */
        if (message.indexOf("need_initial_data") >= 0 ) {
#ifdef DEBUG
            Serial.println("Client sgat: ich brauche initialdaten!");
#endif

            /* Collect Data an send to all Clients */
            /* for now, manualy constructed JSON Obj */
            String JSON_Data =  " {\"str_SSID\": \"some ssid\", \"str_Password\": \"some password\", \"int_Red\": 123, \"int_Green\": 251, \"int_Blue\": 189, \"int_Bright\": 125} ";
            // Debugausgabe
            Serial.println(JSON_Data);

            notifyClients(JSON_Data);
        }

        if (message.indexOf("reconect_wifi") >= 0 ) {
            /* coll CWIFI-Opject methode, reconnect_with_new WIFI() */ 
// FIXME: Uebler crash ...
            if (Pseudo_WiFi != NULL) {
                Pseudo_WiFi->init(SSID_from_Client.c_str(), Password_from_Client.c_str(), l_LittleFS);            
            }


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
