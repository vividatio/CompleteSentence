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




};



#endif