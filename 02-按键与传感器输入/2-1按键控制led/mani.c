#include "stm32f10x.h"  
#include "led.h"
#include "delay.h"
#include "KEY.h"

uint8_t KeyNum;
int main(void)
{
	LED_Init();   //初始化
	Key_Init();
	while(1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum ==1)
		{
			LED1_Turn();
		}
		if (KeyNum == 2)
		{
			LED2_Turn();
		}
			
//		LED1_ON();
//		LED2_OFF();
//		Delay_ms(500);
//		LED1_OFF();
//		LED2_ON();
//		Delay_ms(500);   闪烁
		
		
	}
}