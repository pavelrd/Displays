#include "st7735.hpp"
#include "delay.hpp"

#include "serial.hpp"

/**
 * @brief LCDSt7735::LCDSt7735
 *
 * @param rs               выбор, идет запись команды или данных
 * @param mosi             данные, идущие к дисплею
 * @param sck              синхронизация
 * @param cs               выбор дисплея
 * @param miso             данные, идущие от дисплея
 * @param _backgroundColor цвет фона
 *
 */

LCDSt7735::LCDSt7735( IO::PIN _rs,
                      IO::PIN mosi,
                      IO::PIN sck,
                      IO::PIN miso,
                      IO::PIN cs )
{

    rs              = _rs;

    backgroundColor = WHITE;

    spi.set( mosi, sck, miso, cs, false, false, false, 0 );

}

/**
 * @brief LCDSt7735::drawPixel
 * @param x
 * @param y
 * @param color
 */

void LCDSt7735::drawPixel( uint8_t x, uint8_t y, uint32_t color )
{

    _setWindow( x, y, WIDTH, HEIGHT );

    drawPixel( color );

}

/**
 * @brief drawPixel
 * @param color
 */

void LCDSt7735::drawPixel( uint32_t color )
{
    _writeByte( ( color >> 16 ) & 0xFF );
    _writeByte( ( color >> 8 )  & 0xFF );
    _writeByte(   color         & 0xFF );
}


/**
 *
 * @brief Программный сброс дисплея
 *
 */

void LCDSt7735::reset()
{

    _writeCommand(SOFTWARE_RESET);

    Delay::milliseconds(200);

    _writeCommand( SLEEP_OUT ); // 0x11

    Delay::milliseconds(200);

}

/**
 *
 *  @brief LCDSt7735::init
 *
 */

void LCDSt7735::init()
{

    IO::use(rs);
    IO::out(rs);
    IO::high(rs);

    spi.init();

    reset();

    _writeCommand(NORMAL_DISPLAY_MODE_ON); // 0x13
    _writeCommand(DISPLAY_INVERSION_OFF);  // 0x20
    _writeCommand(INTERFACE_PIXEL_FORMAT); // 0x3A
    _writeByte(PIXEL_FORMAT_18_BIT);       //

    _writeCommand(DISPLAY_ON);                 // 0x29
    _writeCommand(MEMORY_DATA_ACCESS_CONTROL); // 0x36
    _writeByte(0x00);

    _setWindow( 0, 0, WIDTH, HEIGHT );

}

/**
 * @brief LCDSt7735::deinit
 */

void LCDSt7735::deinit()
{

    IO::in(rs);
    IO::unuse(rs);

    spi.deinit();

}

/**
 * @brief LCDSt7735::_writeCommand
 * @param command
 */

void LCDSt7735::_writeCommand( uint8_t command )
{

    IO::low( rs );

    spi.transferByte( command );

    IO::high( rs );

}

/**
 * @brief LCDSt7735::_writeByte
 * @param byte
 */

void LCDSt7735::_writeByte( uint8_t data )
{
    spi.transferByte( data );
}

/**
 * @brief LCDSt7735::clearScreen
 */

void LCDSt7735::clear()
{

    fill(backgroundColor);

}

/**
 *
 * @brief LCDSt7735::fill
 *
 * @param r
 * @param g
 * @param b
 *
 */

void LCDSt7735::fill( uint32_t color )
{

  _setWindow( 0, 0, WIDTH, HEIGHT );

  for( uint32_t i = 0 ; i < WIDTH * HEIGHT; i++ )
  {
      drawPixel( color );
  }

}

/**
 * @brief LCDSt7735::_setWindow
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 */

void LCDSt7735::_setWindow( uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1 )
{
    _writeCommand(COLUMN_ADDRESS_SET);     // set column range (x0,x1)
    _writeByte( 0x00 );
    _writeByte( x0 );
    _writeByte(0x00);
    _writeByte( x0 + ( x1 - 1 ) );
    _writeCommand(ROW_ADDRESS_SET);     // set row range (y0,y1)
    _writeByte( 0x00 );
    _writeByte( y0  );
    _writeByte( 0x00 );
    _writeByte( y0 + ( y1 - 1 ) );
    _writeCommand(MEMORY_WRITE);
}

/**

    @brief Установить текущий шрифт

    @param указатель на объект шрифта, который должен существовать все время использования,
           до конца работы с дисплеем или вызова unsetFont

*/

void LCDSt7735::setFont( Font& _font )
{

    font = &(_font);

}

/**

    @brief Убрать текущий шрифт

*/

void LCDSt7735::unsetFont()
{

    font = 0;

}

/**
 *
 * @brief showChar
 *
 * @param x
 * @param y
 * @param color
 * @param symbol
 *
 */

void LCDSt7735::showChar( uint32_t x, uint32_t y, uint32_t color, uint16_t symbol )
{

    const uint8_t* fontSymbol = font->getSymbolFromUtf8Code(symbol);

    _setWindow( x, y, font->getSymbolWidth() * font->getSize(), font->getSymbolHeight() * font->getSize() );

    for( uint8_t j=0; j < font->getSymbolHeight(); j++ )
    {
        for( uint8_t i = 0 ; i < font->getSymbolWidth() * font->getSize(); i++ )
        {

            uint8_t data = fontSymbol[ i % font->getSymbolWidth() ];

            if( data & (1 << j) )
            {
                for( uint8_t k = 0 ; k < font->getSize(); k++ )
                {
                    drawPixel( color );
                }
            }
            else
            {
                for( uint8_t k = 0 ; k < font->getSize(); k++ )
                {
                    drawPixel( backgroundColor );
                }
            }
        }
    }

}

/**
 *
 * @brief LCDSt7735::showText
 *
 * @param text
 * @param color
 *
 */

void LCDSt7735::showText( Text& text, uint32_t color )
{

    if( 0 == font ) return;

    for( uint16_t i = 0 ; i < text.getMaximumLength(); i++ )
    {

        if( i >= text.getLength() )
        {
            showChar( text.getXPosition() + ( (i * font->getSymbolWidth() * font->getSize() ) + i*2 ), text.getYPosition(), RED, ' ' );
        }
        else
        {
            showChar( text.getXPosition() + ( (i * font->getSymbolWidth() * font->getSize()) + i*2 ), text.getYPosition(), color, text.at(i) );
        }


    }

    return;

}

/**
 *
 * @brief setBackgroundColor
 *
 * @param _backgroundColor
 *
 */

void LCDSt7735::setBackgroundColor( COLORS _backgroundColor )
{
    backgroundColor = _backgroundColor;
}

