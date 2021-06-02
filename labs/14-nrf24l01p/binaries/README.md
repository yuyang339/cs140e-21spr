run first:
    my-install /dev/ttyUSB0 1-client-one-way.bin
run second:
	my-install /dev/ttyUSB1 1-server-one-way.bin


client:

NRF:	CONFIG (1111):	PWR_UP=1, PRIM_RX=1, CRC=1 EN_CRC=1
NRF:	EN_AA (0):	pipes enabled:
NRF:	EN_RXADDR (10):	pipes enabled: pipe1=1 
NRF:	SETUP_AW:	address=3 bytes
NRF:	SETUP_RETR (=11):	retran delay=250usec, attempts=3
NRF:	RF_CH(=1110001):	2.513Mhz
NRF:	RF_SETUP(=1000):	bwidth = 2Mbs, power= -18dBm
NRF:	STATUS(=1110): RX-int=0, TX-int=0, RT-int=0 RXpipe=111 (empty), TX-full=0
NRF:	TX_ADDR = 0x0e7e7e7
NRF:	RX_ADDR_P1 = 0x0a2a2a2
NRF:	RX_PW_P1 (100): 4 byte fixed size packets
NRF:	FIFO_STATUS(=10001): TX_FULL=0, TX_EMPTY=1, RX_FULL=0, RX_EMPTY=1



Server:

NRF:	CONFIG (1111):	PWR_UP=1, PRIM_RX=1, CRC=1 EN_CRC=1
NRF:	EN_AA (0):	pipes enabled:
NRF:	EN_RXADDR (10):	pipes enabled: pipe1=1 
NRF:	SETUP_AW:	address=3 bytes
NRF:	SETUP_RETR (=11):	retran delay=250usec, attempts=3
NRF:	RF_CH(=1110001):	2.513Mhz
NRF:	RF_SETUP(=1000):	bwidth = 2Mbs, power= -18dBm
NRF:	STATUS(=1110): RX-int=0, TX-int=0, RT-int=0 RXpipe=111 (empty), TX-full=0
NRF:	TX_ADDR = 0x0e7e7e7
NRF:	RX_ADDR_P1 = 0x0d7d7d7
NRF:	RX_PW_P1 (100): 4 byte fixed size packets
NRF:	FIFO_STATUS(=10001): TX_FULL=0, TX_EMPTY=1, RX_FULL=0, RX_EMPTY=1
NRF:---------------------------------------------

