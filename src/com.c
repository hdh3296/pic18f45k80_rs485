
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

    RCIE=0;	    // USART RX interrupt enable
    RS_TXIE=0;	    // USART TX interrupt enable

    RCIF=0;	    // USART RX interrupt enable
    TXIF=0;	    // USART TX interrupt enable

	UART1MD=0;
	UART2MD=1;
}


void Interrupt_COM1Tx(void)
{
		
}


void Interrupt_COM1Rx(void)
{

   	unsigned char   buf;
	
	if (RCREG == 0x01)	cntRx = 0;


   	buf = RCREG;		
	Com1RxBuffer[cntRx] = buf;
	
	
	if (Com1RxBuffer[cntRx] == 0x00){
		Com1RxStatus = RX_GOOD;
		cntRx = 0;
	}
	else{
		cntRx++;
	}
}




void SetCom1TxEnable(void)
{
	char i;

    Com1RxStatus = TX_SET;
	index = 0;
	RS_TXREG = Com1RxBuffer[index];
	RS_TXIE = TRUE; 
	LED_TX = !LED_TX;
	TX485_EN = 1;	
}

void Com1_TxData_NextAll()
{
	if (index >=6){
		DelayMs(3);
		RS_TXIE = 0;
		TX485_EN = 0;	
		return;
	}
	
	index++;
	RS_TXREG = Com1RxBuffer[index];
}


void serial_interrupt()
{

	char i;

	if((RS_TXIE)&&(TXIF))										/*transmit interrupt routine*/
	{
        TXIF=0;
		Com1_TxData_NextAll();		
	}	

	if( (RCIE)&&(RCIF) )										/*receive interrupt routine*/
	{
        RCIF = 0;
		if(RS_TXIE != 1){
        	Interrupt_COM1Rx();
		}
	}	

	if(OERR) {
      	TXEN=0;
      	TXEN=1;
      	SPEN=0;
      	SPEN=1;
		CREN=0;
    }

	if( !CREN)	CREN=1;

}




