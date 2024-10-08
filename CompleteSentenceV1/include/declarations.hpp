#ifndef DECLARATIONS_HPP
#define DECLARATIONS_HPP

/* LED zeug */
#define LED_PIN     3 /*RX */
#define NUM_LEDS    153 /* 17*9 */

// default color for onColor 
#define DEFAULT_ONCOLOR_RED   "28"
#define DEFAULT_ONCOLOR_GREEN "128"
#define DEFAULT_ONCOLOR_BLUE  "228"

//default brightness of LED-Stripe
#define DEFAULT_BRIGHTNESS    "128"

constexpr unsigned int MAXIMAL_MILLIAMPERE = 5000;
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
//#define COLOR_ORDER RGB
#define SECONDS_LED 152 /* Sekunden Indikator -> Sign of life*/
//#define SECONDS_LED 2 /* Sekunden Indikator -> Sign of life*/


/* graphical stuff */ 
#define LED_WIDTH     17
#define LED_HEIGHT    9

/* Mapping aktiv? */
#define _DEBUG

/* Error Codes */
#define ERR_NO_ERROR              0
#define ERR_STD_ERROR             -1

#define ERR_LITTLE_FS_INIT_FAILED             -100
#define ERR_LITTLE_FS_FILE_NOT_FOUND          -101
#define ERR_LITTLEFS_WIFI_DELIMITER_NOT_FOUND -110

/* Types */
typedef enum tState {
  st_idle = 0,
  st_init,
  st_error,
  st_loop,
  st_setup
} StateT, *StateP;

typedef struct time {
  unsigned char Hours;
  unsigned char Minutes;
  unsigned char Seconds;
  bool pm;
  bool dst;
  
  time() {
    Hours = 0;
    Minutes = 0;
    Seconds = 0;
    pm = false;
    dst = false;
  }
} timeT, *timeP;

/* Konstanten */
constexpr unsigned long cRefreshTimeInterval = 50 /*Sek*/ * 1000 /*ms*/;
constexpr unsigned long cRefreshLEDArrayinterval =  1/* Sek*/ * 1000 /*ms*/;
constexpr unsigned long cRefreshLEDinterval =  0 /* Sek*/ + 500 /*ms*/;

/* *************************************** MACROS ************************************** */
#define RESTART_ALL()  Serial.println("Restarting Wordclock..."); delay(500); ESP.restart()



#endif