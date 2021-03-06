/*
*  RC522实验开发板例程
*  CPU:STC89C52RC
*  晶振:11.0592MHZ
------------------------------------------------*/
#include <reg52.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "RC522.h"
#include "UART.h"
#include "TIMER.h"

unsigned char g_ucTempbuf[20];
bit clsFlag=0;  // 1-需要清屏
unsigned char menu=0;   // 0-主菜单模式 1-显示ID
extern unsigned char clock;

void delay_ms(unsigned int z)
{
	unsigned int x, y;
	for (x = z; x>0; x--)
	for (y = 110; y>0; y--);
}

int main()
{
	unsigned char status, i;
	unsigned char cardType[3]={0};
    unsigned char cardCode[5]={0};
    
    // 上一次的卡信息
	unsigned char lastCardType[3]={0};
    unsigned char lastCardCode[5]={0};
    
	InitUART();
    InitTimer0();
    
	PcdReset();
	PcdAntennaOff();
	PcdAntennaOn();
	while (1)
	{
		status = PcdRequest(PICC_REQALL, g_ucTempbuf);//寻卡
		if (status != MI_OK)
		{
			PcdReset();
			PcdAntennaOff();
			PcdAntennaOn();
			continue;
		}

		for (i = 0; i<2; i++)
		{
            cardType[i] = g_ucTempbuf[i];
		}

		status = PcdAnticoll(g_ucTempbuf);//防冲撞
		if (status != MI_OK)
		{
			continue;
		}

		for (i = 0; i<4; i++)
		{
			cardCode[i] = g_ucTempbuf[i];
            
            menu = 1;
            clock = 0;
		}
        delay_ms(100);
        
        if ((strcmp(lastCardType, cardType) != 0) || (strcmp(lastCardCode, cardCode) != 0))
		{
            strcpy(lastCardType, cardType);
            strcpy(lastCardCode, cardCode);
            
            // 卡片类型
            Puts_to_SerialPort("Card Type: ");
            Putc_to_SerialPort_Hex(cardType[0]);
            Putc_to_SerialPort_Hex(cardType[1]);
            Puts_to_SerialPort("\r\n");
            
            // 卡片序列号
            Puts_to_SerialPort("Card Code: ");
            Putc_to_SerialPort_Hex(cardCode[0]);
            Putc_to_SerialPort_Hex(cardCode[1]);
            Putc_to_SerialPort_Hex(cardCode[2]);
            Putc_to_SerialPort_Hex(cardCode[3]);
            Puts_to_SerialPort("\r\n");
        }
        if (clsFlag == 1)
        {
            memset(lastCardType, 0, 3);
            memset(lastCardType, 0, 3);
            clsFlag = 0;
            menu = 0;
        }
	}
}

