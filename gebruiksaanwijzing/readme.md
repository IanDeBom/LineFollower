# Gebruiksaanwijzing

### opladen / vervangen batterijen
* De robot werkt op een **2S Li-Ion batterij van 7,4 V** (2000 mAh) met JST-stekker.
* **Opladen:** zet de robot uit met de aan/uit-schakelaar (SW1, achteraan), trek de JST-stekker van de batterij los en sluit hem aan op de meegeleverde **USB-oplaadkabel**.
* **Vervangen:** stekker lostrekken, batterij losmaken (geen hotglue, enkel tape of bandje) en een nieuwe 2S-batterij aansluiten. Let op de polariteit: rood = + (TP1, V+), zwart = − (TP2, V−).
* Volgeladen is de batterij 8,4 V. Onder **6,2 V** stopt de robot vanzelf (`gestopt: batterij leeg`): dan opladen.
* De batterijspanning zie je met het commando `debug`.

### draadloze communicatie
#### verbinding maken
**Met een iPhone (bluetooth, BT05):**
1. Zet de robot aan (SW1).
2. Installeer de app **LightBlue** en tik op **BT05**. De led op de BT05 brandt nu continu = verbonden.
3. Tik op de characteristic **FFE1** en zet rechtsboven het formaat op **UTF-8 String**.
4. Tik op **Listen for notifications** om de antwoorden van de robot te zien.
5. Tik op **Write new value** om een commando te sturen (enter is niet nodig).

**Met een laptop (USB):**
1. Sluit de robot aan met een micro-USB-kabel.
2. Open de seriële monitor van de Arduino IDE op **115200 baud**.

Het antwoord komt altijd terug op de verbinding waarop het commando verstuurd werd.

#### commando's
debug  
start  
stop  
set cycle [µs]  
set power [0..255]  
set kp [0..]  
set ki [0..]  
set kd [0..]  
set threshold [0..1000]  
cal  
cal draai  
sensors  
stream [ms] (0 = uit)  
default  
reset  
help  

|commando|wat doet het|
|--------|------------|
|`debug`|alle parameters, kalibratie, batterijspanning en rekentijd tonen|
|`start` / `stop`|rijden / stoppen|
|`set cycle 5000`|cyclustijd in µs (1000 … 50000)|
|`set power 100`|basissnelheid van de motoren (0 … 255)|
|`set kp 40` / `set ki 0` / `set kd 0.5`|PID-parameters|
|`set threshold 300`|vanaf welke waarde (0 … 1000) een sensor de lijn ziet|
|`cal`|kalibratie, sensoren met de hand over wit en zwart bewegen (5 s)|
|`cal draai`|kalibratie, de robot draait zelf over de lijn (5 s)|
|`sensors`|huidige sensorwaarden en positie van de lijn|
|`stream 200`|elke 200 ms de status sturen, `stream 0` = uit|
|`default`|parameters terug naar de standaardwaarden (kalibratie blijft)|
|`reset`|microcontroller herstarten|

Elke `set` en elke geslaagde kalibratie wordt bewaard in het EEPROM: na het uitschakelen blijven de instellingen behouden.

### kalibratie
De sensoren zien wit en zwart niet allemaal even sterk. Bij de kalibratie onthoudt de robot per sensor de waarde op wit en op zwart, zodat alle sensoren daarna gelijk reageren (0 = wit, 1000 = zwart).

1. Zet de robot met de sensoren **boven de zwarte lijn**, op het parcours.
2. Typ `cal draai`: de robot draait 5 s heen en weer over de lijn.
   Of typ `cal` en beweeg de robot zelf 5 s zodat **elke sensor** over wit en zwart gaat.
3. Na 5 s: `kalibratie klaar en opgeslagen`.
   Krijg je `kalibratie mislukt`, dan heeft niet elke sensor wit en zwart gezien: opnieuw proberen.
4. Controleer met `debug` (cal zwart / cal wit) of `sensors`.

Opnieuw kalibreren is nodig bij een ander parcours, andere verlichting of als de hoogte van de glijder veranderd is.
Zonder geldige kalibratie start de robot niet (`niet gestart: eerst kalibreren (cal)`).

### settings
De robot rijdt stabiel met volgende parameters:  

|parameter|waarde|
|---------|------|
|cycle|nog te bepalen na testen (startwaarde 5000)|
|power|nog te bepalen na testen (startwaarde 100)|
|kp|nog te bepalen na testen (startwaarde 40)|
|ki|nog te bepalen na testen (startwaarde 0)|
|kd|nog te bepalen na testen (startwaarde 0,5)|
|threshold|nog te bepalen na testen (startwaarde 300)|

Hoe je de PID afstelt staat in de uitleg van het [finale programma](../code/finaal/readme.md#pid-afstellen).

### start/stop button
* **SW2** (opdruk op de print), in het midden van de print naast de bluetooth-header: **start/stop**. Eén druk = starten, nog een druk = stoppen. Stoppen gebeurt onmiddellijk (interrupt), ook midden in een bocht.
* **SW3**, naast SW2: **reset** van de microcontroller.
* **SW1**, achteraan: **aan/uit**-schakelaar van de batterij.
* Starten en stoppen kan ook met de commando's `start` en `stop`.
