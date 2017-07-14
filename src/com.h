

#ifndef	_COM_H_
#define	_COM_H_


#ifndef TRUE	
#define	TRUE	1
#endif

#ifndef FALSE	
#define	FALSE	0
#endif


#define TXIE_485 	TXIE
#define RCIE_485 	RCIE
#define	TXREG_485	TXREG


extern	void    init_comms(void);

void Interrupt_COM1Rx(unsigned char buf);

extern	void	Com1TxStart(void);


#define LED_RUN 			LATC5	//0
#define LED_RX       	LATC4 	//0
#define LED_TX       	LATD3	//0

#define TX485_EN       	LATD2	//0



//////////////////////////////////////////////////
//////////////////////////////////////////////////

#define         EOT             0x04
#define         ETX             0x03
#define         ENQ             0x05
#define         ACK             0x06


#define         RTX_CHK         0 // Rx Start
#define         ADDRESS_CHK     1
#define         COMMAND_CHK     2
#define         DATACNT_CHK     3
#define         DATA_CHK        4
#define         CHKSUM_CHK      5
#define         ETX_CHK         6
#define         RX_ERROR        7
#define         RX_GOOD         8
#define         TX_SET          9
#define         CHKSUM_LOW_CHK  10
#define         CHKSUM_HIGH_CHK 11
#define         TX_ALL          12





//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////




#define			COM1_MAX_TX_BUF		100
#define			COM1_MAX_RX_BUF		20
unsigned char	Com1RxBuffer[COM1_MAX_RX_BUF];
unsigned char 	cntRx = 0;

#define MAX_PACKET 12
char com1_buf[MAX_PACKET];
unsigned int index;



unsigned char befTxData = 0;

unsigned char	strPt = 0;
unsigned char 	nEleFloor = 0;



bit			   bTxFinsh = FALSE;





char JUTF_str_SetColor1[] = "$l,0,128,128,128#"; // 회색 
char JUTF_str_RectButton[] = "$l,2,400,10,64,64,0#"; // 버튼 사각형 그리기 (색 칠하기 x)



char JUTF_str4[] = "$l,0,192,0,0#"; // 17
char JUTF_str5[] = "$l,2,100,100,200,200,1#"; // 19
// 이미지 로드용




















char *ptStr; 




extern	unsigned 	char	Com1RxBuffer[COM1_MAX_RX_BUF];
extern  unsigned 	char 	Virt_Com1RxBuffer[];








extern	unsigned 	char	Com1TxCnt;
extern	unsigned 	char	Com1TxThisPt;

extern	unsigned 	char   	Com1RxStatus;
extern	unsigned 	char   	Com1RxCurCnt;
extern	unsigned 	char 	Com1SerialTime;
extern	unsigned 	long	Com1BaudRate;






extern void Init_Com1RxBuffer(void);
extern void Interrupt_COM1Tx(void);
void serial_interrupt();
extern void Com1TxStart(void);
void Com1Tx_init();



#endif

