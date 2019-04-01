#include <reg52.h>
#include "UART.h"
#include <stdio.h>

void delayms(unsigned int z)
{
	unsigned int x, y;
	for (x = z; x>0; x--)
	for (y = 110; y>0; y--);
}

// �򴮿ڷ����ַ�
void Putc_to_SerialPort(unsigned char c)
{
	SBUF = c;
	while (TI == 0);
	TI = 0;
}

// �򴮿ڷ����ַ���
void Puts_to_SerialPort(unsigned char *s)
{
	while (*s != '\0')
	{
		Putc_to_SerialPort(*s);
		s++;
		delayms(5);
	}
}

// �򴮿ڷ���ʮ�������ַ�
void Putc_to_SerialPort_Hex(unsigned char c)
{
    // ��λ����
    if ((c>>4) < 10)
        Putc_to_SerialPort((c>>4) + '0');
    else
        Putc_to_SerialPort((c>>4) - 10 + 'A');
    
    // ��λ����
    if ((c&0xf) < 10)
        Putc_to_SerialPort((c&0xf) + '0');
    else
        Putc_to_SerialPort((c&0xf) - 10 + 'A');
}

// ��ʼ������
void InitUART()
{
	SCON = 0x40;		//����ģʽ1 
	TMOD = 0x20;		//T1����ģʽ2
	TH1 = 0xfd;		//������9600
	TL1 = 0xfd;
	PCON = 0x00;		//�����ʲ�����
	TI = 0;
	TR1 = 1;
	delayms(200);
}