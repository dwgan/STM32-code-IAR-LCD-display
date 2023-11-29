#include "nokia5110.h"

/*
 * Nokia_5110.c
 *  Created on: 2018��6��13��
 *  Last Modify on: 2018��8��6��
 *  Modify Details: �������շ���������ʵ����Ӳ�����룬ʵ���˸����豸�������豸�Ļ���
 *      Author: Limerrence
 */
#include "fontcode.h"
#include "nokia5110.h"
#include "string.h"

#if (SPI)
#if UART_FIFO
#define UART_TXBUFF  1     //ѡ���Ƿ���뷢�ͻ�����
#define UART_RXBUFF  0     //ѡ���Ƿ������ջ�����
#define TXBUF_SIZE  515   //����FIFO������������������һ�����������ܵķ����ֽ���
#define RXBUF_SIZE  64    //����FIFO������������������һ�����������ܵĽ����ֽ���

#if (UART_TXBUFF)
static unsigned char TX_BUFF[TXBUF_SIZE];            //����FIFO����������
static unsigned char ADDR_BUFF[TXBUF_SIZE];            //����FIFO����������
static unsigned int  UART_OutLen=0;                  //����FIFO�ڴ��������ֽ���
static unsigned int  TX_IndexR=0;                    //����FIFO�ڵĶ�ָ��
static unsigned int  TX_IndexW=0;                    //����FIFO�ڵ�дָ��
#endif

#if (UART_RXBUFF)
static unsigned char RX_BUFF[RXBUF_SIZE]; //����FIFO����������
static unsigned int  UART_InpLen=0;                  //����FIFO�ڴ���ȡ���ֽ���
static unsigned int  RX_IndexR=0;                    //����FIFO�Ķ�ָ��
static unsigned int  RX_IndexW=0;                    //����FIFO��дָ��
#endif

unsigned char Tx_FirstFlag=1;
unsigned char UART_EA;
#endif

#if (SPI==1)
unsigned int Adress_BUFF;
unsigned int Data_BUFF;

/******************************************************************************************************
 * ��       �ƣ�spi_Init()
 * ��       �ܣ�spi��ʼ��
 * ��ڲ�������
 * ���ڲ�����д������1-�ɹ���0-ʧ��
 * ˵       ������
 * ��       ������
 ***************************************************************************************************/
