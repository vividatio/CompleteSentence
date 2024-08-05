#include "NTP_timer.hpp"

CNTPtimer::CNTPtimer(){
  m_now = 0;

}

CNTPtimer::~CNTPtimer(){

}

#define MY_NTP_SERVER "europe.pool.ntp.org"           
#define MY_TZ "CET-1CEST,M3.5.0/02,M10.5.0/03"   

int CNTPtimer::init() {
  m_error_code = ERR_NO_ERROR;

  /* damit die Zeit korrekt konvertiert werden kann, auch der NTP-Server wird festgelegt */
  configTime(MY_TZ, MY_NTP_SERVER);

  return m_error_code;
}


int CNTPtimer::restart() {
    m_error_code = ERR_NO_ERROR;

    // neu erstellen
    init();

    return m_error_code;
}

bool CNTPtimer::check() {
  m_error_code = ERR_NO_ERROR;

  return (m_now > 0);
}

int CNTPtimer::update_via_NTP() {
  m_error_code = ERR_NO_ERROR;

  if (false == check()) {
    restart();
  }
  
  /* use time.h to convert time_epoch to local incl. dst*/
  time(&m_now);
  localtime_r(&m_now, &m_tm);

  return m_error_code;
}

String CNTPtimer::getTimeString() {
  m_error_code = ERR_NO_ERROR;

  char timestr[127];
  sprintf(timestr, "%02d.%02d.%4d   %02d:%02d:%02d dst=%d", m_tm.tm_mday, m_tm.tm_mon+1, m_tm.tm_year+1900,m_tm.tm_hour, m_tm.tm_min, m_tm.tm_sec, m_tm.tm_isdst);

  return String(timestr);
}


/* 
    gibt die Stunde von 0 bis 12 aus. 
    Update: nun auch in Verbindung mit der Sommerzeit. 
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