# testprogramma PCB

Met dit programma test je elk onderdeel van de PCB **apart**, voor je het finale programma gebruikt.
Zo weet je zeker dat de print, het solderen en de aansluitingen in orde zijn.

Code: [pcb_test/pcb_test.ino](pcb_test/pcb_test.ino)

## Eerst: bootloader branden (1 keer per nieuwe print)
Een nieuwe ATmega32U4 is leeg. Via de ISP-header (CN1) zet je er eerst de bootloader van een Arduino Leonardo op. Daarna kan je gewoon via USB uploaden.

1. Upload op een Arduino Nano het voorbeeld **File → Examples → ArduinoISP**.
2. Verbind de Nano met CN1 van de lijnvolger:

|Nano|CN1|
|----|---|
|D12|pin 1 (MISO)|
|5V|pin 2 (VCC)|
|D13|pin 3 (SCK)|
|D11|pin 4 (MOSI)|
|D10|pin 5 (RESET)|
|GND|pin 6 (GND)|

3. Kies in de Arduino IDE: bord **Arduino Leonardo**, programmer **Arduino as ISP**, poort van de **Nano**.
4. **Tools → Burn Bootloader**.
5. Haal de Nano los en sluit de lijnvolger via USB aan. Hij verschijnt nu als Arduino Leonardo.

## Uploaden
* bord: **Arduino Leonardo**
* poort: de USB-poort van de lijnvolger
* seriële monitor: **115200 baud**, regeleinde maakt niet uit

Alle commando's werken ook via bluetooth (BT05, bv. LightBlue op iPhone). Het antwoord komt terug op de poort waar het commando binnenkwam.

## Testen

|stap|commando|wat je moet zien|
|----|--------|----------------|
|1. communicatie|`ping`|`pong` (doe dit ook via bluetooth)|
|2. batterij|`batterij`|spanning van de batterij, bv. `7.84 V` (± 0 V met enkel USB)|
|3. IR-leds|`ir aan` / `ir uit`|met een gsm-camera zie je de sensoren paars oplichten|
|4. sensoren|`sensors aan`|elke 200 ms 8 waarden: hoog op wit, laag op zwart|
|5. drukknop|druk op de startknop|`knop ingedrukt (1)`, `(2)`, ... één melding per druk|
|6. motor A|`motor a 100`|één motor draait. Noteer: **links of rechts? vooruit of achteruit?**|
|7. motor B|`motor b 100`|de andere motor draait. Noteer hetzelfde|
|8. motoren uit|`motor stop`|beide motoren stil|
|9. foutmelding|`fault`|`ok (hoog)`|
|10. EEPROM|`eeprom`|`EEPROM ok`|

### Stap 4: volgorde van de sensoren nakijken
Zet `sensors aan` en schuif een zwarte lijn langzaam van **links naar rechts** onder de sensoren (kijkend in rijrichting).
De waarde die laag wordt, moet gaan van **s1 naar s8**. Gebruik eventueel **Tools → Serial Plotter**.

Gaat de volgorde anders, pas dan in het finale programma `SENSOR_ORDER` aan. Daarin staat per fysieke plaats (links → rechts) welke sensor (s1 = 0 … s8 = 7) erop zit.
Voorbeeld: komt eerst s6, dan s5 … s1, dan s7, s8: `{5, 4, 3, 2, 1, 0, 6, 7}`.

### Stap 6 en 7: motoren nakijken
Pas in het finale programma aan wat je gezien hebt:
* `MOTOR_A_IS_LEFT`: `true` als motor A de linkermotor is
* `INVERT_LEFT` / `INVERT_RIGHT`: `true` als die motor **achteruit** draait bij `100`

## Let op
* Test de motoren met de robot **in de hand** of op een blokje: de wielen mogen vrij draaien.
* De motoren draaien enkel met batterij (VM komt van de batterij, niet van USB).
* `motor a -100` laat motor A in de andere richting draaien.
