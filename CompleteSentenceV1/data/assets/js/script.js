
// WebSocket Handle misc 
let socket = new WebSocket(`ws://${window.location.hostname}/ws`);

socket.onopen = function (e) {
    console.log("Websockets Verbindung hergstellt!");
};

socket.onmessage = function (event) {

    /* JSON Object expected */
    const data = JSON.parse(event.data);
    
    // Zugriff auf die Werte
    const str_SSID = data.str_SSID;
    const str_Password = data.str_Password;
    const int_Red = data.int_Red;
    const int_Green = data.int_Green;
    const int_Blue = data.int_Blue;
    const int_Bright = data.int_Bright;

    /* Set the Values */
    const SSID = document.getElementById("ssid");
    const Password = document.getElementById("password");
    const rSlider = document.getElementById("r-slider");
    const gSlider = document.getElementById("g-slider");
    const bSlider = document.getElementById("b-slider");
    const hSlider = document.getElementById("brightness-slider");

    /* update Values incl. dispatch */
    SSID.value = str_SSID;
    SSID.dispatchEvent(new Event("input"));

    Password.value = str_Password;    
    Password.dispatchEvent(new Event("input"));
    
    rSlider.value = int_Red;
    rSlider.dispatchEvent(new Event("input"));
    
    gSlider.value = int_Green;
    gSlider.dispatchEvent(new Event("input"));
    
    bSlider.value = int_Blue;
    bSlider.dispatchEvent(new Event("input"));
    
    hSlider.value = int_Bright;
    hSlider.dispatchEvent(new Event("input"));

};

socket.onclose = function (event) {
    if (event.wasClean) {
        console.log('Verbindung zum Server wurde geschlossen!');
    } else {
        console.log('Verbindung verloren: Timeout!');
    }
};

socket.onerror = function (error) {
    console.log('[error]:', error.message);
};

// call for initial data
window.onload = function() {
    
    is_init = true;

    socket.send("need_initial_data");

    console.log('call for initial data done');
}

// SSID AND Password Change senders 
function send_ssid() {
    SSID_ID = document.getElementById("ssid");
    const SSID_str = SSID_ID.value;
    const sendstring ="change_ssid:" + SSID_str;

    socket.send(sendstring);
    
    //debug
    console.log(sendstring);
}

function send_password() {
    PASSWORD_ID = document.getElementById("password");
    const PASSWORD_str = PASSWORD_ID.value;
    const sendstring = "change_password:" + PASSWORD_str;

    socket.send(sendstring);

    //debug
    console.log(sendstring);
}


function reconnectWiFi() {
   
    console.log('try to call for reconnectin wifi');

    socket.send("reconect_wifi");
   
    console.log('call for reconnectin wifi done');
}

function saveColor() {

    console.log("send: color seve");

    socket.send("save_color");
    
}




// mode change button function
function changeMode() {
    console.log('send a [change mode]');

    socket.send("change_mode");

}



document.addEventListener("DOMContentLoaded", function () {
    const rSlider = document.getElementById("r-slider");
    const gSlider = document.getElementById("g-slider");
    const bSlider = document.getElementById("b-slider");
    const brightnessSlider = document.getElementById("brightness-slider");

    const rValue = document.getElementById("r-value");
    const gValue = document.getElementById("g-value");
    const bValue = document.getElementById("b-value");
    const brightnessValue = document.getElementById("brightness-value");

    const colorPreview = document.getElementById("color-preview");
    const brightnessPreview = document.getElementById("brightness-preview");

    function send_sliders() {
        const red = parseInt(rSlider.value);
        const green = parseInt(gSlider.value);
        const blue = parseInt(bSlider.value);
        const bright = parseInt(brightnessSlider.value);

        // let SliderStruct = {
        //     val_red:red,
        //     val_green:green,
        //     val_blue:blue,
        //     val_bright:bright
        // };
    
        // const sendstring = "change_color:" + JSON.stringify(SliderStruct);

        const sendstring = "change_color:" + String(red).padStart(3, '0') + "-" + String(green).padStart(3, '0') + "-" + String(blue).padStart(3, '0') + "-" + String(bright).padStart(3, '0') + "\0";
    
        socket.send(sendstring);
    
        // debug
        console.log(sendstring);
    }





    // Funktion, um die RGB-Farbe ohne Helligkeit in der Vorschau zu aktualisieren
    function updateColorPreview() {
        const r = parseInt(rSlider.value);
        const g = parseInt(gSlider.value);
        const b = parseInt(bSlider.value);

        rValue.textContent = r;
        gValue.textContent = g;
        bValue.textContent = b;

        colorPreview.style.backgroundColor = `rgb(${r}, ${g}, ${b})`;

         // send Slidervalues to Client 
        send_sliders();
    }

    // Funktion, um die RGB-Farbe mit Helligkeit in der Vorschau zu aktualisieren
    function updateBrightnessPreview() {
        const brightness = parseInt(brightnessSlider.value);

        brightnessValue.textContent = brightness;

        brightnessPreview.style.backgroundColor = `rgb(${brightness}, ${brightness}, ${brightness})`;

         // send Slidervalues to Client 
        send_sliders();
    }

    

    // Event Listener für Slider-Bewegungen
    rSlider.addEventListener("input", updateColorPreview);
    gSlider.addEventListener("input", updateColorPreview);
    bSlider.addEventListener("input", updateColorPreview);
    brightnessSlider.addEventListener("input", updateBrightnessPreview);

    // Initiale Farbe anzeigen
    updateColorPreview();
    updateBrightnessPreview();
   
});