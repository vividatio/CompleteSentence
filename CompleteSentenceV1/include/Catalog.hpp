#ifndef CATALOG_HPP
#define CATALOG_HPP

/* hier stehen die Worte drin, die angezeigt werden. StartIndex der LED-Striefen und 
Laenge des Worte 


Patttern Traumfrau {

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

}


Pattern Farell {

ZESSISTZFÜNF
OPUNKTOZEHNW
ADREIVIERTEL
NACHVORWHALB
DREIZEHNVIER
ELFEINSZWEIU
ZWÖLFÜNFNEUN
MITTERNACHTA
SIEBENJSECHS

nur Worte:

00      .ES.IST.FÜNF    11
12      .PUNKT.ZEHN.    23
24      .DREIVIERTEL    35
36      NACHVOR.HALB    47
48      DREIZEHNVIER    59
60      ELFEINSZWEI.    71
72      ZWÖLFÜNFNEUN    83
84      MITTERNACHT.    95
96      SIEBEN.SECHS    107
108     ************    119

}



*/

/* Pattern Farell : */

/* die Defines sind Tupleangaben  (start, length) */
#define WORD_ES             (1, 2)
#define WORD_IST            (4, 3)
#define WORD_FUENF_1        (8, 4)

#define WORD_PUNKT          (13, 5)
#define WORD_ZEHN_1         (19, 4)

#define WORD_DREIVIERTEL    (25, 11)
#define WORD_VIERTEL        (29, 7)

#define WORD_NACH           (36, 4)
#define WORD_VOR            (40, 3)
#define WORD_HALB           (44, 4)

#define WORD_DREI           (48, 4)
#define WORD_ZEHN_2         (52, 4)
#define WORD_VIER           (56, 4)

#define WORD_ELF            (60, 3)
#define WORD_EINS           (63, 4)
#define WORD_ZWEI           (70, 4)

#define WORD_ZWOELF         (72, 5)
#define WORD_FUENF_2        (76, 4)
#define WORD_NEUN           (80, 4)

#define WORD_MITTERNACHT    (84, 11)
#define WORD_ACHT           (91, 4)

#define WORD_SIEBEN         (96, 6)
#define WORD_SECHS          (103, 5)


// Obsolete
#define WORD_VORMITTAGS     (108, 10)
#define WORD_NACHMITTATGS (122, 11)


#endif