# datasheets

Datasheets van de elektrische componenten uit de [bill of materials](../bill%20of%20materials/readme.md).

|volgnummer|component|datasheet|bron|
|----------|---------|---------|----|
|         1|Arduino Nano|[arduino-nano.pdf](arduino-nano.pdf)|[Arduino](https://docs.arduino.cc/resources/datasheets/A000005-datasheet.pdf)|
|          |ATmega328P (microcontroller op de Nano)|enkel online (± 33 MB)|[Microchip](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/DataSheets/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf)|
|         2|BT05 bluetooth module (BLE)|geen eigen datasheet, zie specificaties hieronder|–|
|          |HM-10 (BT05 is hiermee compatibel)|[hm-10.pdf](hm-10.pdf)|[DSD Tech / Cornell](https://people.ece.cornell.edu/land/courses/ece4760/PIC32/uart/HM10/DSD%20TECH%20HM-10%20datasheet.pdf)|
|          |CC2541 (bluetooth-chip op de BT05)|[cc2541.pdf](cc2541.pdf)|[Texas Instruments](https://www.ti.com/lit/ds/symlink/cc2541.pdf)|
|         3|DRV8833 dual H-brug|[drv8833.pdf](drv8833.pdf)|[Texas Instruments](https://www.ti.com/lit/ds/symlink/drv8833.pdf)|
|          |DRV8833 module (pinout)|zie opmerking hieronder|–|
|         5|QTR-8A reflectiesensor array|[qtr-8a.pdf](qtr-8a.pdf)|[Pololu](https://www.pololu.com/docs/pdf/0J12/QTR-8x.pdf)|
|          |QRE1113 (sensor op de QTR-8A)|[qre1113.pdf](qre1113.pdf)|[SparkFun](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/QRE1113.pdf)|
|         8|Step-down buck converter (OT253-B47)|geen datasheet beschikbaar, zie specificaties hieronder|[Otronic](https://otronic.nl/en/step-down-buck-converter-from-45v-24v-to-5v-3a-4r.html)|

### step-down buck converter OT253-B47

De verkoper publiceert geen datasheet en vermeldt niet welke chip er op de module zit. Specificaties volgens de productpagina:

|parameter|waarde|
|---------|------|
|ingangsspanning|4,5 – 24 V|
|uitgangsspanning|0,8 – 17 V instelbaar via potentiometer, of vast 1,8 / 2,5 / 3,3 / 5 / 9 / 12 V (ADJ-jumper doorknippen en gewenste jumper solderen)|
|uitgangsstroom|max. 3 A bij 5 V, 2,1 A continu|
|rendement|97,5 % (6,5 V → 5 V, 0,7 A), 94 % (12 V → 5 V, 1 A)|
|aansluitingen|IN+, GND, VO+, EN (laag = uitgang uit)|
|afmetingen|20 × 11 × 5 mm|

### bluetooth module BT05

De BT05 is een goedkope **Bluetooth Low Energy (BLE 4.0)** module met een CC2541-chip, compatibel met de HM-10. Er bestaat geen officiële datasheet van de BT05 zelf; de HM-10 datasheet geldt grotendeels (AT-commando's, service/characteristic).

|parameter|waarde|
|---------|------|
|bluetooth|4.0 BLE (werkt met iPhone en Android, **niet** als COM-poort in Windows)|
|chip|TI CC2541|
|voeding (VCC)|3,6 – 6 V (spanningsregelaar op de module), aangesloten op 5 V|
|logica (RXD / TXD)|3,3 V → spanningsdeler op RXD nodig bij een 5 V microcontroller|
|baudrate|9600 (standaard)|
|service / characteristic|`FFE0` / `FFE1`|
|aansluitingen|STATE, RXD, TXD, GND, VCC, EN|
|led op de module|knippert = niet verbonden, brandt = verbonden|

### DRV8833 module

De gebruikte module heeft andere pinnamen dan de chip in de datasheet:

|module|DRV8833 chip|functie|
|------|------------|-------|
|AIN1 / AIN2|AIN1 / AIN2|ingangen motor A|
|BIN1 / BIN2|BIN1 / BIN2|ingangen motor B|
|STBY|nSLEEP|hoog = driver aan|
|AO1 / AO2|AOUT1 / AOUT2|uitgangen motor A|
|BO1 / BO2|BOUT1 / BOUT2|uitgangen motor B|
|VM|VM|voeding motoren (2,7 – 10,8 V)|
|GND|GND||
|NC|–|niet aangesloten|

De nFAULT-uitgang van de chip is op deze module **niet** naar buiten gebracht.
