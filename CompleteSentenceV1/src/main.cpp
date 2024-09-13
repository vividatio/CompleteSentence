/* ****************************************************************************************************************** */
/* WordClock "CompleteSentence" Michal Knorr 2023 Oct, Nov.                                                           */
/* Based on NodeMCU ESP8266 V3 CP2102                                                                                 */
/* ****************************************************************************************************************** */
#include <Arduino.h>

/* ****************************************************************************************************************** */
/* ************************************************ FastLED Einstellungen ******************************************* */
/* ****************************************************************************************************************** */
#include "LED_control.hpp"

/* ****************************************************************************************************************** */
/* ************************************************** WLAN Einstellungen ******************************************** */
/* ****************************************************************************************************************** */
#include "WiFi_misc.hpp"

/* ****************************************************************************************************************** */
/* *************************************************** NTP Einstellungen ******************************************** */
/* ****************************************************************************************************************** */
#include "NTP_timer.hpp"

/* ****************************************************************************************************************** */
/* ************************************************** LittleFS Verwendung ******************************************* */
/* ****************************************************************************************************************** */
#include "PersistentSave.hpp"

/* ****************************************************************************************************************** */
/* ************************************************** globale Variablen ********************************************* */
/* ****************************************************************************************************************** */

unsigned long   prevTimeMillis = 0;
unsigned long   prevLEDArrayMillis = 0;
unsigned long   prevLEDMillis = 0;
unsigned long   actualMillis = millis(); 

uint16_t        GPCnt = 0;

timeT           actualTime;

CWIFI       wifi;
CLEDControl LED;
CNTPtimer   NTP;

CPersistentSave LiFS;


/* ****************************************************************************************************************** */
/* *************************************************** state functions ********************************************** */
/* ****************************************************************************************************************** */


/* Globale Check-Variablen fuer den init-State */
bool displayTestFinished = false;

/* idle state function */
int fnc_idle() {
  //Serial.println("entry idle state");
  
  return ERR_NO_ERROR;
}

/**********************************************************************************************************/
/* ******************************************** init state function ************************************* */
/**********************************************************************************************************/

inline int fnc_init() {
  Serial.println("Doing precalculations... ");
  LED.generate_mapping_table(true);

  NTP.init();

  NTP.update_via_NTP();
  bool CheckNTP = NTP.check();
  if (CheckNTP) {
    Serial.println("Time server connected ... ");
  } else {
    Serial.println("ERROR: Time server NOT connected ... ");
  }
  
  /* interne Uhrzeit-Variablen initialisieren */
  actualTime.Hours = NTP.hour12(&actualTime.pm);
  actualTime.Minutes = NTP.minutes();
  actualTime.Seconds = NTP.seconds();

  // update timers
  actualMillis = millis();
  prevTimeMillis = actualMillis;
  prevLEDArrayMillis = actualMillis;
  prevLEDMillis = actualMillis;

  return ERR_NO_ERROR;
}

/**********************************************************************************************************/
/* ******************************************** Displaytest function ************************************* */
/**********************************************************************************************************/

inline int fnc_Displaytest() {
  LED.clear();
  for (int i = 0; i < NUM_LEDS; i++) {
    LED.set_LEDs_range_direct(i , 1, CRGB::Beige);
    LED.show();
    delay(25);
  }
  return ERR_NO_ERROR;
}

unsigned char oldMinutes = 61;