#if (UART_MODE==3)
void spi_Init()
{
    volatile unsigned int i;

    P1DIR &=~BIT6;
    P2DIR &=~BIT7;
    P3DIR &=~(BIT2+BIT3);

    P1REN |=BIT6;
    P2REN |=BIT7;
    P3REN |=BIT2+BIT3;

    P1OUT |= BIT6;            //������������
    P2OUT |= BIT7;            //������������
    P3OUT |= BIT2 + BIT3;            //������������

    P3SEL |= BIT3;                             // P3.3option select
    P2SEL |= BIT7;                             // P2.7 option select
    RST_DIR;                                  // Set P1.0-2 to output direction
    CE_DIR;
    DC_DIR;

    UCA0CTL1 |= UCSWRST;                       // **Put state machine in reset**
    UCA0CTL0 |= UCMST + UCSYNC + UCCKPH + UCMSB + UCMODE_0;    // 3-pin, 8-bit SPI master
                                              // Clock polarity high, MSB
    UCA0CTL1 |= UCSSEL_1;                      // SMCLK
    UCA0BR0 = 0;                                // /2
    UCA0BR1 = 0;                                //
    UCA0MCTL = 0;                               // No modulation
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

    UCA0IE |= UCTXIE;                         // Enable USCI_A0 RX interrupt
    _EINT();
    __delay_cycles(1);
}
#elif (UART_MODE == 4)
void spi_Init()
{
    volatile unsigned int i;

    P1DIR &= ~BIT6;
    P2DIR &= ~BIT7;
    P3DIR &= ~(BIT2 + BIT3);

    P1REN |= BIT6;
    P2REN |= BIT7;
    P3REN |= BIT2 + BIT3;

    P1OUT |= BIT6;            //������������
    P2OUT |= BIT7;            //������������
    P3OUT |= BIT2 + BIT3;            //������������

    P3SEL |= BIT3;                             // P3.3option select
    P2SEL |= BIT7;                             // P2.7 option select
    P3SEL |= BIT2;
    RST_DIR;                                  // Set P1.0-2 to output direction
    //CE_DIR;
    DC_DIR;
    //UCA0CTLW0 |=UCSTEM;
    UCA0CTL1 |= UCSWRST;                       // **Put state machine in reset**
    UCA0CTL0 |= UCMST + UCSYNC + UCCKPH + UCMSB + UCMODE_2;    // 3-pin, 8-bit SPI master
                                              // Clock polarity high, MSB
    UCA0CTL1 |= UCSSEL_1;                      // SMCLK
    UCA0BR0 = 8;                                // /2
    UCA0BR1 = 0;                                //
    UCA0MCTL = 0;                               // No modulation
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

    UCA0IE |= UCTXIE;                         // Enable USCI_A0 RX interrupt
    _EINT();
    __delay_cycles(1);
}
#endif
#pragma vector = USCI_A0_VECTOR
#if UART_FIFO
__interrupt void USCI_A0_ISR (void)
{
    volatile unsigned int i;
    switch(__even_in_range(UCA0IV , 4))
    {
        case 0: 
            break;                          // Vector 0 - no interrupt
        case 2:                                 // Vector 2 - RXIFG
        // while (!(UCA0IFG&UCTXIFG));           // USCI_A0 TX buffer ready?

        /*if (UCA0RXBUF==SLV_Data)              // Test for correct character RX'd
          P1OUT |= 0x01;                      // If correct, light LED
        else
          P1OUT &= ~0x01;                     // If incorrect, clear LED
        */
        //MST_Data++;                           // Increment data
        //SLV_Data++;
        //UCA0TXBUF = MST_Data;                 // Send next value

        //for(i = 20; i>0; i--);                // Add time between transmissions to
                                              // make sure slave can process information
            break;
        case 4:
            while ((UCA0STAT & UCBUSY));
#if (UART_MODE == 3)
        CE_1;               //��������ˣ���Ƭѡ����
#endif
            if (UART_OutLen>0)                // FIFO���Ƿ��д����͵����ݣ�
            {                                 // �У�����˳��������͡�
#if (UART_MODE==3)
                CE_0;
#endif
                if (ADDR_BUFF[TX_IndexR])
                    DC_1;
                else
                    DC_0;
                UART_OutLen--;                // �����������ֽ�����1
                //while (!(UCA0IFG&UCTXIFG));   // USCI_A0 TX buffer ready?
                UCA0TXBUF = TX_BUFF[TX_IndexR];
                //SBUF=TX_BUFF[TX_IndexR];      // ��βָ���ȡһ���ֽڲ�����
                if (++TX_IndexR >= TXBUF_SIZE)  // ��ָ����������ж��Ƿ��±�Խ��
                {
                    TX_IndexR = 0;              // ���Խ����дָ�����(ѭ������)
                }
            }
            else
                Tx_FirstFlag = 1;       //���������ѷ��꣬��һ���ֽڽ�Ҫֱ������SBUF
            //UCA0TXBUF = Data_BUFF;                    // Transmit first character
            //while ((UCA0STAT&UCBUSY));
            //CE_1;
            UCA0IFG &= ~UCTXIFG;
            break;                          // Vector 4 - TXIFG
        default: 
            break;
    }
}
/********************************************************
 * ��       �ƣ�LCD_Write_Byte()
 * ��       �ܣ��򻺳���д������
 * ��ڲ�����data:���ݣ�command:ѡ�����������,1��ʾ���ݣ�0��ʾ����
 * ���ڲ�����д������1-�ɹ���0-ʧ��
 * ˵       ������
 * ��       ����LCD_Write_Byte(0xff,1);�򻺳���д��һ���ֽڴ�СΪ0xff������
 *******************************************************/
