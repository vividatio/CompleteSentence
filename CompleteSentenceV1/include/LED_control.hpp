#ifndef LED_CONTROL_HPP
#define LED_CONTROL_HPP
/* der Define ist ein Modifizierer fuer die FastLED Lib. Weil kein SPI vorhanden */

#define FASTLED_ESP8266_RAW_PIN_ORDER
//#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
//#define FASTLED_ESP8266
//#define FASTLED_ESP8266_D1_PIN_ORDER
#define FASTLED_INTERNAL
//#define FASTLED_ALLOW_INTERRUPT 0
//#define FASTLED_INTERRUPT_RETRY_COUNT 2
//#define FASTLED_ESP8266_DMA
//#define FASTLED_ALL_PINS_HARDWARE_SPI
#include <FastLED.h>    /* das wird fuer die LED"s gebraucht */

#include "declarations.hpp"

/* Constants and glob. Variables */
/* FastLED stuff */


class CLEDControl {

private:
    int     m_error_code;
    CRGB    leds[NUM_LEDS];
    CRGB    framebuffer[NUM_LEDS];
    int     m_mappingTable[NUM_LEDS];

    CRGB    m_onColor;
    bool    m_SecondsLED;


public:
    CLEDControl();
    ~CLEDControl();

    int init();

    int show();

    /* wenn die LEDs im Streifen im ZigZag verlaufen startet jede Zeile mit der LED die der letzten der vorherigen am 
    nahesten ist. Dann addressieren sich jede zweite Zeil umgekehrt zur vorherigen das erledigt dann diese Funktion */
    int map_array_to_screen(bool zz_on, bool invert_color);

    int generate_mapping_table(bool zz_on);

    int clear();

    /* fuellt den led-Buffer */
    int fill_LED_Buffer();

    int set_LEDs_range(uint16_t pos, uint16_t width);
    int set_LEDs_range_direct(uint16_t pos, uint16_t width);

    int setHour(unsigned char hour);
    
    int decodeTime(timeP actualTimePointer);
    
    int PrintSimulation();

    int toggleSecondsLED();

};

#endif