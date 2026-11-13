## bill of materials
<br />

Kostprijs van **één lijnvolger**, voor twee versies:

* **[plan A](#plan-a-eigen-pcb)**: eigen PCB als chassis (de gekozen versie)
* **[plan B](#plan-b-arduino-nano--breakout-boards)**: Arduino Nano met breakout boards (de opstelling waarop de proof of concepts draaien)

* Prijzen van de elektronica: stukprijs bij [LCSC](https://www.lcsc.com) (laagste prijstrap), omgerekend van USD naar EUR aan de ECB-koers van 09-11-2026 (1 USD = € 0,89342). Voor de onderdelen die zelf besteld zijn (LCSC en AliExpress, 08-11-2026) staat de **werkelijke aankoopprijs** per stuk.
* Bout en moeren van de glijder: [Tinytronics](https://www.tinytronics.nl/nl/gereedschap-en-montage/installatie-en-montagemateriaal/bouten/bout-m3-20mm-draad-100-stuks) (per 100 stuks, omgerekend per stuk).
* Prijzen zijn **per robot**: minimale bestelhoeveelheden en verzending zitten er niet in. LCSC-prijzen zijn zonder btw, de prijzen van AliExpress en Tinytronics met btw. De productie en assemblage van de PCB (JLCPCB) zit er ook niet in: die kost hangt sterk af van hoeveel je zelf soldeert.
* Bron van de onderdelenlijst: BOM-export uit EasyEDA Pro: [BOM_Linefollower_1-PCB_PCB_Linefollower_2026-11-09.xlsx](BOM_Linefollower_1-PCB_PCB_Linefollower_2026-11-09.xlsx).

## plan A: eigen PCB

### 1. elektronica op de PCB

|volgnummer|naam|omschrijving|LCSC|nieuw/recup|kostprijs/stuk|aantal|subtotaal|
|----------|----|------------|----|-----------|--------------|------|---------|
|1|C1, C2, C4, C9, C12, C24|Condensator 10 µF 25 V X5R, 0805|C15850|nieuw|€ 0,028|6|€ 0,17|
|2|C3, C5, C8, C11, C13, C14, C17, C19–C23, C25|Condensator 100 nF X7R, 0805|C49678|nieuw|€ 0,018|13|€ 0,24|
|3|C6|Elco 470 µF 25 V, SMD 8×10 (RVT25V470M8X10)|C47023111|nieuw|€ 0,077|1|€ 0,08|
|4|C7|Condensator 10 nF X7R, 0805|C1710|nieuw|€ 0,009|1|€ 0,01|
|5|C10|Condensator 2,2 µF X7R, 0805|C23692980|nieuw|€ 0,048|1|€ 0,05|
|6|C15, C16|Condensator 12 pF C0G, 0603 (kristal)|C38523|nieuw|€ 0,015|2|€ 0,03|
|7|C18|Condensator 1 µF X7R, 0805|C49420273|nieuw|€ 0,023|1|€ 0,02|
|8|CN1|ISP-header 2×3, 2,54 mm|C53200830|nieuw|€ 0,098|1|€ 0,10|
|9|D1, D2|Schottky diode SS34 (3 A, 40 V), SMA|C8678|nieuw|€ 0,032|2|€ 0,06|
|10|LED1|Led groen, 0805 (power-led)|C2297|nieuw|€ 0,015|1|€ 0,01|
|11|Q1|MOSFET AO3400A, SOT-23 (IR-leds aan/uit)|C20917|nieuw|€ 0,080|1|€ 0,08|
|12|R1, R2|Weerstand 22 Ω, 0805 (USB)|C17561|nieuw|€ 0,004|2|€ 0,01|
|13|R3, R20|Weerstand 1 kΩ, 0805|C17513|nieuw|€ 0,003|2|€ 0,01|
|14|R4–R11, R19, R22–R24|Weerstand 10 kΩ, 0805|C17414|nieuw|€ 0,003|12|€ 0,04|
|15|R12–R15|Weerstand 150 Ω, 0805 (IR-leds)|C17471|nieuw|€ 0,003|4|€ 0,01|
|16|R16|Weerstand 100 Ω, 0805|C17408|nieuw|€ 0,004|1|€ 0,00|
|17|R17, R18, R25, R26|Weerstand 100 kΩ, 0805|C149504|nieuw|€ 0,005|4|€ 0,02|
|18|R21|Weerstand 2 kΩ, 0805|C17604|nieuw|€ 0,003|1|€ 0,00|
|19|S1–S8|Reflectiesensor QRE1113GR|C232862|nieuw|€ 0,576|8|€ 4,60|
|20|SW1|Tuimelschakelaar SMTS-102-C4B-N0 (aan/uit), AliExpress (10 st. € 5,12)|C48393108|nieuw|€ 0,512|1|€ 0,51|
|21|SW2, SW3|Drukknop SKPRAAE010 (start, reset), AliExpress (10 st. € 3,32)|C127484|nieuw|€ 0,332|2|€ 0,66|
|22|U1|Vrouwelijke header 1×6, 2,54 mm (voor de BT05)|C54876728|nieuw|€ 0,118|1|€ 0,12|
|23|U3|Spanningsregelaar AMS1117-5.0 (SK1117-5.0), SOT-223|C165483|nieuw|€ 0,057|1|€ 0,06|
|24|U5|Motordriver DRV8833PWPR, HTSSOP-16|C50506|nieuw|€ 1,455|1|€ 1,46|
|25|U7|Microcontroller ATmega32U4-AU, TQFP-44|C44854|nieuw|€ 6,243|1|€ 6,24|
|26|USB1|Micro-USB-B connector, SMD|C40940|nieuw|€ 0,086|1|€ 0,09|
|27|X2|Kristal 16 MHz, SMD 3225|C13738|nieuw|€ 0,084|1|€ 0,08|
|28|TP1–TP6|Soldeerpads batterij en motoren (geen onderdeel)|–|–|–|6|€ 0,00|
|  |  |  |  |  |**subtotaal PCB**|  |**€ 14,76**|

### 2. mechanische en overige onderdelen

|volgnummer|naam|omschrijving|nieuw/recup|kostprijs/stuk|aantal|subtotaal|
|----------|----|------------|-----------|--------------|------|---------|
|29|BT05|Bluetooth Low Energy module (BLE 4.0, CC2541, HM-10 compatibel), op header U1|nieuw|€ 2,92|1|€ 2,92|
|30|Wielen N20|Rubber wiel met D-gat voor de as van de N20-motor, AliExpress (10 st. € 3,87)|nieuw|€ 0,387|2|€ 0,77|
|31|Li-Ion batterij 2S|Oplaadbare batterij 7,4 V, 2000 mAh, 2S, 15C, JST-stekker, incl. USB-oplaadkabel|nieuw|€ 14,90|1|€ 14,90|
|32|Montagebeugel N20|Witte kunststof motorbeugel met schroeven en moeren, AliExpress (5 st. € 0,51)|nieuw|€ 0,102|2|€ 0,20|
|33|N20 micro gearmotor 30:1|DC-motor met metalen tandwielkast, nominaal 6 V, ± 1000 rpm onbelast bij 6 V (zie berekening), AliExpress|nieuw|€ 1,99|2|€ 3,98|
|34|Glijder: bout M3 × 20 mm|Bout met bolle kop (staal, verzinkt) in het M3-gat (Ø 3,2 mm) van de sensorbalk: derde steunpunt|nieuw|€ 0,03|1|€ 0,03|
|35|Glijder: moer M3|Moer boven en onder de print om de hoogte van de glijder af te stellen|nieuw|€ 0,03|2|€ 0,06|
|  |  |  |  |**subtotaal mechanisch / overige**|  |**€ 22,86**|

### totaal

|deel|kostprijs|
|----|---------|
|1. elektronica op de PCB|€ 14,76|
|2. mechanische en overige onderdelen|€ 22,86|
|**totaal per robot**|**€ 37,62**|

> **Motoren 6 V op een batterij van 7,4 V:** de N20 is gemaakt voor 6 V. In de code wordt de snelheid beperkt met de parameter `power` (PWM), zodat de gemiddelde spanning op de motor rond 6 V blijft (± 70–75 % PWM, zie [berekening toerental](#berekening-toerental-n20-motor-301)).

## plan B: Arduino Nano + breakout boards

Opstelling met kant-en-klare modules, zie het [schema plan B](../technische%20tekeningen/elektronisch%20prototype/readme.md). Hierop draaien de [proof of concepts](../code/proof%20of%20concepts/readme.md).

|volgnummer|naam|omschrijving|nieuw/recup|kostprijs/stuk|aantal|subtotaal|
|----------|----|------------|-----------|--------------|------|---------|
|B1|Arduino Nano|Microcontroller (ATmega328P, 16 MHz, 5 V logica, 14 digitale I/O waarvan 6 PWM, 8 analoge ingangen), met ingebouwde power-led|nieuw|€ 3,74|1|€ 3,74|
|B2|BT05|Bluetooth Low Energy module (BLE 4.0, CC2541, HM-10 compatibel, UART 9600 baud, 3,3 V logica)|nieuw|€ 2,92|1|€ 2,92|
|B3|DRV8833|Dual H-brug motordriver module (2 kanalen, 2,7–10,8 V, 1,2 A continu / 2 A piek per kanaal, STBY-pin, geen nFAULT)|nieuw|€ 1,80|1|€ 1,80|
|B4|N20 micro gearmotor 30:1|DC-motor met metalen tandwielkast, nominaal 6 V, ± 1000 rpm onbelast bij 6 V|nieuw|€ 1,99|2|€ 3,98|
|B5|QTR-8A|Reflectiesensor-array met 8 QRE1113 IR-sensoren, analoge uitgangen, 5 V|nieuw|€ 12,59|1|€ 12,59|
|B6|Wielen N20|Rubber wiel met D-gat voor de as van de N20-motor|nieuw|€ 0,387|2|€ 0,77|
|B7|Li-Ion batterij 2S|Oplaadbare batterij 7,4 V, 2000 mAh, 2S, 15C, JST-stekker, incl. USB-oplaadkabel|nieuw|€ 14,90|1|€ 14,90|
|B8|Step-down buck converter (OT253-B47)|DC-DC converter 4,5–24 V in naar 5 V uit, max. 3 A; voeding 5 V logica vanuit de batterij|nieuw|€ 2,35|1|€ 2,35|
|B9|[Wipschakelaar klein](https://www.tinytronics.nl/nl/schakelaars/manuele-schakelaars/wipschakelaars/standaard-inbouw-wipschakelaar-klein)|Inbouw wipschakelaar aan/uit (2 pinnen), 250 VAC / 3 A; hoofdschakelaar batterij|nieuw|€ 0,45|1|€ 0,45|
|B10|Drukknop SKPRAAE010|Start/stop-knop (D2 naar GND)|nieuw|€ 0,332|1|€ 0,33|
|B11|Weerstand 1 kΩ|Spanningsdeler TX → RXD BT05, met draadjes (THT)|nieuw|€ 0,02|1|€ 0,02|
|B12|Weerstand 2 kΩ|Spanningsdeler TX → RXD BT05, met draadjes (THT)|nieuw|€ 0,02|1|€ 0,02|
|B13|Glijder: bout M3 × 20 mm|Bout met bolle kop (staal, verzinkt): derde steunpunt|nieuw|€ 0,03|1|€ 0,03|
|B14|Glijder: moer M3|Moeren om de hoogte van de glijder af te stellen|nieuw|€ 0,03|2|€ 0,06|
|B15|Montagebeugel N20|Witte kunststof motorbeugel met schroeven en moeren|nieuw|€ 0,102|2|€ 0,20|
|  |  |  |  |**totaal plan B**|  |**€ 44,16**|

> Plan B heeft nog een chassis (bodemplaat) nodig om alles op te monteren; dat is niet uitgewerkt omdat gekozen is voor plan A.

<br />

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

Voorbeeld: met een wiel van 28,6 mm diameter geeft dit bij nominale spanning een theoretische maximale snelheid van
$v = \pi \cdot d \cdot \frac{n}{60} = \pi \cdot 0{,}0286\ \text{m} \cdot \frac{1230}{60}\ \text{s}^{-1} \approx 1{,}8\ \text{m/s}$ (onbelast).
