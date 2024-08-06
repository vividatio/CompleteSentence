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

