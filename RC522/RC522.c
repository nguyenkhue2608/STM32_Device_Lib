
#include "RC522.h"
	
static void MFRC522_CS(MFRC522_Name* MFRC522, uint8_t Value)
{
	HAL_GPIO_WritePin(MFRC522->CS_PORT, MFRC522->CS_PIN, Value);
}
static uint8_t TM_SPI_Send(MFRC522_Name* MFRC522, uint8_t data)
{
	uint8_t data_se[1], data_re[1];
	data_se[0] = data;
	while(!__HAL_SPI_GET_FLAG(MFRC522->SPI, SPI_FLAG_TXE));
	HAL_SPI_TransmitReceive(MFRC522->SPI, data_se, data_re, 1, 10);
	while(__HAL_SPI_GET_FLAG(MFRC522->SPI, SPI_FLAG_BSY));	/*!< Send byte through the SPI1 peripheral */
	return data_re[0];
	
}
static void MFRC522_WriteRegister(MFRC522_Name* MFRC522, uint8_t addr, uint8_t val) 
{
	//CS low
	uint8_t Address = (addr << 1) & 0x7E;
	MFRC522_CS(MFRC522, 0);
	//Send address
	TM_SPI_Send(MFRC522, Address);
	//Send data	
	HAL_Delay(1);
	TM_SPI_Send(MFRC522, val);
	HAL_Delay(1);
	//CS high
	MFRC522_CS(MFRC522, 1);
}

static uint8_t MFRC522_ReadRegister(MFRC522_Name* MFRC522, uint8_t addr) 
{
	uint8_t Value;
	uint8_t Address = ((addr << 1) & 0x7E)|0x80;
	//CS low
	MFRC522_CS(MFRC522, 0);
	TM_SPI_Send(MFRC522, Address);
	Value = TM_SPI_Send(MFRC522, MFRC522_DUMMY);
	//CS high
	MFRC522_CS(MFRC522, 1);

	return Value;	
}

static void MFRC522_SetBitMask(MFRC522_Name* MFRC522, uint8_t reg, uint8_t mask) 
{
	MFRC522_WriteRegister(MFRC522, reg, MFRC522_ReadRegister(MFRC522, reg) | mask);
}

static void MFRC522_ClearBitMask(MFRC522_Name* MFRC522, uint8_t reg, uint8_t mask)
{
	MFRC522_WriteRegister(MFRC522, reg, MFRC522_ReadRegister(MFRC522, reg) & (~mask));
} 
static void MFRC522_AntennaOn(MFRC522_Name* MFRC522)
{
	uint8_t temp;

	temp = MFRC522_ReadRegister(MFRC522, MFRC522_REG_TX_CONTROL);
	if (!(temp & 0x03)) {
		MFRC522_SetBitMask(MFRC522, MFRC522_REG_TX_CONTROL, 0x03);
	}
}

void MFRC522_AntennaOff(MFRC522_Name* MFRC522) 
{
	MFRC522_ClearBitMask(MFRC522, MFRC522_REG_TX_CONTROL, 0x03);
}

static void MFRC522_Reset(MFRC522_Name* MFRC522) 
{
	MFRC522_WriteRegister(MFRC522, MFRC522_REG_COMMAND, PCD_RESETPHASE);
}

