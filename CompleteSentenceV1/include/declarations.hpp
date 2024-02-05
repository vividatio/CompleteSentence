#ifndef DECLARATIONS_HPP
#define DECLARATIONS_HPP

/* LED zeug */
#define LED_PIN     3 /*RX */
#define NUM_LEDS    153 /* 17*9 */
constexpr unsigned char BRIGHTNESS = 50;
constexpr unsigned int MAXIMAL_MILLIAMPERE = 2000;
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
#define ERR_NO_ERROR            0
#define ERR_STD_ERROR           -1

#define ERR_EEPROM_WRITE_COMMIT -100
#define ERR_EEPROM_SSID_LENGTH -110
#define ERR_EEPROM_PWD_LENGTH -111
#define ERR_EEPROM_MISC_LENGTH -112

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
  time() {
    Hours = 0;
    Minutes = 0;
    Seconds = 0;
    pm = false;
  }
} timeT, *timeP;

/* Konstanten */
constexpr unsigned long cRefreshTimeInterval = 50 /*Sek*/ * 1000 /*ms*/;
constexpr unsigned long cRefreshLEDArrayinterval =  1/* Sek*/ * 1000 /*ms*/;
constexpr unsigned long cRefreshLEDinterval =  0 /* Sek*/ + 500 /*ms*/;

#endif