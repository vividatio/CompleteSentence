#ifndef PERSISTENT_SAVE_HPP
#define PERSISTENT_SAVE_HPP

#include "declarations.hpp"
#include "EEPROM_Rotate.h"

constexpr int DataOffset = 10;


/* WLAN EEPROM */
constexpr int ssid_position = 0;
constexpr int ssid_max_length = 128;

constexpr int password_position = ssid_position + ssid_max_length + 4;
constexpr int password_max_length = 256;

constexpr int misc_position = password_position + password_max_length + 4;
constexpr int misc_max_length = 64;

constexpr int time_color_position = misc_position + misc_max_length + 4;
constexpr int time_color_max_length = 4;



class CPersistentSave {
private:
    int                 m_error_code;
    EEPROM_Rotate       m_EEPROM;
    
    /// @brief Write the string at position offset to EEPROM
    /// @param str the string to write
    /// @param offset the offset position in memory
    /// @return != ERR_NO_ERROR if failed
    int write_string(String str, int offset);

public:
    CPersistentSave();
    ~CPersistentSave();

    int init();
    
    /// @brief writes wlan ssid and password and misc. informations to EEPROM
    /// @param ssid WLAN name max 128 Bytes 
    /// @param pws WLAN password max 256 Bytes
    /// @param misc misc. additional Information not specified max 64 Bytes
    /// @return != ERR_NO_ERROR if failed
    int write_network(String ssid, String pws, String misc);

    /// @brief Write the collor of the words into the EEPROM
    /// @param red 0-255
    /// @param green 0-255
    /// @param blue 0-255
    /// @return != ERR_NO_ERROR if failed
    int write_time_color(uint8_t red, uint8_t green, uint8_t blue);



};



#endif