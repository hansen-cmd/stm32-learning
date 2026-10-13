#include "stm32f10x.h"

/**
 * PWM 输出模块
 * 定时器：TIM2_CH2  ->  PA1  (PWM 输出引脚，默认复用，无需重映射)
 *
 * 定时器时钟 = 72MHz (APB1 时钟 36MHz，定时器倍频 x2)
 * 预分频 PSC = 72-1  ->  计数时钟 = 1MHz (即 1us / 计数)
 * 通过修改 ARR 设置输出频率，CCR2 固定为 ARR/2 保持 50% 占空比
 *
 * 注意：本工程用一根跳线把 PA1(PWM) 连到 PA7(输入捕获)，
 *       由 TIM3 输入捕获来测量这里产生的频率。
 */

void PWM_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* PA1 复用推挽输出，作为 TIM2_CH2 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_InternalClockConfig(TIM2);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period            = 1000 - 1;   // 默认 1kHz
    TIM_TimeBaseInitStructure.TIM_Prescaler         = 72 - 1;     // 72MHz / 72 = 1MHz
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse       = 500;                    // 50% 占空比
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);                      // ← 用 CH2

    TIM_Cmd(TIM2, ENABLE);
}

/**
 * 设置 PWM 输出频率 (Hz)，同时保持 50% 占空比。
 * 在 1MHz 计数时钟下，频率范围约 15Hz ~ 1MHz。
 */
void PWM_SetFrequency(uint32_t freq)
{
    if (freq < 15)        freq = 15;
    if (freq > 1000000)   freq = 1000000;

    uint16_t arr = (1000000 / freq) - 1;   // 1MHz / freq = 周期计数值
    TIM_SetAutoreload(TIM2, arr);
    TIM_SetCompare2(TIM2, arr / 2);        // ← 占空比写 CH2
}

void PWM_SetCompare2(uint16_t Compare)
{
    TIM_SetCompare2(TIM2, Compare);
}
