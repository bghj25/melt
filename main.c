#include "MDR32Fx.h"                    // Device header
#include "MDR32F9Qx_rst_clk.h"          // Keil::Drivers:RST_CLK
#include "MDR32F9Qx_port.h"             // Keil::Drivers:PORT
#include "MDR32F9Qx_config.h"           // Keil::Device:Startup

#define DB0_PORT MDR_PORTC
#define DB1_PORT MDR_PORTC
#define DB2_PORT MDR_PORTF
#define DB3_PORT MDR_PORTF
#define WR1_PORT MDR_PORTF
#define WR2_PORT MDR_PORTA
#define A0_PORT MDR_PORTA

#define DB0_PIN PORT_Pin_0
#define DB1_PIN PORT_Pin_1
#define DB2_PIN PORT_Pin_1
#define DB3_PIN PORT_Pin_3
#define WR1_PIN PORT_Pin_0
#define WR2_PIN PORT_Pin_3
#define A0_PIN PORT_Pin_6

#define ONE_L 0x00
#define ONE_H 0x06
#define TWO_L 0x0F
#define TWO_H 0x02
#define THREE_L 0x0D
#define THREE_H 0x06
#define FOUR_L 0x01
#define FOUR_H 0x0E
#define FIVE_L 0x05
#define FIVE_H 0x0C
#define SIX_L 0x0F
#define SIX_H 0x0C
#define SEVEN_L 0x08
#define SEVEN_H 0x06
#define EIGHT_L 0x0F
#define EIGHT_H 0x0E
#define NINE_L 0x0D
#define NINE_H 0x0E
#define ZERO_L 0x0E
#define ZERO_H 0x0E
#define CLR_L 0x00
#define CLR_H 0x00


void SetPins(uint8_t data)
{
	PORT_WriteBit(DB0_PORT, DB0_PIN, data & 0x1);
	PORT_WriteBit(DB1_PORT, DB1_PIN, (data >> 1) & 0x1);
	PORT_WriteBit(DB2_PORT, DB2_PIN, (data >> 2) & 0x1);
	PORT_WriteBit(DB3_PORT, DB3_PIN, (data >> 3) & 0x1);
}


void output(uint8_t adr, uint8_t SGL, uint8_t SGH)
{
	//Выбираем адрес
	PORT_WriteBit(A0_PORT, A0_PIN, Bit_RESET);
	SetPins(adr);
	PORT_WriteBit(WR1_PORT, WR1_PIN, Bit_SET);
	__ASM("NOP");
	__ASM("NOP");
	PORT_WriteBit(WR1_PORT, WR1_PIN, Bit_RESET);
	__ASM("NOP");
	PORT_WriteBit(A0_PORT, A0_PIN, Bit_SET);
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	
	SetPins(SGL);
	PORT_WriteBit(WR1_PORT, WR1_PIN, Bit_SET);
	__ASM("NOP");
	__ASM("NOP");
	PORT_WriteBit(WR1_PORT, WR1_PIN, Bit_RESET);
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	
	SetPins(SGH);
	PORT_WriteBit(WR1_PORT, WR1_PIN, Bit_SET);
	__ASM("NOP");
	__ASM("NOP");
	PORT_WriteBit(WR1_PORT, WR1_PIN, Bit_RESET);
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");	

}


void Init_DISP(void)
{
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTA, ENABLE);
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC, ENABLE);
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTF, ENABLE);
	
	PORT_InitTypeDef PORT_InitStruct;
	PORT_InitStruct.PORT_SPEED = PORT_SPEED_MAXFAST;
	PORT_InitStruct.PORT_FUNC = PORT_FUNC_PORT;
	PORT_InitStruct.PORT_MODE = PORT_MODE_DIGITAL;
	PORT_InitStruct.PORT_OE = PORT_OE_OUT;
	PORT_InitStruct.PORT_Pin = PORT_Pin_7+PORT_Pin_4+PORT_Pin_5;                //указать пины, доинициализировать порты если надо 
	PORT_InitStruct.PORT_PULL_UP = PORT_PULL_UP_OFF;
	PORT_InitStruct.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	
	PORT_InitStruct.PORT_Pin = DB0_PIN+DB1_PIN;
	PORT_Init(MDR_PORTC, &PORT_InitStruct);
	
	PORT_InitStruct.PORT_Pin = DB2_PIN + DB3_PIN + WR1_PIN;
	PORT_Init(MDR_PORTF, &PORT_InitStruct);
	
	PORT_InitStruct.PORT_Pin = WR2_PIN + A0_PIN;
	PORT_Init(MDR_PORTA, &PORT_InitStruct);
	
	//Cбрасываем все пины нахуй
	PORT_WriteBit(DB0_PORT, DB0_PIN, Bit_RESET);
	PORT_WriteBit(DB1_PORT, DB1_PIN, Bit_RESET);
	PORT_WriteBit(DB2_PORT, DB2_PIN, Bit_RESET);
	PORT_WriteBit(DB3_PORT, DB3_PIN, Bit_RESET);
	PORT_WriteBit(WR1_PORT, WR1_PIN, Bit_RESET);
	PORT_WriteBit(WR1_PORT, WR2_PIN, Bit_RESET);
	PORT_WriteBit(A0_PORT, A0_PIN, Bit_SET);
	
	//Жде  на всякий случай
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	
	//Выбираем адресс 0x0F для разблокировки шины
	PORT_WriteBit(A0_PORT, A0_PIN, Bit_RESET);
	SetPins(0x0F);
	PORT_WriteBit(WR1_PORT, WR1_PIN, Bit_SET);
	__ASM("NOP");
	__ASM("NOP");
	PORT_WriteBit(WR1_PORT, WR1_PIN, Bit_RESET);
	__ASM("NOP");
	PORT_WriteBit(A0_PORT, A0_PIN, Bit_SET);
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	
	//Разблокируем шину
	PORT_WriteBit(DB0_PORT, DB0_PIN, Bit_SET);
	PORT_WriteBit(WR1_PORT, WR1_PIN, Bit_SET);
	__ASM("NOP");
	__ASM("NOP");
	PORT_WriteBit(WR1_PORT, WR1_PIN, Bit_RESET);
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	
	//Выключаем всё
	uint8_t i = 0;
	for(i = 0x00; i <= 0x09; i++)
	{
		output(i,CLR_L, CLR_H);
	}
	
}




int main(void)
{
	Init_DISP();
	while(1){		
			
	}
}
