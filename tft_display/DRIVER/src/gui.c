#include <stdarg.h>
#include "string.h"
#include "font.h"
#include "picture.h"
#include "delay.h"
#include "tft.h"
#include "gui.h"



/*画笔颜色,背景颜色*/
u16 POINT_COLOR = RED, BACK_COLOR = YELLOW;
//u16 DeviceCode;
/****************************************************************************
* 名    称： GUI_DrawPoint(u16 x,u16 y,u16 color)
* 功    能： GUI描绘一个点
* 入口参数： x:光标位置x坐标
*            y:光标位置y坐标
*            color:要填充的颜色
* 出口参数： 无
* 说    明：
****************************************************************************/
void GUI_DrawPoint(u16 x, u16 y, u16 color)
{
	LCD_SetCursor(x, y);     //设置光标位置
	LCD_WR_DATA_16Bit(color);
}
/****************************************************************************
* 名    称： LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
* 功    能： 在指定区域内填充颜色
* 入口参数： sx:指定区域开始点x坐标
*            sy:指定区域开始点y坐标
*            ex:指定区域结束点x坐标
*            ey:指定区域结束点y坐标
*           color:要填充的颜色
* 出口参数： 无
* 说    明：
****************************************************************************/
void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color)
{
	u16 i,j;
	u16 width = ex - sx + 1; 		//得到填充的宽度
	u16 height = ey - sy + 1;		//高度
	LCD_SetWindows(sx, sy, ex - 1, ey - 1);//设置显示窗口
	for (i = 0; i < height; i ++)
	{
		for (j = 0; j < width; j ++)
            LCD_WR_DATA_16Bit(color);	//写入数据
	}
	LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1);//恢复窗口设置为全屏
}

/****************************************************************************
* 名    称： LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
* 功    能： GUI画线
* 入口参数： x1,y1:起点坐标
*            x2,y2:终点坐标
* 出口参数： 无
* 说    明：
****************************************************************************/
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;

    delta_x = x2 - x1;                   //计算坐标增量
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
    if (delta_x > 0)
        incx = 1;                        //设置单步方向
    else if (delta_x == 0)
        incx = 0;                        //垂直线
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0;                        //水平线
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)
        distance = delta_x;              //选取基本增量坐标轴
    else
        distance = delta_y;
    for (t = 0; t <= distance + 1; t ++ )//画线输出
    {
        LCD_DrawPoint(uRow, uCol);       //画点
        xerr += delta_x ;
        yerr += delta_y ;
        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

/****************************************************************************
* 名    称： LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
* 功    能： GUI画矩形(非填充)
* 入口参数： (x1,y1),(x2,y2):矩形的对角坐标
* 出口参数： 无
* 说    明：
****************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
    LCD_DrawLine(x1, y1, x2, y1);
    LCD_DrawLine(x1, y1, x1, y2);
    LCD_DrawLine(x1, y2, x2, y2);
    LCD_DrawLine(x2, y1, x2, y2);
}

/****************************************************************************
* 名    称： LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
* 功    能： GUI画矩形(填充)
* 入口参数： (x1,y1),(x2,y2):矩形的对角坐标
* 出口参数： 无
* 说    明：
****************************************************************************/
void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_Fill(x1,y1,x2,y2,POINT_COLOR);
}

/****************************************************************************
* 名    称： _draw_circle_8(int xc, int yc, int x, int y, u16 c)
* 功    能： 8对称性画圆算法(内部调用)
* 入口参数： (xc,yc) :圆中心坐标
*            (x,y):光标相对于圆心的坐标
*            c:填充的颜色
* 出口参数： 无
* 说    明：
****************************************************************************/
void _draw_circle_8(int xc, int yc, int x, int y, u16 c)
{
    GUI_DrawPoint(xc + x, yc + y, c);

    GUI_DrawPoint(xc - x, yc + y, c);

    GUI_DrawPoint(xc + x, yc - y, c);

    GUI_DrawPoint(xc - x, yc - y, c);

    GUI_DrawPoint(xc + y, yc + x, c);

    GUI_DrawPoint(xc - y, yc + x, c);

    GUI_DrawPoint(xc + y, yc - x, c);

    GUI_DrawPoint(xc - y, yc - x, c);
}

