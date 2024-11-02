#ifndef ST7735_HPP
#define ST7735_HPP

#include "io.hpp"
#include "color.hpp"
#include "font.hpp"
#include "spi.hpp"
#include "text.hpp"

class LCDSt7735
{

public:

    typedef enum
    {
        BLACK   = 0x000000,
        WHITE   = 0xFFFFFF,
        RED     = 0xFF0000,
        GREEN   = 0x00FF00,
        BLUE    = 0x0000FF
    } COLORS;

    typedef enum
    {
        PIXEL_FORMAT_12_BIT = 3,
        PIXEL_FORMAT_16_BIT = 5,
        PIXEL_FORMAT_18_BIT = 6
    } PIXEL_FORMAT;

    LCDSt7735( IO::PIN _rs,
               IO::PIN mosi,
               IO::PIN sck,
               IO::PIN miso,
               IO::PIN cs
                );

    void init();
    void deinit();

    void setBackgroundColor( COLORS _backgroundColor );

    void setFont( Font &_font );
    void unsetFont();

    void showChar( uint32_t x, uint32_t y, uint32_t color, uint16_t symbol );
    void showText( Text& text, uint32_t color );

    void reset();

    void clear();

    void fill( uint32_t color );

    void drawPixel( uint8_t x, uint8_t y, uint32_t color );
    void drawPixel( uint32_t color );

private:

    Font*  font;

    COLORS backgroundColor;

    uint8_t colstart = 2;
    uint8_t rowstart = 1;

    SPI spi;

    IO::PIN rs;

    typedef enum
    {
        SOFTWARE_RESET             = 0x01,
        SLEEP_OUT                  = 0x11,
        DISPLAY_OFF                = 0x28,
        DISPLAY_ON                 = 0x29,
        READ_DISPLAY_STATUS        = 0x09,
        READ_DISPLAY_POWER_MODE    = 0x0A,
        NORMAL_DISPLAY_MODE_ON     = 0x13,
        DISPLAY_INVERSION_OFF      = 0x20,
        INTERFACE_PIXEL_FORMAT     = 0x3A,
        MEMORY_DATA_ACCESS_CONTROL = 0x36,
        COLUMN_ADDRESS_SET         = 0x2A,
        ROW_ADDRESS_SET            = 0x2B,
        MEMORY_WRITE               = 0x2C,
        MADCTL                     = 0b00110110  // display control;
    } COMMAND;

    typedef struct
    {
        IO::PIN rs;
        IO::PIN mosi;
        IO::PIN sck;
        IO::PIN cs;
        IO::PIN miso;
    } Lines;

    typedef enum
    {
        WIDTH  = 128, // x
        HEIGHT = 160  // y
    } SIZES;

    void    _writeCommand(uint8_t command );
    void    _writeByte( uint8_t data );
    void    _setWindow( uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1 );

};

#endif
