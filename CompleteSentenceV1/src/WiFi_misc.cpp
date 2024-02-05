#include "WiFi_misc.hpp"

#include "html.hpp"





/* AccessPoint WLAN */
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


/*  
    1. Voreingestellt wird ein WLAN, dass ich kenne. die Verbindung sollte funktionieren 
    2. Sollte das Connect nach eine gewissen Zeit nicht zu stande kommen wird ein ssid und pswd vorgegeben, und damit ein AccessPoint erstellt 
    3. Ober einen Webserver kann dann die neue WLAN Verbindung eingegeben und persitent gespeichert werden
    4 beim restart wird das WLAN aus dem Pers Speicher geladen und ein Connect versuchet. 
    5. Erfolgreich? dann weiter mit dem NTP kram Nicht Erfolgreich: Dann zurueck zu 2. 
*/

int CWIFI::init(const char * ssid, const char * passwd) {

    bool StartAsSoftAP = false;

    /* regular StartUp */
    WiFi.begin(ssid, passwd);

    Serial.print("connecting WIFI");

    int LoopCounter = 50; 

    while ( WiFi.status() != WL_CONNECTED ) {
        delay ( 500 );
        LoopCounter--;
        Serial.print ( "." );
        if (LoopCounter <= 0) {
            StartAsSoftAP = true;
            Serial.println("");
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
        Serial.println("");
        /* Wenn externes WLAN connected */
        // Lokale IP-Adresse im Seriellen Monitor ausgeben und Server starten
        this->print();
    }

   // nun wird der WebServer initialisiert
    WebServerP = new AsyncWebServer(80);

    WebServerP->on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html, processor);
    });

    // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
    WebServerP->on("/sliderRed", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String inputMessage;
        
        // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
        if (request->hasParam(PARAM_INPUT)) {
            inputMessage = request->getParam(PARAM_INPUT)->value();
            sliderValueRed = inputMessage;
            SliderRed = atoi(sliderValueRed.c_str());
        } else {
            inputMessage = "No message sent";
        }
        Serial.print("Red: ");
        Serial.println(inputMessage);
        request->send(200, "text/plain", "OK");
    });

    WebServerP->on("/sliderGreen", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String inputMessage;
        
        // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
        if (request->hasParam(PARAM_INPUT)) {
            inputMessage = request->getParam(PARAM_INPUT)->value();
            sliderValueGreen = inputMessage;
            SliderGreen = atoi(sliderValueGreen.c_str());
        } else {
            inputMessage = "No message sent";
        }
        Serial.print("Green: ");
        Serial.println(SliderGreen);
        request->send(200, "text/plain", "OK");
    });

    WebServerP->on("/sliderBlue", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String inputMessage;
        
        // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
        if (request->hasParam(PARAM_INPUT)) {
            inputMessage = request->getParam(PARAM_INPUT)->value();
            sliderValueBlue = inputMessage;
            SliderBlue = atoi(sliderValueBlue.c_str());
        } else {
            inputMessage = "No message sent";
        }
        Serial.print("Blue: ");
        Serial.println(SliderBlue);
        request->send(200, "text/plain", "OK");
    });

    WebServerP->begin();

    return ERR_NO_ERROR;
}


int CWIFI::get_SliederRed() {
    return SliderRed;
}
    
int CWIFI::get_SliederGreen() {
    return SliderGreen;
}

int CWIFI::get_SliederBlue() {
    return SliderBlue;
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
