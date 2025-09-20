#include "nrf24l01.h"
#include "spi1.h"

void NRF_WriteReg_WithOneByte(uint8_t reg, uint8_t value) 
{
	NRF_CSN_LOW();
	SPI1_Transfer(NRF_CMD_W_REGISTER | reg);
	SPI1_Transfer(value);
	NRF_CSN_HIGH();
}

uint8_t NRF_ReadReg_WithOneByte(uint8_t reg) 
{
	NRF_CSN_LOW();
	SPI1_Transfer(NRF_CMD_R_REGISTER | reg);
	uint8_t data = SPI1_Transfer(0xff);
	NRF_CSN_HIGH();
	return data;
}

void NRF_WriteReg_WithOneBit(uint8_t reg, uint8_t bit, uint8_t value)
{
	uint8_t reg_value = NRF_ReadReg_WithOneByte(reg);
	uart1.print("l: %h - ", reg_value);

	reg_value = value ? (reg_value | bit) : (reg_value & ~bit);
	uart1.print("r: %h\n", reg_value);

	NRF_WriteReg_WithOneByte(reg, reg_value);
}

uint8_t NRF_ReadReg_WithOneBit(uint8_t reg, uint8_t bit)
{
	uint8_t reg_value = NRF_ReadReg_WithOneByte(reg);

	return (reg_value & bit) && 1;
}

void NRF_WriteReg_WithMultiBytes(uint8_t reg, uint8_t *data, uint8_t len) 
{
	NRF_CSN_LOW();
	SPI1_Transfer(NRF_CMD_W_REGISTER | reg);
	while(len--) 
	{
		SPI1_Transfer(*data++);
	}
	NRF_CSN_HIGH();
}

uint8_t NRF_ReadReg_WithMultiBytes(uint8_t reg, uint8_t *data, uint8_t len) 
{
	NRF_CSN_LOW();
	SPI1_Transfer(NRF_CMD_R_REGISTER | reg);
	while(len--) 
	{
		*data++ = SPI1_Transfer(0xff);
	}
	NRF_CSN_HIGH();
	return data;
}

void NRF_Flush_RX(void)
{
	NRF_CSN_LOW();
	SPI1_Transfer(NRF_CMD_FLUSH_RX);
	NRF_CSN_HIGH();
}

void NRF_Flush_TX(void)
{
	NRF_CSN_LOW();
	SPI1_Transfer(NRF_CMD_FLUSH_TX);
	NRF_CSN_HIGH();
}

uint8_t NRF_ReadStatus(void)
{
	uint8_t status;
	NRF_CSN_LOW();
	status = SPI1_Transfer(NRF_CMD_NOP); // đọc STATUS
	NRF_CSN_HIGH();
	return status;
}

/*------------------------------------------ TX Mode ------------------------------------------*/
#ifdef TX_MODE

void NRF_TX_Mode_Init(uint8_t *addr, uint8_t channel, uint8_t size_of_payload)
{
	delay_ms(20);
	
	NRF_WriteReg_WithOneByte(NRF_REG_CONFIG, 				CONFIG_EN_CRC); // CONFIG: EN_CRC = 1, PWR_UP=1
	NRF_WriteReg_WithOneByte(NRF_REG_EN_AA, 				ENAA_P0); // EN_AA: auto-ack
	NRF_WriteReg_WithOneByte(NRF_REG_EN_RXADDR, 		ERX_P0); // EN_RXADDR: enable pipe0
	NRF_WriteReg_WithOneByte(NRF_REG_SETUP_AW, 			AW_5_BYTES); // SETUP_AW: 5 bytes addr
	NRF_WriteReg_WithOneByte(NRF_REG_SETUP_RETR, 		0x3f); // SETUP_RETR: 1000us, 15 retries
	NRF_WriteReg_WithOneByte(NRF_REG_RF_CH, 				channel & 0x7F);   // RF_CH: channel
	NRF_WriteReg_WithOneByte(NRF_REG_RF_SETUP, 			0x0f); // RF_SETUP: 1Mbps, 0dBm
	
	// // Bật DPL
	// NRF_WriteReg_WithOneByte(NRF_REG_FEATURE, 0x04);  // EN_DPL
	// NRF_WriteReg_WithOneByte(NRF_REG_DYNPD,   0x01);  // Bật DPL cho pipe0
	
	NRF_WriteReg_WithMultiBytes(NRF_REG_TX_ADDR, addr, ADDRESS_LENGTH); 
	
	// RX_ADDR_P0 phải trùng với địa chỉ TX để Auto-ACK hoạt động
	NRF_WriteReg_WithMultiBytes(NRF_REG_RX_ADDR_P0, addr, ADDRESS_LENGTH); 
	
	// Set payload width cho pipe0 (32 byte)
	size_of_payload = size_of_payload > 32 ? 32 : size_of_payload;
	NRF_WriteReg_WithOneByte(NRF_REG_RX_PW_P0, size_of_payload);  
	
	// Clear cờ trong STATUS
	NRF_WriteReg_WithOneByte(NRF_REG_STATUS, 		STATUS_RX_DR | STATUS_TX_DS | STATUS_MAX_RT);
	
	NRF_Flush_TX();
	
	// TX mode: CE = 0, chỉ pulse khi gửi
	NRF_CE_LOW();

	NRF_WriteReg_WithOneBit(NRF_REG_CONFIG, CONFIG_PWR_UP, 1); // CONFIG: EN_CRC = 1, PWR_UP=1
	delay_ms(5);
	
}

