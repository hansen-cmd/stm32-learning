#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "dianji.h"


int main(void)
{
	OLED_Init();
	dianji_Init();
	
	dianji_SetSpeed(30);
	
	while (1)
	{
		
	}
} 
