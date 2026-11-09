# elektronisch schema en PCB

Ontworpen in EasyEDA Pro. De PCB is 2-laags, 1,6 mm dik en dient ook als chassis van de lijnvolger.

|bestand|inhoud|
|-------|------|
|[EasyEda_ElektrischSchema_Linefollower.pdf](EasyEda_ElektrischSchema_Linefollower.pdf)|elektrisch schema (voeding, microcontroller, motorsturing, bluetooth, sensoren)|
|[Gerber_1-PCB_PCB_Linefollower_2026-11-09.zip](Gerber_1-PCB_PCB_Linefollower_2026-11-09.zip)|Gerber- en boorbestanden om de PCB te laten maken (bv. bij JLCPCB)|

### PCB laten maken
1. Upload de Gerber-zip bij de PCB-fabrikant (bv. JLCPCB → *Order now*). Pak de zip **niet** uit.
2. Kies 2 lagen, 1,6 mm dikte, LeadFree HASL.
3. Voor assemblage: BOM en plaatsingsbestand (CPL) exporteren uit EasyEDA Pro. De SMD-onderdelen zitten aan de **onderkant**; schakelaars, headers, LED1/R3 en C6 aan de bovenkant worden zelf gesoldeerd.

De onderdelen staan in de [bill of materials](../../bill%20of%20materials/readme.md), de datasheets in [datasheets](../../datasheets/readme.md).