/****************************************************************************
* 名    称： gui_circle(int xc, int yc,u16 c,int r, int fill)
* 功    能： 在指定位置画一个指定大小的圆(填充)
* 入口参数： (xc,yc) :圆中心坐标
*             c:填充的颜色
*             r:圆半径
*             fill:填充判断标志，1-填充，0-不填充
* 出口参数： 无
* 说    明：
****************************************************************************/
void gui_circle(int xc, int yc,u16 c,int r, int fill)
{
	int x = 0, y = r, yi, d;
	d = 3 - 2 * r;
	if (fill)           // 如果填充（画实心圆）
	{
		while (x <= y)
        {
			for (yi = x; yi <= y; yi++)
				_draw_circle_8(xc, yc, x, yi, c);
			if (d < 0)
            {
				d = d + 4 * x + 6;
			}
            else
            {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
    else      // 如果不填充（画空心圆）
	{
		while (x <= y)
        {
			_draw_circle_8(xc, yc, x, y, c);
			if (d < 0)
            {
				d = d + 4 * x + 6;
			}
            else
            {
				d = d + 4 * (x - y) + 10;
				y --;
			}
			x ++;
		}
	}
}

/****************************************************************************
* 名    称： LCD_ShowChar(u16 x, u16 y, u16 fc, u16 bc, u8 num, u8 size, u8 mode)
* 功    能： 显示单个英文字符
* 入口参数： (x,y):字符显示位置起始坐标
*             fc:前置画笔颜色
*             bc:背景颜色
*             num:数值（0-94）
*            size:字体大小(12/16)
*            mode:模式  0,填充模式;1,叠加模式
* 出口参数： 无
* 说    明：
****************************************************************************/
void LCD_ShowChar(u16 x, u16 y, u16 fc, u16 bc, u8 num, u8 size, u8 mode)
{
    u8 temp;
    u8 pos, t;
	u16 colortemp = POINT_COLOR;
	num = num - ' ';//得到偏移后的值
	LCD_SetWindows(x, y, x + size / 2 - 1, y + size - 1);//设置单个文字显示窗口
	if (!mode) //非叠加方式
	{
        if (size == 12)
        {
            for (pos = 0; pos < size; pos ++)
            {
                temp = asc2_1206[num][pos];//调用1206字体
                for (t = 0; t < size / 2; t ++)
                {
                    if (temp & 0x01)
                    {
                        LCD_WR_DATA_16Bit(fc);
                    }
                    else
                    {
                        LCD_WR_DATA_16Bit(bc);
                    }
                    temp >>= 1;
                }
            }
        }
        else
        {
            for (pos = 0; pos < size; pos ++)
            {
                temp = asc2_1608[num][pos];//调用1608字体
                for (t = 0; t < size / 2; t ++)
                {
                    if (temp & 0x01)
                    {
                        LCD_WR_DATA_16Bit(fc);
                    }
                    else
                    {
                        LCD_WR_DATA_16Bit(bc);
                    }
                    temp >>= 1;
                }
            }
        }
	}
    else //叠加方式
    {
        if (size == 12)
        {
            for (pos = 0; pos < size; pos ++)
            {
                temp = asc2_1206[num][pos];//调用1206字体
                for (t = 0; t < size / 2; t ++)
                {
                    POINT_COLOR = fc;
                    if (temp & 0x01)
                    {
                        LCD_DrawPoint(x + t, y + pos);//画一个点
                    }
                    temp >>= 1;
                }
            }
        }
		else
        {
            for (pos = 0; pos < size; pos ++)
            {
                temp = asc2_1608[num][pos];		 //调用1608字体
                for (t = 0; t < size / 2; t ++)
                {
                    POINT_COLOR = fc;
                    if (temp & 0x01)
                    {
                        LCD_DrawPoint(x + t, y + pos);//画一个点
                    }
                    temp >>= 1;
                }
            }
        }
	}
	POINT_COLOR = colortemp;
	LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1);//恢复窗口为全屏
}

/****************************************************************************
* 名    称： LCD_ShowNum2412(u16 x,u16 y,u16 fc, u16 bc,u8 *p ,u8 size,u8 mode)
* 功    能： 显示单个英文字符
* 入口参数： (x,y):字符显示位置起始坐标
*             fc:前置画笔颜色
*             bc:背景颜色
*             num:数值（0-94）
*            size:字体大小
*            mode:模式  0,填充模式;1,叠加模式
* 出口参数： 无
* 说    明：
****************************************************************************/
void LCD_ShowNum2412(u16 x,u16 y,u16 fc, u16 bc,u8 *p ,u8 size,u8 mode)
{
    u16 temp;
    u8 pos, t;
	u16 colortemp = POINT_COLOR;
	u16 x0 = x;
	u16 y0 = y;
	u8 num = 0;
    while ((*p <= '~') && (*p >= ' '))//判断是不是非法字符!
    {
		if (x > (lcddev.width - 1) || y > (lcddev.height - 1))
            return;
		num = *p;
		if (':' == num) //特殊字符":"
            num = 10;
		else if ('.' == num)//特殊字符"."
            num = 11;
		else  //纯数字
            num = num - '0';
		x0 = x;
		for (pos = 0; pos < 48; pos ++)
		{
			temp = asc2_2412[num][pos];
			for (t = 0; t < 8; t ++)
		    {
				POINT_COLOR = fc;
		        if (temp & 0x80)
                    LCD_DrawPoint(x, y);//画一个点
				//else LCD_WR_DATA_16Bit(bc);
		        temp <<= 1;
				x ++;
				if ((x - x0) == 12)
				{
					x = x0;
					y ++;
					break;
				}
		    }
		}
        if (num < 10)
            x += 16;	//人为控制字距，使得排版更好看，原值为12
        else
            x += 8; //人为控制字距，使得排版更好看，原值为12

        y = y0;
        p ++;
    }
	POINT_COLOR = colortemp;
}

/****************************************************************************
* 名    称： LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u8 mode)
* 功    能： 显示英文字符串
* 入口参数： x,y :起点坐标
*            size:字体大小
*            *p:字符串起始地址
*            mode:模式	0,填充模式;1,叠加模式
* 出口参数： 无
* 说    明：
****************************************************************************/
void LCD_ShowString(u16 x, u16 y, u8 *p, u8 size, u8 mode)
{
    while ((*p <= '~') && (*p >= ' '))//判断是不是非法字符!
    {
		if (x > (lcddev.width - 1) || y > (lcddev.height - 1))
            return;
        LCD_ShowChar(x, y, POINT_COLOR, BACK_COLOR, *p, size, mode);
        x += size / 2;
        p ++;
    }
}

/****************************************************************************
* 名    称： mypow(u8 m,u8 n)ode)
* 功    能： 求m的n次方(gui内部调用)
* 入口参数： m:乘数
*             n:幂
* 出口参数： m的n次方
* 说    明：
****************************************************************************/
u32 mypow(u8 m, u8 n)
{
	u32 result = 1;
	while (n --)
        result *= m;
	return
        result;
}

/****************************************************************************
* 名    称： int model(int x, int y)
* 功    能： 取余运算
* 入口参数： x：被取余的数
*            y：模
* 出口参数： x对y的余数
* 说    明：
****************************************************************************/
int model(u32 x, int y)
{
	return x - y * (x / y);
}

/****************************************************************************
* 名    称： numlen(u32 num)
* 功    能： 计算整形变量的有效位数
* 入口参数： num：整形变量
* 出口参数： length：整形变量的有效位数
* 说    明：
****************************************************************************/
u8 numlen(u32 num)
{
    if (num)
    {
        u8 length = 0;
        while (num != 0)
        {
            num /= 10;
            length ++;
        }
        return length;
    }
    else
    {
        return 1;
    }
}

/****************************************************************************
* 名    称： LCD_ShowNum(u16 x,u16 y,u32 num,u8 size)
* 功    能： 显示单个数字变量值
* 入口参数： x,y :起点坐标len :指定显示数字的位数
*			 size:字体大小(12,16)
*			 color:颜色
*			 num:数值(0~4294967295)
* 出口参数： 无
* 说    明：
****************************************************************************/
u8 LCD_ShowNum(u16 x, u16 y, u16 fc, u16 bc, u32 num, u8 size, u8 mode)
{
    u8 length;
    length = numlen(num);
	u32 t, temp1;
    u8 temp;
	u8 enshow = 0;
	for (t = 0; t < length; t ++)
	{
		temp1 = (num / mypow(10, length - t - 1));
        temp = model(temp1, 10);
		if (enshow == 0 && t < (length - 1))
		{
			if (temp == 0)
			{
				LCD_ShowChar(x + (size / 2) * t, y, fc, bc, ' ', size, mode);
                x = x + size / 2;
                if (x >= lcddev.width)
                {
                    x = 0;
                    y += size;
                }
				continue;
			}
            else
            {
                enshow = 1;
            }
		}
        if (x >= lcddev.width)
        {
            x = 0;
            y += size;
        }
	 	LCD_ShowChar(x, y, fc, bc, temp + '0', size, mode);
        x = x + size / 2;
	}
    return length;
}

/****************************************************************************
* 名    称： GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode)
* 功    能： 显示单个16X16中文字体
* 入口参数： x,y :起点坐标
*			fc:前置画笔颜色
*			bc:背景颜色
*			s:字符串地址
*			mode:模式	0,填充模式;1,叠加模式
* 出口参数： 无
* 说    明：
****************************************************************************/
void GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, char *s, u8 mode)
{
    u8 i, j;
    u16 k;
    u16 HZnum;
    u16 x0 = x;
    HZnum = sizeof(tfont16) / sizeof(typFNT_GB16);	//自动统计汉字数目
    for (k = 0; k < HZnum; k ++)
    {
        if ((tfont16[k].Index[0] == *(s)) && (tfont16[k].Index[1] == *(s+1)))
        {
            if (!mode) //非叠加方式
            {
                LCD_SetWindows(x, y, x + 16 - 1, y + 16 - 1);
                for (i = 0; i < 16 * 2; i ++)
                {
                    for (j = 0; j < 8; j ++)
                    {
                        if (tfont16[k].Msk[i] & (0x80 >> j))
                        {
                            LCD_WR_DATA_16Bit(fc);
                        }
                        else
                        {
                            LCD_WR_DATA_16Bit(bc);
                        }
                    }
                }
            }
            else//叠加方式
            {
                POINT_COLOR = fc;
                for (i = 0; i < 16 * 2; i ++)
                {
                    for (j = 0; j < 8; j ++)
                    {
                        if (tfont16[k].Msk[i] & (0x80 >> j))
                        {
                            LCD_DrawPoint(x, y);//画一个点
                        }
                        x ++;
                    }
                    if ((x - x0) == 16)
                    {
                        x = x0;
                        y ++;
                    }
                }
            }
            break;//查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
        }
        else
        {
            continue;
        }
    }
    LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1);//恢复窗口为全屏
}

