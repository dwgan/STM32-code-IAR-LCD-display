#include <stdarg.h>
#include "string.h"
#include "font.h"
#include "picture.h"
#include "delay.h"
#include "tft.h"
#include "gui.h"



/*������ɫ,������ɫ*/
u16 POINT_COLOR = RED, BACK_COLOR = YELLOW;
//u16 DeviceCode;
/****************************************************************************
* ��    �ƣ� GUI_DrawPoint(u16 x,u16 y,u16 color)
* ��    �ܣ� GUI���һ����
* ��ڲ����� x:���λ��x����
*            y:���λ��y����
*            color:Ҫ������ɫ
* ���ڲ����� ��
* ˵    ����
****************************************************************************/
void GUI_DrawPoint(u16 x, u16 y, u16 color)
{
	LCD_SetCursor(x, y);     //���ù��λ��
	LCD_WR_DATA_16Bit(color);
}
/****************************************************************************
* ��    �ƣ� LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
* ��    �ܣ� ��ָ�������������ɫ
* ��ڲ����� sx:ָ������ʼ��x����
*            sy:ָ������ʼ��y����
*            ex:ָ�����������x����
*            ey:ָ�����������y����
*           color:Ҫ������ɫ
* ���ڲ����� ��
* ˵    ����
****************************************************************************/
void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color)
{
	u16 i,j;
	u16 width = ex - sx + 1; 		//�õ����Ŀ��
	u16 height = ey - sy + 1;		//�߶�
	LCD_SetWindows(sx, sy, ex - 1, ey - 1);//������ʾ����
	for (i = 0; i < height; i ++)
	{
		for (j = 0; j < width; j ++)
            LCD_WR_DATA_16Bit(color);	//д������
	}
	LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1);//�ָ���������Ϊȫ��
}

/****************************************************************************
* ��    �ƣ� LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
* ��    �ܣ� GUI����
* ��ڲ����� x1,y1:�������
*            x2,y2:�յ�����
* ���ڲ����� ��
* ˵    ����
****************************************************************************/
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;

    delta_x = x2 - x1;                   //������������
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
    if (delta_x > 0)
        incx = 1;                        //���õ�������
    else if (delta_x == 0)
        incx = 0;                        //��ֱ��
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0;                        //ˮƽ��
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)
        distance = delta_x;              //ѡȡ��������������
    else
        distance = delta_y;
    for (t = 0; t <= distance + 1; t ++ )//�������
    {
        LCD_DrawPoint(uRow, uCol);       //����
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
* ��    �ƣ� LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
* ��    �ܣ� GUI������(�����)
* ��ڲ����� (x1,y1),(x2,y2):���εĶԽ�����
* ���ڲ����� ��
* ˵    ����
****************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
    LCD_DrawLine(x1, y1, x2, y1);
    LCD_DrawLine(x1, y1, x1, y2);
    LCD_DrawLine(x1, y2, x2, y2);
    LCD_DrawLine(x2, y1, x2, y2);
}

/****************************************************************************
* ��    �ƣ� LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
* ��    �ܣ� GUI������(���)
* ��ڲ����� (x1,y1),(x2,y2):���εĶԽ�����
* ���ڲ����� ��
* ˵    ����
****************************************************************************/
void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_Fill(x1,y1,x2,y2,POINT_COLOR);
}

