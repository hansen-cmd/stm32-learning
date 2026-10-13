#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "ic.h"

int main(void)
{
	OLED_Init();
  PWM_Init();
	IC_Init();
	OLED_ShowString(1,1,"Freq:00000HZ");
	PWM_SetCompare2(50);        //duty = CCR/100
	PWM_SetPrescaler(720 -1);    //Freq = 72M/(PSC + 1) / (ARR +1)
	while (1)
	{
		OLED_ShowNum(1,6,IC_Get(),5);
	}
}
	