#ifndef NTP_TIMER_HPP
#define  NTP_TIMER_HPP
#include "declarations.hpp"

#include "WiFI_misc.hpp"

#include <time.h>


//#define MY_NTP_SERVER "at.pool.ntp.org"
#define MY_NTP_SERVER "ptbtime1.ptb.de"
#define MY_TZ "CET-1CEST,M3.5.0/02,M10.5.0/03"   

class CNTPtimer {
private:
    int             m_error_code;
    time_t          m_now;
    tm              m_tm;

/* maybe obsolete */    unsigned long   m_TimeUpdateInterval;
  
    WiFiUDP         m_ntpUDP;


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