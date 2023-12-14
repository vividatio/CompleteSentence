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
/* ************************************************** globale Variablen ********************************************* */
/* ****************************************************************************************************************** */

// static int cnt = 0;

StateT          activeState = st_idle;
unsigned long   prevTimeMillis = 0;
unsigned long   prevLEDArrayMillis = 0;
unsigned long   prevLEDMillis = 0;
unsigned long   actualMillis = millis(); 

uint16_t        GPCnt = 0;

timeT           actualTime;

CWIFI       wifi;
CLEDControl LED;
CNTPtimer   NTP;



/* ****************************************************************************************************************** */
/* *************************************************** state functions ********************************************** */
/* ****************************************************************************************************************** */



/* Globale Check-Variablen fuer den init-State */
bool g_precalcMapping = false;
bool g_firstTimeSetup = false;
bool displayTestFinished = false;


/* idle state function */
int fnc_idle() {
  //Serial.println("entry idle state");
  
  // init-state-check-variablen resetten
  g_precalcMapping = false;
  g_firstTimeSetup = false;


  // next state
  activeState = st_init;

  return ERR_NO_ERROR;
}






/* init state function */
inline int fnc_init() {
  //Serial.println("entry init state");

  if (!g_precalcMapping) {
    Serial.println("Doing precalculations... ");
    LED.generate_mapping_table(true);
    g_precalcMapping = true;
  }

  if (!g_firstTimeSetup) {
    NTP.update_via_NTP();
    g_firstTimeSetup = NTP.check();
  }

  // update timers
  actualMillis = millis();
  prevTimeMillis = actualMillis;
  prevLEDArrayMillis = actualMillis;
  prevLEDMillis = actualMillis;

/* State erfuellt?*/
  if (g_precalcMapping && g_firstTimeSetup) {
    activeState = st_loop;
    Serial.println("Init state finished. Switching to loop");
    return ERR_NO_ERROR;
  }

  activeState = st_init;

  return ERR_NO_ERROR;
}


unsigned char oldMinutes = 61;

/* loop state function */
inline int fnc_loop() {

  /* Interval neu beginnen */
  actualMillis = millis();
  
  #ifdef DEBUG
  Serial.printf("%ld\n", actualMillis - prevTimeMillis);
  #endif

  /* Zeiterfassung */
  if ((actualMillis - prevTimeMillis) >= cRefreshTimeInterval ) {
    prevTimeMillis = actualMillis;

    /* ----------------------------------------- Timer Kram ---------------------------------------------- */
    /* Refreshtime ausloesen */
    NTP.update_via_NTP();


    actualTime.Hours = NTP.hour12(&actualTime.pm);
    actualTime.Minutes = NTP.minutes();
    actualTime.Seconds = NTP.seconds();
    
    /* Ausgabe */
    Serial.printf("Aktuelle Uhrzeit: %02d:%02d:%02d (%s)\n", actualTime.Hours, actualTime.Minutes, actualTime.Seconds, (actualTime.pm) ? "nachmittag" :"vormittag");
    /* Zeit updaten - Pauschal, denn NTP update macht erst ein update, wenn UpdateTime erreicht ist */

#ifdef DEBUG
    Serial.printf("%ld\n", actualMillis - prevLEDMillis);
#endif
  }


  /* LED - Array refresh */
  if ((actualMillis - prevLEDArrayMillis) >= cRefreshLEDArrayinterval ) {
    prevLEDArrayMillis = actualMillis;

    actualTime.Hours = NTP.hour12(&actualTime.pm);
    actualTime.Minutes = NTP.minutes();
    actualTime.Seconds = NTP.seconds();

    
    /* --------------------------------------- Uhrzeit auswerten ----------------------------------------- */    
    /* Die Anzeige aendert sich */
    if (actualTime.Minutes != oldMinutes) {
      oldMinutes = actualTime.Minutes;

      LED.decodeTime(&actualTime);

#ifdef DEBUG
      LED.PrintSimulation();
#endif
/* ----------------------------------------- LEDArray fuellen ------------------------------------------ */
#ifdef MAPPING
      LED.fill_LED_Buffer();
#endif
    }


    /* Sekunden-Indikator aktualisieren */
    LED.toggleSecondsLED();
  }


  /* LED Refresh */
  if ((actualMillis - prevLEDMillis) >= cRefreshLEDArrayinterval ) {
    prevLEDMillis = actualMillis;
    /* ----------------------------------------- LEDs ansteuern ------------------------------------------ */

    LED.show();
  }


  activeState = st_loop;

  return ERR_NO_ERROR;
}


/* ****************************************************************************************************************** */
/* *************************************************** globales Setup *********************************************** */
/* ****************************************************************************************************************** */

void setup() {
  /* Misc. init */
  delay( 200 ); // power-up safety delay

  Serial.begin(115200);


  /* WIFI init */
  wifi.init("i come from a LAN down under", "56710588139461966274");

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

  /* Displaytest */
  if (!displayTestFinished) {
    LED.clear();
    for (int i = 0; i < NUM_LEDS; i++) {
      LED.set_LEDs_range_direct(i , 1, CRGB::Beige);
      LED.show();
      delay(50);
    }
    displayTestFinished = true;
    delay(1000);

  } else {
/* Normaler Loop*/
    fnc_loop();

    delay(5); // damit das Ding nicht durchdreht
  }


}