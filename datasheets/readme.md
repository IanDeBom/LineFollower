# datasheets

Datasheets van de elektrische componenten uit de [bill of materials](../bill%20of%20materials/readme.md). Het volgnummer verwijst naar de BOM.

|volgnummer|component|datasheet|bron|
|----------|---------|---------|----|
|9|SS34 schottky diode (D1, D2)|[ss34.pdf](ss34.pdf)|[LCSC C8678](https://www.lcsc.com/product-detail/C8678.html)|
|11|AO3400A MOSFET (Q1)|[ao3400a.pdf](ao3400a.pdf)|[LCSC C20917](https://www.lcsc.com/product-detail/C20917.html)|
|19|QRE1113GR reflectiesensor (S1–S8)|[qre1113.pdf](qre1113.pdf)|[SparkFun](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/QRE1113.pdf)|
|23|AMS1117-5.0 / SK1117-5.0 spanningsregelaar (U3)|[ams1117-5.0.pdf](ams1117-5.0.pdf)|[LCSC C165483](https://www.lcsc.com/product-detail/C165483.html)|
|24|DRV8833 dual H-brug (U5)|[drv8833.pdf](drv8833.pdf)|[Texas Instruments](https://www.ti.com/lit/ds/symlink/drv8833.pdf)|
|25|ATmega32U4 microcontroller (U7)|[atmega32u4.pdf](atmega32u4.pdf)|[LCSC C44854](https://www.lcsc.com/product-detail/C44854.html)|
|29|BT05 bluetooth module (BLE)|geen eigen datasheet, zie specificaties hieronder|–|
|  |HM-10 (BT05 is hiermee compatibel)|[hm-10.pdf](hm-10.pdf)|[DSD Tech / Cornell](https://people.ece.cornell.edu/land/courses/ece4760/PIC32/uart/HM10/DSD%20TECH%20HM-10%20datasheet.pdf)|
|  |CC2541 (bluetooth-chip op de BT05)|[cc2541.pdf](cc2541.pdf)|[Texas Instruments](https://www.ti.com/lit/ds/symlink/cc2541.pdf)|

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
