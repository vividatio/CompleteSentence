#include "NTP_timer.hpp"



CNTPtimer::CNTPtimer(){
    m_TimeUpdateInterval = 600000; // Millisekunden
}

CNTPtimer::~CNTPtimer(){

}

int CNTPtimer::init() {
  m_error_code = ERR_NO_ERROR;

  configTime(MY_TZ, MY_NTP_SERVER); // init the esp timer lib

  return m_error_code;
}



int CNTPtimer::restart() {
    m_error_code = ERR_NO_ERROR;

    // erst alles schliessen


    // neu erstellen
    init();

    return m_error_code;
}




bool CNTPtimer::check() {
  
return true;
}

int CNTPtimer::update_via_NTP() {
  m_error_code = ERR_NO_ERROR;

  if (false == check()) {
    restart();
  }
 
  time(&m_now);                       // read the current time
  localtime_r(&m_now, &m_tm);         // update the structure tm with the current time
    
  return m_error_code;
}

String CNTPtimer::getTimeString() {

  m_error_code = ERR_NO_ERROR;

  char   TimeCharStr[128];

  sprintf(TimeCharStr, "%4d.%2d.%2d_%2d:%2d:%2d (dst: %1d)", m_tm.tm_year+1900, \
    m_tm.tm_mon + 1, \
    m_tm.tm_mday, \
    m_tm.tm_hour, \
    m_tm.tm_min, \
    m_tm.tm_sec, \
    m_tm.tm_isdst);

  String TimeString = String(TimeCharStr);
  return TimeString; 
    
}

/* 
    gibt die Stunde von 0 bis 12 aus. 
*/
unsigned char CNTPtimer::hour12(bool *pm) {
  m_error_code = ERR_NO_ERROR;
  
  unsigned char NTPHours = (unsigned char)m_tm.tm_hour;

/* Info: 
       0:00 bis 11:59 -> am 
      12:00 bis 23:59 -> pm
*/

  *pm = (12 <= NTPHours); 
  unsigned char result = *pm ? (NTPHours - 12) : NTPHours;

  if (result == 0) result = 12; // meine Uhr kann keine NULL 0 Uhr ist dann 12 pm
  
  return result;
}

unsigned char CNTPtimer::minutes(){
  m_error_code = ERR_NO_ERROR;
  return (unsigned char)m_tm.tm_min;
}

unsigned char CNTPtimer::seconds(){
  m_error_code = ERR_NO_ERROR;
  return (unsigned char)m_tm.tm_sec;
}