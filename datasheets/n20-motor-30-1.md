# N20 micro gearmotor 30:1 (6 V)

De gebruikte motor is een **generieke N20 30:1, 6 V** (± 1000 rpm). Van deze generieke motor bestaat geen datasheet.
Als referentie worden de specificaties gebruikt van de motor met dezelfde maten, overbrenging en spanning van Pololu: **30:1 Micro Metal Gearmotor HP 6V (#1093)**.

|parameter|waarde|
|---------|------|
|nominale spanning|6 V|
|overbrenging|29,86:1 (± 30:1)|
|toerental onbelast|1000 rpm|
|stroom onbelast|100 mA|
|blokkeerstroom (stall)|1,6 A (theoretisch, geëxtrapoleerd)|
|blokkeerkoppel (stall)|0,57 kg·cm (theoretisch, geëxtrapoleerd)|
|max. vermogen|1,5 W|
|doorsnede tandwielkast|10 × 12 mm|
|uitgaande as|3 mm, D-profiel, 9 mm lang|

Bronnen:
* productpagina: [Pololu #1093 – 30:1 Micro Metal Gearmotor HP 6V](https://www.pololu.com/product/1093)
* datasheet van de reeks: [pololu-micro-metal-gearmotors.pdf](pololu-micro-metal-gearmotors.pdf) ([Pololu](https://www.pololu.com/file/0J1487/pololu-micro-metal-gearmotors.pdf))

### Opmerkingen voor de lijnvolger
* De motor is gemaakt voor **6 V**, de batterij levert 7,4 V (8,4 V volgeladen). In de code wordt de snelheid daarom beperkt met de parameter `power` (PWM), zodat de gemiddelde spanning op de motor rond 6 V blijft (± 70–75 % PWM).
* De blokkeerstroom (1,6 A) ligt boven de 1,5 A continu per kanaal van de DRV8833: de motor niet langdurig laten blokkeren.
