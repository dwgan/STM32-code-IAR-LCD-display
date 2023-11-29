#ifndef __GUI_H__
#define __GUI_H__


#include "tft.h"
#include "usart.h"
#include "delay.h"

void GUI_DrawPoint(u16 x,u16 y,u16 color);
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void Draw_Circle(u16 x0,u16 y0,u16 fc,u8 r);
void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode);
void LCD_ShowNum2412(u16 x,u16 y,u16 fc, u16 bc,u8 *p ,u8 size,u8 mode);
u8 LCD_ShowNum(u16 x, u16 y, u16 fc, u16 bc, u32 num, u8 size, u8 mode);
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len,u8 size,u8 mode);
void LCD_ShowString(u16 x, u16 y, u8 *p, u8 size, u8 mode);
void GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, char *s,u8 mode);
void GUI_DrawFont24(u16 x, u16 y, u16 fc, u16 bc, char *s,u8 mode);
void GUI_DrawFont32(u16 x, u16 y, u16 fc, u16 bc, char *s,u8 mode);
void Show_Str(u16 x, u16 y, u16 fc, u16 bc, char *str, u8 size,u8 mode);
void Gui_Drawbmp16(u16 start_x, u16 start_y, u16 length, u16 width, const unsigned char *p);
void gui_circle(int xc, int yc,u16 c,int r, int fill);
void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, char *str,u8 size,u8 mode);
void tft_printf(const u8 a, ...);
int my_printf(char* str,...);
void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
#endif

