# [Klokkurino](https://github.com/lassebrenden/klokkurino/)
Prosjektoppgave i INF1510

## Beskrivelse
Klokkurino er en Arduino-styrt alarmklokke.

### Lysspill
I tillegg til vanlig klokke- og alarmfunksjonalitet inneholder Klokkurino et enkelt lysspill som brukeren må fullføre for at alarmen skal slå seg av. Hensikten er å tvinge brukeren til å stå opp i stedet for å sove videre (derfor har vi heller ingen slumrefunksjon.)

### Oppvåkningslys
Klokkurino kan også kobles til en hvilken som helst lampe med glødepære eller halogenpære vha. en dimmer. En halvtime før alarmen slår inn vil Klokkurino gradivs justere lysstyrken – vha. en servomotor – for å gi en mest mulig naturlig oppvåkning.

## Komponenter
* 1 x 100uF/25V Electrolytic Capacitor
* 1 x 8 GB USB minnepinne
* 1 x [CO/TECH dimmer for vegguttak](http://www.clasohlson.com/no/Dimmer/36-2337)
* 1 x [Elechouse Arduino USB-SD MP3 Shield](http://www.elechouse.com/elechouse/index.php?main_page=product_info&cPath=168_170&products_id=2193)
* 1 x [MyAmp Høyttaler](http://www.coolstuff.no/MyAmp-Hoyttaler)
* 1 x Arduino koblingsbrett
* 1 x Arduino Liquid Crystal Display
* 1 x Arduino Uno
* 1 x Potensiometer
* 1 x Skrivebordslampe
* 4 x 10K ohm resistor
* 4 x LEDs (blå, grønn, gul, rød)
* 4 x Trykknapper
* 5 x 220 ohm resistor
* Trepinner, papp, maskeringsteip, lim, maling, ledninger

## Eksterne bibliotek

### [Bounce](http://playground.arduino.cc/code/bounce)
Laget av Thomas Ouellet Fredericks. Forhindrer at ett trykk på knappene blir registrert som flere suksessive trykk.

### [MP3](http://www.elechouse.com/elechouse/index.php?main_page=product_info&cPath=168_170&products_id=2193)
Laget av Elechouse. Kontrollerer et [Elechouse Arduino USB-SD MP3 Shield](http://www.elechouse.com/elechouse/index.php?main_page=product_info&cPath=168_170&products_id=2193).

### [Time](http://www.pjrc.com/teensy/td_libs_Time.html)
Laget av Michael Margolis. Utvider Arduino med tidtakingsfunksjonalitet.

### [TimeAlarms](http://www.pjrc.com/teensy/td_libs_TimeAlarms.html)
Laget av Michael Margolis. Utfører funksjoner til bestemte tider (vha. [Time](http://www.pjrc.com/teensy/td_libs_Time.html)).

### [Timer](https://github.com/lassebrenden/klokkurino/tree/master/Documents/Arduino/libraries/Timer)
Laget av Lasse Brenden. Utvider Arduino med et alternativ til Arduinos `delay()`.  [Timer](https://github.com/lassebrenden/klokkurino/tree/master/Documents/Arduino/libraries/Timer) gjør det mulig å forsinke programmet uten å stoppe det helt.