/**********************************************************************************************************/
/* ******************************************** loop state function ************************************* */
/**********************************************************************************************************/
inline int fnc_loop() {

  /* Interval neu beginnen */
  actualMillis = millis();

  /* ----------------------------------------- Zeit-Update ---------------------------------------------- */
  if ((actualMillis - prevTimeMillis) >= cRefreshTimeInterval ) {
    prevTimeMillis = actualMillis;
  
    /* Refreshtime ausloesen */
    NTP.update_via_NTP();

    Serial.println( NTP.getTimeString() );


#ifdef DEBUG
    actualTime.Hours = NTP.hour12(&actualTime.pm);
    actualTime.Minutes = NTP.minutes();
    actualTime.Seconds = NTP.seconds();
 
    /* Ausgabe */
    Serial.printf("Aktuelle Uhrzeit: %02d:%02d:%02d (%s)\n", actualTime.Hours, actualTime.Minutes, actualTime.Seconds, (actualTime.pm) ? "nachmittag" :"vormittag");
#endif
    
  }

  /* --------------------------------------- Uhrzeit auswerten ----------------------------------------- */    
  if ((actualMillis - prevLEDArrayMillis) >= cRefreshLEDArrayinterval ) {
    prevLEDArrayMillis = actualMillis;

    actualTime.Hours = NTP.hour12(&actualTime.pm);
    actualTime.Minutes = NTP.minutes();
    actualTime.Seconds = NTP.seconds();
    
    /* Die Anzeige aendert sich weil sich die Minuten geaendert haben */
    if (actualTime.Minutes != oldMinutes) {
      oldMinutes = actualTime.Minutes;

      LED.decodeTime(&actualTime);

#ifdef DEBUG
      LED.PrintSimulation();
#endif
    }


    /* TEST: Farbe aus dem Webinterface hoeln */
//  ToDo:    LED.set_onColor( wifi.get_SliederRed(), wifi.get_SliederGreen(), wifi.get_SliederBlue() );


    /* LEDArray fuellen mit der MappingTable */
    LED.fill_LED_Buffer();

    /* Sekunden-Indikator aktualisieren */
    LED.toggleSecondsLED(); 
   
  }


  /* ----------------------------------------- LEDs refresh ------------------------------------------ */
  if ((actualMillis - prevLEDMillis) >= cRefreshLEDArrayinterval ) {
    prevLEDMillis = actualMillis;
  
    LED.show();
  }

  return ERR_NO_ERROR;
}


/* ****************************************************************************************************************** */
/* *************************************************** globales Setup *********************************************** */
/* ****************************************************************************************************************** */

void setup() {
  /* Misc. init */
  delay( 200 ); // power-up safety delay

  Serial.begin(115200);

  /* hier stehem Init-daten drin. und die Webdaten fuer die Web-Config */
  LiFS.init();

  String SSID = "Oh Happy Day"; /* "i come from a LAN down under" */
  String PASSWORD =  "62113249";/* "56710588139461966274" */

  /* Read Data from LittleFS */  
  int returnValue = LiFS.get_WiFi_Data(SSID, PASSWORD);

  if (returnValue != ERR_NO_ERROR) {
    /* not found? try this one */
    // SSID = "Oh Happy Day"; /* "i come from a LAN down under" */
    // PASSWORD =  "62113249";/* "56710588139461966274" */
    
    // just to force locale AccessPoint
    SSID = "Reiner"; /* "i come from a LAN down under" */
    PASSWORD =  "Unsinn";/* "56710588139461966274" */

  }

  /* WIFI init */
   wifi.init(SSID.c_str(),PASSWORD.c_str(), &LiFS);
  


  /* --------------------- Here should wifi running ---------------------- */

  /* init FastLed */
  LED.init();

  /* NTP_timer init */
  NTP.init();
 
  /* Initialisierungen */

  fnc_idle();

  fnc_init();

}


/* ****************************************************************************************************************** */
/* *************************************************** globaler Loop ************************************************ */
/* ****************************************************************************************************************** */
void loop() {
  
  if (displayTestFinished) {
    /* Normaler Loop*/
    fnc_loop();




  } else {
    /* Displaytest - hier, weil das delaz im Setup nicht funktioniert */    
    fnc_Displaytest();
    displayTestFinished = true;
    delay(2000);
  } 

  delay(5); // damit das Ding nicht durchdreht
}