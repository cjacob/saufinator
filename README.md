# saufinator

## install

LED-Matrix 8x8 hat die LED Reihenfolge
7,6,5,4,3,2,1,0,
8,9,10,12,13,14,15,
……,18,17,16
also etwas unüblich vin rechts nach links und das sogenannte „Serpentinen-Layout“.

„Besser“ wäre:
0,1,2,3,4,5,6,7
8,9,10,11,12,13,14,15,
16,17,18….
Aber so ist es halt...

Die LEDs sind WS2812B.

Das Board ist ein NodeMCU. Es kommt eigentlich mit einer anderen Firmware, die mit Lua programmiert wird. Ich habe es aber mit Arduino ersetzt. Dadurch ist der Chip ziemlich kompatibel mit Arduino und kann mit der IDE von Arduino.cc programmiert werden.

Daher:
1. Arduino Software herunterladen und installieren
https://www.arduino.cc/en/Main/Software

2. esp8266-Arduino installieren/einfügen
https://github.com/esp8266/Arduino

Start Arduino and open Preferences window.
Enter http://arduino.esp8266.com/stable/package_esp8266com_index.json into Additional Board Manager URLs field. You can add multiple URLs, separating them with commas.
Open Boards Manager from Tools > Board menu and install esp8266 platform (and don't forget to select your ESP8266 board from Tools > Board menu after installation).

3. Fast herunterladen, um die LEDs anzusteuern
Nur die aktuelle Version von Github enthält die Bibliothek für ESP8266
Download über https://github.com/FastLED/FastLED
Mitte oben rechts „Download Zip“ herunterladen und entpacken
Ordner in den Arduino-Ordner „libraries“ entdecken und in „FastLED“ umbenennen

4. Kontrollieren, ob alles da ist:
Sketch -> Bibliothek einbinden -> Bibliotheken verwalten schauen, ob Adafruit ESP8266 (bei mir 1.0.0) und FastLED (sollte 3.1.1 sein, 3.1 hat noch nicht ESP dabei)

5. Einstellungen für Nodemcu unter Werkzeuge:
Board: „NodeMCU 1. …“
Upload using: serial
CPU 80 Mhz
Flash size 4m
upload speed 115200
port: usb-uart (kann ich gerne nicht sehen, taucht auf, wenn du den Baustein angesteckt hast
programmer weiß ich nicht genau: USBTinyISP geht bei mir

6. Den Sketch im Anhang entdecken und mittels „Datei öffnen“ laden

7. Mit dem Pfeill nach rechts Programm auf den ESP8266 hochladen.

## Verkabelung

V+ am LED-Panel an VIN am NodeMCU
V- am LED-Panel an GND am NodeMCU
DATA_IN am LED-Panel an D6 am NodeMCU
DATA_OUT nicht verbinden

Die Stromversorgung über NodeMCU reicht nur knapp aus. In anderen Anleitungen gab es deutliche Spannungsabfälle. Für den Saufinator, wo nie alle LEDs gleichzeitig blinken sollte es ausreichen. Aber ohne Garantie...
