/*
 * Christmas Tree demo for Arduino HobbytronicsSerialTFT library.
 * Example of basic animation showing a christmas tree with lights,
 *  snow flakes, and the text "Merry Christmas".
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

// The colours which will be used for the tree decorations.
const HSTColour treeCols[] = {
  HSTColour::Red,
  HSTColour::Yellow,
  HSTColour::Blue,
  HSTColour::Cyan,
  HSTColour::Magenta
};

// The colours which will be used for the animated text.
const HSTColour textCols[] = {
  HSTColour::Green,
  HSTColour::White,
  HSTColour::Red,
  HSTColour::White
};

// Store the number of colours in the arrays above.
const uint8_t numTreeCols = sizeof(treeCols) / sizeof(treeCols[0]);
const uint8_t numTextCols = sizeof(textCols) / sizeof(textCols[0]);

// This will keep track of the current colour at any given time in each animation.
uint8_t treeColIndex = 0;
uint8_t textColIndex = 0;

// Get the numbered colour from the tree colour sequence.
HSTColour getTreeCol(uint8_t offset)
{
  return treeCols[(treeColIndex + offset) % numTreeCols];
}

// Get the numbered colour from the text colour sequence.
HSTColour getTextCol(uint8_t offset)
{
  return textCols[(textColIndex + offset) % numTextCols];
}

// Array of vectors storing the positions of snowflakes.
// Within each element is a sub-array containg X,Y,S.
// X,Y is the position, S is the downward speed in pixels per frame.
const uint8_t numSnowflakes = 40;
uint8_t snowflakes[numSnowflakes][3] = {0};

// This stores the positions of the snowflakes on the previous frame.
// This is used to blank them out.
// Note that only X,Y positions are stored, not speed.
uint8_t oldSnowflakes[numSnowflakes][2] = {0};

// Initialise the snowflakes with random positions.
void initSnowflakes()
{
  for (uint8_t i = 0; i < numSnowflakes; ++i) {
    snowflakes[i][0] = static_cast<uint8_t>(random(0, 160)); // x position
    snowflakes[i][1] = static_cast<uint8_t>(random(0, 128)); // y position
    snowflakes[i][2] = static_cast<uint8_t>(random(1, 4)); // downward speed in pixels per frame
  }
}

// Advance the animations by one frame.
void advanceAnim()
{
  // Cycle the colours in the tree and text animations.
  treeColIndex = (treeColIndex + 1) % numTreeCols;
  textColIndex = (textColIndex + 1) % numTextCols;

  // Advance the snowflakes.
  for (uint8_t i = 0; i < numSnowflakes; ++i) {
    oldSnowflakes[i][0] = snowflakes[i][0]; // x
    oldSnowflakes[i][1] = snowflakes[i][1]; // y
    
    // Move this snowflake down according to its stored speed.
    snowflakes[i][1] += snowflakes[i][2];
    
    // Has it gone off the bottom?
    if (snowflakes[i][1] > 128) {
      // Yes. Reposition it at the top with a random position and speed.
      snowflakes[i][0] = static_cast<uint8_t>(random(0, 160)); // x position
      snowflakes[i][1] = 0; // y position
      snowflakes[i][2] = static_cast<uint8_t>(random(1, 4)); // downward speed in pixels per frame
      
    } else {
      // Sometimes move it sideways slightly.
      switch (random(4))
      {
        case 0:
          // Move left, but don't let it go off the side.
          snowflakes[i][0] = max(0, snowflakes[i][0] - 1);
          break;

        case 1:
          // Move right, but don't let it go off the side.
          snowflakes[i][0] = min(159, snowflakes[i][0] + 1);
          break;

        default:
          // No horizontal movement
          break;
      }
    }
  }
}


void setup()
{
  // Open the serial connection to the display.
  tft.begin();

  initSnowflakes();
  
  // The display seems to need as much as 4 seconds to be fully online
  //  and ready to receive commands.
  delay(4000);
  
  // General setup.
  tft.setBackgroundColour(HSTColour::Black);
  tft.setScreenRotation(HSTRotation::Landscape);
  tft.setFontSize(HSTFontSize::Medium);
  tft.flush();
  tft.clearScreen();
}

void loop()
{
  // Draw the new snowflakes.
  tft.setLineColour(HSTColour::White);
  for (uint8_t i = 0; i < numSnowflakes; ++i) {
    tft.drawPixel(snowflakes[i][0], snowflakes[i][1]);
  }
  tft.flush();

  // Erase the snowflakes from the previous frame.
  tft.setLineColour(HSTColour::Black);
  for (uint8_t i = 0; i < numSnowflakes; ++i) {
    tft.drawPixel(oldSnowflakes[i][0], oldSnowflakes[i][1]);
  }
  tft.flush();
  
  // Draw a very blocky christmas tree.
  tft.setFillColour(HSTColour::Green);
  tft.drawBox(75, 6, 85, 26, HSTShapeStyle::Fill);
  tft.drawBox(65, 26, 95, 46, HSTShapeStyle::Fill);
  tft.drawBox(55, 46, 105, 66, HSTShapeStyle::Fill);
  tft.setFillColour(HSTColour::Red);
  tft.drawBox(73, 66, 87, 82, HSTShapeStyle::Fill);
  tft.flush();

  // Draw some lights on it.
  tft.setFillColour(getTreeCol(0));
  tft.drawCircle(79, 45, 3, HSTShapeStyle::FilledOutline);
  tft.setFillColour(getTreeCol(1));
  tft.drawCircle(86, 30, 3, HSTShapeStyle::FilledOutline);
  tft.setFillColour(getTreeCol(2));
  tft.drawCircle(63, 53, 3, HSTShapeStyle::FilledOutline);
  tft.setFillColour(getTreeCol(3));
  tft.drawCircle(96, 56, 3, HSTShapeStyle::FilledOutline);
  tft.setFillColour(getTreeCol(4));
  tft.drawCircle(69, 32, 3, HSTShapeStyle::FilledOutline);
  tft.flush();
  
  // Display the word "Merry" under the tree using the current colour
  //  in the animation sequence.
  tft.setLineColour(getTextCol(0));
  tft.gotoPixelPosition(50, 88);
  tft.print("Merry");
  tft.flush();

  // Display the word "Christmas" under that using the next colour
  //  in the animation sequence.
  tft.setLineColour(getTextCol(1));
  tft.gotoPixelPosition(27, 104);
  tft.print("Christmas");
  tft.flush();

  // Advance the animation sequences by one frame.
  advanceAnim();

  // Wait for a while before advancing to the next animation frame.
  // (This is a very lazy way to do this!)
  delay(100);
}