/****************************************************************************
* 名    称： GUI_DrawFont24(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode)
* 功    能： 显示单个24X24中文字体
* 入口参数： x,y :起点坐标
*			fc:前置画笔颜色
*			bc:背景颜色
*			s:字符串地址
*			mode:模式	0,填充模式;1,叠加模式
* 出口参数： 无
* 说    明：
****************************************************************************/
void GUI_DrawFont24(u16 x, u16 y, u16 fc, u16 bc, char *s,u8 mode)
{
    u8 i, j;
    u16 k;
    u16 HZnum;
    u16 x0 = x;
    HZnum = sizeof(tfont24) / sizeof(typFNT_GB24);	//自动统计汉字数目
    for (k = 0; k < HZnum; k ++)
    {
        if ((tfont24[k].Index[0] == *(s)) && (tfont24[k].Index[1] == *(s+1)))
        {
            if (!mode) //非叠加方式
            {
                LCD_SetWindows(x, y, x + 24 - 1, y + 24 - 1);
                for (i = 0; i < 24 * 3; i ++)
                {
                    for (j = 0; j < 8; j ++)
                    {
                        if (tfont24[k].Msk[i] & (0x80 >> j))
                        {
                            LCD_WR_DATA_16Bit(fc);
                        }
                        else
                        {
                            LCD_WR_DATA_16Bit(bc);
                        }
                    }
                }
            }
            else//叠加方式
            {
                POINT_COLOR = fc;
                for (i = 0; i < 24 * 3; i ++)
                {
                    for (j = 0; j < 8; j ++)
                    {
                        if (tfont24[k].Msk[i] & (0x80 >> j))
                        {
                            LCD_DrawPoint(x, y);//画一个点
                        }
                        x ++;
                    }
                    if ((x - x0) == 24)
                    {
                        x = x0;
                        y ++;
                    }
                }
            }
            break;//查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
        }
        else
        {
            continue;
        }
    }
    LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1);//恢复窗口为全屏
}

