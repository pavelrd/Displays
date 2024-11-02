#include "nokia1616.hpp"

/**
 * @brief LCD1616::LCD1616
 */

LCD1616::LCD1616()
{
	fontSize = 1;
}

/**
 * @brief LCD1616::~LCD1616
 */

LCD1616::~LCD1616()
{

}

/**
 * @brief LCD1616::getFontSize
 * @return
 */

uint8_t LCD1616::getFontSize()
{
	return fontSize;
}

/**
 * @brief LCD1616::setFontSize
 * @param _fontSize
 */

void LCD1616::setFontSize(uint8_t _fontSize)
{
	fontSize = _fontSize;
}
	
/**

    @brief Создание объекта дисплея
	
    @param clockLine      номер вывода к которому подключена линия синхронизации           - SCLK
    @param serialDataOut  номер вывода к которому подключена линия передачи данных дисплею - SDA
    @param chipSelectLine номер вывода к которому подключена линия выбора дисплея          - CS
    @param resetLine      номер вывода  к которому подлкючена линия сброса дисплея        - RST
    @param light      номер вывода  к которому подлкючена подсветка
	
*/

LCD1616::LCD1616(IO::PIN clock, IO::PIN serialDataOut, IO::PIN chipSelect, IO::PIN reset, IO::PIN light  )
{
	
	fontSize = 1;
	
    lines.clock          = clock;
    lines.serialDataOut  = serialDataOut;
    lines.chipSelect     = chipSelect;
    lines.reset          = reset;
    lines.light          = light;
	
}

/**

	\brief Инициализация, вызывать перед началом работы с дисплеем
	
*/

void LCD1616::init()
{
	
    IO::use( lines.chipSelect );
    IO::use( lines.serialDataOut );
    IO::use( lines.clock );
    IO::use( lines.reset );
    IO::use( lines.light );

    IO::out( lines.chipSelect );
    IO::out( lines.serialDataOut );
    IO::out( lines.clock );
    IO::out( lines.reset );
    IO::out( lines.light );

    IO::high( lines.light );
    IO::high( lines.reset );
    IO::high( lines.chipSelect);
    IO::high( lines.serialDataOut );

    IO::low( lines.reset );
    Delay::milliseconds(100);

    IO::high( lines.reset );
    Delay::milliseconds(100);
	
	_send(CMD_SLPOUT);
	_send(CMD_COLMOD);
	_send(SEND_DATA | CMD_COLMOD_MCU16bit);
	_send(CMD_DISPON);
	_send(CMD_INVOFF);
	_send(CMD_NORON);

}

/**

    @brief Денициализация, вызывать после окончания работы с дисплеем
	
*/

void LCD1616::deinit()
{
	
    IO::in( lines.chipSelect );
    IO::in( lines.serialDataOut );
    IO::in( lines.clock );
    IO::in( lines.reset );
    IO::in( lines.light );

    IO::unuse( lines.chipSelect );
    IO::unuse( lines.serialDataOut );
    IO::unuse( lines.clock );
    IO::unuse( lines.reset );
    IO::unuse( lines.light );

}

/**
	
    @brief Установить текущий шрифт
	
    @param указатель на объект шрифта, который должен существовать все время использования, до конца работы с дисплеем или вызова unsetFont
	
*/

void  LCD1616::setFont( Font& _font )
{
	
	font = &(_font);
	
}

/**
	
    @brief Убрать текущий шрифт
		
*/

void LCD1616::unsetFont()
{
	
	font = 0;	

}

/**

    @brief Очистить дисплей
	
*/


void LCD1616::clear()
{	
	_setWindow(0, 0, WIDTH, HEIGHT );
	_senPixels( backgroundColor, WIDTH * HEIGHT );
	_sendCmd(CMD_NOP);
}

/**
 * @brief LCD1616::_senPixels
 * @param color
 * @param n
 */

void LCD1616::_senPixels( uint16_t color, uint16_t n)
{
	while(n--) _sendPixel(color);
}

/**
 * @brief LCD1616::_sendPixel
 * @param color
 */

void LCD1616::_sendPixel( uint16_t color )
{
	_sendData(color >> 8);
	_sendData(color & 0xFF);
}

/**
 * @brief LCD1616::_sendCmd
 * @param data
 */

void LCD1616::_sendCmd(uint8_t data)
{ 
	_send( SEND_CMD | data );
}

/**
 * @brief LCD1616::_sendData
 * @param data
 */

void LCD1616::_sendData(uint8_t data) 
{ 
	_send( SEND_DATA | data );
}

/**
 * @brief LCD1616::_cmd1616
 * @param cmd
 * @param a
 * @param b
 */