char LCD_Write_Byte (unsigned char data, unsigned char command)
{
    if(UART_OutLen == TXBUF_SIZE) //���FIFO����
    {
        return (0);                 // �����ٷ������ݣ����ط���ʧ�ܱ�־
    }
    _DINT();
    if(Tx_FirstFlag == 1)            // ����ǵ�һ���ֽڣ�����ǰû�����ݵȴ�������
    {
#if (UART_MODE==3)
        CE_0;
#endif
        if (command)
            DC_1;
        else
            DC_0;
        UCA0TXBUF = data;               // ��һ���ֽ�ֱ������SBUF�ڣ��ڷ���ʱ�ᴥ���ж�
        Tx_FirstFlag = 0;
        _EINT();
        return(1);
    }                             // �����ǵ�һ�ֽڣ�˵�������������Ŷӵȴ�����
    UART_OutLen++;                  // �����ֽ�ѹ�뷢�Ͷ��У��������ֽ�����1
    if (command)
        ADDR_BUFF[TX_IndexW] = 1;
    else
        ADDR_BUFF[TX_IndexW] = 0;
    TX_BUFF[TX_IndexW] = data;     // ����������ͨ��дָ��д��FIFO
    if (++TX_IndexW >= TXBUF_SIZE) // дָ����������ж��Ƿ��±�Խ��
    {
        TX_IndexW = 0;              // ���Խ����дָ�����(ѭ������)
    }
    UCA0IFG |= UCTXIFG;
    _EINT();                        // FIFO������ϣ��ָ��ж�����
    return (1);                   // ���ط��ͳɹ���־
}
#else
__interrupt void USCI_A0_ISR (void)
{
    volatile unsigned int i;
    switch(__even_in_range(UCA0IV , 4))
    {
        case 0: 
            break;                          // Vector 0 - no interrupt
        case 2:                                 // Vector 2 - RXIFG
         // while (!(UCA0IFG&UCTXIFG));           // USCI_A0 TX buffer ready?

          /*if (UCA0RXBUF==SLV_Data)              // Test for correct character RX'd
            P1OUT |= 0x01;                      // If correct, light LED
          else
            P1OUT &= ~0x01;                     // If incorrect, clear LED
    */
          //MST_Data++;                           // Increment data
          //SLV_Data++;
          //UCA0TXBUF = MST_Data;                 // Send next value

          //for(i = 20; i>0; i--);                // Add time between transmissions to
                                                // make sure slave can process information
            break;
        case 4:
            CE_0;
            if (Adress_BUFF)
                DC_1;
            else
                DC_0;
            UCA0TXBUF = Data_BUFF;                    // Transmit first character
            while ((UCA0STAT & UCBUSY));
            CE_1;
            UCA0IFG &= ~UCTXIFG;
            break;                          // Vector 4 - TXIFG
        default: 
            break;
    }
}
/********************************************************
 * ��       �ƣ�LCD_Write_Byte()
 * ��       �ܣ��򻺳���д������
 * ��ڲ�����data:���ݣ�command:ѡ�����������,1��ʾ���ݣ�0��ʾ����
 * ���ڲ�����д������1-�ɹ���0-ʧ��
 * ˵       ������
 * ��       ����LCD_Write_Byte(0xff,1);�򻺳���д��һ���ֽڴ�СΪ0xff������
 *******************************************************/
char LCD_Write_Byte (unsigned char data, unsigned char command)
{
    if (command)
        Adress_BUFF = 1;
    else
        Adress_BUFF = 0;
    Data_BUFF = data;
    UCA0IFG |= UCTXIFG;
    return (1);
}
#endif
#endif

#else
/********************************************************
 * ��       �ƣ�LCD_Write_Byte()
 * ��       �ܣ��򻺳���д������
 * ��ڲ�����data:���ݣ�command:ѡ�����������,1��ʾ���ݣ�0��ʾ����
 * ���ڲ�����д������1-�ɹ���0-ʧ��
 * ˵       ������
 * ��       ����LCD_Write_Byte(0xff,1);�򻺳���д��һ���ֽڴ�СΪ0xff������
 *******************************************************/
char LCD_Write_Byte (unsigned char data, unsigned char command)
{
    unsigned int i;
    CE_0;                       //Ƭѡ��
    if (command == Command)
        DC_0;
    else
        DC_1;
    for (i = 8 ; i > 0 ; i--)           //ѭ��8�Σ�д��8λ����
    {
        if (data & 0x80)
            Din_1;
        else
            Din_0;
        Clk_0;
        data = data << 1;           //����
        Clk_1;
    }
    CE_1;                       //Ƭѡ��
    return (1);
}
/******************************************************************************************************
 * ��       �ƣ�spi_Init()
 * ��       �ܣ�spi��ʼ��
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ������
 * ��       ������
 ***************************************************************************************************/
