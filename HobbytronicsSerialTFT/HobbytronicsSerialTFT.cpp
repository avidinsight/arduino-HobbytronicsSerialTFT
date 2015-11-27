/*
 * HobbytronicsSerialTFT.cpp
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

#include "HobbytronicsSerialTFT.h"

// The byte which signals the beginning of a command.
constexpr static uint8_t g_beginCmd = 0x1B;

// The byte which signals the end of a command.
constexpr static uint8_t g_endCmd = 0xFF;


//------------------------------------------------------------------------------
// Construction / destruction.

HobbytronicsSerialTFT::HobbytronicsSerialTFT(HardwareSerial &hwserial) :
    m_serialMode(SerialMode::Hardware),
    m_output(&hwserial),
    m_resetPin(0),
    m_hasResetPin(false),
    m_lastBGCol(255),
    m_lastFGCol(255),
    m_colLine(HSTColour::White),
    m_colFill(HSTColour::Blue),
    m_colBackground(HSTColour::Black)
{
}

HobbytronicsSerialTFT::HobbytronicsSerialTFT(HardwareSerial &hwserial, uint8_t resetPin) :
    HobbytronicsSerialTFT(hwserial)
{
    setupReset(resetPin);
}

HobbytronicsSerialTFT::HobbytronicsSerialTFT(SoftwareSerial &swserial) :
    m_serialMode(SerialMode::SoftwareExternal),
    m_output(&swserial),
    m_resetPin(0),
    m_hasResetPin(false),
    m_lastBGCol(255),
    m_lastFGCol(255),
    m_colLine(HSTColour::White),
    m_colFill(HSTColour::Blue),
    m_colBackground(HSTColour::Black)
{
}

HobbytronicsSerialTFT::HobbytronicsSerialTFT(SoftwareSerial &swserial, uint8_t resetPin) :
    HobbytronicsSerialTFT(swserial)
{
    setupReset(resetPin);
}

HobbytronicsSerialTFT::HobbytronicsSerialTFT(uint8_t rx, uint8_t tx) :
    m_serialMode(SerialMode::SoftwareInternal),
    m_output(new SoftwareSerial(rx, tx)),
    m_resetPin(0),
    m_hasResetPin(false),
    m_lastBGCol(255),
    m_lastFGCol(255),
    m_colLine(HSTColour::White),
    m_colFill(HSTColour::Blue),
    m_colBackground(HSTColour::Black)
{
}

HobbytronicsSerialTFT::HobbytronicsSerialTFT(uint8_t rx, uint8_t tx, uint8_t resetPin) :
    HobbytronicsSerialTFT(rx, tx)
{
    setupReset(resetPin);
}

HobbytronicsSerialTFT::~HobbytronicsSerialTFT()
{
    // Important: Destroy the software serial object we created, if applicable.
    if (m_serialMode == SerialMode::SoftwareInternal) {
        delete static_cast<SoftwareSerial*>(m_output);
    }
    m_output = nullptr;
}


//------------------------------------------------------------------------------
// Hardware/connection control.

void HobbytronicsSerialTFT::reset()
{
    if (m_hasResetPin) {
        digitalWrite(m_resetPin, LOW);
        delay(1);
        digitalWrite(m_resetPin, HIGH);
    }
}

void HobbytronicsSerialTFT::begin(unsigned long speed)
{
    switch (m_serialMode)
    {
    case SerialMode::Hardware:
        static_cast<HardwareSerial*>(m_output)->begin(speed);
        break;
        
    case SerialMode::SoftwareExternal:
    case SerialMode::SoftwareInternal:
        // For some reason, SoftwareSerial::begin() takes a signed long instead
        //    of an unsigned long.
        static_cast<SoftwareSerial*>(m_output)->begin(static_cast<long>(speed));
        break;
    }
}

void HobbytronicsSerialTFT::flush()
{
    m_output->flush();
}

//------------------------------------------------------------------------------
// Colour functions.

void HobbytronicsSerialTFT::setBackgroundColour(const HSTColour col)
{
    m_colBackground = col;
}

void HobbytronicsSerialTFT::setLineColour(const HSTColour col)
{
    m_colLine = col;
}

void HobbytronicsSerialTFT::setFillColour(const HSTColour col)
{
    m_colFill = col;
}


//------------------------------------------------------------------------------
// General display functions.

void HobbytronicsSerialTFT::setScreenRotation(const HSTRotation rtn)
{
    sendCommand(3, static_cast<uint8_t>(rtn));
}

void HobbytronicsSerialTFT::setBacklightBrightness(uint8_t level)
{
    sendCommand(14, level);
}

void HobbytronicsSerialTFT::clearScreen()
{
    
    sendCommand(0);
}

void HobbytronicsSerialTFT::drawBitmap(uint8_t x, uint8_t y, String filename)
{
    m_output->write(g_beginCmd);
    m_output->write(13);
    m_output->write(x);
    m_output->write(y);
    m_output->print(filename);
    m_output->write(g_endCmd);
}


//------------------------------------------------------------------------------
// Shape drawing functions.

void HobbytronicsSerialTFT::drawPixel(uint8_t x, uint8_t y)
{
    applyLineColour();
    drawLine(x, y, x, y);
}

void HobbytronicsSerialTFT::drawHorizontalLine(uint8_t y)
{
    applyLineColour();
    drawLine(0, y, 159, y);
}

void HobbytronicsSerialTFT::drawHorizontalLine(uint8_t x1, uint8_t y, uint8_t x2)
{
    applyLineColour();
    drawLine(x1, y, x2, y);
}

void HobbytronicsSerialTFT::drawVerticalLine(uint8_t x)
{
    applyLineColour();
    drawLine(x, 0, x, 159);
}

void HobbytronicsSerialTFT::drawVerticalLine(uint8_t x, uint8_t y1, uint8_t y2)
{
    applyLineColour();
    drawLine(x, y1, x, y2);
}

void HobbytronicsSerialTFT::drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    applyLineColour();
    sendCommand(8, x1, y1, x2, y2);
}

void HobbytronicsSerialTFT::drawBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, HSTShapeStyle style)
{
    if (style == HSTShapeStyle::Fill || style == HSTShapeStyle::FilledOutline) {
        applyFillColour();
        sendCommand(10, x1, y1, x2, y2);
    }
    
    if (style == HSTShapeStyle::Outline || style == HSTShapeStyle::FilledOutline) {
        applyLineColour();
        sendCommand(9, x1, y1, x2, y2);
    }
}

void HobbytronicsSerialTFT::drawCircle(uint8_t x, uint8_t y, uint8_t radius, HSTShapeStyle style)
{
    if (style == HSTShapeStyle::Fill || style == HSTShapeStyle::FilledOutline) {
        applyFillColour();
        sendCommand(12, x, y, radius);
    }
    
    if (style == HSTShapeStyle::Outline || style == HSTShapeStyle::FilledOutline) {
        applyLineColour();
        sendCommand(11, x, y, radius);
    }
}

void HobbytronicsSerialTFT::drawTriangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, HSTShapeStyle style)
{
    // TODO: Implement
}


//------------------------------------------------------------------------------
// Text functions.

void HobbytronicsSerialTFT::setFontSize(const HSTFontSize size)
{
    sendCommand(4, static_cast<uint8_t>(size));
}

void HobbytronicsSerialTFT::gotoTextLineStart()
{
    sendCommand(5);
}

void HobbytronicsSerialTFT::gotoCharacterPosition(uint8_t x, uint8_t y)
{
    sendCommand(6, x, y);
}

void HobbytronicsSerialTFT::gotoPixelPosition(uint8_t x, uint8_t y)
{
    sendCommand(7, x, y);
}

size_t HobbytronicsSerialTFT::write(uint8_t data)
{
    applyLineColour();
    m_output->write(data);
}


//------------------------------------------------------------------------------
// Internal operations.

void HobbytronicsSerialTFT::setupReset(uint8_t pin)
{
    m_resetPin = pin;
    m_hasResetPin = true;
    // The reset line is active-low so we need to bring it high for the display to work.
    digitalWrite(pin, HIGH);
    pinMode(pin, OUTPUT);
}

void HobbytronicsSerialTFT::sendCommand(uint8_t cmd)
{
    m_output->write(g_beginCmd);
    m_output->write(cmd);
    m_output->write(g_endCmd);
}

void HobbytronicsSerialTFT::sendCommand(uint8_t cmd, uint8_t par1)
{
    m_output->write(g_beginCmd);
    m_output->write(cmd);
    m_output->write(par1);
    m_output->write(g_endCmd);
}

void HobbytronicsSerialTFT::sendCommand(uint8_t cmd, uint8_t par1, uint8_t par2)
{
    m_output->write(g_beginCmd);
    m_output->write(cmd);
    m_output->write(par1);
    m_output->write(par2);
    m_output->write(g_endCmd);
}

void HobbytronicsSerialTFT::sendCommand(uint8_t cmd, uint8_t par1, uint8_t par2, uint8_t par3)
{
    m_output->write(g_beginCmd);
    m_output->write(cmd);
    m_output->write(par1);
    m_output->write(par2);
    m_output->write(par3);
    m_output->write(g_endCmd);
}

void HobbytronicsSerialTFT::sendCommand(uint8_t cmd, uint8_t par1, uint8_t par2, uint8_t par3, uint8_t par4)
{
    m_output->write(g_beginCmd);
    m_output->write(cmd);
    m_output->write(par1);
    m_output->write(par2);
    m_output->write(par3);
    m_output->write(par4);
    m_output->write(g_endCmd);
}

void HobbytronicsSerialTFT::sendBackgroundColour(HSTColour col)
{
    if (m_lastBGCol != static_cast<uint8_t>(col)) {
        m_lastBGCol = static_cast<uint8_t>(col);
        sendCommand(2, m_lastBGCol);
    }
}
    
void HobbytronicsSerialTFT::sendForegroundColour(HSTColour col)
{
    if (m_lastFGCol != static_cast<uint8_t>(col)) {
        m_lastFGCol = static_cast<uint8_t>(col);
        sendCommand(1, m_lastFGCol);
    }
}

void HobbytronicsSerialTFT::applyBackgroundColour()
{
    sendBackgroundColour(m_colBackground);
}

void HobbytronicsSerialTFT::applyLineColour()
{
    sendForegroundColour(m_colLine);
}

void HobbytronicsSerialTFT::applyFillColour()
{
    sendForegroundColour(m_colFill);
}

