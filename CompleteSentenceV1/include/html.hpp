#ifndef HTML_HPP
#define HTML_HPP


#include <ESPAsyncTCP.h>

const char* PARAM_INPUT = "value";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Wordclock von Traumfrau</title>
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 2.3rem;}
    p {font-size: 1.9rem;}
    body {max-width: 400px; margin:0px auto; padding-bottom: 25px;}
    .sliderRed { -webkit-appearance: none; appearance: none; margin: 14px; width: 360px; height: 25px; background: #FFD65C; outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .sliderRed::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #FF0000; cursor: pointer;}
    .sliderRed::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; } 

    .sliderGreen { -webkit-appearance: none; appearance: none; margin: 14px; width: 360px; height: 25px; background: #FFD65C; outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .sliderGreen::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #00FF00; cursor: pointer;}
    .sliderGreen::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; } 

    .sliderBlue { -webkit-appearance: none; appearance: none; margin: 14px; width: 360px; height: 25px; background: #FFD65C; outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .sliderBlue::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #0000FF; cursor: pointer;}
    .sliderBlue::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; } 
  </style>
</head>
<body>
  <h2>Wordclock von Traumfrau</h2>
  <p><span id="textSliderValueRed">%SLIDERVALUE_RED%</span></p>
  <p><input type="range" onchange="updateSliderPWMRed(this)" id="pwmSliderRed" min="0" max="255" value="%SLIDERVALUE_RED%" step="1" class="sliderRed"></p>
  
  <p><span id="textSliderValueGreen">%SLIDERVALUE_GREEN%</span></p>
  <p><input type="range" onchange="updateSliderPWMGreen(this)" id="pwmSliderGreen" min="0" max="255" value="%SLIDERVALUE_GREEN%" step="1" class="sliderGreen"></p>

  <p><span id="textSliderValueBlue">%SLIDERVALUE_RED%</span></p>
  <p><input type="range" onchange="updateSliderPWMBlue(this)" id="pwmSliderBlue" min="0" max="255" value="%SLIDERVALUE_BLUE%" step="1" class="sliderBlue"></p>
<script>

function updateSliderPWMRed(element) {
  var sliderValue = document.getElementById("pwmSliderRed").value;
  document.getElementById("textSliderValueRed").innerHTML = sliderValue;
  console.log(sliderValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/sliderRed?value="+sliderValue, true);
  xhr.send();
}

function updateSliderPWMGreen(element) {
  var sliderValue = document.getElementById("pwmSliderGreen").value;
  document.getElementById("textSliderValueGreen").innerHTML = sliderValue;
  console.log(sliderValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/sliderGreen?value="+sliderValue, true);
  xhr.send();
}

function updateSliderPWMBlue(element) {
  var sliderValue = document.getElementById("pwmSliderBlue").value;
  document.getElementById("textSliderValueBlue").innerHTML = sliderValue;
  console.log(sliderValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/sliderBlue?value="+sliderValue, true);
  xhr.send();
}

</script>
</body>
</html>
)rawliteral";



#endif