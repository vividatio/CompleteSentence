#include "PersistentSave.hpp"

CPersistentSave::CPersistentSave() {
    m_error_code = ERR_NO_ERROR;

}

CPersistentSave::~CPersistentSave() {
    m_error_code = ERR_NO_ERROR;

}

int CPersistentSave::init() {
    m_error_code = ERR_NO_ERROR;

    if (!LittleFS.begin()) {
        m_error_code = ERR_LITTLE_FS_INIT_FAILED;
        Serial.println("File system LittleFS not initialized!");
    } else {
        Serial.println("File system LittleFS initialized.");
    }
 
    return m_error_code;
}

/* in der Datei wifi_data.txt stehen SSID:PASWORD drin*/
int CPersistentSave::get_WiFi_Data(String& SSID, String& Password) {
    m_error_code = ERR_NO_ERROR;

    /* wifi Datei Laden und auswerten */
    File wififile = LittleFS.open("wifi_data.txt", "r");

    if (!wififile) {
        m_error_code = ERR_LITTLE_FS_FILE_NOT_FOUND;
        Serial.println("ERROR: reading wifi_data.txt!");
        return m_error_code;
    } 
    String content = wififile.readString();

    /* content nach SSID und PASSWORD durchsuchen */
    int TrennPosition = content.indexOf(':');

    if (TrennPosition != -1 ) {
        SSID = content.substring(0,TrennPosition);
        Password = content.substring(TrennPosition + 1);

        /* Debugausgabe */
        Serial.println("Loaded from LittleFS: wifi_data.txt with content:");
        Serial.printf("SSID     : %s \n Password : %s \n", SSID.c_str(), Password.c_str());
    } else {
        m_error_code = ERR_LITTLEFS_WIFI_DELIMITER_NOT_FOUND;
        Serial.println("ËRROR: delimiter not found!");   
    }

    return m_error_code;
}

int CPersistentSave::set_WiFi_Data(String SSID, String Password) {
    m_error_code = ERR_NO_ERROR;
    
    /* wifi Datei oeffen */
    File wififile = LittleFS.open("wifi_data.txt", "w");
    
    String content = SSID + ":" + Password; 

    wififile.write(content.c_str(), content.length() );

    Serial.println("wirte SSID and PAssword to wifi_data.txt on LittleFS");
    
    return m_error_code;
}
