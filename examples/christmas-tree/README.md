# Christmas Tree demo
This program is a demo for the [HobbytronicsSerialTFT Arduino library][1].

You can see a video of this program in action here:

 * http://www.youtube.com/watch?v=76kL5rqrEVo
 
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

Open the christmas-tree.ino file in the Arduino IDE (or your preferred IDE). Compile it, and upload it to your Arduino. It will take a few seconds to startup. The christmas tree should soon appear with the "Merry Christmas" text below it.

If the display seems garbled, try resetting the Arduino.


[1]: https://github.com/avidinsight/arduino-HobbytronicsSerialTFT
