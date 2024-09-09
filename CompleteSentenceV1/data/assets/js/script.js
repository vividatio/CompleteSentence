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

    // Funktion, um die RGB-Farbe ohne Helligkeit in der Vorschau zu aktualisieren
    function updateColorPreview() {
        const r = parseInt(rSlider.value);
        const g = parseInt(gSlider.value);
        const b = parseInt(bSlider.value);

        rValue.textContent = r;
        gValue.textContent = g;
        bValue.textContent = b;

        colorPreview.style.backgroundColor = `rgb(${r}, ${g}, ${b})`;
    }

    // Funktion, um die RGB-Farbe mit Helligkeit in der Vorschau zu aktualisieren
    function updateBrightnessPreview() {
        const r = parseInt(rSlider.value);
        const g = parseInt(gSlider.value);
        const b = parseInt(bSlider.value);
        const brightness = parseInt(brightnessSlider.value);

        brightnessValue.textContent = brightness;

        brightnessPreview.style.backgroundColor = `rgb(${brightness}, ${brightness}, ${brightness})`;
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