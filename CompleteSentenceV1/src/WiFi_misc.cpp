#include "WiFi_misc.hpp"

#include "html.hpp"

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


int CWIFI::init(const char * ssid, const char * passwd) {

    // IPAddress l_IP {192,168,68,155};
    // IPAddress l_SNM {255,255,255,0};
    // IPAddress l_GTW {192,168,68,1};


    // WiFi.config(l_IP, l_GTW, l_SNM);

    WiFi.begin(ssid, passwd);

    Serial.print("connecting WIFI");
    while ( WiFi.status() != WL_CONNECTED ) {
        delay ( 500 );
        Serial.print ( "." );
    }

    // Lokale IP-Adresse im Seriellen Monitor ausgeben und Server starten
    this->print();

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
