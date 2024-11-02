#ifndef _COLOR_H
#define _COLOR_H

#include "platform.hpp"

class Color
{

public:

    Color();

    Color(const char* hex);

    Color(uint16_t _cc565);

    inline uint16_t get_cc565() const
    {
        return cc565;
    }

    inline const char* get_hexcode() const
    {
        return hexcode;
    }

private:

    uint8_t r;
    uint8_t g;
    uint8_t b;

    char hexcode[7];

    uint16_t cc565;

    uint8_t hex2int(char c);

    void calculate_cc565();

};

#endif // _COLOR_H
