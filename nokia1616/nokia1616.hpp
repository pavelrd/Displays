#ifndef LCD_NOKIA_1202_H
#define LCD_NOKIA_1202_H

#include "text.hpp"
#include "font.hpp"
#include "io.hpp"
#include "delay.hpp"

typedef uint8_t tDisplaySize;

/**
	
    @brief Работа с дисплеем nokia1616.
					  
	Пример работы под Arduino:
		
		//
		// Программа показывает на дисплее текст "Привет мир!" с отступом от левого верхего угла в 5 пикселей по обоим осям. 
		//  и завершает работу с дисплеем.
		//
		// Дисплей подключен следующим образом:
		//
		//   D3 ---> SCLK
		//   D4 ---> SDA
		//   D5 ---> CS
		//   D6 ---> RST
		//   D7 ---> Light
		//
		
		#include "nokia1616.hpp"
		
		Font font(1); // шрифт единичного размера
		
        LCD1616 display( IO::D3, IO::D4, IO::D5, IO::D6, IO::D7 );
		
        void setup(){
		
			display.init();
			
			display.setFont(font);
			
			display.setBackgroundColor(LCD1616::WHITE);
			
			display.clear();
				
			Text mytext( 0, 0, 10, "Привет мир!" );
	
			display.showText( mytext, LCD1616::BLACK );
	
			display.deinit();
				
			while(1);
			
		}
		
	Координаты дисплея идут от левого верхего угла в правый нижний угол.
	
                        Y0  Y1  Y2 ... ... ... ... ...  Y(getWidth()-1) 
                       ___ ___ ___ ___ ___ ___ ___ ___ ___ 
                    X0| .   .   .   .   .   .   .   .   .
	                X1| .   .   .   .   .   .   .   .   .
	                X2| .   .   .   .   .   .   .   .   .
	               ...| .   .   .   .   .   .   .   .   .
	               ...| .   .   .   .   .   .   .   .   .
	               ...| .   .   .   .   .   .   .   .   .
	               ...| .   .   .   .   .   .   .   .   . 
				   ...| .   .   .   .   .   .   .   .   .
X(getHeight()-1)      | .   .   .   .   .   .   .   .   .
                      --------------------------------------
					  
*/

class LCD1616
{

public:
	
	LCD1616();
				 
    LCD1616( IO::PIN  clock,
             IO::PIN  serialDataOut,
             IO::PIN  chipSelect,
             IO::PIN  reset,
             IO::PIN  light
           );
				 
	virtual ~LCD1616();
	
	void setFont( Font &_font );
	void unsetFont();
	
	void init();
	void deinit();
	
	typedef uint16_t color_t;
	
	void setBackgroundColor( color_t color );

	void clear();
	
	void showText( Text &text, uint16_t color );
	void showChar( uint32_t x, uint32_t y, uint16_t color, uint16_t symbol );

    enum orientation_t
    {
		ORIENTATION_NORMAL=0,
		ORIENTATION_90,
		ORIENTATION_180,
		ORIENTATION_270 
	};
	
	void setOrientation( orientation_t orientation );
	
	uint8_t getFontSize();
	void setFontSize(uint8_t _fontSize);
	
	uint8_t getWidth();
	uint8_t getHeight();
	
    enum COLORS
    {
		BLACK   = 0x0000,
		WHITE   = 0xFFFF,
		RED     = 0xF800,
		GREEN   = 0x1FE0,
		BLUE    = 0x001F,
		SILVER  = 0xC618,
		GRAY    = 0x8410,
		MAROON  = 0x8000,
		YELLOW  = 0xFFE0,
		OLIVE   = 0x8400,
		LIME    = 0x07E0,
		AQUA    = 0x07FF,
		TEAL    = 0x0410,
		NAVY    = 0x0010,
		FUCHSIA = 0xF81F,
		PURPLE  = 0x8010
	};
	
protected:
	
    typedef struct
    {
        IO::PIN clock;
        IO::PIN serialDataOut;
        IO::PIN chipSelect;
        IO::PIN reset;
        IO::PIN light;
	} Lines;
	
private:

	Font* font;
		
	Lines lines;

	uint8_t fontSize;
	
	uint16_t backgroundColor;
	
	uint8_t orientation;
	
    enum SIZES
    {
		WIDTH             =	129,
		HEIGHT         	  = 161,
		LCD_CTRL_X_OFFSET = 2,
		LCD_CTRL_Y_OFFSET = 1
	};
	
    enum RESOLUTION_BITS
    {
		COLUMN_BIT_VOLUME =  8	
	};

    enum COMMANDS
    {

        SEND_CMD            = 0,
        SEND_DATA	        = 0x100,

        CMD_NOP		        = 0x00,
        CMD_SLPOUT	        = 0x11,
        CMD_NORON	        = 0x13,

        CMD_INVOFF	        = 0x20,
        CMD_DISPON	        = 0x29,
        COLUMN_ADDRESS_SET  = 0x2A,
        ROW_ADDRESS_SET	    = 0x2B,
        MEMORY_WRITE	    = 0x2C,
        CMD_RGBSET	        = 0x2D,

        CMD_MADCTR	        = 0x36,
        CMD_VSCSAD	        = 0x37,
        CMD_COLMOD	        = 0x3A,

        CMD_COLMOD_MCU12bit	= 3,        // MCU interface 12bit
        CMD_COLMOD_MCU16bit	= 5,        // MCU interface 16bit
        CMD_COLMOD_MCU18bit	= 6,        // MCU interface 18bit
        CMD_COLMOD_RGB16bit	= 50,       // RGB interface 16bit
        CMD_COLMOD_RGB18bit	= 60,       // RGB interface 18bit

        CMD_MADCTR_MY       = (1 << 7),	// Row Address Order
        CMD_MADCTR_MX       = (1 << 6),	// Column Address Order
        CMD_MADCTR_MV       = (1 << 5),	// Row/Column Exchange
        CMD_MADCTR_ML       = (1 << 4),	// Vertical Refresh Order
        CMD_MADCTR_RGB      = (1 << 3)	// RGB-BGR ORDER
		 
	};

	void _setWindow( uint8_t x, uint8_t y, uint8_t w, uint8_t h );
    void _sendPixel( uint16_t color );

	void _senPixels( uint16_t color, uint16_t n);
    void _sendCmd( uint8_t data);
    void _cmd1616( uint8_t cmd, uint16_t a, uint16_t b );
    void _sendData( uint8_t data );
    void _send( uint16_t data );
	

	
};

#endif
