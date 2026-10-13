#include "stm32f10x.h"                  // Device header

void IC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//引脚重映射需AFIO
	//GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);//引脚重映射配置
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//解除PB15引脚调试端口JTAG
	//需要利用重映射把PB0改为PB15时，需要用到这些
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536-1;//自动重装器   ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;//预分频器    PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//重复计数器
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICFilter = 0x00;//滤波器，，增大参数可以避免干扰
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;//边沿检测极性选择
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;//分频器
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI ;
	
	TIM_ICInit(TIM3,&TIM_ICInitStruct);
	TIM_SelectInputTrigger(TIM3,TIM_TS_TI2FP2);//主模式
	TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Reset);//从模式
	
	TIM_Cmd(TIM3,ENABLE);
	
	
}
uint32_t IC_Get(void)
{
	return 1000000 / TIM_GetCapture2(TIM3);//返回的是最新周期的频率值
}
		