## bill of materials
<br />

Kostprijs van **één lijnvolger** met de eigen PCB.

* Prijzen van de elektronica: stukprijs bij [LCSC](https://www.lcsc.com) (laagste prijstrap), omgerekend van USD naar EUR aan de ECB-koers van 09-11-2026 (1 USD = € 0,89342).
* Prijzen zijn **per robot**: minimale bestelhoeveelheden, btw en verzending zitten er niet in. De productie en assemblage van de PCB (JLCPCB) zit er ook niet in: die kost hangt sterk af van hoeveel je zelf soldeert.
* Bron van de onderdelenlijst: BOM-export uit EasyEDA Pro: [BOM_Linefollower_1-PCB_PCB_Linefollower_2026-11-09.xlsx](BOM_Linefollower_1-PCB_PCB_Linefollower_2026-11-09.xlsx).

### 1. elektronica op de PCB

|volgnummer|naam|omschrijving|LCSC|nieuw/recup|kostprijs/stuk|aantal|subtotaal|
|----------|----|------------|----|-----------|--------------|------|---------|
|1|C1, C2, C4, C9, C12, C24|Condensator 10 µF 25 V X5R, 0805|C15850|nieuw|€ 0,028|6|€ 0,17|
|2|C3, C5, C8, C11, C13, C14, C17, C19–C23, C25|Condensator 100 nF X7R, 0805|C49678|nieuw|€ 0,018|13|€ 0,24|
|3|C6|Elco 470 µF 25 V, SMD 8×10 (RVT25V470M8X10)|C47023111|nieuw|€ 0,080|1|€ 0,08|
|4|C7|Condensator 10 nF X7R, 0805|C1710|nieuw|€ 0,009|1|€ 0,01|
|5|C10|Condensator 2,2 µF X7R, 0805|C23692980|nieuw|€ 0,048|1|€ 0,05|
|6|C15, C16|Condensator 12 pF C0G, 0603 (kristal)|C38523|nieuw|€ 0,015|2|€ 0,03|
|7|C18|Condensator 1 µF X7R, 0805|C49420273|nieuw|€ 0,023|1|€ 0,02|
|8|CN1|ISP-header 2×3, 2,54 mm|C53200830|nieuw|€ 0,139|1|€ 0,14|
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
|20|SW1|Tuimelschakelaar SMTS-102-C4B-N0 (aan/uit)|C48393108|nieuw|€ 0,880|1|€ 0,88|
|21|SW2, SW3|Drukknop SKPRAAE010 (start, reset)|C127484|nieuw|€ 0,417|2|€ 0,83|
|22|U1|Vrouwelijke header 1×6, 2,54 mm (voor de BT05)|C54876728|nieuw|€ 0,124|1|€ 0,12|
|23|U3|Spanningsregelaar AMS1117-5.0 (SK1117-5.0), SOT-223|C165483|nieuw|€ 0,057|1|€ 0,06|
|24|U5|Motordriver DRV8833PWPR, HTSSOP-16|C50506|nieuw|€ 1,455|1|€ 1,46|
|25|U7|Microcontroller ATmega32U4-AU, TQFP-44|C44854|nieuw|€ 6,243|1|€ 6,24|
|26|USB1|Micro-USB-B connector, SMD|C40940|nieuw|€ 0,091|1|€ 0,09|
|27|X2|Kristal 16 MHz, SMD 3225|C13738|nieuw|€ 0,084|1|€ 0,08|
|28|TP1–TP6|Soldeerpads batterij en motoren (geen onderdeel)|–|–|–|6|€ 0,00|
|  |  |  |  |  |**subtotaal PCB**|  |**€ 15,36**|

### 2. mechanische en overige onderdelen

|volgnummer|naam|omschrijving|nieuw/recup|kostprijs/stuk|aantal|subtotaal|
|----------|----|------------|-----------|--------------|------|---------|
|29|BT05|Bluetooth Low Energy module (BLE 4.0, CC2541, HM-10 compatibel), op header U1|nieuw|€ 2,92|1|€ 2,92|
|30|Silicone wielen (5908)|Wielen met siliconen band|nieuw|€ 2,75|2|€ 5,50|
|31|Li-Ion batterij 2S|Oplaadbare batterij 7,4 V, 2000 mAh, 2S, 15C, JST-stekker, incl. USB-oplaadkabel|nieuw|€ 14,90|1|€ 14,90|
|32|Montagebeugel N20|Metalen motorbeugel voor N20 motor|nieuw|€ 0,30|2|€ 0,60|
|33|N20 micro gearmotor 30:1|DC-motor met metalen tandwielkast, nominaal 6 V, ± 1000 rpm onbelast bij 6 V (zie berekening)|nieuw|€ 2,01|2|€ 4,02|
|  |  |  |  |**subtotaal mechanisch / overige**|  |**€ 27,94**|

### totaal

|deel|kostprijs|
|----|---------|
|1. elektronica op de PCB|€ 15,36|
|2. mechanische en overige onderdelen|€ 27,94|
|**totaal per robot**|**€ 43,30**|

> Het subtotaal van de PCB is berekend op de onafgeronde stukprijzen; de afgeronde regels tellen op tot € 15,34.

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
