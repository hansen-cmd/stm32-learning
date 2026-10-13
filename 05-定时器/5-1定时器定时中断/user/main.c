#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"


uint16_t Num;


int main(void)
{
	OLED_Init();
	Timer_Init();
	
	OLED_ShowString(1,1,"num:");
	while (1)
	{
		Num++ ;
		OLED_ShowNum(1,5,Num,5);
	}
}