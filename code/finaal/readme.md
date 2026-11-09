# finaal programma

Het programma van de lijnvolger. Gebaseerd op het cyclus-programma van de leerkracht (`SerialCommand` + `EEPROMAnything`).

Code: [linefollower/linefollower.ino](linefollower/linefollower.ino)

## Wat doet het?
* **Vaste cyclus (zoals een PLC):** elke `cycle` µs (standaard 5000 µs = 200× per seconde) worden de sensoren gelezen, de PID berekend en de motoren aangestuurd. Tussen de cycli worden commando's ingelezen.
* **Sensoren:** elke cyclus 2 metingen: IR-leds uit en IR-leds aan. Het verschil is het gereflecteerde licht, dus zonlicht of tl-licht stoort niet.
* **Kalibratie:** per sensor wordt de waarde op wit en op zwart onthouden. Daarmee wordt elke sensor omgezet naar 0 (wit) … 1000 (zwart), zodat alle sensoren gelijk reageren.
* **PID-regeling:** de positie van de lijn (0 … 7000, midden = 3500) wordt omgezet naar een verschil in snelheid tussen links en rechts.
* **EEPROM:** alle parameters en de kalibratie blijven bewaard, ook als de batterij eraf gaat.
* **Commando's** via USB én bluetooth (BT05), zonder dat de regeling stopt. Het antwoord gaat naar de poort waar het commando vandaan kwam.
* **Start/stop** met de drukknop (interrupt: stopt onmiddellijk) of met een commando.
* **Veiligheid:** stopt automatisch bij een lege batterij (< 6,2 V) of een fout van de motordriver.

## Voor de eerste keer
1. Doe eerst het [testprogramma](../test/readme.md) en pas bovenaan in `linefollower.ino` aan wat je gevonden hebt: `SENSOR_ORDER`, `MOTOR_A_IS_LEFT`, `INVERT_LEFT`, `INVERT_RIGHT`.
2. Upload (bord **Arduino Leonardo**).
3. Kalibreer: zet de robot op de lijn en typ `cal` → beweeg 5 s de sensoren over wit en zwart. Of `cal draai`: de robot draait zelf heen en weer over de lijn.
4. Druk op de startknop of typ `start`.

## Commando's

|commando|wat doet het|
|--------|------------|
|`start` / `stop`|rijden / stoppen (ook met de drukknop)|
|`cal`|kalibratie 5 s, sensoren met de hand over wit en zwart bewegen|
|`cal draai`|kalibratie 5 s, robot draait zelf over de lijn|
|`set kp 40`|P-factor|
|`set ki 0`|I-factor|
|`set kd 0.5`|D-factor|
|`set power 100`|basissnelheid (0 … 255)|
|`set threshold 300`|vanaf welke waarde (0 … 1000) een sensor de lijn "ziet"|
|`set cycle 5000`|cyclustijd in µs (1000 … 50000)|
|`debug`|alle parameters, kalibratie, batterij en rekentijd tonen|
|`sensors`|huidige sensorwaarden en positie|
|`stream 200`|elke 200 ms de status sturen (positie, motoren, batterij), `stream 0` = uit|
|`default`|parameters terug naar standaard (kalibratie blijft)|
|`reset`|microcontroller herstarten|
|`help`|lijst met commando's|

Elke `set` wordt meteen in het EEPROM bewaard.

## PID afstellen
Begin met lage snelheid en stel stap voor stap bij:
1. `set power 80`, `set ki 0`, `set kd 0`.
2. Verhoog `kp` tot de robot de lijn volgt maar begint te slingeren.
3. Verhoog `kd` tot het slingeren weg is.
4. Verhoog `power` en herhaal. `ki` is meestal niet nodig (klein houden, bv. 0 … 5).

Het **foutsignaal** is in "sensorafstanden": 1,0 = de lijn ligt één sensor naast het midden.
Voorbeeld: `kp 40` → bij 1 sensor afwijking is het snelheidsverschil 2 × 40 = 80 (links +40, rechts −40).

## Let op
* `debug` tijdens het rijden geeft enkel een korte regel (bluetooth is traag en mag de regeling niet ophouden).
* Kijk met `debug` naar **calculation time**: die moet kleiner blijven dan `cycle`.
* Ziet geen enkele sensor de lijn (onderbreking), dan rijdt de robot verder met de laatste positie.