/****************************************************************************
* ��    �ƣ� _draw_circle_8(int xc, int yc, int x, int y, u16 c)
* ��    �ܣ� 8�Գ��Ի�Բ�㷨(�ڲ�����)
* ��ڲ����� (xc,yc) :Բ��������
*            (x,y):��������Բ�ĵ�����
*            c:������ɫ
* ���ڲ����� ��
* ˵    ����
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
* ��    �ƣ� gui_circle(int xc, int yc,u16 c,int r, int fill)
* ��    �ܣ� ��ָ��λ�û�һ��ָ����С��Բ(���)
* ��ڲ����� (xc,yc) :Բ��������
*             c:������ɫ
*             r:Բ�뾶
*             fill:����жϱ�־��1-��䣬0-�����
* ���ڲ����� ��
* ˵    ����
****************************************************************************/
void gui_circle(int xc, int yc,u16 c,int r, int fill)
{
	int x = 0, y = r, yi, d;
	d = 3 - 2 * r;
	if (fill)           // �����䣨��ʵ��Բ��
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
    else      // �������䣨������Բ��
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
* ��    �ƣ� LCD_ShowChar(u16 x, u16 y, u16 fc, u16 bc, u8 num, u8 size, u8 mode)
* ��    �ܣ� ��ʾ����Ӣ���ַ�
* ��ڲ����� (x,y):�ַ���ʾλ����ʼ����
*             fc:ǰ�û�����ɫ
*             bc:������ɫ
*             num:��ֵ��0-94��
*            size:�����С(12/16)
*            mode:ģʽ  0,���ģʽ;1,����ģʽ
* ���ڲ����� ��
* ˵    ����
****************************************************************************/
void LCD_ShowChar(u16 x, u16 y, u16 fc, u16 bc, u8 num, u8 size, u8 mode)
{
    u8 temp;
    u8 pos, t;
	u16 colortemp = POINT_COLOR;
	num = num - ' ';//�õ�ƫ�ƺ��ֵ
	LCD_SetWindows(x, y, x + size / 2 - 1, y + size - 1);//���õ���������ʾ����
	if (!mode) //�ǵ��ӷ�ʽ
	{
        if (size == 12)
        {
            for (pos = 0; pos < size; pos ++)
            {
                temp = asc2_1206[num][pos];//����1206����
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
                temp = asc2_1608[num][pos];//����1608����
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
    else //���ӷ�ʽ
    {
        if (size == 12)
        {
            for (pos = 0; pos < size; pos ++)
            {
                temp = asc2_1206[num][pos];//����1206����
                for (t = 0; t < size / 2; t ++)
                {
                    POINT_COLOR = fc;
                    if (temp & 0x01)
                    {
                        LCD_DrawPoint(x + t, y + pos);//��һ����
                    }
                    temp >>= 1;
                }
            }
        }
		else
        {
            for (pos = 0; pos < size; pos ++)
            {
                temp = asc2_1608[num][pos];		 //����1608����
                for (t = 0; t < size / 2; t ++)
                {
                    POINT_COLOR = fc;
                    if (temp & 0x01)
                    {
                        LCD_DrawPoint(x + t, y + pos);//��һ����
                    }
                    temp >>= 1;
                }
            }
        }
	}
	POINT_COLOR = colortemp;
	LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1);//�ָ�����Ϊȫ��
}

/****************************************************************************
* ��    �ƣ� LCD_ShowNum2412(u16 x,u16 y,u16 fc, u16 bc,u8 *p ,u8 size,u8 mode)
* ��    �ܣ� ��ʾ����Ӣ���ַ�
* ��ڲ����� (x,y):�ַ���ʾλ����ʼ����
*             fc:ǰ�û�����ɫ
*             bc:������ɫ
*             num:��ֵ��0-94��
*            size:�����С
*            mode:ģʽ  0,���ģʽ;1,����ģʽ
* ���ڲ����� ��
* ˵    ����
****************************************************************************/
void LCD_ShowNum2412(u16 x,u16 y,u16 fc, u16 bc,u8 *p ,u8 size,u8 mode)
{
    u16 temp;
    u8 pos, t;
	u16 colortemp = POINT_COLOR;
	u16 x0 = x;
	u16 y0 = y;
	u8 num = 0;
    while ((*p <= '~') && (*p >= ' '))//�ж��ǲ��ǷǷ��ַ�!
    {
		if (x > (lcddev.width - 1) || y > (lcddev.height - 1))
            return;
		num = *p;
		if (':' == num) //�����ַ�":"
            num = 10;
		else if ('.' == num)//�����ַ�"."
            num = 11;
		else  //������
            num = num - '0';
		x0 = x;
		for (pos = 0; pos < 48; pos ++)
		{
			temp = asc2_2412[num][pos];
			for (t = 0; t < 8; t ++)
		    {
				POINT_COLOR = fc;
		        if (temp & 0x80)
                    LCD_DrawPoint(x, y);//��һ����
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
            x += 16;	//��Ϊ�����־࣬ʹ���Ű���ÿ���ԭֵΪ12
        else
            x += 8; //��Ϊ�����־࣬ʹ���Ű���ÿ���ԭֵΪ12

        y = y0;
        p ++;
    }
	POINT_COLOR = colortemp;
}

/****************************************************************************
* ��    �ƣ� LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u8 mode)
* ��    �ܣ� ��ʾӢ���ַ���
* ��ڲ����� x,y :�������
*            size:�����С
*            *p:�ַ�����ʼ��ַ
*            mode:ģʽ	0,���ģʽ;1,����ģʽ
* ���ڲ����� ��
* ˵    ����
****************************************************************************/
void LCD_ShowString(u16 x, u16 y, u8 *p, u8 size, u8 mode)
{
    while ((*p <= '~') && (*p >= ' '))//�ж��ǲ��ǷǷ��ַ�!
    {
		if (x > (lcddev.width - 1) || y > (lcddev.height - 1))
            return;
        LCD_ShowChar(x, y, POINT_COLOR, BACK_COLOR, *p, size, mode);
        x += size / 2;
        p ++;
    }
}

/****************************************************************************
* ��    �ƣ� mypow(u8 m,u8 n)ode)
* ��    �ܣ� ��m��n�η�(gui�ڲ�����)
* ��ڲ����� m:����
*             n:��
* ���ڲ����� m��n�η�
* ˵    ����
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
* ��    �ƣ� int model(int x, int y)
* ��    �ܣ� ȡ������
* ��ڲ����� x����ȡ�����
*            y��ģ
* ���ڲ����� x��y������
* ˵    ����
****************************************************************************/
int model(u32 x, int y)
{
	return x - y * (x / y);
}

/****************************************************************************
* ��    �ƣ� numlen(u32 num)
* ��    �ܣ� �������α�������Чλ��
* ��ڲ����� num�����α���
* ���ڲ����� length�����α�������Чλ��
* ˵    ����
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
* ��    �ƣ� LCD_ShowNum(u16 x,u16 y,u32 num,u8 size)
* ��    �ܣ� ��ʾ�������ֱ���ֵ
* ��ڲ����� x,y :�������len :ָ����ʾ���ֵ�λ��
*			 size:�����С(12,16)
*			 color:��ɫ
*			 num:��ֵ(0~4294967295)
* ���ڲ����� ��
* ˵    ����
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
* ��    �ƣ� GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode)
* ��    �ܣ� ��ʾ����16X16��������
* ��ڲ����� x,y :�������
*			fc:ǰ�û�����ɫ
*			bc:������ɫ
*			s:�ַ�����ַ
*			mode:ģʽ	0,���ģʽ;1,����ģʽ
* ���ڲ����� ��
* ˵    ����
****************************************************************************/
void GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, char *s, u8 mode)
{
    u8 i, j;
    u16 k;
    u16 HZnum;
    u16 x0 = x;
    HZnum = sizeof(tfont16) / sizeof(typFNT_GB16);	//�Զ�ͳ�ƺ�����Ŀ
    for (k = 0; k < HZnum; k ++)
    {
        if ((tfont16[k].Index[0] == *(s)) && (tfont16[k].Index[1] == *(s+1)))
        {
            if (!mode) //�ǵ��ӷ�ʽ
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
            else//���ӷ�ʽ
            {
                POINT_COLOR = fc;
                for (i = 0; i < 16 * 2; i ++)
                {
                    for (j = 0; j < 8; j ++)
                    {
                        if (tfont16[k].Msk[i] & (0x80 >> j))
                        {
                            LCD_DrawPoint(x, y);//��һ����
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
            break;//���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
        }
        else
        {
            continue;
        }
    }
    LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1);//�ָ�����Ϊȫ��
}

/****************************************************************************
* ��    �ƣ� GUI_DrawFont24(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode)
* ��    �ܣ� ��ʾ����24X24��������
* ��ڲ����� x,y :�������
*			fc:ǰ�û�����ɫ
*			bc:������ɫ
*			s:�ַ�����ַ
*			mode:ģʽ	0,���ģʽ;1,����ģʽ
* ���ڲ����� ��
* ˵    ����
****************************************************************************/
void GUI_DrawFont24(u16 x, u16 y, u16 fc, u16 bc, char *s,u8 mode)
{
    u8 i, j;
    u16 k;
    u16 HZnum;
    u16 x0 = x;
    HZnum = sizeof(tfont24) / sizeof(typFNT_GB24);	//�Զ�ͳ�ƺ�����Ŀ
    for (k = 0; k < HZnum; k ++)
    {
        if ((tfont24[k].Index[0] == *(s)) && (tfont24[k].Index[1] == *(s+1)))
        {
            if (!mode) //�ǵ��ӷ�ʽ
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
            else//���ӷ�ʽ
            {
                POINT_COLOR = fc;
                for (i = 0; i < 24 * 3; i ++)
                {
                    for (j = 0; j < 8; j ++)
                    {
                        if (tfont24[k].Msk[i] & (0x80 >> j))
                        {
                            LCD_DrawPoint(x, y);//��һ����
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
            break;//���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
        }
        else
        {
            continue;
        }
    }
    LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1);//�ָ�����Ϊȫ��
}

/****************************************************************************
* ��    �ƣ� GUI_DrawFont32(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode)
* ��    �ܣ� ��ʾ����32X32��������
* ��ڲ����� x,y :�������
*			fc:ǰ�û�����ɫ
*			bc:������ɫ
*			s:�ַ�����ַ
*			mode:ģʽ	0,���ģʽ;1,����ģʽ
* ���ڲ����� ��
* ˵    ����
****************************************************************************/
void GUI_DrawFont32(u16 x, u16 y, u16 fc, u16 bc, char *s, u8 mode)
{
    u8 i, j;
    u16 k;
    u16 HZnum;
    u16 x0 = x;
    HZnum = sizeof(tfont32) / sizeof(typFNT_GB32);	//�Զ�ͳ�ƺ�����Ŀ
    for (k = 0; k < HZnum; k ++)
    {
        if ((tfont32[k].Index[0] == *(s)) && (tfont32[k].Index[1] == *(s+1)))
        {
            if (!mode) //�ǵ��ӷ�ʽ
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
            else//���ӷ�ʽ
            {
                POINT_COLOR = fc;
                for (i = 0; i < 32 * 4; i ++)
                {
                    for (j = 0; j < 8; j ++)
                    {
                        if (tfont32[k].Msk[i] & (0x80 >> j))
                        {
                            LCD_DrawPoint(x, y);//��һ����
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
            break;//���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
        }
        else
        {
            continue;
        }
    }
    LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1);//�ָ�����Ϊȫ��
}

/****************************************************************************
* ��    �ƣ� Show_Str(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
* ��    �ܣ� ��ʾһ���ַ���,������Ӣ����ʾ
* ��ڲ����� x,y :�������
* 			fc:ǰ�û�����ɫ
*			bc:������ɫ
*			str :�ַ���
*			size:�����С
*			mode:ģʽ	0,���ģʽ;1,����ģʽ
* ���ڲ����� ��
* ˵    ����
****************************************************************************/
void Show_Str(u16 x, u16 y, u16 fc, u16 bc, char *str, u8 size, u8 mode)
{
    u16 x0 = 0;
    u8 bHz = 0;        //�ַ���������
    while (*str != 0)  //����δ����
    {
        if (!bHz)
        {
            if (x > (lcddev.width - size / 2) || y > (lcddev.height - size))
            {
                y += size;
                x = x0;
                continue;
            }
            if (*str > 0x80)  //���Ϊ��1��Ϊ���֣�����ΪӢ��
                bHz = 1;      //����
            else              //�ַ�
            {
                if (*str == '\n')//���з���
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
                        x += size / 2; //�ַ�,Ϊȫ�ֵ�һ��
                    }
                    else//�ֿ���û�м���16X32��Ӣ������,��8X16����
                    {
                        LCD_ShowChar(x, y, fc, bc, *str, 16, mode);
                        x += 8; //�ַ�,Ϊȫ�ֵ�һ��
                    }
                }
                str ++;
            }
        }
        else//����
        {
            if (x > (lcddev.width - size) || y > (lcddev.height - size))
            {
                y += size;
                x = x0;
                continue;
            }
            bHz = 0;//�к��ֿ�
            if (size == 32)
                GUI_DrawFont32(x, y, fc, bc, str, mode);
            else if (size == 24)
                GUI_DrawFont24(x, y, fc, bc, str, mode);
            else
                GUI_DrawFont16(x, y, fc, bc, str, mode);
            str += 2;
            x += size;//��һ������ƫ��
        }
    }
}

/****************************************************************************
* ��    �ƣ� Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
* ��    �ܣ� ������ʾһ���ַ���,������Ӣ����ʾ
* ��ڲ����� x,y :�������
* 			fc:ǰ�û�����ɫ
*			bc:������ɫ
*			str :�ַ���
*			size:�����С
*			mode:ģʽ	0,���ģʽ;1,����ģʽ
* ���ڲ����� ��
* ˵    ����
****************************************************************************/
void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, char *str, u8 size, u8 mode)
{
	u16 len = strlen((const char *)str);
	u16 x1 = (lcddev.width - len * 8) / 2;
	Show_Str(x + x1, y, fc, bc, str, size, mode);
}

/****************************************************************************
* ��    �ƣ� my_printf(char* string, ...)
* ��    �ܣ� ��ʽ����ʾ�ַ���
* ��ڲ����� *string���ַ���
*            ...���ɱ����
* ���ڲ����� count���ַ����ܸ�����һ������ռ�����ַ���
* ʹ�÷����� int result;
*            int a = 666;
*            result = my_printf("Hello World!\n%d\n%s" a, "Good Luck!");
* ˵    ���� ע��ɱ���������벻�ü�&
****************************************************************************/
int my_printf(char* string, ...)
{
    u8 str_length;
    u8 x, y;
    x = START_X;
    y = START_Y;
    u32 u32_temp;
//    float num_float;
    char char_temp = NULL;       //��ʱ����
    char* str_temp = NULL;
    va_list ap;         //����char* ���� ap
    int count = 0;
    va_start(ap, string);//Ϊarg���г�ʼ��
    while(*string != '\0')
    {
        if (*string < 0x80)  //���Ϊ��0��ΪӢ���ַ�������Ϊ����
        {
            if (x > lcddev.width - PRINTF_SIZE / 2)
            {
                y += PRINTF_SIZE;
                x = 0;
            }
            if (*string != '%' && *string != '\n')//ΪӢ����ĸ���ճ����
            {
                LCD_ShowChar(x, y, PRINTF_FC, PRINTF_BC, *string, PRINTF_SIZE, PRINTF_MODE);
                x += PRINTF_SIZE / 2;
                string ++;
                count ++;
                continue;
            }
            else if (*string == '\n')
            {
                y += PRINTF_SIZE;//����
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
                    str_temp = (char*)va_arg(ap,int);//ȡ��һ�������ĵ�ַ����Ϊ������ַ���
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
        else//����
        {
            if (x > lcddev.width - PRINTF_SIZE)
            {
                y += PRINTF_SIZE;
                x = 0;
            }
            GUI_DrawFont16(x, y, PRINTF_FC, PRINTF_BC, string, PRINTF_MODE);
            string += 2;
            x += PRINTF_SIZE;//��һ������ƫ��
            continue;
        }
    }
    va_end(ap);//��argָ��գ���ֹҰָ��
    return count;
}

/****************************************************************************
* ��    �ƣ� Gui_Drawbmp16(u16 start_x,u16 start_y,const unsigned char *p)
* ��    �ܣ� ��ʾһ��16λBMPͼ��
* ��ڲ����� start_x,start_y :�������
*            length��ͼ��ĳ��ȣ��������ң�
*            width��ͼ��Ŀ�ȣ��������£�
* 			 *p :ͼ��������ʼ��ַ
* ���ڲ����� ��
* ˵    ����
****************************************************************************/
void Gui_Drawbmp16(u16 start_x, u16 start_y, u16 length, u16 width, const unsigned char *p)
{
  	int i;
	unsigned char picH, picL;
	LCD_SetWindows(start_x, start_y, start_x +length - 1, start_y + width - 1);//��������
    for (i = 0; i < length * width; i ++)
	{
	 	picL = *(p + i * 2);	//���ݵ�λ��ǰ
		picH = *(p + i * 2 + 1);
		LCD_WR_DATA_16Bit(picH << 8 | picL);
	}
	LCD_SetWindows(0, 0, lcddev.width - 1,lcddev.height - 1);//�ָ���ʾ����Ϊȫ��
}
