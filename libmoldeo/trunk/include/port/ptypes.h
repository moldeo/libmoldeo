#ifndef _PTYPES_H_
#define _PTYPES_H_

typedef unsigned short int BYTE;

struct RGBTRIPLE 
{
    BYTE   rgbtBlue;
    BYTE   rgbtGreen;
    BYTE   rgbtRed;
    RGBTRIPLE(BYTE r, BYTE g, BYTE b)
      : rgbtBlue(b), rgbtGreen(g), rgbtRed(r) {}
};

#endif
 
