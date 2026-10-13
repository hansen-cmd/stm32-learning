#include "stm32f10x.h"                  // Device header

/**
  * 函    数：定时器初始化（TIM2，1 秒定时中断）
  * 说    明：用于周期性采样编码器计数值，从而计算出“速度”。
  *           时钟 72MHz，预分频 7200 -> 计数频率 10kHz，
  *           自动重装 10000 -> 每 1 秒产生一次更新中断。
  */
void Timer_Init(void)
{
    /* 1. 开启时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    /* 2. 选择内部时钟（TIM2 仅作计时，不接外部引脚） */
    TIM_InternalClockConfig(TIM2);
    
    /* 3. 时基单元：72MHz / 7200 = 10kHz，计 10000 次 = 1 秒 */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    
    /* 4. 清除刚配置时产生的更新标志，避免一上电就进中断 */
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    
    /* 5. 配置 NVIC 中断 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
    
    /* 6. 启动定时器 */
    TIM_Cmd(TIM2, ENABLE);
}
