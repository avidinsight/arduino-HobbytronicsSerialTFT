/*
 * Christmas Tree demo for Arduino HobbytronicsSerialTFT library.
 *
 * License: GNU GPL v3
 * Author: Peter R. Bloomfield
 * Web: http://avidinsight.uk
 *
 * See the accompanying README for further information, including setup instructions.
 */

#include <HobbytronicsSerialTFT.h>

// Specify which pins are connected to which point on the display board.
const int tft_reset = 5;
const int tft_tx = 6;
const int tft_rx = 7;

// Construct our serial TFT object.
// It will construct its own SoftwareSerial object internally.
HobbytronicsSerialTFT tft(tft_tx, tft_rx, tft_reset);

// The colours which will be used for the animated text.
const HSTColour col[] = {
  HSTColour::Green,
  HSTColour::White,
  HSTColour::Red,
  HSTColour::White
};
// Store the number of colours in the array above.
const uint8_t numCols = sizeof(col) / sizeof(col[0]);
// This will keep track of the current colour at any given time.
uint8_t colIndex = 0;

void setup()
{
  // Open the serial connection to the display.
  tft.begin();
  
  // The display seems to need as much as 4 seconds to be fully online
  //  and ready to receive commands.
  delay(4000);
  
  // General setup.
  tft.setBackgroundColour(HSTColour::Black, true);
  tft.setScreenRotation(HSTRotation::Landscape);
  tft.setFontSize(HSTFontSize::Medium);

  // Draw a very blocky christmas tree.
  tft.setForegroundColour(HSTColour::Green);
  tft.drawFilledBox(75, 1, 85, 21);
  tft.drawFilledBox(65, 21, 95, 41);
  tft.drawFilledBox(55, 41, 105, 61);
  tft.setForegroundColour(HSTColour::Red);
  tft.drawFilledBox(73, 61, 87, 77);
  tft.flush();
}

void loop()
{
  // Display the word "Merry" under the tree using the current colour
  //  in the animation sequence.
  tft.setForegroundColour(col[colIndex]);
  tft.gotoCharacterPosition(4, 5);
  tft.print("Merry");

  // Display the word "Christmas" under that using the next colour
  //  in the animation sequence.
  tft.setForegroundColour(col[(colIndex + 1) % numCols]);
  tft.gotoCharacterPosition(2, 6);
  tft.print("Christmas");

  // Advance the animation sequence by one colour.
  colIndex = (colIndex + 1) % numCols;

  // Wait for a while before advancing to the next animation frame.
  // (This is a very lazy way to do this!)
  delay(650);
}