/****************************************************************************
* 名    称： GUI_DrawFont32(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode)
* 功    能： 显示单个32X32中文字体
* 入口参数： x,y :起点坐标
*			fc:前置画笔颜色
*			bc:背景颜色
*			s:字符串地址
*			mode:模式	0,填充模式;1,叠加模式
* 出口参数： 无
* 说    明：
****************************************************************************/
void GUI_DrawFont32(u16 x, u16 y, u16 fc, u16 bc, char *s, u8 mode)
{
    u8 i, j;
    u16 k;
    u16 HZnum;
    u16 x0 = x;
    HZnum = sizeof(tfont32) / sizeof(typFNT_GB32);	//自动统计汉字数目
    for (k = 0; k < HZnum; k ++)
    {
        if ((tfont32[k].Index[0] == *(s)) && (tfont32[k].Index[1] == *(s+1)))
        {
            if (!mode) //非叠加方式
            {
                LCD_SetWindows(x, y, x + 32 - 1, y + 32 - 1);
                for (i = 0; i < 32 * 4; i ++)
                {
                    for (j = 0; j < 8; j ++)
                    {
                        if (tfont32[k].Msk[i] & (0x80 >> j))
                        {
                            LCD_WR_DATA_16Bit(fc);
                        }
                        else
                        {
                            LCD_WR_DATA_16Bit(bc);
                        }
                    }
                }
            }
            else//叠加方式
            {
                POINT_COLOR = fc;
                for (i = 0; i < 32 * 4; i ++)
                {
                    for (j = 0; j < 8; j ++)
                    {
                        if (tfont32[k].Msk[i] & (0x80 >> j))
                        {
                            LCD_DrawPoint(x, y);//画一个点
                        }
                        x ++;
                    }
                    if ((x - x0) == 32)
                    {
                        x = x0;
                        y ++;
                    }
                }
            }
            break;//查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
        }
        else
        {
            continue;
        }
    }
    LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1);//恢复窗口为全屏
}

