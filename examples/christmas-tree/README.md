# Christmas Tree demo
This program is a demo for the HobbytronicsSerialTFT Arduino library.

The library is available from here:

 * https://github.com/avidinsight/arduino-HobbytronicsSerialTFT
 
You will need the corresponding display from Hobbytronics, which is available from here:

 * http://www.hobbytronics.co.uk/tft-serial-display-18
 
Note that the library assumes you are using the original firmware, or something compatible with it.


## Setup instructions
### Hardware

Connect the ground (GND) and 5V from your Arduino the display board.

You will also need to make the following connections from your Arduino to the display board:

 * Pin 5 -> reset
 * Pin 6 -> tx
 * Pin 7 -> rx

Obviously you will also need to connect your Arduino to your computer to upload the program.

 
### Software
Ensure you have installed the HobbytronicsSerialTFT library.

Open the christmas-tree.ino file in the Arduino IDE (or your preferred IDE). Compile it, and upload it to your Arduino. The christmas tree and an animated "Merry Christmas" message should appear.

If the display seems garbled, try resetting the Arduino.
