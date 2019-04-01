/*
*  RC522ʵ�鿪��������
*  CPU:STC89C52RC
*  ����:11.0592MHZ
------------------------------------------------*/
#include <reg52.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "RC522.h"
#include "UART.h"
#include "TIMER.h"

unsigned char g_ucTempbuf[20];
bit clsFlag=0;  // 1-��Ҫ����
unsigned char menu=0;   // 0-���˵�ģʽ 1-��ʾID
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
    
    // ��һ�εĿ���Ϣ
	unsigned char lastCardType[3]={0};
    unsigned char lastCardCode[5]={0};
    
	InitUART();
    InitTimer0();
    
	PcdReset();
	PcdAntennaOff();
	PcdAntennaOn();
	while (1)
	{
		status = PcdRequest(PICC_REQALL, g_ucTempbuf);//Ѱ��
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

		status = PcdAnticoll(g_ucTempbuf);//����ײ
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
            
            // ��Ƭ����
            Puts_to_SerialPort("Card Type: ");
            Putc_to_SerialPort_Hex(cardType[0]);
            Putc_to_SerialPort_Hex(cardType[1]);
            Puts_to_SerialPort("\r\n");
            
            // ��Ƭ���к�
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

