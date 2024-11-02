#include "color.hpp"

Color::Color()
{
    r = 0;
    g = 0;
    b = 0;
}

Color::Color(const char* hex)
{

    for(int i=0; i<6; i++)
    {
        hexcode[i] = hex[i];
    }

    hexcode[6] = '\0';

    r = hex2int(hex[0]) * 16 + hex2int(hex[1]);
    g = hex2int(hex[2]) * 16 + hex2int(hex[3]);
    b = hex2int(hex[4]) * 16 + hex2int(hex[5]);

    calculate_cc565();

}

Color::Color(uint16_t _cc565)
{
    cc565 = _cc565;
}

uint8_t Color::hex2int(char c)
{
    if(c >= 48 && c <= 57)
    {
        return c - 48;
    }
    if(c >= 65 && c <= 70)
    {
        return c - 55;
    }
    if(c >= 97 && c <= 102)
    {
        return c - 87;
    }

    return 0;
}

void Color::calculate_cc565()
{

    cc565 = 0;

    uint16_t j = 15;

    for( int8_t i=7; i>=3; --i )
    {
        if( r & (1 << i))
        {
            this->cc565 |= (1 << j);
        }
        --j;
    }

    for(int8_t i=7; i>=2; --i)
    {
        if( g & (1 << i))
        {
            cc565 |= (1 << j);
        }
        --j;
    }

    for( int8_t i=7; i>=3; --i )
    {
        if( b & (1 << i) )
        {
            cc565 |= (1 << j);
        }
        --j;
    }
}
