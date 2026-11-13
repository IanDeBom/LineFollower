# batterij 2S Li-Ion 7,4 V 2000 mAh

Van deze batterij bestaat geen datasheet. De specificaties komen uit de **productomschrijving bij de aankoop**:
"7,4 V 2000 mAh Li-Ion-batterij 15 C 2S met JST-stekker en USB-oplaadkabel".

|parameter|waarde|
|---------|------|
|type|Li-Ion, 2 cellen in serie (2S)|
|nominale spanning|7,4 V (2 × 3,7 V)|
|capaciteit|2000 mAh|
|max. ontlaadstroom|15C = 15 × 2 A = 30 A|
|aansluiting|JST-stekker|
|laden|meegeleverde USB-oplaadkabel|

Spanningen van een 2S Li-Ion batterij (2 cellen van 3,0 – 4,2 V):

|toestand|spanning|
|--------|--------|
|volgeladen|8,4 V|
|nominaal|7,4 V|
|leeg (niet verder ontladen)|± 6,0 V|

### Opmerkingen voor de lijnvolger
* De robot gebruikt ± 0,1 – 3 A (elektronica + 2 motoren): ruim onder de 30 A van de batterij.
* Het finale programma meet de batterijspanning en stopt de robot onder **6,2 V**, zodat de batterij niet te diep ontladen wordt.
