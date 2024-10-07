#ifndef PERSISTENT_SAVE_HPP
#define PERSISTENT_SAVE_HPP

#include "declarations.hpp"

#include "LittleFS.h"

class CPersistentSave {
private:
    int                 m_error_code;

public:
    CPersistentSave();
    ~CPersistentSave();

    int init();

    int get_WiFi_Data(String& SSID, String& Password);
    int set_WiFi_Data(String SSID, String Password);
    
    int get_Color_Data(int& red, int& green, int& blue, int& bright);
    int set_Color_Data(int red, int green, int blue, int bright);

};



#endif