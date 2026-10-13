#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void duoji_Init(void)
{
	PWM_Init();
}

//0   500
//180  2500

void duoji_SetAngle(float Angle)
{
	PWM_SetCompare2(Angle/180*2000+500);
}