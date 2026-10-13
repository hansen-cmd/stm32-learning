#include "stm32f10x.h"                  // Device header

/**
  * 函    数：编码器初始化（TIM3 编码器接口模式）
  * 说    明：A 相 -> PA6 (TIM3_CH1)，B 相 -> PA7 (TIM3_CH2)
  *           采用 TI1+TI2 双沿计数（4 倍频），硬件自动识别方向：
  *           正转 -> 计数器递增；反转 -> 计数器递减。
  *           相比上次的“外部中断计次”，这种方式不占用 CPU、不丢脉冲、
  *           且天然支持正反转，是测速的标准做法。
  */
void Encoder_Init(void)
{
    /* 1. 开启时钟：TIM3 在 APB1，引脚在 GPIOA（APB2） */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    /* 2. 配置编码器引脚为上拉输入（旋转编码器模块通常开漏/推挽输出，加上拉更稳） */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* 3. 定时器时基单元：16 位计数器，不分频，满量程 0~65535 */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    
    /* 4. 配置编码器接口：TI1 和 TI2 都参与计数，上升沿有效 */
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12,
                               TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    
    /* 5. 启动定时器（编码器模式下计数器随 A/B 相自动增减） */
    TIM_Cmd(TIM3, ENABLE);
}

/**
  * 函    数：获取当前编码器计数值
  * 返 回 值：有符号 16 位计数值（相对上电初值，正转增大、反转减小）
  */
int16_t Encoder_Get(void)
{
    return TIM_GetCounter(TIM3);
}
