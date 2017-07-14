
#include    <pic18.h>

#include        "com.h"
#include        "cpu18f45k80.h"
#include	"delay.h"

unsigned    char  	MainTimer=0;
unsigned    char	msec100=0;

bit bVirt_RxGood = 0;
unsigned int Virt_RxGoodTimer = 0;


void    PortInit(void)
{
	TRISA=0;
	TRISB=0;
	TRISC=0;
	TRISD=0;
	TRISE=0;

  	LATA=0xff;	
  	LATB=0xff;	
  	LATC=0xff;	
  	LATD=0xff;	
  	LATE=0xff;	
}


void    UserBaudRate(void)
{
	unsigned char val;
	val=0;

	TRISE0=1;
	TRISE1=1;
	TRISE2=1;
	if(!RE0)	val= (val | 0x01);		
	if(!RE1)	val= (val | 0x02);		
	if(!RE2)	val= (val | 0x04);
	switch(val){
		case	0:	
			Com1BaudRate=9600;
			break;
		case	1:	// �뼺 ���ùݿ� 
			Com1BaudRate=19200;
			TX_DATA_CNT = 40; 
			break;
		case	2:	// ���� ���ùݿ� 
			Com1BaudRate=38400;
			TX_DATA_CNT = 88; 
			break;
		case	3:	// LCD JUTF��
			Com1BaudRate=115200;
			TX_DATA_CNT = 40; 
			break;
		default:
			Com1BaudRate=115200;
			break;		
	}			
}


void LoadCom1buf_StartLCDCmd(unsigned char *buf)
{
	char i, j;

	// 초기화 
	for (i=0; i<MAX_PACKET; i++)
	{
		com1_buf[i] = 0;
	}	
	
	for (i=0; buf[i]!=0; i++)
	{
		com1_buf[i] = buf[i];
	}

	SetCom1TxEnable();
}




volatile unsigned int mytest=0;
/*
// Rx 데이타를 받으면 받은 데이타를 Tx로 다시 돌려 보내는 프로그램이다. 	 
*/
void main(void)
{
	unsigned char i;	
    
    di();
    Initial();
    PortInit();
    Timer0Init(); 
	UserBaudRate();
    init_comms();
    ei();

	MainTimer=0;
	msec100=0;
	Com1SerialTime=0;		

	TXIE_485=0;
	RCIE_485=1;

	TRISD5=0;
	TX485_EN=0;

	Com1RxStatus=RTX_CHK; 
	strPt = 0;

    while (1)
	{ 		
        CLRWDT();
		/*
		if (MainTimer > 3)
		{
			MainTimer = 0;
			SetCom1TxEnable();
		}*/
		
		if(Com1RxStatus == RX_GOOD){
			Com1RxStatus = RTX_CHK;
			LED_RX = !LED_RX;			
			// LoadCom1buf_StartLCDCmd(Com1RxBuffer);
			SetCom1TxEnable();
		}
		

    }
}

	

void interrupt isr(void)
{
   
    if( TMR0IF )		
    {    
        TMR0IF = 0 ;
        TMR0L=MSEC_L;
        TMR0H=MSEC_H;    

        Com1SerialTime++;
        msec100++;

        if(msec100>100){
            msec100=0;
			
			MainTimer++;			
        } 

		mytest++;
    }

	serial_interrupt();	
}




