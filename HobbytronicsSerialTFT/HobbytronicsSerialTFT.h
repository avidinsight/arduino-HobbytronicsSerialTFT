/*
 * HobbytronicsSerialTFT.h
 * Arduino library for controlling the Hobbytronics Serial TFT 1.8 inch display.
 * 
 * NOTE: This library and the author are not affiliated with or endorsed by
 *    Hobbytronics in any way.
 * 
 * Product website: http://www.hobbytronics.co.uk/tft-serial-display-18
 * Display firmware: https://github.com/hobbytronics/serial_tft_18
 *
 * Library author: Peter R. Bloomfield ( http://peter.avidinsight.uk )
 * License: GNU GPL v3
 */

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

// Enumeration of drawing styles for shapes.
enum class HSTShapeStyle : uint8_t
{
    Outline,        // Outline only, using outline colour.
    Fill,           // Fill only, using fill colour.
    FilledOutline   // Outline and fill, using their respective colours.
};


// This class constructs and sends commands to control the Hobbytronics Serial TFT 1.8 inch display.
// Note that this derives from Print, so text can be sent to the display using the usual text
//    printing member functions, such as print("foo").
class HobbytronicsSerialTFT : public Print
{
public:
    //------------------------------------------------------------------------------
    // Construction / destruction.

    // Initialise this object to connect on the given hardware serial port.
    // Note that this will not open the serial connection. You need to call the
    //    begin() function on this class or directly on the serial object itself
    //    before you can communicate with the screen.
    // This does not specify a reset pin. It assumes you will handle that yourself.
    HobbytronicsSerialTFT(HardwareSerial &hwserial);

    // Initialise this object to connect on the given hardware serial port.
    // Note that this will not open the serial connection. You need to call the
    //    begin() function on this class or directly on the serial object itself
    //    before you can communicate with the screen.
    // resetPin specifies which pin is connected to the display's reset line.
    // This will ensure the reset pin is held high, but it will not actually cause
    //  a reset unless you call reset().
    HobbytronicsSerialTFT(HardwareSerial &hwserial, uint8_t resetPin);

    // Initialise this object to connect using the given SoftwareSerial object.
    // Note that this will not open the serial connection. You need to call the
    //    begin() function on this class or directly on the serial object itself
    //    before you can communicate with the screen.
    // This does not specify a reset pin. It assumes you will handle that yourself.
    // WARNING: It is essential that the provided swserial object exists for as long
    //    as this object is trying to talk to it. You must manage this yourself.
    HobbytronicsSerialTFT(SoftwareSerial &swserial);

    // Initialise this object to connect using the given SoftwareSerial object.
    // Note that this will not open the serial connection. You need to call the
    //    begin() function on this class or directly on the serial object itself
    //    before you can communicate with the screen.
    // resetPin specifies which pin is connected to the display's reset line.
    // This will ensure the reset pin is held high, but it will not actually cause
    //  a reset unless you call reset().
    // WARNING: It is essential that the provided swserial object exists for as long
    //    as this object is trying to talk to it. You must manage this yourself.
    HobbytronicsSerialTFT(SoftwareSerial &swserial, uint8_t resetPin);

    // Initialise this object to connect via software serial on the specified pins.
    // Note that this will not open the serial connection. You need to call the
    //    begin() function on this class before you can communicate with the screen.
    // Also note that tx corresponds to the pin which is designated tx on _this_ Arduino.
    // It should be connected to the display's rx line, and vice versa for rx->tx.
    // This does not specify a reset pin. It assumes you will handle that yourself.
    HobbytronicsSerialTFT(uint8_t rx, uint8_t tx);

    // Initialise this object to connect via software serial on the specified pins.
    // Note that this will not open the serial connection. You need to call the
    //    begin() function on this class before you can communicate with the screen.
    // Also note that tx corresponds to the pin which is designated tx on _this_ Arduino.
    // It should be connected to the display's rx line, and vice versa for rx->tx.
    // This will ensure the reset pin is held high, but it will not actually cause
    //  a reset unless you call reset().
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
    
