#include "LED_control.hpp"


CLEDControl::CLEDControl() {
  m_error_code = ERR_NO_ERROR;
}

CLEDControl::~CLEDControl(){
  m_error_code = ERR_NO_ERROR;
}

int CLEDControl::init() {
  m_error_code = ERR_NO_ERROR;

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  return m_error_code;
}

 
int CLEDControl::show() {

   FastLED.show();

   return ERR_NO_ERROR;
}




int CLEDControl::map_array_to_screen(bool zz_on, bool invert_color) {

  int addr = 0;
  int row = 0;
  int col = 0;
  int offset = 0;
  int count = 0;

  do{
    /* jede aeite zeile wird rueckwaerts gezaehlt wenn zigzag aktiv ist */
    if ( ((count % 2) == 1) && (zz_on) ) {
      offset = LED_WIDTH - col -1; 
    } else {
      offset = col;
    }

    addr = offset + (row * LED_WIDTH);
    if(invert_color) {
      leds[count] = -framebuffer[addr];
    } else {
      leds[count] = framebuffer[addr];
    }
    
    col++;
    if (col == LED_WIDTH) {
      col = 0;
      row++;
    }
    count ++;
  } while (count < NUM_LEDS);

  return 0;
}


int CLEDControl::fill_LED_Buffer(int counter) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].r = (i + counter) & 255;
    leds[i].g = (i - counter) & 255;
    leds[i].b = (i + (2 * counter)) & 255;
  }
  return 0;
}


/* dekodiert die aktuelle Uhrzeit in den Framebuffer */
int CLEDControl::decodeTime(timeP actualTimePointer) {
  m_error_code = ERR_NO_ERROR;



  return m_error_code;
}

