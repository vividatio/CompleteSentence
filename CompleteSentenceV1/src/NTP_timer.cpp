#include "NTP_timer.hpp"



CNTPtimer::CNTPtimer(){
    m_utcOffsetInSeconds = 3600;  // Sekunden
    m_TimeUpdateInterval = 600000; // Millisekunden

    m_timeNTPClient = NULL;
}

CNTPtimer::~CNTPtimer(){
    if (m_timeNTPClient != NULL) {
      delete m_timeNTPClient;
    }

}

int CNTPtimer::init() {
    m_error_code = ERR_NO_ERROR;    
    
    m_utcOffsetInSeconds = 3600;
    m_timeNTPClient = new NTPClient(m_ntpUDP, "pool.ntp.org", m_utcOffsetInSeconds, m_TimeUpdateInterval);
    
    /* NTP Client Setup */
    m_timeNTPClient->begin();

    return m_error_code;
}


bool CNTPtimer::check() {
  
return m_timeNTPClient->isTimeSet();

}

int CNTPtimer::update_via_NTP() {
    m_error_code = ERR_NO_ERROR;

    m_timeNTPClient->update(); // das wird intern nur danch dem Timeinterval Ausgefuehrt (constructor)
    
    return m_error_code;
}

String CNTPtimer::getTimeString() {

    m_error_code = ERR_NO_ERROR;

    return m_timeNTPClient->getFormattedTime();
}

unsigned char CNTPtimer::hour12(bool *pm) {
  m_error_code = ERR_NO_ERROR;
  
  unsigned char NTPHours = (unsigned char)m_timeNTPClient->getHours();
  
  *pm = (12 < NTPHours); 
  unsigned char result = *pm ? (NTPHours - 12) : NTPHours;

  if (result == 0) result = 12; // meine Uhr kann keine NULL 0 Uhr ist dann 12 pm
  
  return result;
}

unsigned char CNTPtimer::minutes(){
  m_error_code = ERR_NO_ERROR;
  return (unsigned char)m_timeNTPClient->getMinutes();
}

unsigned char CNTPtimer::seconds(){
  m_error_code = ERR_NO_ERROR;
  return (unsigned char)m_timeNTPClient->getSeconds();
}