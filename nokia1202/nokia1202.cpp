#include "nokia1202.hpp"

/**
 * @brief LCDNokia1202::~LCDNokia1202
 */

LCDNokia1202::~LCDNokia1202()
{

}

/**
	
    @brief Нарисовать линию
	
    @param xStartPosition начальная координата по х
    @param yStartPosition начальная координата по y
    @param xEndPosition конечная координата по х
    @param yEndPosition конечная координата по y
	
	
*/

void LCDNokia1202::drawLine( tDisplaySize xStartPosition, tDisplaySize yStartPosition, tDisplaySize xEndPosition, tDisplaySize yEndPosition )
{
	
    if( ( xEndPosition >= LCD_RESOLUTION_X ) || ( yEndPosition >= LCD_RESOLUTION_Y ) )
    {
		return;
	}
		
	tDisplaySize x;
	tDisplaySize y;
	
	x = xStartPosition;
	y = yStartPosition;
	
    while( ( x < xEndPosition ) || ( y < yEndPosition ) )
    {
		
        if( x < xEndPosition )
        {
			x += 1;
		}
		
        if( y < yEndPosition )
        {
			y += 1;
		}
		
		setPixel( x, y );
		
	}
	
}

/**

    @brief Отключение временной задержки при передаче данных
	
	Вызов метода будет полезен, если картинка на дисплее слишком медленно обновляется.
	
*/

void LCDNokia1202::setNoDelayWhenSendFrame()
{
	
	nodelay = true;
	
}

/**

    @brief Включение временной задержки при передаче данных, по-умолчанию она включена
	
*/

void LCDNokia1202::unsetNoDelayWhenSendFrame()
{
	
	nodelay = false;
	
}

/**

    @brief Создание объекта дисплея
	
    @param clockLine      номер вывода к которому подключена линия синхронизации           - SCLK
    @param serialDataOut  номер вывода к которому подключена линия передачи данных дисплею - SDA
    @param chipSelectLine номер вывода к которому подключена линия выбора дисплея          - CS
    @param resetLine      номер вывода  к которому подлкючения линия сброса дисплея        - RST
	
*/

LCDNokia1202::LCDNokia1202( IO::PIN clock, IO::PIN serialDataOut, IO::PIN chipSelect, IO::PIN reset )
{
	
	lines.clock          = clock;
	lines.serialDataOut  = serialDataOut;
	lines.chipSelect     = chipSelect;
	lines.reset          = reset;
		
	nodelay = false;
	
	mirrorHorizontal = 0;

	mirrorVertical = 0;
	
}


/**

    @brief Создание объекта дисплея
	
    @param clockLine      номер вывода к которому подключена линия синхронизации           - SCLK
    @param serialDataOut  номер вывода к которому подключена линия передачи данных дисплею - SDA
    @param chipSelectLine номер вывода к которому подключена линия выбора дисплея          - CS
    @param resetLine      номер вывода  к которому подлкючения линия сброса дисплея        - RST
	
*/


LCDNokia1202::LCDNokia1202( IO::PIN  clock, IO::PIN  serialDataOut, IO::PIN  chipSelect, IO::PIN reset, bool _mirrorHorizontal, bool _mirrorVertical )
{
		
	lines.clock          = clock;
    lines.serialDataOut  = serialDataOut;
    lines.chipSelect     = chipSelect;
    lines.reset          = reset;
	
	mirrorHorizontal = _mirrorHorizontal;
	
	mirrorVertical   = _mirrorVertical;
	
	nodelay = 0;
	
}

/**

    @brief Инициализация, вызывать перед началом работы с дисплеем
	
*/

