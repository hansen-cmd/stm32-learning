#include "stm32f10x.h"                  // Device header
int16_t Count;

void xuansensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);//初始化GPIOB外设
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);//AFIO
	
	EXTI_InitTypeDef EXTI_InitStructure;//AFIO转EXIT
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//NVIC分组
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //响应优先级
	NVIC_Init(&NVIC_InitStructure);
}
int16_t xuansensor_Get(void)
{
	int16_t Temp;
	Temp = Count;
	Count = 0;
	return Temp;
}

void EXTI0_IRQHandler(void)//A相中断（PB0）
{
    if (EXTI_GetITStatus(EXTI_Line0) == SET) 
		{    
			if (GPIO_ReadInputDataBit(GPIOB ,GPIO_Pin_0) ==0)//读b相（PB0）的电平来判断方向
			{
					Count --;
			}
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}
void EXTI1_IRQHandler(void)
{
		if (EXTI_GetITStatus(EXTI_Line1) == SET) 
		{    
			if (GPIO_ReadInputDataBit(GPIOB ,GPIO_Pin_1) ==0)//读a相（PB1）的电平来判断方向
			{
				Count ++;
			}
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}