/****************************************************************************
* 名    称： Show_Str(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
* 功    能： 显示一个字符串,包含中英文显示
* 入口参数： x,y :起点坐标
* 			fc:前置画笔颜色
*			bc:背景颜色
*			str :字符串
*			size:字体大小
*			mode:模式	0,填充模式;1,叠加模式
* 出口参数： 无
* 说    明：
****************************************************************************/
void Show_Str(u16 x, u16 y, u16 fc, u16 bc, char *str, u8 size, u8 mode)
{
    u16 x0 = 0;
    u8 bHz = 0;        //字符或者中文
    while (*str != 0)  //数据未结束
    {
        if (!bHz)
        {
            if (x > (lcddev.width - size / 2) || y > (lcddev.height - size))
            {
                y += size;
                x = x0;
                continue;
            }
            if (*str > 0x80)  //最高为是1，为汉字，否则为英文
                bHz = 1;      //中文
            else              //字符
            {
                if (*str == '\n')//换行符号
                {
                    y += size;
                    x = x0;
                    str ++;
                    continue;
                }
                else
                {
                    if(size == 12 || size == 16)
                    {
                        LCD_ShowChar(x, y, fc, bc, *str, size, mode);
                        x += size / 2; //字符,为全字的一半
                    }
                    else//字库中没有集成16X32的英文字体,用8X16代替
                    {
                        LCD_ShowChar(x, y, fc, bc, *str, 16, mode);
                        x += 8; //字符,为全字的一半
                    }
                }
                str ++;
            }
        }
        else//中文
        {
            if (x > (lcddev.width - size) || y > (lcddev.height - size))
            {
                y += size;
                x = x0;
                continue;
            }
            bHz = 0;//有汉字库
            if (size == 32)
                GUI_DrawFont32(x, y, fc, bc, str, mode);
            else if (size == 24)
                GUI_DrawFont24(x, y, fc, bc, str, mode);
            else
                GUI_DrawFont16(x, y, fc, bc, str, mode);
            str += 2;
            x += size;//下一个汉字偏移
        }
    }
}

/****************************************************************************
* 名    称： Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
* 功    能： 居中显示一个字符串,包含中英文显示
* 入口参数： x,y :起点坐标
* 			fc:前置画笔颜色
*			bc:背景颜色
*			str :字符串
*			size:字体大小
*			mode:模式	0,填充模式;1,叠加模式
* 出口参数： 无
* 说    明：
****************************************************************************/
void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, char *str, u8 size, u8 mode)
{
	u16 len = strlen((const char *)str);
	u16 x1 = (lcddev.width - len * 8) / 2;
	Show_Str(x + x1, y, fc, bc, str, size, mode);
}