    // Reset the display using the reset pin, if one was specified in the constructor.
    // WARNING: After a reset, you may need to wait up to 4 seconds before the
    //  display will respond to any commands.
    // Note also that the reset doesn't seem to clear the display.
    // In fact it's probably best not to use this.
    void reset();
    
    // Open the serial connection to the display.
    // If no speed (baud rate) is specified, this uses 9600 by default.
    // If you provided an external HardwareSerial or SoftwareSerial object in the
    //    constructor then this may not be necessary. You can open the serial
    //    using the original object directly instead.
    // However, if you only provided pin numbers in the constructor then you must
    //    call this before you can communicate with the display.
    void begin(unsigned long speed = 9600);
    
    // Wait until any pending outgoing data has finished.
    // This is useful for ensuring drawing can complete before continuing.
    void flush();
    

    //------------------------------------------------------------------------------
    // Colour functions.
    
    /// Set the background colour which will be used in subsequent drawing.
    /// It is used when clearing the screen, and as the background for text.
    /// This doesn't send the colour to the display until it's needed.
    void setBackgroundColour(const HSTColour col);
    
    /// Same as setBackgroundColor(), but with American spelling.
    void setBackgroundColor(const HSTColor col) { setBackgroundColour(col); }
    
    
    /// Set the colour which will be used in line drawing and text.
    /// This doesn't send the colour to the display until it's needed.
    void setLineColour(const HSTColour col);
    
    /// Same as setLineColour(), but with American spelling.
    void setLineColor(const HSTColor col) { setLineColour(col); }
    
    
    /// Set the colour which will be used for filling shapes.
    /// This doesn't send the colour to the display until it's needed.
    void setFillColour(const HSTColour col);
    
    /// Same as setFillColour() but with American spelling.
    void setFillColor(const HSTColor col) { setFillColour(col); }
    
    
    //------------------------------------------------------------------------------
    // General display functions.
    
    /// Set the orientation of the screen for subsequent drawing operations.
    /// This doesn't affect the content currently on the screen.
    /// Example usage: setScreenRotation(HSTRotation::Portrait)
    void setScreenRotation(const HSTRotation rtn);
    
    /// Set the brightness of the display's backlight.
    /// Range is 0 (off) to 100 (maximum).
    void setBacklightBrightness(uint8_t level);

    // Clear the screen.
    // This uses the currently set background colour. Default is black.
    void clearScreen();
    
    /// Draw a bitmap file at the specified coordinates.
    /// The bitmap is read from a micro SD card inserted into the display's slot.
    /// Example usage: drawBitmap(10, 13, "logo.bmp")
    void drawBitmap(uint8_t x, uint8_t y, String filename);
    
    
    //------------------------------------------------------------------------------
    // Shape drawing functions.
    
    /// Draw a pixel at the specified location.
    /// Screen dimensions are 160x128 pixels (landscape), or 128x160 (portrait).
    /// This draws using the current foreground colour.
    /// Caution: Drawing pixels one-at-a-time is slow!
    /// Example usage: drawPixel(64, 89)
    void drawPixel(uint8_t x, uint8_t y);
    
    /// Draw a horizontal line across the whole width of the display at the specified y position.
    void drawHorizontalLine(uint8_t y);
    
    /// Draw a horizontal line from x1,y to x2,y.
    void drawHorizontalLine(uint8_t x1, uint8_t y, uint8_t x2);

    /// Draw a vertical line across the whole height of the display at the specified x position.
    void drawVerticalLine(uint8_t x);
    
    /// Draw a vertical line from x,y1 to x,y2.
    void drawVerticalLine(uint8_t x, uint8_t y1, uint8_t y2);

    /// Draw a line from point x1,y1 to point x2,y2.
    /// This draws using the current line colour.
    /// Example usage: drawLine(12,0,100,90)
    void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

