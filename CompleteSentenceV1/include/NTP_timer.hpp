#ifndef NTP_TIMER_HPP
#define  NTP_TIMER_HPP
#include "declarations.hpp"

#include "WiFI_misc.hpp"

//#include <NTPClient.h>

#include <time.h>

class CNTPtimer {
private:
    int             m_error_code;

    WiFiUDP         m_ntpUDP;

    time_t          m_now;                         // this are the seconds since Epoch (1970) - UTC
    tm              m_tm;                                


public:
    
    CNTPtimer();

    ~CNTPtimer();

    int init();

    int restart();

    int update_via_NTP();

    bool check();

    String getTimeString();

    unsigned char hour12(bool *pm);
    unsigned char minutes();
    unsigned char seconds();

};

#endif