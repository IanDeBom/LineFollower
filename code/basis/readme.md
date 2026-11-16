# basis programma

volgende zaken werken samen in dit template project  

* instelbare parameters mbv. SerialCommands 
  * cyclus tijd
  * debug on/off
  * start/stop
* parameters worden opgeslagen in eeprom geheugen
* start/stop kan ook geactiveerd worden mbv. een externe interrupt
* draadloze communicatie

Met behulp van de cyclus kan je bvb. een knipperende led programmeren.

Code: [basis/basis.ino](basis/basis.ino), gebaseerd op het cyclus-programma van de leerkracht (`SerialCommand` + `EEPROMAnything`).
Dit programma is de "kapstok" van het [finale programma](../finaal/readme.md): daar komen de sensoren, de PID en de motoren erbij.

## Hoe werkt het?
* **Vaste cyclus:** de functie `cycle()` wordt om de `cycle` µs uitgevoerd (standaard 5000 µs), zoals bij een PLC. Tussen de cycli worden de commando's ingelezen.
* **Commando's:** via USB (seriële monitor, 115200 baud) én via bluetooth (BT05, bv. LightBlue op iPhone). Het antwoord gaat terug naar de poort van het commando. Commando's worden letter per letter ingelezen, de loop wordt nooit geblokkeerd.
* **EEPROM:** elke `set` wordt meteen bewaard. Na een reset of zonder batterij blijven de waarden behouden.
* **Start/stop:** met het commando `start` / `stop` of met de drukknop. De drukknop werkt via een **interrupt** (INT0): de toestand wisselt meteen, wat het programma ook aan het doen is.
* **Knipperende led:** zolang de robot "gestart" is, knipperen de IR-leds van de sensoren (de PCB heeft geen aparte led op een vrije pin). IR-licht zie je niet met het oog, wel met een **gsm-camera**.

## Commando's

|commando|wat doet het|
|--------|------------|
|`set cycle 5000`|cyclustijd in µs|
|`set blink 500`|knippertijd van de led in ms|
|`set debug on` / `set debug off`|elke seconde de status doorsturen (aan/uit)|
|`start` / `stop`|robot starten / stoppen (ook met de drukknop)|
|`debug`|alle parameters en de rekentijd (calculation time) tonen|
|`help`|lijst met commando's|

## Testen
1. Upload `basis.ino` (bord: **Arduino Leonardo**) en open de seriële monitor op **115200 baud**.
2. `debug` → je ziet de parameters. `calculation time` moet kleiner zijn dan `cycle time`.
3. `start` → `gestart`, de IR-leds knipperen (bekijk ze met een gsm-camera).
4. Druk op de drukknop → `gestopt`. Nog eens drukken → `gestart`.
5. `set blink 100` → de led knippert sneller.
6. Haal de USB-kabel uit en steek hem terug in. `debug` → de knippertijd is nog altijd 100: bewaard in EEPROM.
7. Doe hetzelfde via bluetooth (LightBlue → BT05 → FFE1): bv. `start`, `stop`, `debug`.
8. `set debug on` → elke seconde een statusregel.

## Let op
* De motoren blijven in dit programma uit (nSleep van de DRV8833 laag).
* PD5 (nFault) wordt in `setup()` terug als ingang gezet, omdat de Arduino-code voor de Leonardo die pin als TX-led gebruikt.
