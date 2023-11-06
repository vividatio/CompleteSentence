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
unsigned long   prevLEDMillis = 0;
unsigned long   actualMillis = millis(); 

uint16_t        GPCnt = 0;

timeT           actualTime;

CWIFI       wifi;
CLEDControl LED;
CNTPtimer   NTP;

/* ****************************************************************************************************************** */
/* *************************************************** globales Setup *********************************************** */
/* ****************************************************************************************************************** */

void setup() {
  /* Misc. init */
  delay( 3000 ); // power-up safety delay
  Serial.begin(115200);


/* WIFI init */
  wifi.init("i come from a LAN down under", "56710588139461966274");

  /* init FastLed */
  LED.init();

/* NTP_timer init */
  NTP.init();
  
}


/* ****************************************************************************************************************** */
/* *************************************************** state functions ********************************************** */
/* ****************************************************************************************************************** */

/* idle state function */
int fnc_idle() {

  Serial.println("entry idle state");
  activeState = st_init;

  return ERR_NO_ERROR;
}

/* init state function */
int fnc_init() {

  Serial.println("entry init state");

  actualMillis = millis();
  prevTimeMillis = actualMillis;
  prevLEDMillis = actualMillis;


  NTP.update_via_NTP();

  if (NTP.check()) {
    activeState = st_loop;
    Serial.println("time set. Switching to loop");
    return ERR_NO_ERROR;
  }
  Serial.println("time not set yet. waiting for NTP-update..");
  activeState = st_init;

  return ERR_NO_ERROR;
}




/* loop state function */
int fnc_loop() {



  /* Interval neu beginnen */
  actualMillis = millis();
  
  //Serial.printf("%d\n", actualMillis - prevTimeMillis);

  /* Zeiterfassung */
  if ((actualMillis - prevTimeMillis) >= cRefreshTimeInterval ) {
    prevTimeMillis = actualMillis;

    /* ----------------------------------------- Timer Kram ---------------------------------------------- */
    /* Refreshtime ausloesen */
    actualTime.Hours = NTP.hour12(&actualTime.pm);
    actualTime.Minutes = NTP.minutes();
    actualTime.Seconds = NTP.seconds();
    
    /* Ausgabe */
    Serial.printf("Aktuelle Uhrzeit: %02d:%02d:%02d (%s)\n", actualTime.Hours, actualTime.Minutes, actualTime.Seconds, (actualTime.pm) ? "nachmittag" :"vormittag");
    /* Zeit updaten - Pauschal, denn NTP update macht erst ein update, wenn UpdateTime erreicht ist */
    NTP.update_via_NTP();

    /* --------------------------------------- Uhrzeit auswerten ----------------------------------------- */
    LED.decodeTime(&actualTime);
    LED.PrintSimulation();
  }

  /* LED refresh */
  if ((actualMillis - prevLEDMillis) >= cRefreshLEDinterval ) {
    prevLEDMillis = actualMillis;
/* ----------------------------------------- LEDs ansteuern ------------------------------------------ */
    //LED.fill_LED_Buffer(GPCnt);
    LED.show();

    GPCnt++;
    if (GPCnt > NUM_LEDS) GPCnt = 0;
   
  }

  activeState = st_loop;

  return ERR_NO_ERROR;
}




/* ****************************************************************************************************************** */
/* *************************************************** globaler Loop ************************************************ */
/* ****************************************************************************************************************** */
/* Stat driven */

void loop() {

  switch (activeState) {

    case st_idle:
      fnc_idle();
      break;
    case st_init:
      fnc_init();
      break;
    case st_loop:
      fnc_loop();
      break;
    default:
      break;
  }


  delay(50);

// /* LEDs aktualisieren */
//   // put your main code here, to run repeatedly:
//     cnt ++;

//     LED.fill_LED_Buffer(cnt);
//     LED.show();

//     delay(50);

// /* Uhrzeit abfragen */
//   if ((cnt % 200) == 0) {  // alle 10 Sekunden Zeit lesen
//     NTP.update_via_NTP();
//     Serial.println(NTP.getTimeString());

//   }

}