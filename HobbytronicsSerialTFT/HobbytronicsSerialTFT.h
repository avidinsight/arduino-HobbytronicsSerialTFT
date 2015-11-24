/*
 * HobbytronicsSerialTFT.h
 * Arduino library for controlling the Hobbytronics Serial TFT 1.8 inch display.
 * 
 * NOTE: This library and the author are not affiliated with or endorsed by
 *  Hobbytronics in any way.
 * 
 * Product website: http://www.hobbytronics.co.uk/tft-serial-display-18
 * Display firmware: https://github.com/hobbytronics/serial_tft_18
 *
 * Library author: Peter R. Bloomfield ( http://peter.avidinsight.uk )
 * License: GNU GPL v3
 */

// TODO: Add a serial "begin" function.
// The constructor cannot begin serial if it's created globally.

#ifndef Arduino_HobbytronicsSerialTFT_h
#define Arduino_HobbytronicsSerialTFT_h

#include "Arduino.h"
#include <SoftwareSerial.h>

// Enumeration of colours supported by the Hobbytronics Serial TFT display.
enum class HSTColour : uint8_t
{
  Black = 0,
  Blue,
  Red,
  Green,
  Cyan,
  Magenta,
  Yellow,
  White
};

// Alias for people who prefer the American spelling. :)
typedef HSTColour HSTColor;

// Enumeration of screen rotations support by the Hobbytronics Serial TFT display.
// The default is Landscape.
enum class HSTRotation : uint8_t
{
  PortraitInverted = 0, // Screen size in pixels: 128x160
  LandscapeInverted,    // Screen size in pixels: 160x128
  Portrait,             // Screen size in pixels: 128x160
  Landscape             // Screen size in pixels: 160x128
};

// Enumeration of font sizes support by the Hobbytronics Serial TFT display.
// The default is Medium.
enum class HSTFontSize : uint8_t
{
  Small = 1, // Screen size in whole characters: 26x16 (landscape), 21x20 (portrait)
  Medium,    // Screen size in whole characters: 13x8  (landscape), 10x10 (portrait)
  Large      // Screen size in whole characters:  8x5  (landscape),  7x6  (portrait)
};

// This class constructs and sends commands to control the Hobbytronics Serial TFT 1.8 inch display.
// Note that this derives from Print, so text can be sent to the display using the usual text
//  printing member functions, such as print("foo").
class HobbytronicsSerialTFT : public Print
{
public:
  //------------------------------------------------------------------------------
  // Construction / destruction.

  // Initialise this object to connect on the given hardware serial port.
  // Note that this will not open the serial connection. You need to call the
  //  begin() function on this class or directly on the serial object itself
  //  before you can communicate with the screen.
  // This does not specify a reset pin. It assumes you will handle that yourself.
  HobbytronicsSerialTFT(HardwareSerial &hwserial);

  // Initialise this object to connect on the given hardware serial port.
  // Note that this will not open the serial connection. You need to call the
  //  begin() function on this class or directly on the serial object itself
  //  before you can communicate with the screen.
  // resetPin specifies which pin is connected to the display's reset line.
  // This will perform a reset immediately.
  // If you want to reset it again later, call reset() after construction.
  HobbytronicsSerialTFT(HardwareSerial &hwserial, uint8_t resetPin);

  // Initialise this object to connect using the given SoftwareSerial object.
  // Note that this will not open the serial connection. You need to call the
  //  begin() function on this class or directly on the serial object itself
  //  before you can communicate with the screen.
  // This does not specify a reset pin. It assumes you will handle that yourself.
  // WARNING: It is essential that the provided swserial object exists for as long
  //  as this object is trying to talk to it. You must manage this yourself.
  HobbytronicsSerialTFT(SoftwareSerial &swserial);

  // Initialise this object to connect using the given SoftwareSerial object.
  // Note that this will not open the serial connection. You need to call the
  //  begin() function on this class or directly on the serial object itself
  //  before you can communicate with the screen.
  // resetPin specifies which pin is connected to the display's reset line.
  // This will perform a reset immediately.
  // If you want to reset it again later, call reset() after construction.
  // WARNING: It is essential that the provided swserial object exists for as long
  //  as this object is trying to talk to it. You must manage this yourself.
  HobbytronicsSerialTFT(SoftwareSerial &swserial, uint8_t resetPin);