void spi_Init()
{
    /**************��LCD�������Ž�������******************/
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE); //�� GPIOA ʱ��
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 
      | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //GPIO�Ĺ���״̬Ϊ�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO���������������� 50Hz
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ�� GPIOA
    
    /*RST_DIR;
    CE_DIR;                 //RST,CE
    DC_DIR;
    Din_DIR;                //DC,Din
    Clk_DIR;                //Clk
*/
}
#endif
/*
void delay_us (vu32 nCount) //��ʱ������������
{
     for(; nCount != 0; nCount--);
}
*/
/*************************************
 * �������ƣ�LCD_set_XY
 * �������ܣ�����д�����ݵ���ʼ����
 * ��ڲ�����X  ��0��83
 *           Y  ��0��5
 * ���ڲ�������
 * �� ע��
 **************************************/
void LCD_Set_XY (unsigned char X, unsigned char Y)
{
    X=6*X;
    LCD_Write_Byte(0x40 | Y, Command);    //����������
    LCD_Write_Byte(0x80 | X, Command);    //���ú�����
}
/**************************************
 * ���ƣ�LCD_Write_char()
 * ����:д��һ���ַ�
 * ˵����
 *        ***#**
 *        **#*#*
 *        *#***#
 *        *#***#
 *        *#***#
 *        *#***#
 *        *#***#
 *        ******
 ***********************************/
void LCD_Write_Char(unsigned char c)
{
    unsigned char line;
    c -= 32;
    for (line = 6; line > 0; line--)
        LCD_Write_Byte(Font_Code[c][6-line], Data);
}
/*************************************
�������ƣ�LCD_Write_String()
�������ܣ�����д�����ݵ���ʼ����
��ڲ�����X  ��0��83
          Y  ��0��5
���ڲ�������
�� ע��
**************************************/
void LCD_Write_String (unsigned char X, unsigned char Y, unsigned char *s)
{
    LCD_Set_XY (X, Y);
    while (*s)
    {
        LCD_Write_Char(*s);
        s++;
    }
}
/***************************************
�������ƣ�LCD_Clear_All()
�������ܣ���ʾӢ���ַ�
��ڲ�����c      :  ��ʾ���ַ�
���ڲ�������
�� ע��
****************************************/
void LCD_Clear_All (void)
{
    unsigned char i,j;
    LCD_Set_XY (0, 0);
    for (i = 6; i > 0; i--)
        for (j = 84; j > 0; j--)
            LCD_Write_Byte(0x00, Data);
    //UCA0IFG |=UCTXIFG;
}
/**************************************
 * ���ƣ�LCD_Init()
 * ����:LCD��ʼ��
 * ˵����
 ***********************************/
