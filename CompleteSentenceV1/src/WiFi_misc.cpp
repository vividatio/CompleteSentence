#include "WiFi_misc.hpp"


/* Global Variable to last intance of CWIFI. Very horrorble Hack */
CWIFI* Main_WiFi_Reference = NULL;
CLEDControl* Main_LEDControl = NULL;
CPersistentSave* Main_LittleFS = NULL; 

int * Main_Mode_Ptr = NULL;


/* WebSocket holder */
AsyncWebSocket* WebSocketP;
/* Socket outside the class, because got callback troubles */

/* AccessPoint WLAN for initalization */
const char *AP_ssid     = "WLAN VON DER UHR";
const char *AP_password = "08154711";


/* Values, set by Webfrontend*/
String SSID_from_Client;
String Password_from_Client;


CWIFI::CWIFI() {
    /* damit gebe ich nicht-member_methoden zugriff auf die aktuelle instanz. da es nur eine Instanz geben wird, ist das ok */
    Main_WiFi_Reference = NULL; 
    Main_LEDControl = NULL;
    Main_LittleFS = NULL;
 }

CWIFI::~CWIFI() {
    Main_WiFi_Reference = NULL;
    Main_LEDControl = NULL;
    Main_LittleFS = NULL; 
}

CWIFI::CWIFI(String ssid, String passwd, CWIFI *MainWIFI, CLEDControl *LEDControl, CPersistentSave* LiFS, int* ModePtr) {

    m_erroro_code = init(ssid, passwd, MainWIFI, LEDControl, LiFS, ModePtr);
    
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

int CWIFI::init(String ssid, String passwd, CWIFI *MainWIFI, CLEDControl *LEDControl,  CPersistentSave* LiFS, int* ModePtr) {

    //INIT Varialble 
    SSID_from_Client = ssid;
    Password_from_Client = passwd;

    Main_LittleFS = LiFS;
    Main_LEDControl = LEDControl;
    Main_WiFi_Reference = MainWIFI;

    Main_Mode_Ptr = ModePtr;

    bool StartAsSoftAP = false;

    /* Falls eine Connection etableirt ist: beenden. */
    if ( WiFi.isConnected() ) {
        WiFi.disconnect();
    }

    /* regular StartUp */
    WiFi.begin(SSID_from_Client, Password_from_Client);

    Serial.printf("connecting WIFI like defined (SSID:'%s'; Password:'%s'\n", SSID_from_Client.c_str(), Password_from_Client.c_str());

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

        Serial.printf("WordClock starts as Access Point with \n\tSSID    : '%s'\n\tPASSWORD: '%s'\n", mySSID.c_str(), myIP.toString().c_str());

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
            Serial.println("Ein neuer Client hat sich verbunden");
            return; // fertig
        }

        /* call for actual ssid, password, r,g,b and h data */
        if (message.indexOf("need_initial_data") >= 0 ) {
            // String JSON_Data =  " {\"str_SSID\": \"some ssid\", \"str_Password\": \"some password\", \"int_Red\": 123, \"int_Green\": 251, \"int_Blue\": 189, \"int_Bright\": 125} ";
            // Debugausgabe
            //  Serial.println(JSON_Data);
            //  send
            //  notifyClients(JSON_Data);

            int r, g, b, h;
            Main_LEDControl->get_onColor(r, g, b);
            h = Main_LEDControl->get_brightnes();

            char jsonString[256];
            sprintf(jsonString, "{\"str_SSID\": \"%s\", \"str_Password\": \"%s\", \"int_Red\": %3d, \"int_Green\": %3d, \"int_Blue\": %3d, \"int_Bright\": %3d}", \
                SSID_from_Client.c_str(), \
                Password_from_Client.c_str(), \
                r, g, b, h);

            /* send it to client(s) */
            notifyClients(jsonString);
            
            return; // fertig
        }

        if (message.indexOf("reconect_wifi") >= 0 ) {
            /* coll CWIFI-Opject methode, reconnect_with_new WIFI() */ 
        
            // in LittleFS speichern
            Main_LittleFS->set_WiFi_Data(SSID_from_Client, Password_from_Client);

            /* RESTART Macro */
            RESTART_ALL();

            return; // fertig
        }

        if (message.indexOf("change_ssid") >= 0) {
            String SSID_String = message.substring(message.indexOf(":") + 1, message.indexOf("_____finish"));

            SSID_from_Client = SSID_String;

            Serial.printf("Websocket got SSID: %s \n", SSID_from_Client.c_str());

            return; // fertig
        }


        if (message.indexOf("change_password") >= 0) {
            
            String Password_String = message.substring(message.indexOf(":") + 1, message.indexOf("_____finish"));

            Password_from_Client = Password_String;

            Serial.printf("Websocket got Password: %s \n", Password_from_Client.c_str());

            return; // fertig
        }

        if (message.indexOf("change_mode") >= 0) {
            /* zwei Modi implemented yet */
            *Main_Mode_Ptr = (*Main_Mode_Ptr) + 1;
            
            if  (*Main_Mode_Ptr > 1) *Main_Mode_Ptr = 0;
            
            Serial.printf("Change mode to [%d]\n", *Main_Mode_Ptr);

            return; //fertig
        }

        if (message.indexOf("change_color") >= 0) {

            String Color_String = message.substring(message.indexOf(":") + 1);

            /* setColor*/
            // ToDo: Set Color

            int red_send = Color_String.substring(0, 3).toInt();
            int green_send = Color_String.substring(4, 7).toInt();
            int blue_send = Color_String.substring(8, 11).toInt();
            int bright_send = Color_String.substring(12, 15).toInt();

            // Serial.printf("Websocket got Color: %s \n", Color_String.c_str());

            // Serial.println("interpret:");
            // Serial.printf("red = %d: grenn = %d; blue = %d, bright = %d\n", red_send, green_send, blue_send, bright_send);

            /* set Color of LEDs */
            if (Main_LEDControl != NULL) {
                Main_LEDControl->set_onColor(red_send, green_send, blue_send);
                Main_LEDControl->set_brightnes(bright_send);
            }
            return; // fertig
        }

        if (message.indexOf("save_color") >= 0) {
            int r, g, b, h;

            Main_LEDControl->get_onColor(r,g,b);
            h = Main_LEDControl->get_brightnes();

            Main_LittleFS->set_Color_Data(r, g, b, h);
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
