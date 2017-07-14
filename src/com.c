
#include    <pic18.h>

#include    "com.h"
#include    "serial.h"
#include  	"cpu18f45k80.h"
#include	"delay.h"


///////////////////////
#define     ASCTOHEX(x) ((x <= '9') ? (x - '0') : (x - '7')) 
#define     L_SHIFT(x)  (x << 4)

unsigned char 	Virt_Com1RxBuffer[6] = {0x00, 0xFE, 0x23, 0x02, 0x00, 0x00};	

unsigned char	Com1TxCnt=0;
unsigned char	Com1TxThisPt=0;

unsigned char   Com1RxStatus=0;
unsigned char	Com1RxCurCnt=0;
unsigned char	Com1SerialTime=0x0;
unsigned long	Com1BaudRate=115200;


void    init_comms(void)
{
	unsigned int tmpbaudrate;

	SPBRG = DIVIDER;     	

	tmpbaudrate=((int)(FOSC/(16UL * Com1BaudRate) -1));
	SPBRG = tmpbaudrate;     	

	TXSTA = (SPEED|NINE_BITS|0x20);
	RCSTA = (NINE_BITS|0x90);
	TRISC6=OUTPUT;
	TRISC7=INPUT;

    RCIE_485=0;	    // USART RX interrupt enable
    TXIE_485=0;	    // USART TX interrupt enable

    RCIF=0;	    // USART RX interrupt enable
    TXIF=0;	    // USART TX interrupt enable

	UART1MD=0;
	UART2MD=1;
}


void Interrupt_COM1Tx(void)
{
		
}


void Interrupt_COM1Rx(unsigned char RxData)
{	
	if (RxData == 0x01)	cntRx = 0;
		
	Com1RxBuffer[cntRx] = RxData;	
	
	if (Com1RxBuffer[cntRx] == 0x00){
		Com1RxStatus = RX_GOOD;
		cntRx = 0;
	}
	else{
		cntRx++;
	}
}




void Com1TxStart(void)
{
	char i;

    Com1RxStatus = TX_SET;
	index = 0;
	TXREG_485 = Com1RxBuffer[index];
	TXIE_485 = TRUE; 
	LED_TX = !LED_TX;
	TX485_EN = 1;	
}

void Com1TxNextToEnd()
{
	if (Com1RxBuffer[index] == 0x00){
		DelayMs(3);
		TXIE_485 = 0;
		TX485_EN = 0;	
		return;
	}
	
	index++;
	TXREG_485 = Com1RxBuffer[index];
}


void serial_interrupt()
{
	char i;
	unsigned char   buf;

	if((TXIE_485)&&(TXIF))										/*transmit interrupt routine*/
	{
        TXIF=0;
		Com1TxNextToEnd();		
	}	

	if((RCIE_485)&&(RCIF))										/*receive interrupt routine*/
	{
        RCIF = 0;
		buf = RCREG;	
		if(TXIE_485 != 1){
        	Interrupt_COM1Rx(buf);
		}
	}	

	if(OERR){
      	TXEN=0;
      	TXEN=1;
      	SPEN=0;
      	SPEN=1;
		CREN=0;
    }

	if( !CREN )	CREN=1;

}




