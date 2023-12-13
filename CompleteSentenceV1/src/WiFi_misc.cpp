#include "WiFi_misc.hpp"


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

    WiFi.begin(ssid, passwd);

    Serial.print("connecting WIFI");
    while ( WiFi.status() != WL_CONNECTED ) {
        delay ( 500 );
        Serial.print ( "." );
    }

    // Lokale IP-Adresse im Seriellen Monitor ausgeben und Server starten
    this->print();
    return ERR_NO_ERROR;
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


// int WiFi_WebServer::init() {

//     WebServer = WiFiServer(80);

//     WebServer.begin();

//     return ERR_NO_ERROR;
// }


// int WiFi_WebServer::WebServerLoop(){

//     WebClient = WebServer.available();
//     if (WebClient) {
//         Serial.println("Client available!");
//     }

//     return ERR_NO_ERROR;
// }