  // Initialise this object to connect via software serial on the specified pins.
  // Note that this will not open the serial connection. You need to call the
  //  begin() function on this class before you can communicate with the screen.
  // Also note that tx corresponds to the pin which is designated tx on _this_ Arduino.
  // It should be connected to the display's rx line, and vice versa for rx->tx.
  // This does not specify a reset pin. It assumes you will handle that yourself.
  HobbytronicsSerialTFT(uint8_t rx, uint8_t tx);

  // Initialise this object to connect via software serial on the specified pins.
  // Note that this will not open the serial connection. You need to call the
  //  begin() function on this class before you can communicate with the screen.
  // Also note that tx corresponds to the pin which is designated tx on _this_ Arduino.
  // It should be connected to the display's rx line, and vice versa for rx->tx.
  // resetPin specifies which pin is connected to the display's reset line.
  // This will perform a reset immediately.
  // If you want to reset it again later, call reset() after construction.
  HobbytronicsSerialTFT(uint8_t rx, uint8_t tx, uint8_t resetPin);

  // Destructor.
  ~HobbytronicsSerialTFT();

  //------------------------------------------------------------------------------
  // Disallowed operations.

  // Default construction is not allowed.
  // This is to ensure the object cannot exist
  HobbytronicsSerialTFT() = delete;

  // Copy construction is not permitted.
  HobbytronicsSerialTFT(const HobbytronicsSerialTFT &) = delete;

  // Copy assignment is not permitted.
  void operator = (const HobbytronicsSerialTFT &) = delete;


  //------------------------------------------------------------------------------
  // Hardware/connection control.
  
  // Reset the display using the reset line.
  // This brings the reset pin LOW and then HIGH again. It does not close the
  //  serial connection.
  // NOTE: This will only work if resetPin was specified in the constructor.
  // It will do nothing otherwise.
  void reset();
  
  // Open the serial connection with the specified baud rate.
  // If you provided an external HardwareSerial or SoftwareSerial object in the
  //  constructor then this may not be necessary. You can open the serial
  //  using the original object directly instead.
  // However, if you only provided pin numbers in the constructor then you must
  //  call this before you can communicate with the display.
  void begin(unsigned long speed = 9600);
  

  //------------------------------------------------------------------------------
  // Core commands.
  // These functions map directly onto the display's firmware functionality.

  // Clear the screen.
  void clearScreen();

  // Set the colour which is used for text and drawing.
  // Example usage: setForegroundColour(HSTColour::Blue)
  void setForegroundColour(const HSTColour col);

  // Set the color which is used for text and drawing.
  // Example usage: setForegroundColor(HSTColor::Blue)
  // This is really just an alias of setForegroundColour for people who prefer American spelling. :)
  void setForegroundColor(const HSTColor col) { setForegroundColour(col); }

  // Set the colour of the background.
  // If clear is true then the screen is immediately cleared to this colour too.
  // Example usage: setBackgroundColour(HSTColour::Black, true)
  void setBackgroundColour(const HSTColour col, const bool clear);

  // Set the color of the background.
  // If clear is true then the screen is immediately cleared to this colour too.
  // Example usage: setBackgroundColor(HSTColor::Black, true)
  // This is really just an alias of setBackgroundColor for people who prefer American spelling. :)
  void setBackgroundColor(const HSTColor col, const bool clear) { setBackgroundColour(col, clear); }

  /// Set the orientation of the screen.
  /// Example usage: setScreenRotation(HSTRotation::Portrait)
  void setScreenRotation(const HSTRotation rtn);

  /// Set the font size for all subsequent text operations.
  /// Example usage: setFontSize(HSTFontSize::Small)
  void setFontSize(const HSTFontSize size);

  /// Move the text cursor to the beginning of the current line of text.
  void gotoTextLineStart();

  /// Move the text cursor to the specified character position.
  /// The meaning of character position depends on the current font size.
  /// Example usage: gotoCharacterPosition(10, 4)
  void gotoCharacterPosition(uint8_t x, uint8_t y);

  /// Move the text cursor to the specified pixel position.
  /// Screen dimensions are 159x127 pixels (landscape), or 127x159 (portrait).
  /// Example usage: gotoPixelPosition(52, 27)
  void gotoPixelPosition(uint8_t x, uint8_t y);

  /// Draw a line from point x1,y1 to point x2,y2 (specified in pixels).
  /// Screen dimensions are 159x127 pixels (landscape), or 127x159 (portrait).
  /// This draws using the current foreground colour.
  /// Example usage: drawLine(12,0,100,90)
  void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

  /// Draw a box outline with one corner at point x1,y1 and the other at point x2,y2.
  /// Screen dimensions are 159x127 pixels (landscape), or 127x159 (portrait).
  /// This draws using the current foreground colour.
  /// Example usage: drawBox(51,5,72,20)
  void drawBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

