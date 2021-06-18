#include "oled12864.h"


OLED_12864::OLED_12864()
{
    
}

OLED_12864::~OLED_12864()
{
    
}

void OLED_12864::qrcode(uint32_t x, uint32_t y, const String& str, uint8_t scale)
{
    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(3)];
    uint8_t ex, ey;
    uint8_t _brushWidth = brushWidth;
    int16_t _brushColor = brushColor;
    qrcode_initText(&qrcode, qrcodeData, 3, 0, str.c_str());
    lineWidth(1);
    for (uint8_t _y = 0; _y < qrcode.size; _y++) {
        // Each horizontal module
        for (uint8_t _x = 0; _x < qrcode.size; _x++) {
            if(qrcode_getModule(&qrcode, _x, _y))
                setBrushColor(1);
            else
                setBrushColor(0);
            rect(_x*scale + x ,_y*scale + y, scale, scale);
            ex = _x*scale + x + scale;
            ey = _y*scale + y + scale;
        }
        Serial.print("\n");
    }
    regionalRefresh(x, y, ex-x+1, ey-y+1);
    setBrushColor(_brushColor);
    lineWidth(_brushWidth);
}

void OLED_12864::setLineWidth(uint32_t w)
{
    lineWidth(w);
}

uint32_t OLED_12864::getLineWidth()
{
    return brushWidth;
}

void OLED_12864::rect(int32_t x, int32_t y, int32_t width, int32_t height, bool fill)
{
    int backspace = brushWidth/2 + brushWidth%2 - 1;
    if(width < 0)
    {
        x += width;
        width = -width;
    }
    if(height < 0)
    {
        y += height;
        height = -height;
    }
    if(width == 0 && height == 0){
        return;
    }else if(width == 0){
        for(int i=0; i<brushWidth; i++) 
            VLineOnePixel(x-backspace+i, y-backspace, height+backspace*2);
    }else if(height == 0){
        for(int i=0; i<brushWidth; i++) 
            HLineOnePixel(x-backspace, y-backspace+i, width+backspace*2);
    }else if(fill){
        for(int i=0; i<height+2*backspace; i++)
            HLineOnePixel(x-backspace, y-backspace+i, width+backspace*2);
    }else{
        for(int i=0; i<brushWidth; i++) 
            HLineOnePixel(x-backspace, y-backspace+i, width+backspace*2);
        for(int i=0; i<brushWidth; i++) 
            HLineOnePixel(x-backspace, y+height-1-brushWidth/2+i, width+backspace*2);
        for(int i=0; i<brushWidth; i++) 
            VLineOnePixel(x-backspace+i, y-backspace, height+backspace*2);
        for(int i=0; i<brushWidth; i++) 
            VLineOnePixel(x+width-1-brushWidth/2+i, y-backspace, height+backspace*2);
    }
}

