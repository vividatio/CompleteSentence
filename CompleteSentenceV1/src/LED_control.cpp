#include <Arduino.h>

#include "LED_control.hpp"

#include "Catalog.hpp"

CLEDControl::CLEDControl() {
  m_error_code = ERR_NO_ERROR;
}

CLEDControl::~CLEDControl(){
  m_error_code = ERR_NO_ERROR;
}

int CLEDControl::init() {
  m_error_code = ERR_NO_ERROR;

  //FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  FastLED.setMaxPowerInMilliWatts(2000);

  // m_onColor.r = 64;
  // m_onColor.g = 44;
  // m_onColor.b = 14;

  // m_onColor = CRGB::Fuchsia;
  m_onColor = CRGB::Orange;

  clear();

  return m_error_code;
}

 
int CLEDControl::show() {

  FastLED.show();
  FastLED.show(); // found in some forums

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
    if ( ((row % 2) == 1) && (zz_on) ) {
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

int CLEDControl::generate_mapping_table(bool zz_on) {
  m_error_code = ERR_NO_ERROR;

  int addr = 0;
  int row = 0;
  int col = 0;
  int offset = 0;
  int count = 0;

  do{
    /* jede aeite zeile wird rueckwaerts gezaehlt wenn zigzag aktiv ist */
    if ( ((row % 2) == 1) && (zz_on) ) {
      offset = LED_WIDTH - col -1; 
    } else {
      offset = col;
    }

    addr = offset + (row * LED_WIDTH);
    m_mappingTable[count] = addr;

//    Serial.println(addr); 

    col++;
    if (col == LED_WIDTH) {
      col = 0;
      row++;
    }
    count ++;
  } while (count < NUM_LEDS);


  return m_error_code;
}



int CLEDControl::clear() {
  m_error_code = ERR_NO_ERROR;
  
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
    framebuffer[i] = CRGB::Black;
  }

  return m_error_code;
}


int CLEDControl::fill_LED_Buffer() {
  m_error_code = ERR_NO_ERROR;
  int u = 0;
  for (uint16_t i = 0; i < NUM_LEDS; i++) {

#ifdef MAPPING    
    u = m_mappingTable[i];
#else
    u = i;
#endif

    leds[i] = framebuffer[u];
  }
  return m_error_code;
}

int CLEDControl::set_LEDs_range(uint16_t pos, uint16_t width) {
  m_error_code = ERR_NO_ERROR;
  
  for (uint16_t i = 0; i < width; i++) {
    //Framebuffer verwenden
    framebuffer[pos + i] = m_onColor;
  }

  return m_error_code;
}

int CLEDControl::set_LEDs_range_direct(uint16_t pos, uint16_t width) {
  m_error_code = ERR_NO_ERROR;
  
  for (uint16_t i = 0; i < width; i++) {
    leds[pos + i] = m_onColor;
  }

  return m_error_code;
}

int CLEDControl::set_LEDs_range_direct(uint16_t pos, uint16_t width, CRGB color) {
  m_error_code = ERR_NO_ERROR;
  
  for (uint16_t i = 0; i < width; i++) {
    leds[pos + i] = color;
  }

  return m_error_code;
}

int CLEDControl::setHour(unsigned char hour) {
  m_error_code = ERR_NO_ERROR;

  switch (hour) {
    case 1 : 
      set_LEDs_range WORD_EINS;
      break;
    case 2 : 
      set_LEDs_range WORD_ZWEI;
      break;
    case 3 : 
      set_LEDs_range WORD_DREI;
      break;
    case 4 : 
      set_LEDs_range WORD_VIER;
      break;
    case 5 : 
      set_LEDs_range WORD_FUENF_2;
      break;
    case 6 : 
      set_LEDs_range WORD_SECHS;
      break;
    case 7 : 
      set_LEDs_range WORD_SIEBEN;
      break;
    case 8 : 
      set_LEDs_range WORD_ACHT;
      break;
    case 9 : 
      set_LEDs_range WORD_NEUN;
      break;
    case 10 : 
      set_LEDs_range WORD_ZEHN_2;
      break;
    case 11 : 
      set_LEDs_range WORD_ELF;
      break;
    case 12 : 
      set_LEDs_range WORD_ZWOELF;
      break;
    default:
    // z.b. NULL
      set_LEDs_range WORD_ZWOELF;
      break;
  }

  return m_error_code;
}



/* dekodiert die aktuelle Uhrzeit in den Framebuffer */
/* die einzelnen Worte haben einen Start und einen Endindex */
int CLEDControl::decodeTime(timeP actualTimePointer) {
  m_error_code = ERR_NO_ERROR;
  
  /* das sind die Zeit-Komponenten */
  unsigned char l_h = actualTimePointer->Hours;
  unsigned char l_m = actualTimePointer->Minutes;
  bool l_pm = actualTimePointer->pm;

  unsigned char minute_dots = (l_m % 5); // die vier "Fuell"-Minuten

  /* wann leuchten die Worte auf */
  clear();


  /* Es ist: */
  set_LEDs_range WORD_ES;
  set_LEDs_range WORD_IST;

  /* die Punkt Stunde Ansage */
  if (l_m < 5) {
    set_LEDs_range WORD_PUNKT;
    setHour((unsigned char) l_h);
  }
  
  /* Vor und Nachmittag */
  m_error_code = (l_pm) ? set_LEDs_range WORD_NACHMITTATGS : set_LEDs_range WORD_VORMITTAGS;

  /* Minutenpunkte */
  if ((l_m % 5) > 0) set_LEDs_range(136, minute_dots);

/* nun die ganzen Ossi-Zeiten */
  // 5 nach 
  if ((l_m >= 5) && (l_m < 10)) {
    set_LEDs_range WORD_FUENF_1;
    set_LEDs_range WORD_NACH;
    setHour(l_h);
  }

  // 10 nach
  if ((l_m >= 10) && (l_m < 15)) {
    set_LEDs_range WORD_ZEHN_1;
    set_LEDs_range WORD_NACH;
    setHour(l_h);
  }

  if ((l_m >= 15) && (l_m < 20)) {
    set_LEDs_range WORD_VIERTEL;
    setHour((l_h + 1) % 12); // nachste Stunden 
  }

 if ((l_m >= 20) && (l_m < 25)) {
    set_LEDs_range WORD_ZEHN_1; 
    set_LEDs_range WORD_VOR; 
    set_LEDs_range WORD_HALB;
    setHour((l_h + 1) % 12); // nachste Stunden 
  }

 if ((l_m >= 25) && (l_m < 30)) {
    set_LEDs_range WORD_FUENF_1; 
    set_LEDs_range WORD_VOR; 
    set_LEDs_range WORD_HALB;
    setHour((l_h + 1) % 12); // nachste Stunden 
  }

  if ((l_m >= 30) && (l_m < 35)) {
    set_LEDs_range WORD_HALB_STUNDE;
    setHour((l_h + 1) % 12); // nachste Stunden 
  }

  if ((l_m >= 35) && (l_m < 40)) {
    set_LEDs_range WORD_FUENF_1; 
     set_LEDs_range WORD_NACH; 
    set_LEDs_range WORD_HALB;
    setHour((l_h + 1) % 12); // nachste Stunden 
  }

  if ((l_m >= 40) && (l_m < 45)) {
    set_LEDs_range WORD_ZEHN_1; 
    set_LEDs_range WORD_NACH; 
    set_LEDs_range WORD_HALB;
    setHour((l_h + 1) % 12); // nachste Stunden 
  }

  if ((l_m >= 45) && (l_m < 50)) {
    set_LEDs_range WORD_DREIVIERTEL;
    setHour((l_h + 1) % 12); // nachste Stunden 
  }

  if ((l_m >= 50) && (l_m < 55)) {
    set_LEDs_range WORD_ZEHN_1; 
    set_LEDs_range WORD_VOR; 
    setHour((l_h + 1) % 12); // nachste Stunden 
  }

  if ((l_m >= 55) && (l_m <= 59)) {
    set_LEDs_range WORD_FUENF_1; 
    set_LEDs_range WORD_VOR; 
    setHour((l_h + 1) % 12); // nachste Stunden 
  }

  return m_error_code;
}

char LEDS_SimArray[153] = "EsOistEhalbpunktQLdreiviertelBzehnfunfCnachvorIhalbzehnfunfneunsechsFzwolfelfzweivierJsiebenVdreiachtHGeinsXvormittagsZAQUnachmittagsMNW.........";
int CLEDControl::PrintSimulation() {
  m_error_code = ERR_NO_ERROR;
  
  Serial.println(LEDS_SimArray);

  // for (uint16_t i = 0; i < NUM_LEDS; i++) {
  //   Serial.print(LEDS_SimArray[i]);
  // }
  // Serial.printf("\n");
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    if (framebuffer[i].r > 0) {
      Serial.print(LEDS_SimArray[i]);
    } else {
      Serial.print(" ");
    }
  }
  Serial.printf("\n");

  return m_error_code;
}

int CLEDControl::toggleSecondsLED() {
  m_error_code = ERR_NO_ERROR;

  m_SecondsLED =!m_SecondsLED;

 if (m_SecondsLED) {
    leds[SECONDS_LED] = CRGB::Black;
    // Serial.println("aus");
  } else {
    leds[SECONDS_LED] = m_onColor;
    // Serial.println("an");
  }
  

  return m_error_code;
}



/* EFFEKTE */