  /// Draw a filled box with one corner at point x1,y1 and the other at point x2,y2.
  /// Screen dimensions are 159x127 pixels (landscape), or 127x159 (portrait).
  /// This draws using the current foreground colour.
  /// Example usage: drawFilledBox(51,5,72,20)
  void drawFilledBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

  /// Draw a circle outline, with the centre at x,y and the specified radius.
  /// Screen dimensions are 159x127 pixels (landscape), or 127x159 (portrait).
  /// This draws using the current foreground colour.
  /// Example usage: drawCircle(79,63,40)
  void drawCircle(uint8_t x, uint8_t y, uint8_t radius);

  /// Draw a filled circle, with the centre at x,y and the specified radius.
  /// Screen dimensions are 159x127 pixels (landscape), or 127x159 (portrait).
  /// This draws using the current foreground colour.
  /// Example usage: drawFilledCircle(79,63,40)
  void drawFilledCircle(uint8_t x, uint8_t y, uint8_t radius);

  /// Draw a bitmap file at the specified coordinates.
  /// The bitmap is read from a micro SD card inserted into the display's slot.
  /// Example usage: drawBitmap(10, 13, "logo.bmp")
  void drawBitmap(uint8_t x, uint8_t y, String filename);

  /// Set the brightness of the display's backlight.
  /// Range is 0 (off) to 100 (maximum).
  void setBacklightBrightness(uint8_t level);


  //------------------------------------------------------------------------------
  // Extended commands.
  // These functions are provided for convenience. They don't add any new
  //  functionality, but are wrappers around the core commands as used above.

  /// Draw a pixel at the specified location.
  /// Screen dimensions are 159x127 pixels (landscape), or 127x159 (portrait).
  /// This draws using the current foreground colour.
  /// Caution: Drawing pixels one-at-a-time is slow!
  /// Example usage: drawPixel(64, 89)
  void drawPixel(uint8_t x, uint8_t y);

  /// Draw a horizontal line across the whole width of the display at the specified y position.
  void drawHorizontalLine(uint8_t y);

  /// Draw a vertical line across the whole height of the display at the specified x position.
  void drawVerticalLine(uint8_t x);

  // TODO: draw/fill triangle


  //------------------------------------------------------------------------------
  // Text printing.

  // Write a single character to the display.
  // This allows all the usual print() and println() commands to work.
  size_t write(uint8_t) override;


private:
  //------------------------------------------------------------------------------
  // Internal operations.

  // Setup the reset line on the specified pin.
  // This is called by the constructor.
  void setupReset(uint8_t pin);

  // Send a command without any data.
  void sendCommand(uint8_t cmd);

  // Send a command with 1 parameter byte.
  void sendCommand(uint8_t cmd, uint8_t par1);

  // Send a command with 2 parameter bytes.
  void sendCommand(uint8_t cmd, uint8_t par1, uint8_t par2);

  // Send a command with 3 parameter bytes.
  void sendCommand(uint8_t cmd, uint8_t par1, uint8_t par2, uint8_t par3);

  // Send a command with 4 parameter bytes.
  void sendCommand(uint8_t cmd, uint8_t par1, uint8_t par2, uint8_t par3, uint8_t par4);
  
  
  //------------------------------------------------------------------------------
  // Internal declarations.
  
  // Enumeration of the serial modes we can have.
  enum class SerialMode
  {
      Hardware,         // Using Hardware Serial. The object is always provided externally in this case.
      SoftwareExternal, // Using Software Serial. The object was provided externally.
      SoftwareInternal  // Using Software Serial. The object was created and is owned by this class.
  };
  
  
  //------------------------------------------------------------------------------
  // Data.
  
  // Specifies what kind of serial object is stored in m_output.
  // This also indicates whether or not we own the object.
  // If this is SoftwareInternal then we need to delete the target m_output when
  //  this object is destroyed.
  SerialMode m_serialMode;
  
  // Pointer to the object we're sending serial commands/data to.
  // This could be pointing to a HardwareSerial or SoftwareSerial object provided
  //  externally, or it could simply be a copy of m_internalSerial.
  // The actual type can be inferred from m_serialMode.
  // We assume that it is always valid after construction of this object.
  Stream * m_output;

  // The reset pin, if one was provided at construction.
  uint8_t m_resetPin;

  // Indicates if a reset pin was provided at construction.
  bool m_hasResetPin;
};

#endif //Arduino_HobbytronicsSerialTFT_h