    /// Draw a box with one corner at point x1,y1 and the other at point x2,y2.
    /// It is drawn using the current line and fill colours, as applicable.
    /// Example usage: drawBox(51, 5, 72, 20, HSTShapeStyle::Fill)
    void drawBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, HSTShapeStyle style = HSTShapeStyle::Outline);

    /// Draw a circle outline, with the centre at x,y and the specified radius.
    /// It is drawn using the current line and fill colours, as applicable.
    /// Example usage: drawCircle(79, 63, 40, HSTShapeStyle::Outline)
    void drawCircle(uint8_t x, uint8_t y, uint8_t radius, HSTShapeStyle style = HSTShapeStyle::Outline);

    /// Draw a triangle between the three given points. Order of points doesn't matter.
    /// It is drawn using the current line and fill colours, as applicable.
    /// Example usage: drawTriangle(5, 5, 5, 80, 80, 80, HSTShapeStyle::FilledOutline)
    void drawTriangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, HSTShapeStyle style = HSTShapeStyle::Outline);


    //------------------------------------------------------------------------------
    // Text functions.
    
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
    /// Screen dimensions are 160x128 pixels (landscape), or 128x160 (portrait).
    /// Example usage: gotoPixelPosition(52, 27)
    void gotoPixelPosition(uint8_t x, uint8_t y);

    // Write a single character to the display.
    // Note that you can call the print() and println() functions derived from the
    //  Print class to draw strings and numbers.
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
    
    // Set the display's background colour to the specified value.
    // This won't do anything if display already has the specified background colour.
    void sendBackgroundColour(HSTColour col);
    
    // Set the display's foreground colour to the specified value.
    // This won't do anything if display already has the specified foreground colour.
    void sendForegroundColour(HSTColour col);
    
    // Update the display's background colour to match our stored background colour.
    // This only sends the command if the colour has changed.
    void applyBackgroundColour();
    
    // Update the display's foreground colour to match our stored line colour.
    // This only sends the command if the colour has changed.
    void applyLineColour();
    
    // Update the display's foreground colour to match our stored fill colour.
    // This only sends the command if the colour has changed.
    void applyFillColour();
    
    
    //------------------------------------------------------------------------------
    // Internal declarations.
    
    // Enumeration of the serial modes we can have.
    enum class SerialMode
    {
            Hardware,                 // Using Hardware Serial. The object is always provided externally in this case.
            SoftwareExternal, // Using Software Serial. The object was provided externally.
            SoftwareInternal    // Using Software Serial. The object was created and is owned by this class.
    };
    
    
    //------------------------------------------------------------------------------
    // Data.
    
    // Specifies what kind of serial object is stored in m_output.
    // This also indicates whether or not we own the object.
    // If this is SoftwareInternal then we need to delete the target m_output when
    //    this object is destroyed.
    SerialMode m_serialMode;
    
    // Pointer to the object we're sending serial commands/data to.
    // This could be pointing to a HardwareSerial or SoftwareSerial object provided
    //    externally, or it could simply be a copy of m_internalSerial.
    // The actual type can be inferred from m_serialMode.
    // We assume that it is always valid after construction of this object.
    Stream * m_output;

    // The reset pin, if one was provided at construction.
    uint8_t m_resetPin;

    // Indicates if a reset pin was provided at construction.
    bool m_hasResetPin;
    
    
    // The background colour value most recently sent to the display.
    // This is used to avoid sending it more often than necessary.
    // It is initially set to an invalid colour value so that the first
    //  colour used is always sent.
    uint8_t m_lastBGCol;
    
    // The foreground colour value most recently sent to the display.
    // This is used to avoid sending it more often than necessary.
    // It is initially set to an invalid colour value so that the first
    //  colour used is always sent.
    uint8_t m_lastFGCol;
    
    
    // The current colour for drawing lines and text.
    // Default is white.
    HSTColour m_colLine;
    
    // The current colour for filling shapes.
    // Default is blue.
    HSTColour m_colFill;
    
    // The current colour for clearing the screen and for text background.
    // Default is black;
    HSTColour m_colBackground;
};

#endif //Arduino_HobbytronicsSerialTFT_h