void LCD1616::_cmd1616(uint8_t cmd, uint16_t a, uint16_t b)
{
	_sendCmd(cmd);
	_sendData(a>>8);
	_sendData(a);
	_sendData(b>>8);
	_sendData(b);
}

/**
 * @brief LCD1616::_setWindow
 * @param x
 * @param y
 * @param w
 * @param h
 */

void LCD1616::_setWindow(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
//	x += LCD_X_OFFSET;
//	y += LCD_Y_OFFSET;
/*#ifdef LCD_ROTATE90
	_nlcdCmd1616(CMD_CASET, y, y+h-1);	// column start/end
	_nlcdCmd1616(CMD_RASET, LCD_MAX_X-x-w, LCD_MAX_X-x-1);	// page start/end
#else*/
	_cmd1616(COLUMN_ADDRESS_SET, x, x+w-1);	// column start/end
	_cmd1616(ROW_ADDRESS_SET, y, y+h-1);	// page start/end
//#endif
	_sendCmd(MEMORY_WRITE);				// RAMWR
}

/**
	
    @brief Показать один символ на дисплее
	
*/

void LCD1616::showChar( uint32_t x, uint32_t y, uint16_t color, uint16_t symbol )
{
	
	const uint8_t* sd = font->getSymbolFromUtf8Code(symbol);
	
	_setWindow( x, y, font->getSymbolWidth()*fontSize, font->getSymbolHeight()*fontSize );
	
	for( uint8_t j=0; j < font->getSymbolHeight(); j++ )
	{
		for(uint8_t i=0; i < font->getSymbolWidth()*fontSize; i++)
		{
			uint8_t data = sd[ i % font->getSymbolWidth() ];
            if( data & (1 << j) )
            {
                for( uint8_t k = 0 ; k < fontSize; k++ )
                {
					_sendPixel(color);
				}
            }
            else
            {
                for( uint8_t k = 0 ; k < fontSize; k++ )
                {
					_sendPixel(backgroundColor);
				}
			}
		}
	}
	
}

/**

    @brief Отобразить(скопировать) текст в буфер дисплея
	
    @param text объект с текстом
	
*/

void LCD1616::showText( Text& text, uint16_t color )
{
	
	if( 0 == font ) return;
	
    for( uint16_t i = 0 ; i < text.getMaximumLength(); i++ )
    {
		
        if( i >= text.getLength() )
        {
			showChar( text.getXPosition() + ( (i * font->getSymbolWidth() * fontSize) + i*2 ), text.getYPosition(), RED, ' ' );
        }
        else
        {
			showChar( text.getXPosition() + ( (i * font->getSymbolWidth() * fontSize) + i*2 ), text.getYPosition(), color, text.at(i) );
		}
		
		
	}
	
	return;
	
}

/**
 * @brief LCD1616::_send
 * @param data
 */

void LCD1616::_send(uint16_t data) 
{
	
    IO::low( lines.chipSelect );
	
	for(uint16_t i=0; i<9; i++)
	{
		
        if( data & (256 >> i))
        {
			
            IO::high( lines.serialDataOut );
			
		} else {
			
            IO::low( lines.serialDataOut );
			
		}
		
        IO::high( lines.clock );
		
        IO::low( lines.clock );
		
	}

    IO::high( lines.chipSelect );
	 
}

/**
 * @brief LCD1616::getWidth
 * @return
 */

uint8_t LCD1616::getWidth()
{
    switch( orientation )
    {
		case ORIENTATION_NORMAL:
		case ORIENTATION_180:
			return WIDTH;
	}
	return HEIGHT;
}

/**
 * @brief LCD1616::getHeight
 * @return
 */

uint8_t LCD1616::getHeight()
{
    switch( orientation )
    {
		case ORIENTATION_NORMAL:
		case ORIENTATION_180:
		return HEIGHT;
	}
	return WIDTH;
}

/**
 * @brief LCD1616::setOrientation
 * @param _orientation
 */

void LCD1616::setOrientation( orientation_t _orientation )
{
	uint8_t data = 0;
	orientation = _orientation;

    switch(orientation)
    {
		case ORIENTATION_90:
			data = CMD_MADCTR_MV | CMD_MADCTR_MX;
			break;
		case ORIENTATION_180:
			data = CMD_MADCTR_MY | CMD_MADCTR_MX;
			break;
		case ORIENTATION_270:
			data = CMD_MADCTR_MV | CMD_MADCTR_MY;
			break;
	}
	_sendCmd(CMD_MADCTR);
	_sendData(data);
}

/**
 * @brief LCD1616::setBackgroundColor
 * @param color
 */

void LCD1616::setBackgroundColor( uint16_t color )
{
	
	backgroundColor = color;
	
}