void NRF_SendData(uint8_t *data, uint8_t len) 
{
	NRF_CSN_LOW();
	SPI1_Transfer(NRF_CMD_W_TX_PAYLOAD);
	while(len--) SPI1_Transfer(*data++);
	NRF_CSN_HIGH();

	// Pulse CE để phát
	NRF_CE_HIGH();
	delay_us(10); // delay ngắn ~10us
	NRF_CE_LOW();

	// Đợi TX_DS hoặc MAX_RT
	uint8_t status;
	do 
	{
		status = NRF_ReadStatus();

	} while (!(status & (STATUS_TX_DS | STATUS_MAX_RT))); // TX_DS hoặc MAX_RT

	// Xóa cờ
	NRF_WriteReg_WithOneByte(NRF_REG_STATUS, 0x70);
}

/*------------------------------------------ RX Mode ------------------------------------------*/
#else

void NRF_RX_Mode_Init(uint8_t *addr, uint8_t channel, uint8_t size_of_payload)
{
	delay_ms(20);
	
	NRF_WriteReg_WithOneByte(NRF_REG_CONFIG,				CONFIG_EN_CRC | CONFIG_PRIM_RX); // CONFIG: EN_CRC = 1, PWR_UP=1, PRIM_RX=1
	NRF_WriteReg_WithOneByte(NRF_REG_EN_AA,					ENAA_P0); // EN_AA: auto-ack
	NRF_WriteReg_WithOneByte(NRF_REG_EN_RXADDR,			ERX_P0); // EN_RXADDR: enable pipe0
	NRF_WriteReg_WithOneByte(NRF_REG_SETUP_AW,			AW_5_BYTES); // SETUP_AW: 5 bytes addr
	NRF_WriteReg_WithOneByte(NRF_REG_SETUP_RETR,		0x3f); // SETUP_RETR: 1000us, 15 retries
	NRF_WriteReg_WithOneByte(NRF_REG_RF_CH,					channel & 0x7F);   // RF_CH: channel
	NRF_WriteReg_WithOneByte(NRF_REG_RF_SETUP,			0x0f); // RF_SETUP: 1Mbps, 0dBm
	
	// // Bật DPL
	// NRF_WriteReg_WithOneByte(NRF_REG_FEATURE, 0x04);  // EN_DPL
	// NRF_WriteReg_WithOneByte(NRF_REG_DYNPD,   0x01);  // Bật DPL cho pipe0
	
	NRF_WriteReg_WithMultiBytes(NRF_REG_TX_ADDR, addr, ADDRESS_LENGTH); 
	
	NRF_WriteReg_WithMultiBytes(NRF_REG_RX_ADDR_P0, addr, ADDRESS_LENGTH); 
	
	// Set payload width cho pipe0 (32 byte)
	size_of_payload = size_of_payload > 32 ? 32 : size_of_payload;
	NRF_WriteReg_WithOneByte(NRF_REG_RX_PW_P0, size_of_payload); 
	
	// Clear interrupt flags trong STATUS
	NRF_WriteReg_WithOneByte(NRF_REG_STATUS,  	(STATUS_RX_DR | STATUS_TX_DS | STATUS_MAX_RT));
	
	NRF_WriteReg_WithOneBit(NRF_REG_CONFIG,	CONFIG_PWR_UP, 1);
	delay_ms(5);
	
	NRF_Flush_RX();

	NRF_StartListening();
}

void NRF_StartListening(void)
{
	NRF_CE_HIGH();
}

void NRF_StopListening(void)
{
	NRF_CE_LOW();
}


uint8_t NRF_DataReady(void) 
{
	uint8_t status = NRF_ReadStatus();
	return status & STATUS_RX_DR;
}

void NRF_Read_RX_Payload(uint8_t *data, uint8_t len)
{
	NRF_CSN_LOW();
	SPI1_Transfer(NRF_CMD_R_RX_PAYLOAD);
	while(len--) *data++ = SPI1_Transfer(0xFF);
	NRF_CSN_HIGH();
}

void NRF_ReadData(uint8_t *data, uint8_t len) 
{
	NRF_Read_RX_Payload(data, len);

	// Xóa cờ RX_DR
	NRF_WriteReg_WithOneByte(NRF_REG_STATUS, STATUS_RX_DR);
}

#endif
