// https://www.waveshare.com/wiki/0.96inch_OLED_Module

#include "OLED_Driver.h"
#include "GUI_paint.h"
#include "DEV_Config.h"
#include "Debug.h"
#include "ImageData.h"
#include "MyImage.h"
#include <avr/pgmspace.h>

UBYTE *ImageBuffer;

void setup()
{
  System_Init();
  Serial.print(F("OLED_Init()...\r\n"));
  OLED_0in96_Init();
  OLED_0in96_clear();


  UWORD Imagesize = ((OLED_0in96_WIDTH % 8 == 0) ? (OLED_0in96_WIDTH / 8) : (OLED_0in96_WIDTH / 8 + 1)) * OLED_0in96_HEIGHT;
  if ((ImageBuffer = (UBYTE *)malloc(Imagesize)) == NULL)
  {

    Serial.print(F("Failed to apply for black memory...\r\n"));
    display_freeram();
    return;
  }

  Serial.print(F("Paint_NewImage\r\n"));
  Paint_NewImage(ImageBuffer, OLED_0in96_WIDTH, OLED_0in96_HEIGHT, 90, BLACK);
  Paint_SetMirroring(MIRROR_VERTICAL);
  //Paint_Clear(BLACK);
  //Driver_Delay_ms(500);
}

void loop()
{
  const int d = 100;
  if(ImageBuffer == NULL)
  {
    delay(100000);
    return;
  }
  drawBlank();
  Driver_Delay_ms(d);

  drawShapes();
  Driver_Delay_ms(d);

  drawText();
  Driver_Delay_ms(d);

  drawCN();
  Driver_Delay_ms(d);

  drawImage();
  Driver_Delay_ms(d);

  OLED_0in96_clear();
}

void drawBlank()
{
  Serial.print(F("Drawing: blank page\r\n"));
  Paint_Clear(WHITE);
  OLED_0in96_display(ImageBuffer);
}

void drawShapes()
{
  Serial.print(F("Drawing: shapes\r\n"));
  Paint_Clear(BLACK);
  Paint_DrawPoint(20, 10, WHITE, DOT_PIXEL_1X1, DOT_STYLE_DFT);
  Paint_DrawPoint(30, 10, WHITE, DOT_PIXEL_2X2, DOT_STYLE_DFT);
  Paint_DrawPoint(40, 10, WHITE, DOT_PIXEL_3X3, DOT_STYLE_DFT);
  Paint_DrawLine(10, 10, 10, 20, WHITE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
  Paint_DrawLine(20, 20, 20, 30, WHITE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
  Paint_DrawLine(30, 30, 30, 40, WHITE, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
  Paint_DrawLine(40, 40, 40, 50, WHITE, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
  Paint_DrawCircle(60, 30, 15, WHITE, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
  Paint_DrawCircle(100, 40, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
  Paint_DrawRectangle(50, 30, 60, 40, WHITE, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
  Paint_DrawRectangle(90, 30, 110, 50, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
  Paint_DrawNum(10, 43, "1", &Font24, 0, WHITE, WHITE);
  OLED_0in96_display(ImageBuffer);
}

void drawText()
{
  Serial.print(F("Drawing: text\r\n"));
  Paint_Clear(BLACK);
  Paint_DrawString_EN(10, 0, "waveshare", &Font16, WHITE, WHITE);
  Paint_DrawString_EN(10, 17, "hello world", &Font8, WHITE, WHITE);
  Paint_DrawNum(10, 30, "123.456789", &Font8, 4, WHITE, WHITE);
  Paint_DrawNum(10, 43, "987654", &Font12, 5, WHITE, WHITE);
  OLED_0in96_display(ImageBuffer);
}

void drawCN()
{
  Serial.print(F("Drawing: cn\r\n"));
  Paint_Clear(BLACK);
  Paint_DrawString_CN(10, 0, "你好Abc", &Font12CN, WHITE, WHITE);
  Paint_DrawString_CN(0, 20, "微雪电子", &Font24CN, WHITE, WHITE);
  OLED_0in96_display(ImageBuffer);
}

void drawImage()
{
  Serial.print(F("Drawing: image\r\n"));
  OLED_0in96_display_Array(gImage_0in96);
  Paint_Clear(BLACK);
  Paint_DrawBitMap_PM(imgData_0in96);
  OLED_0in96_display(ImageBuffer);
}

void display_freeram() {
  Serial.print(F("- SRAM left: "));
  Serial.println(freeRam());
}

int freeRam() {
  extern int __heap_start,*__brkval;
  int v;
  return (int)&v - (__brkval == 0  
    ? (int)&__heap_start : (int) __brkval);  
}

void Paint_DrawBitMap_PM(const unsigned char* image_buffer)
{
    UWORD x, y;
    UDOUBLE Addr = 0;

    for (y = 0; y < Paint.HeightByte; y++) {
        for (x = 0; x < Paint.WidthByte; x++) {//8 pixel =  1 byte
            Addr = x + y * Paint.WidthByte;
            Paint.Image[Addr] = pgm_read_byte(image_buffer+Addr); 
            
        }
    }
}