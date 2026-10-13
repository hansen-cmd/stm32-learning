#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "xuansensor.h"


int16_t Num;


int main(void)
{
	OLED_Init();
	xuansensor_Init();
	
	OLED_ShowString(1,1,"num:");
	while (1)
	{
		Num += xuansensor_Get();
		OLED_ShowSignedNum(1,5,Num,5);
	}
}