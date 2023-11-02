#ifndef LED_CONTROL_HPP
#define LED_CONTROL_HPP
/* der Define ist ein Modifizierer fuer die FastLED Lib. Weil kein SPI vorhanden */
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include <FastLED.h>    /* das wird fuer die LED"s gebraucht */

#include "declarations.hpp"

/* Constants and glob. Variables */
/* FastLED stuff */


class CLEDControl {

private:
    int         m_error_code;

    CRGB        leds[NUM_LEDS];         // das geht direkt nach draussen an die NEO-Pixels
    CRGB        framebuffer[NUM_LEDS];   // hier wird vorgerendert, muss noch irgend wie nach leds kopiert werden

public:
    CLEDControl();
    ~CLEDControl();

    int init();

    int show();

    /* wenn die LEDs im Streifen im ZigZag verlaufen startet jede Zeile mit der LED die der letzten der vorherigen am 
    nahesten ist. Dann addressieren sich jede zweite Zeil umgekehrt zur vorherigen das erledigt dann diese Funktion */
    int map_array_to_screen(bool zz_on, bool invert_color);


    /* fuellt den led-Buffer */
    int fill_LED_Buffer(int counter);

    int decodeTime(timeP actualTimePointer);
};

#endif