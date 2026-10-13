#include "stm32f10x.h"  
#include "delay.h"

uint16_t CountSensor_Count;

void CountSensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);//初始化GPIOB外设
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);//AFIO
	
	EXTI_InitTypeDef EXTI_InitStructure;//AFIO转EXIT
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//NVIC分组
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
}
uint16_t CountSensor_Get(void) 
{
    return CountSensor_Count; // 这里返回你的变量
}
void EXTI15_10_IRQHandler(void)//中断函数   还需判断是否是exti14进来的	
{
	 if (EXTI_GetITStatus(EXTI_Line14)== SET)
	 { 
		 Delay_ms(20);
		 CountSensor_Count ++;
		 EXTI_ClearITPendingBit(EXTI_Line14);
	 }
		 
}


