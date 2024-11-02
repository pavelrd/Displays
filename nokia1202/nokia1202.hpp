#ifndef LCD_NOKIA_1202_H
#define LCD_NOKIA_1202_H

#include "text.hpp"
#include "font.hpp"

#include "io.hpp"
#include "delay.hpp"

typedef uint8_t tDisplaySize;

/**
	
    @brief Работа с дисплеем nokia1202.
	
	Координаты дисплея идут от левого верхего угла в правый нижний угол.
	
                        Y0  Y1  Y2 ... ... ... ... ...  Y(LCD_RESOLUTION_Y-1) 
                       ___ ___ ___ ___ ___ ___ ___ ___ ___ 
                    X0| .   .   .   .   .   .   .   .   .
	                X1| .   .   .   .   .   .   .   .   .
	                X2| .   .   .   .   .   .   .   .   .
	               ...| .   .   .   .   .   .   .   .   .
	               ...| .   .   .   .   .   .   .   .   .
	               ...| .   .   .   .   .   .   .   .   .
	               ...| .   .   .   .   .   .   .   .   . 
				   ...| .   .   .   .   .   .   .   .   .
X(LCD_RESOLUTION_X-1) | .   .   .   .   .   .   .   .   .
                      --------------------------------------
					  
    Пример работы:
		
		//
		// Программа показывает на дисплее текст "Привет мир!" с отступом от левого верхего угла в 20 пикселей по обоим осям. 
		//  и завершает работу с дисплеем.
		//
		// Дисплей подключен следующим образом:
		//
		//   D3 ---> SCLK
		//   D4 ---> SDA
		//   D5 ---> CS
		//   D6 ---> RST
		//
		
        #include "nokia1202.hpp"
		
        Font defaultFont;
		
        LCDNokia1202 display;
		
        void setup(){

            display = new LCDNokia1202( IO::D3, IO::D4, IO::D5, IO::D6 );
		
            display.init();
			
            display.setFont(&defaultFont);
			
			Text *mytext = new Text( 20, 20, "Привер мир!" );
			
            display.clear();
			
            display.showText(mytext);
			
			delete mytext;
			
            display.sendFrame();
			
            display.deinit();
			
			delete display;
			
			while(1);
		
		}
		
*/

class LCDNokia1202
{

public:

    LCDNokia1202( IO::PIN  clock,
                  IO::PIN  serialDataOut,
                  IO::PIN  chipSelect,
                  IO::PIN  reset
				 );
				 
    LCDNokia1202( IO::PIN  clock,
                  IO::PIN  serialDataOut,
                  IO::PIN  chipSelect,
                  IO::PIN  reset,
				  bool     mirrorHorizontal,
				  bool     mirrorVertical
				 );
				 
	LCDNokia1202();
	
	virtual ~LCDNokia1202();
	
	void setFont( Font *_font );
	void unsetFont();
	
	void init();
	void deinit();
	
	void drawLine( tDisplaySize xStartPosition, tDisplaySize yStartPosition, tDisplaySize xEndPosition, tDisplaySize yEndPosition );

	void setPixel( tDisplaySize xPosition, tDisplaySize yPosition );
	
	void clearPixel( tDisplaySize xPosition, tDisplaySize yPosition );
	
	void showText( Text *text );
	
	void clear();
					
	void sendFrame();
	
	void setNoDelayWhenSendFrame();
	
	void unsetNoDelayWhenSendFrame();

    enum RESOLUTIONS
    {
		LCD_RESOLUTION_X  = 68, //
        LCD_RESOLUTION_Y  = 96  // must be higher and multiply COLUMN_BIT_VOLUME
	};
	
protected:


    typedef struct
    {
        IO::PIN clock;
        IO::PIN serialDataOut;
        IO::PIN chipSelect;
        IO::PIN reset;
	} Lines;

private:

	bool mirrorHorizontal;

	bool mirrorVertical; 
	
	bool nodelay;
	
    enum RESOLUTION_BITS
    {
		COLUMN_BIT_VOLUME =  8	
	};

    enum COMMANDS
    {
		COMMAND_RESET                  = 0xE2,
		COMMAND_NOP                    = 0xE3,
		COMMAND_OFF	                   = 0xAE,
		COMMAND_ON	                   = 0xAF,
		COMMAND_NORMAL_MODE            = 0xA4,
		COMMAND_ALL_POINTS_ON          = 0xA5,
		COMMAND_POWER_ON               = 0x2F,
		COMMAND_POWER_OFF              = 0x28, // booster,voltage regulator, voltage follower
		COMMAND_SET_CONTRAST           = 0xE1,
		COMMAND_DEFAULT_CONTRAST       = 0x16,
		COMMAND_SET_VOLUME             = 0x80,
		COMMAND_VOLUME_MAX             = 0x9F,
		COMMAND_SET_CHARGE_PUMP_MUL    = 0x3D,
		COMMAND_PUMP_UNUSED	           = 0x03,
		COMMAND_PUMP_X3                = 0x02,
		COMMAND_PUMP_X4                = 0x01,
		COMMAND_PUMP_X5                = 0x00,
		COMMAND_SET_ADDRESS_0          = 0x40,
		COMMAND_MAX_ADDRESS            = 0x7F,
		COMMAND_PAGE_ADDRESS_0         = 0xB0,
		COMMAND_PAGE_MAX_ADDRESS       = 0xB8,
		COMMAND_COLUMN_ADDRESS_0       = 0x00,
		COMMAND_COLUMN_MAX_ADDRESS     = 0x5F,
		COMMAND_NORMAL_TOP_DOWN_VIEW   = 0xC0,
		COMMAND_MIRROR_TOP_DOWN_VIEW   = 0xC8,
		COMMAND_NORMAL_LEFT_RIGHT_VIEW = 0xC0,
		COMMAND_MIRROR_LEFT_RIGHT_VIEW = 0xA1
	};

	Font *font;
	
	uint8_t displayBuffer[LCD_RESOLUTION_X][LCD_RESOLUTION_Y/COLUMN_BIT_VOLUME];
		
	Lines lines;
	
	void _writeData( uint8_t dataByte ); 
	void _writeCommand( uint8_t command );
	void _write( uint8_t dataByte );
		
};

#endif
