#ifndef __NRF24L01__
#define __NRF24L01__
#ifdef __cplusplus
extern "C"{
#endif
#include "all_header.h"     

 #define TX_MODE


#define ADDRESS_LENGTH  5
#define PACKET_SIZE     32  

// SPI Commands
#define NRF_CMD_R_REGISTER         0x00
#define NRF_CMD_W_REGISTER         0x20
#define NRF_CMD_R_RX_PAYLOAD       0x61
#define NRF_CMD_W_TX_PAYLOAD       0xA0
#define NRF_CMD_FLUSH_TX           0xE1
#define NRF_CMD_FLUSH_RX           0xE2
#define NRF_CMD_REUSE_TX_PL        0xE3
#define NRF_CMD_R_RX_PL_WID        0x60
#define NRF_CMD_W_ACK_PAYLOAD      0xA8
#define NRF_CMD_W_TX_PAYLOAD_NOACK 0xB0
#define NRF_CMD_ACTIVATE           0x50
#define NRF_CMD_NOP                0xFF

// Register Addresses
#define NRF_REG_CONFIG             0x00
#define NRF_REG_EN_AA              0x01
#define NRF_REG_EN_RXADDR          0x02
#define NRF_REG_SETUP_AW           0x03
#define NRF_REG_SETUP_RETR         0x04
#define NRF_REG_RF_CH              0x05
#define NRF_REG_RF_SETUP           0x06
#define NRF_REG_STATUS             0x07
#define NRF_REG_OBSERVE_TX         0x08
#define NRF_REG_CD                 0x09
#define NRF_REG_RX_ADDR_P0         0x0A
#define NRF_REG_RX_ADDR_P1         0x0B
#define NRF_REG_RX_ADDR_P2         0x0C
#define NRF_REG_RX_ADDR_P3         0x0D
#define NRF_REG_RX_ADDR_P4         0x0E
#define NRF_REG_RX_ADDR_P5         0x0F
#define NRF_REG_TX_ADDR            0x10
#define NRF_REG_RX_PW_P0           0x11
#define NRF_REG_RX_PW_P1           0x12
#define NRF_REG_RX_PW_P2           0x13
#define NRF_REG_RX_PW_P3           0x14
#define NRF_REG_RX_PW_P4           0x15
#define NRF_REG_RX_PW_P5           0x16
#define NRF_REG_FIFO_STATUS        0x17
#define NRF_REG_DYNPD              0x1C
#define NRF_REG_FEATURE            0x1D

// CONFIG Register Bits
#define CONFIG_MASK_RX_DR          0x40
#define CONFIG_MASK_TX_DS          0x20
#define CONFIG_MASK_MAX_RT         0x10
#define CONFIG_EN_CRC              0x08
#define CONFIG_CRCO                0x04
#define CONFIG_PWR_UP              0x02
#define CONFIG_PRIM_RX             0x01

// EN_AA Register Bits
#define ENAA_P5                    0x20
#define ENAA_P4                    0x10
#define ENAA_P3                    0x08
#define ENAA_P2                    0x04
#define ENAA_P1                    0x02
#define ENAA_P0                    0x01

// EN_RXADDR Register Bits
#define ERX_P5                     0x20
#define ERX_P4                     0x10
#define ERX_P3                     0x08
#define ERX_P2                     0x04
#define ERX_P1                     0x02
#define ERX_P0                     0x01

// SETUP_AW Register Bits
#define AW_3_BYTES   0x01
#define AW_4_BYTES   0x02
#define AW_5_BYTES   0x03

// RF_SETUP Register Bits
#define RF_SETUP_CONT_WAVE        0x80
#define RF_SETUP_RF_DR_LOW        0x20
#define RF_SETUP_PLL_LOCK         0x10
#define RF_SETUP_RF_DR_HIGH       0x08
#define RF_SETUP_RF_PWR_MASK      0x06

// STATUS Register Bits
#define STATUS_RX_DR              0x40
#define STATUS_TX_DS              0x20
#define STATUS_MAX_RT             0x10
#define STATUS_RX_P_NO_MASK       0x0E
#define STATUS_TX_FULL            0x01

// FIFO_STATUS Register Bits
#define FIFO_TX_REUSE             0x40
#define FIFO_TX_FULL              0x20
#define FIFO_TX_EMPTY             0x10
#define FIFO_RX_FULL              0x02
#define FIFO_RX_EMPTY             0x01


// DYNPD Register Bits
#define DPL_P5                    0x20
#define DPL_P4                    0x10
#define DPL_P3                    0x08
#define DPL_P2                    0x04
#define DPL_P1                    0x02
#define DPL_P0                    0x01

// FEATURE Register Bits
#define EN_DPL                    0x04
#define EN_ACK_PAY                0x02
#define EN_DYN_ACK                0x01

#ifdef TX_MODE
void NRF_TX_Mode_Init(uint8_t *addr, uint8_t channel);
void NRF_SendData(uint8_t *data, uint8_t len);
void NRF_Flush_TX(void);

#else
void NRF_RX_Mode_Init(uint8_t *addr, uint8_t channel);
void NRF_StartListening(void);
void NRF_StopListening(void);
uint8_t NRF_DataReady(void);
void NRF_ReadData(uint8_t *data, uint8_t len);
void NRF_Flush_RX(void);
#endif

void NRF_WriteCmd(uint8_t cmd, uint8_t *value, uint8_t len);
void NRF_ReadCmd(uint8_t cmd, uint8_t *value, uint8_t len);

void NRF_WriteReg_WithOneBit(uint8_t reg, uint8_t bit, uint8_t value);
uint8_t NRF_ReadReg_WithOneBit(uint8_t reg, uint8_t bit);

void NRF_WriteReg_WithOneByte(uint8_t reg, uint8_t value);
uint8_t NRF_ReadReg_WithOneByte(uint8_t reg);

void NRF_WriteReg_WithMultiBytes(uint8_t reg, uint8_t *data, uint8_t len);
void NRF_ReadReg_WithMultiBytes(uint8_t reg, uint8_t *data, uint8_t len);

#ifdef __cplusplus
}
#endif
#endif