void LCD_Init()
{
    spi_Init();
    RST_0;
    delay_us(100);
    RST_1;                            //����һ����λ�͵�ƽ����
    LCD_Write_Byte (0x21, Command);     //ʹ������ָ������LCD������ʽ
    LCD_Write_Byte (0xc8, Command);     //�趨Һ��ƫ�õ�ѹ
    LCD_Write_Byte (0x06, Command);     //�¶�У��
    LCD_Write_Byte (0x13, Command);     //1��48
    LCD_Write_Byte (0x20, Command);     //ʹ�û���ָ��,ˮƽѰַ
    LCD_Clear_All();
    LCD_Write_Byte (0x0c, Command);     //�趨��ʾģʽ
    //UCA0IFG |=UCTXIFG;
}
void Display_Mark()
{
    LCD_Write_String (0, 1, "Flag1:");
    LCD_Write_String (0, 2, "Flag2:");
    LCD_Write_String (0, 3, "Flag3:");
    LCD_Write_String (0, 4, "Flag4:");
}
/*
void LCD_Write_Num(unsigned char X,unsigned char Y,unsigned char num)
{
    unsigned char line;
    num=num+16;
    LCD_Set_XY(X,Y);
    for (line=6;line>0;line--)
        LCD_Write_Byte(Font_Code[num][6-line],Data);
}
void LCD_WriteResult(unsigned char X,unsigned char Y,unsigned long Result)
{

    static unsigned int i;
    static int DispBuff[8];
    for (i=0;i<ByteLengh;i++)
    {
        DispBuff[i]=Result%10;
        Result=Result/10;
    }
    for (i=ByteLengh;i>0;i--)
    {
        if (DispBuff[i-1]==0)
            DispBuff[i-1]=0-16;
        else
            break;
    }
    for (i=0;i<ByteLengh;i++)
    {
        if (X>=14)
        {
            X-=14;
            Y+=1;
        }
        LCD_Write_Num(X--,Y,DispBuff[i]);
    }
}*/
void printf (unsigned char *String, volatile unsigned long *Adress)
{
    static int DISPBUFF[84];
    static unsigned long Num;
    Num = *Adress;
    memset (DISPBUFF, 0, sizeof(DISPBUFF));
    unsigned char X_Disp, Y_Disp;
    X_Disp = 0;
    Y_Disp = 0;
    unsigned char i;
    static unsigned char X_Set;
    X_Set = 0;
    while (*String)
    {
        if (*String == '%')
        {
            String++;
            switch(*String)
            {
                case 'd':
                {
                    for (i = 0; i < ByteLengh; i++)
                    {
                        DISPBUFF[X_Set+ByteLengh-i-1] = Num%10 + 48;
                        Num = Num/10;
                    }
                    for (i = 0; i < ByteLengh - 1; i++)
                    {
                        if (DISPBUFF[X_Set+i] == 48)
                            DISPBUFF[X_Set+i] = 32;
                        else
                            break;
                    }
                    X_Set += ByteLengh;
                    String++;
                    break;
                }
                /*
                case 'f':
                {
                    float valflt = va_arg(ap,double);
                    printfloat(valflt);
                    String++;
                    break;
                } */
                default:
                {
                    String--;
                    DISPBUFF[X_Set] = '%';
                    String++;
                }
            }
        }
        else if(*String == '\n')
        {
            DISPBUFF[X_Set] = '\n';
            X_Set++;
            String++;
        }
        else
        {
            DISPBUFF[X_Set] = *String;
            X_Set++;
            String++;
        }
    }
    LCD_Set_XY (X_Disp, Y_Disp);

    unsigned char Line_Num;
    Line_Num = 0;
    for (i = 0; i < X_Set; i++)
    {
        if (DISPBUFF[i] == '\n')
        {
            Line_Num++;
            LCD_Set_XY(0, Y_Disp+Line_Num);
        }
        else
            LCD_Write_Char (DISPBUFF[i]);
    }
    //UCA0IFG |=UCTXIFG;
}
/*
                   _ooOoo_
                  o8888888o
                  88" . "88
                  (| -_- |)
                  O\  =  /O
               ____/`---'\____
             .'  \\|     |//  `.
            /  \\|||  :  |||//  \
           /  _||||| -:- |||||-  \
           |   | \\\  -  /// |   |
           | \_|  ''\---/''  |   |
           \  .-\__  `-`  ___/-. /
         ___`. .'  /--.--\  `. . __
      ."" '<  `.___\_<|>_/___.'  >'"".
     | | :  `- \`.;`\ _ /`;.`/ - ` : | |
     \  \ `-.   \_ __\ /__ _/   .-` /  /
======`-.____`-.___\_____/___.-`____.-'======
                   `=---='
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                         ���汣��       ����BUG
*/
/**
 *                      ������ . ����Ա֮��
 *
 *                  ʮ��������ãã��д���򣬵�������
 *                      ǧ�д��룬Bug�δ��ء�
 *                  ��ʹ����������������ģ�Ϧ�ϳ���
 *
 *                  �쵼ÿ�����뷨������ģ�����æ��
 *                      ������ԣ�Ω����ǧ�С�
 *                  ÿ��ƻ���ɺ����ҹ���£��Ӱ��
*/
/**
 * ����������   ���������Щ������Щ������Щ������� ���������Щ������Щ������Щ������� ���������Щ������Щ������Щ������� ���������Щ������Щ�������
 * ��Esc��   �� F1�� F2�� F3�� F4�� �� F5�� F6�� F7�� F8�� �� F9��F10��F11��F12�� ��P/S��S L��P/B��  ����    ����    ����
 * ����������   ���������ة������ة������ة������� ���������ة������ة������ة������� ���������ة������ة������ة������� ���������ة������ة�������  ����    ����    ����
 * ���������Щ������Щ������Щ������Щ������Щ������Щ������Щ������Щ������Щ������Щ������Щ������Щ������Щ��������������� ���������Щ������Щ������� ���������Щ������Щ������Щ�������
 * ��~ `��! 1��@ 2��# 3��$ 4��% 5��^ 6��& 7��* 8��( 9��) 0��_ -��+ =�� BacSp �� ��Ins��Hom��PUp�� ��N L�� / �� * �� - ��
 * ���������ة��Щ��ة��Щ��ة��Щ��ة��Щ��ة��Щ��ة��Щ��ة��Щ��ة��Щ��ة��Щ��ة��Щ��ة��Щ��ة��Щ��ة��Щ����������� ���������੤�����੤������ ���������੤�����੤�����੤������
 * �� Tab �� Q �� W �� E �� R �� T �� Y �� U �� I �� O �� P ��{ [��} ]�� | \ �� ��Del��End��PDn�� �� 7 �� 8 �� 9 ��   ��
 * �������������ةЩ����ةЩ����ةЩ����ةЩ����ةЩ����ةЩ����ةЩ����ةЩ����ةЩ����ةЩ����ةЩ����ةЩ����ة����������� ���������ة������ة������� ���������੤�����੤������ + ��
 * �� Caps �� A �� S �� D �� F �� G �� H �� J �� K �� L ��: ;��" '�� Enter  ��               �� 4 �� 5 �� 6 ��   ��
 * ���������������ة��Щ��ة��Щ��ة��Щ��ة��Щ��ة��Щ��ة��Щ��ة��Щ��ة��Щ��ة��Щ��ة��Щ��ة��Щ��ة�����������������     ����������     ���������੤�����੤�����੤������
 * �� Shift  �� Z �� X �� C �� V �� B �� N �� M ��< ,��> .��? /��  Shift   ��     �� �� ��     �� 1 �� 2 �� 3 ��   ��
 * �������������Щ����ة��Щ��ة����Щة������ة������ة������ة������ة������ة����Щة������੤�����ةЩ��������Щ��������� ���������੤�����੤������ ���������ة������੤������ E����
 * �� Ctrl��    ��Alt ��         Space         �� Alt��    ��    ��Ctrl�� �� �� �� �� �� �� �� ��   0   �� . ����������
 * �������������ة��������ة��������ة����������������������������������������������ة��������ة��������ة��������ة��������� ���������ة������ة������� �����������������ة������ة�������
 */
