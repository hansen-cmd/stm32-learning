#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "encoder.h"

uint8_t i;
int main(void)
{
	OLED_Init();
  Encoder_Init();
	OLED_ShowString(1,1,"NUM:");
	while (1)
	{
		OLED_ShowNum(1,6,encoder_Get(),5);
	}
}