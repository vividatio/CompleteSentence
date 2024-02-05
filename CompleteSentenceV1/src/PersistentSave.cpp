#include "PersistentSave.hpp"



CPersistentSave::CPersistentSave() {
    m_error_code = ERR_NO_ERROR;

}

CPersistentSave::~CPersistentSave() {
    m_error_code = ERR_NO_ERROR;

}

int CPersistentSave::init() {
    m_error_code = ERR_NO_ERROR;

    m_EEPROM.size(4);       /* 4 sectors to rotate */
    m_EEPROM.begin(4096);   /* 4096 Byte DataSize per sector */

    return m_error_code;
}

int CPersistentSave::write_string(String str, int offset) {
    m_error_code = ERR_NO_ERROR;
    for (int i = 0; i < str.length(); i++) {
        m_EEPROM.write(DataOffset + offset + i, str[i]); 
    }
    m_EEPROM.write(DataOffset + offset + str.length(), '\0'); // 0 terminirter string 
    if (!m_EEPROM.commit()) {
        m_error_code = ERR_EEPROM_WRITE_COMMIT;
    }
    return m_error_code;
}


int CPersistentSave::write_network(String ssid, String pws, String misc) {
    m_error_code = ERR_NO_ERROR;

    if (ssid.length() <= ssid_max_length) {
        this->write_string(ssid, ssid_position);
    } else {
        m_error_code = ERR_EEPROM_SSID_LENGTH;
        return m_error_code;
    }

    if (pws.length() <= password_max_length) {
        this->write_string(pws, password_position);
    } else {
        m_error_code = ERR_EEPROM_PWD_LENGTH;
        return m_error_code;
    }

    if (misc.length() <= misc_max_length) {
        this->write_string(misc, misc_position);
    } else {
        m_error_code = ERR_EEPROM_MISC_LENGTH;
        return m_error_code;
    }

    return m_error_code;
}

int CPersistentSave::write_time_color(uint8_t red, uint8_t green, uint8_t blue) {
    m_error_code = ERR_NO_ERROR;

    String s = "aaa";
    s[0] = red;
    s[1] = green;
    s[2] = blue;

    return this->write_string(s, time_color_position);
}