/**
 **************************************************************
 *                                                            *
 *   .=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-.       *
 *    |                     ______                     |      *
 *    |                  .-"      "-.                  |      *
 *    |                 /            \                 |      *
 *    |     _          |              |          _     |      *
 *    |    ( \         |,  .-.  .-.  ,|         / )    |      *
 *    |     > "=._     | )(__/  \__)( |     _.=" <     |      *
 *    |    (_/"=._"=._ |/     /\     \| _.="_.="\_)    |      *
 *    |           "=._"(_     ^^     _)"_.="           |      *
 *    |               "=\__|IIIIII|__/="               |      *
 *    |              _.="| \IIIIII/ |"=._              |      *
 *    |    _     _.="_.="\          /"=._"=._     _    |      *
 *    |   ( \_.="_.="     `--------`     "=._"=._/ )   |      *
 *    |    > _.="                            "=._ <    |      *
 *    |   (_/                                    \_)   |      *
 *    |                                                |      *
 *    '-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-='      *
 *                                                            *
 *           LASCIATE OGNI SPERANZA, VOI CH'ENTRATE           *
 **************************************************************
 */
/*
                       ::
                      :;J7, :,                        ::;7:
                      ,ivYi, ,                       ;LLLFS:
                      :iv7Yi                       :7ri;j5PL
                     ,:ivYLvr                    ,ivrrirrY2X,
                     :;r@Wwz.7r:                :ivu@kexianli.
                    :iL7::,:::iiirii:ii;::::,,irvF7rvvLujL7ur
                   ri::,:,::i:iiiiiii:i:irrv177JX7rYXqZEkvv17
                ;i:, , ::::iirrririi:i:::iiir2XXvii;L8OGJr71i
              :,, ,,:   ,::ir@mingyi.irii:i:::j1jri7ZBOS7ivv,
                 ,::,    ::rv77iiiriii:iii:i::,rvLq@huhao.Li
             ,,      ,, ,:ir7ir::,:::i;ir:::i:i::rSGGYri712:
           :::  ,v7r:: ::rrv77:, ,, ,:i7rrii:::::, ir7ri7Lri
          ,     2OBBOi,iiir;r::        ,irriiii::,, ,iv7Luur:
        ,,     i78MBBi,:,:::,:,  :7FSL: ,iriii:::i::,,:rLqXv::
        :      iuMMP: :,:::,:ii;2GY7OBB0viiii:i:iii:i:::iJqL;::
       ,     ::::i   ,,,,, ::LuBBu BBBBBErii:i:i:i:i:i:i:r77ii
      ,       :       , ,,:::rruBZ1MBBqi, :,,,:::,::::::iiriri:
     ,               ,,,,::::i:  @arqiao.       ,:,, ,:::ii;i7:
    :,       rjujLYLi   ,,:::::,:::::::::,,   ,:i,:,,,,,::i:iii
    ::      BBBBBBBBB0,    ,,::: , ,:::::: ,      ,,,, ,,:::::::
    i,  ,  ,8BMMBBBBBBi     ,,:,,     ,,, , ,   , , , :,::ii::i::
    :      iZMOMOMBBM2::::::::::,,,,     ,,,,,,:,,,::::i:irr:i:::,
    i   ,,:;u0MBMOG1L:::i::::::  ,,,::,   ,,, ::::::i:i:iirii:i:i:
    :    ,iuUuuXUkFu7i:iii:i:::, :,:,: ::::::::i:i:::::iirr7iiri::
    :     :rk@Yizero.i:::::, ,:ii:::::::i:::::i::,::::iirrriiiri::,
     :      5BMBBBBBBSr:,::rv2kuii:::iii::,:i:,, , ,,:,:i@petermu.,
          , :r50EZ8MBBBBGOBBBZP7::::i::,:::::,: :,:,::i;rrririiii::
              :jujYY7LS0ujJL7r::,::i::,::::::::::::::iirirrrrrrr:ii:
           ,:  :@kevensun.:,:,,,::::i:i:::::,,::::::iir;ii;7v77;ii;i,
           ,,,     ,,:,::::::i:iiiii:i::::,, ::::iiiir@xingjief.r;7:i,
        , , ,,,:,,::::::::iiiiiiiiii:,:,:::::::::iiir;ri7vL77rrirri::
         :,, , ::::::::i:::i:::i:i::,,,,,:,::i:i:::iir;@Secbone.ii:::
 */
/**********//**********//**********//**********//**********//**********//**********

  .--,       .--,
 ( (  \.---./  ) )
  '.__/o   o\__.'
     {=  ^  =}
      >  -  <
     /       \
    //       \\
   //|   .   |\\
   "'\       /'"_.-~^`'-.
      \  _  /--'         `
    ___)( )(___
   (((__) (__)))    ��ɽ��ֹ,������ֹ.�䲻����,������֮��

**********//**********//**********//**********//**********//**********//**********/
//
//                       .::::.
//                     .::::::::.
//                    :::::::::::
//                 ..:::::::::::'
//              '::::::::::::'
//                .::::::::::
//           '::::::::::::::..
//                ..::::::::::::.
//              ``::::::::::::::::
//               ::::``:::::::::'        .:::.
//              ::::'   ':::::'       .::::::::.
//            .::::'      ::::     .:::::::'::::.
//           .:::'       :::::  .:::::::::' ':::::.
//          .::'        :::::.:::::::::'      ':::::.
//         .::'         ::::::::::::::'         ``::::.
//     ...:::           ::::::::::::'              ``::.
//    ```` ':.          ':::::::::'                  ::::..
//                       '.:::::'                    ':'````..

