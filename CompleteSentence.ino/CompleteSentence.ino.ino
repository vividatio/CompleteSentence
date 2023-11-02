/* ****************************************************************************************************************** */
/* WordClock "CompleteSentence" Michal Knorr 2023 Oct, Nov.                                                           */
/* Based on NodeMCU ESP8266 V3 CP2102                                                                                 */
/* ****************************************************************************************************************** */




/* ****************************************************************************************************************** */
/* ************************************************** WLAN Einstellungen ******************************************** */
/* ****************************************************************************************************************** */
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
const char *ssid     = "i come from a LAN down under";
const char *password = "56710588139461966274";


/* ****************************************************************************************************************** */
/* *************************************************** NTP Einstellungen ******************************************** */
/* ****************************************************************************************************************** */
#include <NTPClient.h>

/* Zeitkram */
const long utcOffsetInSeconds = 3600;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);



/* ****************************************************************************************************************** */
/* ************************************************ FastLED Einstellungen ******************************************* */
/* ****************************************************************************************************************** */

/* der Define ist ein Modifizierer fuer die FastLED Lib. Weil kein SPI vorhanden */
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include <FastLED.h>    /* das wird fuer die LED"s gebraucht */

/* Constants and glob. Variables */
/* FastLED stuff */
#define LED_PIN     6
#define NUM_LEDS    90
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];            /* Arbeits Array */
CRGB leds_set[NUM_LEDS];        /* Ausgabe Arraz */

#define UPDATES_PER_SECOND 20

/* graphical stuff */ 

#define LED_WIDTH     18
#define LED_HEIGHT    8 


/* ****************************************************************************************************************** */
/* ****************************************************** Funktionen ************************************************ */
/* ****************************************************************************************************************** */

/* wenn die LEDs im Streifen im ZigZag verlaufen startet jede Zeile mit der LED die der letzten der vorherigen am 
nahesten ist. Dann addressieren sich jede zweite Zeil umgekehrt zur vorherigen das erledigt dann diese Funktion */
int map_array_to_screen(bool zz_on, bool invert_color) {

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

    addr = col + (row * LED_WIDTH);
    if(invert_color) {
      leds_set[count] = -leds[addr];
    } else {
      leds_set[count] = leds[addr];
    }
    
    col++;
    if (col = LED_WIDTH) {
      col = 0;
      row++;
    }
    count ++;
  } while (count < NUM_LEDS);

  return 0;
}


int fill_LED_Buffer(int counter) {

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].r = (i + counter) & 255;
    leds[i].g = (i - counter) & 255;
    leds[i].b = (i + (2 * counter)) & 255;
  }
  return 0;
}



static int cnt = 0;



/* ****************************************************************************************************************** */
/* *************************************************** globales Setup *********************************************** */
/* ****************************************************************************************************************** */

void setup() {
  /* init FastLed */
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds_set, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  /* WIFI Setup */
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("connecting WIFI");
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 1000 );
    Serial.print ( "." );
  }

  /* NTP Client Setup */
  timeClient.begin();


}



/* ****************************************************************************************************************** */
/* *************************************************** globaler Loop ************************************************ */
/* ****************************************************************************************************************** */

typedef enum tState {
  idle = 0,
  init,
  error,
  loop,
  setup
}



void loop() {


/* LEDs aktualisieren */
  // put your main code here, to run repeatedly:
    cnt ++;
    fill_LED_Buffer(cnt);

    map_array_to_screen(false, false);
  /* LED Muster anzeigen */
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);




/* Uhrzeit abfragen */
  if ((cnt % 200) == 0) {  // alle 10 Sekunden Zeit lesen
    timeClient.update();

    Serial.print(timeClient.getDay());
    Serial.print("\r");
    Serial.print(timeClient.getHours());
    Serial.print("\r");
    Serial.print(timeClient.getMinutes());
    Serial.print("\r");
    Serial.print(timeClient.getSeconds));
    Serial.print("\r");




  }

}
