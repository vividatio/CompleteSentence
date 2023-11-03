#ifndef DECLARATIONS_HPP
#define DECLARATIONS_HPP

/* LED zeug */
#define LED_PIN     6
#define NUM_LEDS    144 /* 18*8 */
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

/* graphical stuff */ 
#define LED_WIDTH     18
#define LED_HEIGHT    8 S



/* Error Codes */
#define ERR_NO_ERROR    0
#define ERR_STD_ERROR   -1

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
const unsigned long cRefreshTimeInterval = 3 /*Sek*/ * 1000 /*ms*/;
const unsigned long cRefreshLEDinterval =  /*0 Sek*/ 200 /*ms    Update mit 10 Hz*/;

#endif