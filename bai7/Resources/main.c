#include "all_header.h"

uint8_t addr[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
uint8_t channel = 64;

uint8_t data[] = "Hello. My name is Tan";

#ifdef TX_MODE

int main()
{
  Delay_Init();

  uart1.config(115200, REMAP);

  SPI1_Config();
	
  NRF_TX_Mode_Init(addr, channel);

  uart1.print("\n%h - %d - %d\n", NRF_ReadReg_WithOneByte(NRF_REG_CONFIG), \
                                NRF_ReadReg_WithOneByte(NRF_REG_RX_PW_P0), \
                                NRF_ReadReg_WithOneByte(NRF_REG_RF_CH));

	uint8_t addr[ADDRESS_LENGTH];
  NRF_ReadReg_WithMultiBytes(NRF_REG_RX_ADDR_P0, addr, ADDRESS_LENGTH);
  uart1.print("\n0x");
  for(int i=0; i<ADDRESS_LENGTH; i++) uart1.print("%h", addr[i]);
  uart1.print("\n");


  while (1)
  {
		uart1.print("fifo: %h\n", NRF_ReadReg_WithOneByte(NRF_REG_FIFO_STATUS));
    NRF_SendData(data, sizeof(data));
		
    delay_ms(1000);
  }
}

#else

int main()
{
  Delay_Init();

  uart1.config(115200, REMAP);

  SPI1_Config();
	
  NRF_RX_Mode_Init(addr, channel);

  NRF_StartListening();
 
  uart1.print("\n%h - %d - %d\n", NRF_ReadReg_WithOneByte(NRF_REG_CONFIG), \
                                NRF_ReadReg_WithOneByte(NRF_REG_RX_PW_P0), \
                                NRF_ReadReg_WithOneByte(NRF_REG_RF_CH));

  NRF_ReadReg_WithMultiBytes(NRF_REG_RX_ADDR_P0, data, ADDRESS_LENGTH);
  uart1.print("\n0x");
  for(int i=0; i<ADDRESS_LENGTH; i++) uart1.print("%h", data[i]);
  uart1.print("\n");

  while (1)
  {
		// uart1.print("%h\n", NRF_ReadReg_WithOneByte(NRF_REG_STATUS));
    if(NRF_DataReady())
    {
			uart1.print("\nData ready: ");

      NRF_ReadData(data, sizeof(data));

      uart1.print("%s\n", data);
    }
  }
}
#endif