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

    //close file
    wififile.close();

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

    //close file
    wififile.close();

    Serial.println("wrote SSID and PAssword to wifi_data.txt on LittleFS with content:");
    Serial.println(content);
    
    return m_error_code;
}

/* in der Datei wifi_data.txt stehen SSID:PASWORD drin*/
int CPersistentSave::get_Color_Data(int& red, int& green, int& blue, int& bright) {
    m_error_code = ERR_NO_ERROR;

    /* wifi Datei Laden und auswerten */
    File colorfile = LittleFS.open("color_data.txt", "r");

    if (!colorfile) {
        m_error_code = ERR_LITTLE_FS_FILE_NOT_FOUND;
        Serial.println("ERROR: reading color_data.txt!");
        return m_error_code;
    } 

    String content = colorfile.readString();

    // close File
    colorfile.close();


    /* content nach SSID und PASSWORD durchsuchen */
    String RedStr = DEFAULT_ONCOLOR_RED;
    int redpos = content.indexOf("red:");
    if (redpos >= 0) RedStr = content.substring(redpos + 4, redpos + 4 + 3);
    Serial.printf("load Red-Value from LittleFS: %d\n", (int)RedStr.toInt());
    red = RedStr.toInt();
    
    String GreenStr = DEFAULT_ONCOLOR_GREEN;
    int greenpos = content.indexOf("green:");
    if (greenpos >= 0) GreenStr = content.substring(greenpos + 6, greenpos + 6 + 3);
    Serial.printf("load Green-Value from LittleFS: %d\n", (int)GreenStr.toInt());
    green = GreenStr.toInt();
    
    String BlueStr = DEFAULT_ONCOLOR_BLUE;
    int bluepos = content.indexOf("blue:");
    if (bluepos >= 0) BlueStr = content.substring(bluepos + 5, bluepos + 5 + 3);
    Serial.printf("load Blue-Value from LittleFS: %d\n", (int)BlueStr.toInt());
    blue = BlueStr.toInt();

    String BrightStr = DEFAULT_BRIGHTNESS;
    int brightpos = content.indexOf("bright:");
    if (brightpos >= 0) BrightStr = content.substring(brightpos + 7, brightpos + 7 + 3);
    Serial.printf("load Bright-Value from LittleFS: %d\n", (int)BrightStr.toInt());
    bright = BrightStr.toInt();

    return m_error_code;
}

int CPersistentSave::set_Color_Data(int red, int green, int blue, int bright) {
    m_error_code = ERR_NO_ERROR;
    
    /* colo Datei oeffen */
    File colorfile = LittleFS.open("color_data.txt", "w");
    
    char buffer[64];

    sprintf(buffer, "red:%03d\tgreen:%03d\tblue:%03d\tbrigth:%03d", red, green, blue, bright);

    colorfile.write(buffer, strlen(buffer));

    // close File
    colorfile.close();

    Serial.println("wrote RGB-BRIGHT to COLOR_data.txt on LittleFS");
    
    return m_error_code;
}