void MFRC522_Init(MFRC522_Name* MFRC522, SPI_HandleTypeDef* SPI_In, GPIO_TypeDef*	CS_PORT, uint16_t CS_PIN)
{
	MFRC522->SPI = SPI_In;
	MFRC522->CS_PORT = CS_PORT;
	MFRC522->CS_PIN = CS_PIN;
	MFRC522_Reset(MFRC522);
	
	MFRC522_WriteRegister(MFRC522, MFRC522_REG_T_MODE, 0x8D);
	MFRC522_WriteRegister(MFRC522, MFRC522_REG_T_PRESCALER, 0x3E);
	MFRC522_WriteRegister(MFRC522, MFRC522_REG_T_RELOAD_L, 30);           
	MFRC522_WriteRegister(MFRC522, MFRC522_REG_T_RELOAD_H, 0);

	/* 48dB gain */
	MFRC522_WriteRegister(MFRC522, MFRC522_REG_RF_CFG, 0x70);
	
	MFRC522_WriteRegister(MFRC522, MFRC522_REG_TX_AUTO, 0x40);
	MFRC522_WriteRegister(MFRC522, MFRC522_REG_MODE, 0x3D);

	MFRC522_AntennaOn(MFRC522);		//Open the antenna
}
MFRC522_Status_t MFRC522_Check(MFRC522_Name* MFRC522, uint8_t* id) 
{
	MFRC522_Status_t status;
	//Find cards, return card type
	status = MFRC522_Request(MFRC522, PICC_REQIDL, id);	
	if (status == MI_OK) {
		//Card detected
		//Anti-collision, return card serial number 4 bytes
		status = MFRC522_Anticoll(MFRC522, id);	
	}
	MFRC522_Halt(MFRC522);			//Command card into hibernation 

	return status;
}

MFRC522_Status_t MFRC522_Compare(MFRC522_Name* MFRC522, uint8_t* CardID, uint8_t* CompareID) 
{
	uint8_t i;
	for (i = 0; i < 5; i++) {
		if (CardID[i] != CompareID[i]) {
			return MI_ERR;
		}
	}
	return MI_OK;
}
MFRC522_Status_t MFRC522_Request(MFRC522_Name* MFRC522, uint8_t reqMode, uint8_t* TagType) 
{
	MFRC522_Status_t status;  
	uint16_t backBits;			//The received data bits

	MFRC522_WriteRegister(MFRC522, MFRC522_REG_BIT_FRAMING, 0x07);		//TxLastBists = BitFramingReg[2..0]	???

	TagType[0] = reqMode;
	status = MFRC522_ToCard(MFRC522, PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);

	if ((status != MI_OK) || (backBits != 0x10)) {    
		status = MI_ERR;
	}

	return status;
}
MFRC522_Status_t MFRC522_ToCard(MFRC522_Name* MFRC522, uint8_t command, uint8_t* sendData, uint8_t sendLen, uint8_t* backData, uint16_t* backLen)
{
	MFRC522_Status_t status = MI_ERR;
	uint8_t irqEn = 0x00;
	uint8_t waitIRq = 0x00;
	uint8_t lastBits;
	uint8_t n;
	uint16_t i;

	switch (command) {
		case PCD_AUTHENT: {
			irqEn = 0x12;
			waitIRq = 0x10;
			break;
		}
		case PCD_TRANSCEIVE: {
			irqEn = 0x77;
			waitIRq = 0x30;
			break;
		}
		default:
			break;
	}

	MFRC522_WriteRegister(MFRC522, MFRC522_REG_COMM_IE_N, irqEn | 0x80);
	MFRC522_ClearBitMask(MFRC522, MFRC522_REG_COMM_IRQ, 0x80);
	MFRC522_SetBitMask(MFRC522, MFRC522_REG_FIFO_LEVEL, 0x80);

	MFRC522_WriteRegister(MFRC522, MFRC522_REG_COMMAND, PCD_IDLE);

	//Writing data to the FIFO
	for (i = 0; i < sendLen; i++) {   
		MFRC522_WriteRegister(MFRC522, MFRC522_REG_FIFO_DATA, sendData[i]);    
	}

	//Execute the command
	MFRC522_WriteRegister(MFRC522, MFRC522_REG_COMMAND, command);
	if (command == PCD_TRANSCEIVE) {    
		MFRC522_SetBitMask(MFRC522, MFRC522_REG_BIT_FRAMING, 0x80);		//StartSend=1,transmission of data starts  
	}   

	//Waiting to receive data to complete
	i = 2000;	//i according to the clock frequency adjustment, the operator M1 card maximum waiting time 25ms???
	do {
		//CommIrqReg[7..0]
		//Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
		n = MFRC522_ReadRegister(MFRC522, MFRC522_REG_COMM_IRQ);
		i--;
	} while ((i!=0) && !(n&0x01) && !(n&waitIRq));

	MFRC522_ClearBitMask(MFRC522, MFRC522_REG_BIT_FRAMING, 0x80);			//StartSend=0

	if (i != 0)  {
		if (!(MFRC522_ReadRegister(MFRC522, MFRC522_REG_ERROR) & 0x1B)) {
			status = MI_OK;
			if (n & irqEn & 0x01) {   
				status = MI_NOTAGERR;			
			}

			if (command == PCD_TRANSCEIVE) {
				n = MFRC522_ReadRegister(MFRC522, MFRC522_REG_FIFO_LEVEL);
				lastBits = MFRC522_ReadRegister(MFRC522, MFRC522_REG_CONTROL) & 0x07;
				if (lastBits) {   
					*backLen = (n - 1) * 8 + lastBits;   
				} else {   
					*backLen = n * 8;   
				}

				if (n == 0) {   
					n = 1;    
				}
				if (n > MFRC522_MAX_LEN) {   
					n = MFRC522_MAX_LEN;   
				}

				//Reading the received data in FIFO
				for (i = 0; i < n; i++) {   
					backData[i] = MFRC522_ReadRegister(MFRC522, MFRC522_REG_FIFO_DATA);    
				}
			}
		} else {   
			status = MI_ERR;  
		}
	}

	return status;
}

