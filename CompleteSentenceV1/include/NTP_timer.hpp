#ifndef NTP_TIMER_HPP
#define  NTP_TIMER_HPP
#include "declarations.hpp"

#include "WiFI_misc.hpp"

#include <NTPClient.h>


class CNTPtimer {
private:
    int             m_error_code;

    long            m_utcOffsetInSeconds;
    unsigned long   m_TimeUpdateInterval;
  
    NTPClient       *m_timeNTPClient;
    WiFiUDP         m_ntpUDP;


public:
    
    CNTPtimer();

    ~CNTPtimer();

    int init();

    int update_via_NTP();

    bool check();

    String getTimeString();

    unsigned char hour12(bool *pm);
    unsigned char minutes();
    unsigned char seconds();

};

#endif