/****************************************************************************
* 名    称： my_printf(char* string, ...)
* 功    能： 格式化显示字符串
* 入口参数： *string：字符串
*            ...：可变参数
* 出口参数： count：字符的总个数（一个中文占两个字符）
* 使用范例： int result;
*            int a = 666;
*            result = my_printf("Hello World!\n%d\n%s" a, "Good Luck!");
* 说    明： 注意可变参数的输入不用加&
****************************************************************************/
int my_printf(char* string, ...)
{
    u8 str_length;
    u8 x, y;
    x = START_X;
    y = START_Y;
    u32 u32_temp;
//    float num_float;
    char char_temp = NULL;       //临时变量
    char* str_temp = NULL;
    va_list ap;         //定义char* 变量 ap
    int count = 0;
    va_start(ap, string);//为arg进行初始化
    while(*string != '\0')
    {
        if (*string < 0x80)  //最高为是0，为英文字符，否则为汉字
        {
            if (x > lcddev.width - PRINTF_SIZE / 2)
            {
                y += PRINTF_SIZE;
                x = 0;
            }
            if (*string != '%' && *string != '\n')//为英文字母，照常输出
            {
                LCD_ShowChar(x, y, PRINTF_FC, PRINTF_BC, *string, PRINTF_SIZE, PRINTF_MODE);
                x += PRINTF_SIZE / 2;
                string ++;
                count ++;
                continue;
            }
            else if (*string == '\n')
            {
                y += PRINTF_SIZE;//换行
                x = 0;
                string++;
                count ++;
                continue;
            }
            else
            {
                switch(*++string)
                {
                    case 'd':
                    u32_temp = va_arg(ap, int);
                    str_length = LCD_ShowNum(x, y, PRINTF_FC, PRINTF_BC, u32_temp, PRINTF_SIZE, PRINTF_MODE);
                    string ++;
                    x += str_length * PRINTF_SIZE / 2;
                    count += str_length;
                    break;
                    case 'c':
                    char_temp = va_arg(ap,int);
                    LCD_ShowChar(x, y, PRINTF_FC, PRINTF_BC, (u8)char_temp, PRINTF_SIZE, PRINTF_MODE);
                    x += PRINTF_SIZE / 2;
                    string ++;
                    count ++;
                    break;
                    case 's':
                    str_temp = (char*)va_arg(ap,int);//取下一个参数的地址，因为这个是字符串
                    Show_Str(x, y, PRINTF_FC, PRINTF_BC, str_temp, PRINTF_SIZE, PRINTF_MODE);
                    str_length = strlen(str_temp);
                    x += str_length * PRINTF_SIZE / 2;
                    if (x >= lcddev.width)
                    {
                        y += (x / lcddev.width) * PRINTF_SIZE;
                        x %= lcddev.width;
                    }
                    string ++;
                    count += str_length;
                    break;
//                    case 'f':
//                    break;
                    default:   //
                    LCD_ShowChar(x, y, PRINTF_FC, PRINTF_BC, *--string, PRINTF_SIZE, PRINTF_MODE);
                    x += PRINTF_SIZE / 2;
                    LCD_ShowChar(x, y, PRINTF_FC, PRINTF_BC, *++string, PRINTF_SIZE, PRINTF_MODE);
                    x += PRINTF_SIZE / 2;
                    string ++;
                    count ++;
                    break;
                }
            }
        }
        else//中文
        {
            if (x > lcddev.width - PRINTF_SIZE)
            {
                y += PRINTF_SIZE;
                x = 0;
            }
            GUI_DrawFont16(x, y, PRINTF_FC, PRINTF_BC, string, PRINTF_MODE);
            string += 2;
            x += PRINTF_SIZE;//下一个汉字偏移
            continue;
        }
    }
    va_end(ap);//将arg指向空，防止野指针
    return count;
}

/****************************************************************************
* 名    称： Gui_Drawbmp16(u16 start_x,u16 start_y,const unsigned char *p)
* 功    能： 显示一副16位BMP图像
* 入口参数： start_x,start_y :起点坐标
*            length：图像的长度（从左往右）
*            width：图像的宽度（从上往下）
* 			 *p :图像数组起始地址
* 出口参数： 无
* 说    明：
****************************************************************************/
void Gui_Drawbmp16(u16 start_x, u16 start_y, u16 length, u16 width, const unsigned char *p)
{
  	int i;
	unsigned char picH, picL;
	LCD_SetWindows(start_x, start_y, start_x +length - 1, start_y + width - 1);//窗口设置
    for (i = 0; i < length * width; i ++)
	{
	 	picL = *(p + i * 2);	//数据低位在前
		picH = *(p + i * 2 + 1);
		LCD_WR_DATA_16Bit(picH << 8 | picL);
	}
	LCD_SetWindows(0, 0, lcddev.width - 1,lcddev.height - 1);//恢复显示窗口为全屏
}
