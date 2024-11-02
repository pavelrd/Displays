#ifndef TEXT_H
#define TEXT_H

#include <stdint.h>
#include <string.h>

/**
	
    @brief Хранит текст в кодировке utf-8. Используется для вывода текста на дисплей.
	
*/

class Text
{

public:

	Text( const char* string );
	Text( uint16_t _xPosition, uint16_t _yPosition, const char* string );
    Text( uint16_t _xPosition, uint16_t _yPosition, uint16_t _maximumLength, const char* string );

	uint16_t getXPosition();
	uint16_t getYPosition();
	uint16_t getMaximumLength();
	
	uint16_t getLength();
	uint16_t at( uint16_t index );

    void setValue( const char* string );

private:
	
	uint8_t value[100];
	
    enum START_UNICODE_CODE
    {
        START_UNICODE = 128
    };
	
    enum TEXT_MAXIMUMS
    {
		TEXT_SIZE_MAX   = 65535
	};
	
	uint16_t xPosition;
	uint16_t yPosition;
	
	uint16_t maximumLength;
	
	uint16_t   length;
	
};


#endif