void LCDNokia1202::init()
{

    IO::use( lines.clock );
    IO::use( lines.serialDataOut );
    IO::use( lines.chipSelect );
    IO::use( lines.reset );

    IO::out( lines.clock );
    IO::out( lines.serialDataOut );
    IO::out( lines.chipSelect );
    IO::out( lines.reset );

    IO::low( lines.serialDataOut );
    IO::low( lines.clock );
	
    Delay::microseconds(50000);
		
    IO::low( lines.chipSelect );
    IO::low( lines.reset );
		
    Delay::microseconds(500000);
		
    IO::high( lines.reset );
					
    Delay::microseconds(500000);

	_writeCommand( COMMAND_RESET );
	_writeCommand( COMMAND_SET_CHARGE_PUMP_MUL );
	_writeCommand( COMMAND_PUMP_X4 );
	_writeCommand( COMMAND_SET_CONTRAST );
	_writeCommand( COMMAND_DEFAULT_CONTRAST ); // -127..+127
	_writeCommand( COMMAND_NORMAL_MODE );
	_writeCommand( COMMAND_POWER_ON );
	
    if( mirrorHorizontal )
    {
		
		_writeCommand( COMMAND_MIRROR_TOP_DOWN_VIEW ); // COMMAND_MIRROR_TOP_DOWN_VIEW

    }
    else
    {
	
		_writeCommand( COMMAND_NORMAL_TOP_DOWN_VIEW ); // COMMAND_MIRROR_TOP_DOWN_VIEW

	}
	
    if( mirrorVertical )
    {
	
		_writeCommand( COMMAND_MIRROR_LEFT_RIGHT_VIEW ); // COMMAND_MIRROR_LEFT_RIGHT_VIEW

    }
    else
    {
	
		_writeCommand( COMMAND_NORMAL_LEFT_RIGHT_VIEW ); // COMMAND_MIRROR_LEFT_RIGHT_VIEW

	}
	
	_writeCommand( COMMAND_ON );
	
	clear();
	sendFrame();

}

/**

    @brief Денициализация, вызывать после окончания работы с дисплеем
	
*/

void LCDNokia1202::deinit()
{

    IO::in( lines.clock );
    IO::in( lines.serialDataOut );
    IO::in( lines.chipSelect );
    IO::in( lines.reset );

    IO::unuse( lines.clock );
    IO::unuse( lines.serialDataOut );
    IO::unuse( lines.chipSelect );
    IO::unuse( lines.reset );
	
}

/**
	
    @brief Установить текущий шрифт
	
    @param указатель на объект шрифта, который должен существовать все время использования, до конца работы с дисплеем или вызова unsetFont
	
*/

void  LCDNokia1202::setFont( Font *_font )
{
	
	font = _font;
	
}

/**
	
    @brief Убрать текущий шрифт
		
*/

void LCDNokia1202::unsetFont()
{
	
	font = 0;	

}

/**

    @brief Очистить буфер дисплея
	
*/

void LCDNokia1202::clear()
{
		
    for( int i = 0 ; i < LCD_RESOLUTION_X; i++ )
    {
		
        for( int j = 0 ; j < LCD_RESOLUTION_Y/COLUMN_BIT_VOLUME; j++ )
        {
			
			displayBuffer[i][j] = 0x00;
			
		}
	
	}
	
}


/**
	
    @brief Перевести отдельный пиксель в буфере дисплея во включенное состояние
	
    @param xPosition координата пикселя по оси X
    @param yPosition координата пикселя по оси Y
	
*/

void LCDNokia1202::setPixel( tDisplaySize xPosition, tDisplaySize yPosition )
{
	
    if( ( xPosition >= LCD_RESOLUTION_X ) || ( yPosition >= LCD_RESOLUTION_Y ) )
    {
		return;
	}
	
	displayBuffer[xPosition][ ( yPosition / COLUMN_BIT_VOLUME ) ] |= 1 << ( yPosition % COLUMN_BIT_VOLUME );
	
}

/**
	
    @brief Перевести отдельный пиксель в буфере дисплея в отключенное состояние
	
    @param xPosition координата пикселя по оси X
    @param yPosition координата пикселя по оси Y
	
*/

void LCDNokia1202::clearPixel( tDisplaySize xPosition, tDisplaySize yPosition )
{
	
    if( ( xPosition >= LCD_RESOLUTION_X ) || ( yPosition >= LCD_RESOLUTION_Y ) )
    {
		return;
	}
	
	displayBuffer[xPosition][ ( yPosition / COLUMN_BIT_VOLUME ) ] &= ~( 1 << ( yPosition % COLUMN_BIT_VOLUME ) );
	
}

/**

    @brief Отобразить(скопировать) текст в буфер дисплея
	
    @param text объект с текстом
	
*/