MFRC522_Status_t MFRC522_Anticoll(MFRC522_Name* MFRC522, uint8_t* serNum)
{
	MFRC522_Status_t status;
	uint8_t i;
	uint8_t serNumCheck = 0;
	uint16_t unLen;

	MFRC522_WriteRegister(MFRC522, MFRC522_REG_BIT_FRAMING, 0x00);		//TxLastBists = BitFramingReg[2..0]

	serNum[0] = PICC_ANTICOLL;
	serNum[1] = 0x20;
	status = MFRC522_ToCard(MFRC522, PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);

	if (status == MI_OK) {
		//Check card serial number
		for (i = 0; i < 4; i++) {   
			serNumCheck ^= serNum[i];
		}
		if (serNumCheck != serNum[i]) {   
			status = MI_ERR;    
		}
	}
	return status;
} 

void MFRC522_CalculateCRC(MFRC522_Name* MFRC522, uint8_t*  pIndata, uint8_t len, uint8_t* pOutData) {
	uint8_t i, n;

	MFRC522_ClearBitMask(MFRC522, MFRC522_REG_DIV_IRQ, 0x04);			//CRCIrq = 0
	MFRC522_SetBitMask(MFRC522, MFRC522_REG_FIFO_LEVEL, 0x80);			//Clear the FIFO pointer
	//Write_MFRC522(CommandReg, PCD_IDLE);

	//Writing data to the FIFO	
	for (i = 0; i < len; i++) {   
		MFRC522_WriteRegister(MFRC522, MFRC522_REG_FIFO_DATA, *(pIndata+i));   
	}
	MFRC522_WriteRegister(MFRC522, MFRC522_REG_COMMAND, PCD_CALCCRC);

	//Wait CRC calculation is complete
	i = 0xFF;
	do {
		n = MFRC522_ReadRegister(MFRC522, MFRC522_REG_DIV_IRQ);
		i--;
	} while ((i!=0) && !(n&0x04));			//CRCIrq = 1

	//Read CRC calculation result
	pOutData[0] = MFRC522_ReadRegister(MFRC522, MFRC522_REG_CRC_RESULT_L);
	pOutData[1] = MFRC522_ReadRegister(MFRC522, MFRC522_REG_CRC_RESULT_M);
}

