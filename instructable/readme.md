# Instructable

Een instructable is een stappenplan - zonder verdere uitleg - hoe je vertrekkend van de bill of materials en gebruik makend van de technische tekeningen de robot kan nabouwen. Ook de nodige stappen om de microcontroller te compileren en te uploaden staan beschreven.  

### stap 1
bestel alle componenten uit de bill of materials  

* PCB + assemblage van de onderkant: upload [Gerber_1-PCB_PCB_Linefollower_2026-11-09.zip](../technische%20tekeningen/elektronisch/Gerber_1-PCB_PCB_Linefollower_2026-11-09.zip) bij JLCPCB (2 lagen, 1,6 mm, LeadFree HASL). Zet **PCB Assembly** aan: Economic, **Bottom Side**. BOM en plaatsingsbestand (CPL) exporteer je uit het EasyEDA Pro-project [Linefollower_IDB.eprj2](../technische%20tekeningen/elektronisch/Linefollower_IDB.eprj2).
* Vink bij de assemblage uit (zelf solderen): SW1, SW2, SW3, C6, LED1, R3, U1, CN1, TP1–TP6.
* Bestel de onderdelen voor de bovenkant en de mechanische onderdelen apart (zie [bill of materials, plan A](../bill%20of%20materials/readme.md#plan-a-eigen-pcb)).

### stap 2
soldeer de onderdelen op de **bovenkant** van de PCB (zie [PCB-layout, pagina 1 en 5](../technische%20tekeningen/elektronisch/PCB_1-PCB_PCB_Linefollower_2026-11-13.pdf))  

1. R3 (1 kΩ) en LED1: kathode aan de kant van de pijl in de opdruk.
2. SW2 (start/stop) en SW3 (reset).
3. C6 (470 µF): **+** aan de kant van het +-teken in de opdruk.
4. U1: vrouwelijke header 1×6 voor de BT05.
5. CN1: ISP-header 2×3.
6. SW1: aan/uit-schakelaar, hendel naar achteren.

### stap 3
monteer de motoren  

1. Klik elke N20-motor in een montagebeugel.
2. Schroef de beugels met de bijgeleverde schroeven en moeren vast in de 4 motorgaten (bovenkant, achteraan).
3. Soldeer 2 draden aan elke motor.
4. Soldeer de draden van de ene motor aan **TP3** (AOUT1) en **TP4** (AOUT2), die van de andere motor aan **TP5** (BOUT1) en **TP6** (BOUT2).
5. Duw de wielen op de assen.

### stap 4
monteer de glijder  

1. Draai een moer M3 op de bout M3 × 20 mm (bolle kop).
2. Steek de bout van onder door het M3-gat in de sensorbalk.
3. Zet hem vast met de tweede moer bovenaan.
4. Stel de hoogte zo in dat de sensoren ± 3 mm boven de vloer zitten en de print horizontaal staat.

### stap 5
sluit de batterij aan  

1. Zet SW1 op uit.
2. Soldeer de draden van de batterij rechtstreeks aan **TP1** (V+, rood) en **TP2** (V−, zwart).
3. Bevestig de batterij bovenop de print tussen de motoren (dubbelzijdige tape of een bandje, geen hotglue).
4. Plaats de BT05 in header U1.

### stap 6
brand de bootloader (eenmalig)  

1. Installeer de [Arduino IDE](https://www.arduino.cc/en/software).
2. Upload op een Arduino Nano **File → Examples → 11.ArduinoISP → ArduinoISP**.
3. Verbind de Nano met CN1: D12 → pin 1 (MISO), 5V → pin 2, D13 → pin 3 (SCK), D11 → pin 4 (MOSI), D10 → pin 5 (RESET), GND → pin 6.
4. Kies **Tools → Board → Arduino Leonardo**, **Tools → Programmer → Arduino as ISP**, poort van de Nano.
5. **Tools → Burn Bootloader**.
6. Haal de Nano los.

### stap 7
test de hardware  

1. Installeer in de Arduino IDE niets extra: de bibliotheken zitten in de map van elke sketch.
2. Sluit de lijnvolger aan via micro-USB.
3. Open [code/test/pcb_test/pcb_test.ino](../code/test/pcb_test/pcb_test.ino), bord **Arduino Leonardo**, poort van de lijnvolger, **Upload**.
4. Seriële monitor op 115200 baud, voer de tests uit zoals beschreven in [code/test](../code/test/readme.md).
5. Noteer de volgorde van de sensoren en welke motor links zit en in welke richting hij draait.

### stap 8
upload het finale programma  

1. Open [code/finaal/linefollower/linefollower.ino](../code/finaal/linefollower/linefollower.ino).
2. Pas bovenaan `SENSOR_ORDER`, `MOTOR_A_IS_LEFT`, `INVERT_LEFT` en `INVERT_RIGHT` aan volgens stap 7.
3. Bord **Arduino Leonardo**, poort van de lijnvolger, **Upload**.

### stap 9
kalibreer en rijd  

1. Zet SW1 aan en zet de robot op de lijn.
2. Verbind via bluetooth (zie [gebruiksaanwijzing](../gebruiksaanwijzing/readme.md)).
3. Stuur `cal draai`.
4. Druk op SW2 of stuur `start`.
5. Stel de PID af met `set kp`, `set kd` en `set power` (zie [finaal programma](../code/finaal/readme.md#pid-afstellen)).