void LCDNokia1202::showText( Text *text )
{
	
	if( 0 == font ) return;
	
	uint16_t xText;
	uint16_t yText;
	
	xText = text->getXPosition();
	yText = text->getYPosition();
	
    for( uint16_t i = 0 ; i < text->getLength() ; i++, yText += 6 )
    {
		
        if( yText >= ( LCD_RESOLUTION_Y - 6 ) )
        {
			yText = text->getYPosition();
			xText += 8;
		}
		
		const uint8_t *currentSymbol = font->getSymbolFromUtf8Code( text->at(i) );
		
        if( text->at(i) == 0x20 )
        {
			yText -= 3;
			continue;
		}
		
		// Получили символ из 5 байт, размещаем его в буфере
		
        for( uint8_t j = 0 ; j < 5 ; j++ )
        {
		
            for( uint8_t k = 0 ; k < 8 ; k++ )
            {
				
                if( (currentSymbol[j] >> k) & 0x01 )
                {
					
					setPixel( xText + k, yText + j );
					
                }
                else
                {
				
					clearPixel( xText + k, yText + j );
					
				}
				
			}			
		
		}
		
	}
  
}

/**
	
    @brief Отобразить текущий буфер на дисплее
	
*/

void LCDNokia1202::sendFrame()
{
	
	_writeCommand( COMMAND_SET_ADDRESS_0 ); // display address
	_writeCommand( COMMAND_PAGE_ADDRESS_0 ); // page address
	_writeCommand( 0x10 ); 
	_writeCommand( COMMAND_COLUMN_ADDRESS_0 );
  
	// LCD_RESOLUTION_X][LCD_RESOLUTION_Y/COLUMN_BIT_VOLUME
	
    // int maxX;
    // int maxY;
  
    // maxX = LCD_RESOLUTION_X;
    // maxY = LCD_RESOLUTION_Y / COLUMN_BIT_VOLUME;
	
    for( int i = 0 ; i < LCD_RESOLUTION_X ; i += COLUMN_BIT_VOLUME )
    {
		
        for( int j = 0 ; j < ( LCD_RESOLUTION_Y / COLUMN_BIT_VOLUME ) ; j += 1 )
        {
			
			uint8_t sarr[COLUMN_BIT_VOLUME];
			
            for( int k = 0 ; k < COLUMN_BIT_VOLUME ; k++ )
            {
				sarr[k] = 0x00;
			}
			
            for( int l = 0 ; l < COLUMN_BIT_VOLUME ; l++ )
            {
				
				int byte;
				
				byte = displayBuffer[i+l][j];
	
				sarr[7] |= ((byte >> 7)&0x01) << l;
				sarr[6] |= ((byte >> 6)&0x01) << l;
				sarr[5] |= ((byte >> 5)&0x01) << l;
				sarr[4] |= ((byte >> 4)&0x01) << l;
				sarr[3] |= ((byte >> 3)&0x01) << l;
				sarr[2] |= ((byte >> 2)&0x01) << l;
				sarr[1] |= ((byte >> 1)&0x01) << l;
				sarr[0] |= ((byte >> 0)&0x01) << l;
				
			}
			
            for( int m = 0 ; m < COLUMN_BIT_VOLUME ; m++ )
            {
				_writeData( sarr[m] );
			}
			
		}
  }
 
  
}

/**
	
    @brief Передать команду дисплею
	
    @param command номер команды
	
*/

void LCDNokia1202::_writeCommand( uint8_t command )
{

	
    IO::low( lines.chipSelect );
    IO::low( lines.clock );
    IO::low( lines.serialDataOut );

    Delay::microseconds(1);

    IO::high( lines.clock );

    Delay::microseconds(1);
	
	_write(command);
	
}

/**
	
    @brief Передать данные дисплею
	
    @param dataByte байт данных
	
*/

void LCDNokia1202::_writeData( uint8_t dataByte )
{
	
    IO::low( lines.chipSelect );
    IO::low( lines.clock );

    IO::high( lines.serialDataOut );
			
    if( !nodelay )
    {
        Delay::microseconds(1);
	}
	
    IO::high( lines.clock );

    if( !nodelay )
    {
        Delay::microseconds(1);
	}
	
	_write(dataByte);
	
}

/**
    @brief Посылает один байт дисплею
*/

void LCDNokia1202::_write( uint8_t dataByte )
{

	for(uint8_t i=0;i<8;i++)
	{

        IO::low( lines.clock );

		if( dataByte & 0x80 )
		{
            IO::high( lines.serialDataOut );
		}
		else
		{
		
            IO::low( lines.serialDataOut );

		}

        IO::high( lines.clock );

		dataByte <<= 1;
		
        if( !nodelay )
        {
            Delay::microseconds(2);
		}
		
	}
	
    IO::high( lines.chipSelect );

    if( !nodelay )
    {
        Delay::microseconds(1);
	}
	
}
