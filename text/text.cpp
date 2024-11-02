#include "text.hpp"

/**
 * @brief Text::Text
 * @param string
 */

Text::Text( const char* string )
{
	
	xPosition = 0;
	yPosition = 0;
	maximumLength = TEXT_SIZE_MAX;

    setValue(string);
	
}

/**
 * @brief Text::Text
 * @param _xPosition
 * @param _yPosition
 * @param string
 */

Text::Text( uint16_t _xPosition, uint16_t _yPosition, const char* string )
{

    xPosition     = _xPosition;
    yPosition     = _yPosition;
    maximumLength = strlen(string);
	
    setValue(string);
	
}

/**
	
    @brief Создание текстового поля со значением
	
    @param _xPosition координата текстового поля по оси X
    @param _yPosition координата текстового поля по оси Y
    @param _maximumLength
    @param string значение текстового поля, строка в кодировке utf8
	
*/

Text::Text( uint16_t _xPosition, uint16_t _yPosition, uint16_t _maximumLength, const char* string )
{
	
    xPosition     = _xPosition;
    yPosition     = _yPosition;
    maximumLength = _maximumLength;
	
    setValue( string );
	
}

/**
 *
 * @brief Text::_setValue
 * @param string
 *
 */

void Text::setValue( const char* string )
{
	
	if( 0 == string ) return;
	
    for( length = 0 ; ( length < TEXT_SIZE_MAX ) && ( string[length] != '\0' ) ; length++ )
    {
		
	}
  
	if( 0 == length ) return;
    
    if( 0 == value )
    {
		return;
	}
	
	uint16_t i;
	
    for( i = 0; i < length; i++ )
    {
		
		value[i] = string[i];
		
	}
	
	value[i] = '\0';
	
}

/**
	
    @brief Получить положение текстового поля по оси Х
	
    @return положение от начала координат по оси Х в пикселях
	
*/

uint16_t Text::getXPosition()
{
	
	return xPosition;
	
}

/**
	
    @brief Получить положение текстового поля по оси Y
	
    @return положение от начала координат по оси Х в пикселях
	
*/

uint16_t Text::getYPosition()
{
	
	return yPosition;
	
}

/**
	
    @brief Получить максимальную ширину текстового поля в пикселях
	
    @return максимальная ширина текстового поля в пикселях
	
*/

uint16_t Text::getMaximumLength()
{
	
	return maximumLength;
	
}

/**
	
    @brief Возвращает длину текста в символах
	
    @return длина текста в символах utf8
	
*/

uint16_t Text::getLength()
{
	
	uint16_t textLengthInSymbols;
	
	textLengthInSymbols = 0;
	
    for( uint16_t i = 0 ; i < length ; i++ )
    {
		
        if( value[i] >= START_UNICODE )
        {
			
			i += 1;
			
            if( i <= length )
            {
				textLengthInSymbols += 1;
			}
			
        }
        else
        {
			textLengthInSymbols += 1;
		}
		
	}
	
	return textLengthInSymbols;
	
}

/**
	
    @brief Возвращает символ/букву по индексу
	
    @param index индекс/номер символа от 0 до getLength()-1
	
    @return символ в кодировке utf8
	
*/

uint16_t Text::at( uint16_t index )
{
	
    if( index > getLength() )
    {
		
	}
	
	// L L UB0 UB1 L L L UB0 UB1 UB0 UB1 L 
	
    for( uint16_t i = 0, j = 0 ; i < length ; i++, j++ )
    {
		
        if( value[i] >= START_UNICODE )
        {
			
            if( j >= index )
            {
			
				uint16_t symbol;
				
				symbol = ( value[i+1] ) | ( value[i] << 8 );
				
				return symbol;
					
            }
            else
            {
				
				i += 1;
				
			}
			
        }
        else
        {
			
            if( j >= index )
            {
				
				uint16_t symbol;
				
				symbol = value[i];
				
				return symbol;
				
			}
			
		}
		
	}

    return 0;

}
