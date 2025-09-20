#include "all_header.h"

uint8_t addr[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};

#ifdef TX_MODE

uint8_t data[10] = "Hello Tan";

int main()
{
  Delay_Init();

  uart1.config(115200, REMAP);

  SPI1_Config();
	
  NRF_TX_Mode_Init(addr, 40, 32);

  uart1.print("\n%h - %d - %d\n", NRF_ReadReg_WithOneByte(NRF_REG_CONFIG), \
                                NRF_ReadReg_WithOneByte(NRF_REG_RX_PW_P0), \
                                NRF_ReadReg_WithOneByte(NRF_REG_RF_CH));

	uint8_t addr[5];
  NRF_ReadReg_WithMultiBytes(NRF_REG_RX_ADDR_P0, addr, 5);
  uart1.print("\n0x");
  for(int i=0; i<5; i++) uart1.print("%h", addr[i]);
  uart1.print("\n");


  while (1)
  {
		// uart1.print("fifo: %h - ", NRF_ReadReg_WithOneByte(NRF_REG_FIFO_STATUS));
    // if(NRF_ReadReg_WithOneBit(NRF_REG_FIFO_STATUS, FIFO_TX_FULL)) 
    // {
    //   NRF_Flush_TX();
    // }

		// uart1.print("fifo: %h\n", NRF_ReadReg_WithOneByte(NRF_REG_FIFO_STATUS));
    NRF_SendData(data, sizeof(data));
		
    delay_ms(1000);
  }
}

#else

uint8_t data[10];

int main()
{
  Delay_Init();

  uart1.config(115200, REMAP);

  SPI1_Config();
	
  NRF_RX_Mode_Init(addr, 40, 32);

  NRF_StartListening();

  uart1.print("\n%h - %d - %d\n", NRF_ReadReg_WithOneByte(NRF_REG_CONFIG), \
                                NRF_ReadReg_WithOneByte(NRF_REG_RX_PW_P0), \
                                NRF_ReadReg_WithOneByte(NRF_REG_RF_CH));

  NRF_ReadReg_WithMultiBytes(NRF_REG_RX_ADDR_P0, data, 5);
  uart1.print("\n0x");
  for(int i=0; i<5; i++) uart1.print("%h", data[i]);
  uart1.print("\n");

  while (1)
  {
		// uart1.print("%h\n", NRF_ReadReg_WithOneByte(NRF_REG_STATUS));
    if(NRF_DataReady())
    {
			uart1.print("\nData ready: ");

      NRF_ReadData(data, 32);

      uart1.print("%s\n", data);
    }
		delay_ms(100);
  }
}
#endif