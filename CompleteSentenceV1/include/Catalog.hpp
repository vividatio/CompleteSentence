#ifndef CATALOG_HPP
#define CATALOG_HPP

/* hier stehen die Worte drin, die angezeigt werden. StartIndex der LED-Striefen und 
Laenge des Worte 

Das Pattern sieht so aus:

    EsOistEhalbpunktQ
    LdreiviertelBzehn
    fünfCnachvorIhalb
    zehnfünfneunsechs
    Fzwölfelfzweivier
    JsiebenVdreiachtH
    GeinsXvormittagsZ
    AQUnachmittagsMNW


nur die Worte:

    Es.ist.halbpunkt.
    .dreiviertel.zehn
    fünf.nachvor.halb
    zehnfünfneunsechs
    .zwölfelfzweivier
    .sieben.dreiacht.
    .eins.vormittags.
    ...nachmittags...

*/

/* alle INV, sind so definiert, dass sie von links nach Rechts auf der Matrix liegen wuerden. 
Da der LED-Streifen aber ZIGZAG macht, muss das Umgedreht werden (LED-Streifen laeuft von 
rechts nach links) */
/* die Defines sind Tupleangaben  (start, length) */
#define WORD_ES             (0, 2)
#define WORD_IST            (3, 3)
#define WORD_HALB_STUNDE    (7, 4)
#define WORD_PUNKT          (11, 5)

/* nun die definition mit Mapping (LED-Umkehr jede zweite Zeile) */
#define WORD_DREIVIERTEL (18, 11)
#define WORD_VIERTEL     (22, 7)
#define WORD_ZEHN_1      (30, 4)


#define WORD_FUENF_1        (34, 4)
#define WORD_NACH           (39, 4)
#define WORD_VOR            (43, 3)
#define WORD_HALB           (47, 4)

#
#define WORD_ZEHN_2      (51, 4)
#define WORD_FUENF_2     (55, 4)
#define WORD_NEUN        (59, 4)
#define WORD_SECHS       (63, 5)


#define WORD_ZWOELF         (69, 5)
#define WORD_ELF            (74, 3)
#define WORD_ZWEI           (77, 4)
#define WORD_VIER           (81, 4)

#
#define WORD_SIEBEN      (86, 6)
#define WORD_DREI        (93, 4)
#define WORD_ACHT        (97, 4)


#define WORD_EINS           (103, 4)
#define WORD_VORMITTAGS     (108, 10)


#define WORD_NACHMITTATGS (122, 11)



// die Punkte sind ab LED 8*17 + 1 = 137


#endif