uint8_t MFRC522_SelectTag(MFRC522_Name* MFRC522, uint8_t* serNum) 
{
	uint8_t i;
	MFRC522_Status_t status;
	uint8_t size;
	uint16_t recvBits;
	uint8_t buffer[9]; 

	buffer[0] = PICC_SElECTTAG;
	buffer[1] = 0x70;
	for (i = 0; i < 5; i++) {
		buffer[i+2] = *(serNum+i);
	}
	MFRC522_CalculateCRC(MFRC522, buffer, 7, &buffer[7]);		//??
	status = MFRC522_ToCard(MFRC522, PCD_TRANSCEIVE, buffer, 9, buffer, &recvBits);

	if ((status == MI_OK) && (recvBits == 0x18)) {   
		size = buffer[0]; 
	} else {   
		size = 0;    
	}

	return size;
}

MFRC522_Status_t MFRC522_Auth(MFRC522_Name* MFRC522, uint8_t authMode, uint8_t BlockAddr, uint8_t* Sectorkey, uint8_t* serNum) 
{
	MFRC522_Status_t status;
	uint16_t recvBits;
	uint8_t i;
	uint8_t buff[12]; 

	//Verify the command block address + sector + password + card serial number
	buff[0] = authMode;
	buff[1] = BlockAddr;
	for (i = 0; i < 6; i++) {    
		buff[i+2] = *(Sectorkey+i);   
	}
	for (i=0; i<4; i++) {    
		buff[i+8] = *(serNum+i);   
	}
	status = MFRC522_ToCard(MFRC522, PCD_AUTHENT, buff, 12, buff, &recvBits);

	if ((status != MI_OK) || (!(MFRC522_ReadRegister(MFRC522, MFRC522_REG_STATUS2) & 0x08))) {   
		status = MI_ERR;   
	}

	return status;
}

MFRC522_Status_t MFRC522_Read(MFRC522_Name* MFRC522, uint8_t blockAddr, uint8_t* recvData)
{
	MFRC522_Status_t status;
	uint16_t unLen;

	recvData[0] = PICC_READ;
	recvData[1] = blockAddr;
	MFRC522_CalculateCRC(MFRC522, recvData,2, &recvData[2]);
	status = MFRC522_ToCard(MFRC522, PCD_TRANSCEIVE, recvData, 4, recvData, &unLen);

	if ((status != MI_OK) || (unLen != 0x90)) {
		status = MI_ERR;
	}

	return status;
}

MFRC522_Status_t MFRC522_Write(MFRC522_Name* MFRC522, uint8_t blockAddr, uint8_t* writeData) 
{
	MFRC522_Status_t status;
	uint16_t recvBits;
	uint8_t i;
	uint8_t buff[18]; 

	buff[0] = PICC_WRITE;
	buff[1] = blockAddr;
	MFRC522_CalculateCRC(MFRC522, buff, 2, &buff[2]);
	status = MFRC522_ToCard(MFRC522, PCD_TRANSCEIVE, buff, 4, buff, &recvBits);

	if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A)) {   
		status = MI_ERR;   
	}

	if (status == MI_OK) {
		//Data to the FIFO write 16Byte
		for (i = 0; i < 16; i++) {    
			buff[i] = *(writeData+i);   
		}
		MFRC522_CalculateCRC(MFRC522, buff, 16, &buff[16]);
		status = MFRC522_ToCard(MFRC522, PCD_TRANSCEIVE, buff, 18, buff, &recvBits);

		if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A)) {   
			status = MI_ERR;   
		}
	}

	return status;
}

void MFRC522_Halt(MFRC522_Name* MFRC522)
{
	uint16_t unLen;
	uint8_t buff[4]; 

	buff[0] = PICC_HALT;
	buff[1] = 0;
	MFRC522_CalculateCRC(MFRC522, buff, 2, &buff[2]);

	MFRC522_ToCard(MFRC522, PCD_TRANSCEIVE, buff, 4, buff, &unLen);
}

