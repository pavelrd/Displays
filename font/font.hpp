#ifndef FONT_H
#define FONT_H

#include <stdint.h>

/**
	
    @brief Класс с русским и английским шрифтами
	
*/

class Font
{

public:

    Font();

    Font( uint8_t _size );

	const uint8_t* getSymbolFromUtf8Code( uint16_t utf8Code );

    void    setSize( uint8_t _size );
    uint8_t getSize();
    uint8_t getSymbolWidth();
    uint8_t getSymbolHeight();
    uint8_t getHorizontalSpacing();
    uint8_t getVerticalSpacing();

    uint8_t size;

private:
	
    enum SYMBOL_CODES
    {
		CYRILLIC_CAPITAL_LETTER_A    = 53392,
		CYRILLIC_CAPITAL_LETTER_YA   = 53423, // я
		CYRILLIC_SMALL_LETTER_A      = 53424,
		CYRILLIC_SMALL_LETTER_PE     = 53439, // п
		CYRILLIC_SMALL_LETTER_ER     = 53632, // р
		CYRILLIC_SMALL_LETTER_YA     = 53647,
		CYRILLIC_CAPITAL_LETTER_IO   = 53377,// Ё 
		CYRILLIC_SMALL_LETTER_IO     = 53649,// ё
		SPACE_SYMBOL_CODE            = 32,
		DELETE_SYMBOL_CODE           = 127
	};

    enum FONT_OFFSETS
    {
		FONT_SYMBOLS_COUNT          = 160, //  number of chars	
		UTF8_START_ASCII_OFFSET     =  0,
		CYRILLIC_UPPER_CASE_OFFSET  = 96,
		CYRILLIC_LOWER_CASE_OFFSET  = 128,
		CYRILLIC_SMALL_ER_OFFSET    = 144,
		UNKNOWN_SYMBOL_OFFSET       = 13,
		EXTENDED_SYMBOLS_OFFSET     = 162,
		CYRILLIC_CAPITAL_IO_OFFSET  = 160, 
		CYRILLIC_SMALL_IO_OFFSET    = 161,
        SYMBOL_SIZE_IN_BYTES        = 5,
        WIDTH_IN_DOTS               = 0x05,
        HEIGHT_IN_DOTS              = 0x08,
        HORIZONTAL_SPACING          = 0x01,
        VERTICAL_SPACING            = 0x00
	};
	
    enum FONT_VALUES
    {
		COLUMN_BIT_SHIFT = 3, // 2^(COLUMN_BIT_SHIFT) must be equal COLUMN_BIT_VOLUME
		TEXT_X_SIZE      = 8, // font size X
        TEXT_Y_SIZE      = 5, // font size Y //           0001 1111  one bits, right to left
	};
		
	static const uint8_t font[FONT_SYMBOLS_COUNT][SYMBOL_SIZE_IN_BYTES];
	
};


#endif
