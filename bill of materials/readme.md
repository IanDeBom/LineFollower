## bill of materials
<br />

|volgnummer|naam|omschrijving|nieuw/recup|kostprijs/stuk|aantal|subtotaal|
|----------|----|------------|-----------|--------------|------|---------|
|         1|Arduino Nano|Microcontroller (ATmega328P, 16 MHz, 5 V logica, 14 digitale I/O waarvan 6 PWM, 8 analoge ingangen)|nieuw|€ 5,00|1|€ 5,00|
|         2|HC-05|Bluetooth-module (Bluetooth 2.0 SPP, UART-communicatie, standaard 9600 baud)|nieuw|€ 6,00|1|€ 6,00|
|         3|DRV8833|Dual H-brug motordriver (2 kanalen, 2,7–10,8 V, 1,2 A continu / 2 A piek per kanaal)|nieuw|€ 3,00|1|€ 3,00|
|         4|N20 micro gearmotor 30:1|DC-motor met metalen tandwielkast, nominaal 6 V, ± 1000 rpm onbelast bij 6 V (zie berekening)|nieuw|€ 5,00|2|€ 10,00|
|         5|QTR-8A|Reflectiesensor-array met 8 QRE1113 IR-sensoren, analoge uitgangen, 5 V|nieuw|€ 10,00|1|€ 10,00|
|         6|Silicone wielen (5908)|Wielen met siliconen band, voor TT-motor / SG90 servo|nieuw|€ 1,50|2|€ 3,00|
|         7|Li-Ion batterij 2S|Oplaadbare batterij 7,4 V, 2000 mAh, 2S, 15C (max. 30 A), JST-stekker, incl. USB-oplaadkabel|nieuw|€ 15,00|1|€ 15,00|
|         8|Step-down buck converter (OT253-B47)|DC-DC converter 4,5–24 V in naar 5 V uit, max. 3 A, uitgang instelbaar (4R7 spoel); voeding 5 V logica vanuit de batterij|nieuw|€ 3,00|1|€ 3,00|
|         9|[Wipschakelaar klein](https://www.tinytronics.nl/nl/schakelaars/manuele-schakelaars/wipschakelaars/standaard-inbouw-wipschakelaar-klein)|Inbouw wipschakelaar aan/uit (SPST, 2 pinnen), 250 VAC / 3 A, inbouwmaat 8,5 × 13,5 mm; hoofdschakelaar batterij|nieuw|€ 0,45|1|€ 0,45|
|          |      |            |           |**totaal**|      |**€ 55,45**|

<br />

> De prijzen zijn richtprijzen en moeten nog aangepast worden aan de effectieve aankoopprijs.

### berekening toerental N20 motor (30:1)

Een N20 motor zonder tandwielkast draait onbelast ongeveer **30 000 rpm** bij 6 V.
De tandwielkast heeft een overbrengingsverhouding van **30:1**, dus:

$$
n_{uitgang} = \frac{n_{motor}}{i} = \frac{30\,000\ \text{rpm}}{30} = 1000\ \text{rpm} \quad \text{(bij 6 V)}
$$

Het toerental is ongeveer evenredig met de spanning. De robot wordt gevoed met een 2S Li-Ion batterij:

$$
n \approx 1000\ \text{rpm} \cdot \frac{U}{6\ \text{V}}
$$

|batterijtoestand|spanning|toerental (onbelast)|
|----------------|--------|--------------------|
|volledig geladen|8,4 V|± 1400 rpm|
|nominaal|7,4 V|± 1230 rpm|
|leeg|6,0 V|± 1000 rpm|

> De N20 is gespecificeerd voor 6 V. Op 7,4–8,4 V draait hij sneller maar slijt hij ook sneller. Via PWM (duty cycle ≤ ± 70 %) kan de gemiddelde motorspanning beperkt worden tot ± 6 V.

Voorbeeld: met een wiel van 34 mm diameter geeft dit bij nominale spanning een theoretische maximale snelheid van
$v = \pi \cdot d \cdot \frac{n}{60} = \pi \cdot 0{,}034\ \text{m} \cdot \frac{1230}{60}\ \text{s}^{-1} \approx 2{,}2\ \text{m/s}$ (onbelast).
