#include "stm32f10x.h"

/**
 * 输入捕获测频率 / 测占空比模块
 * 定时器：TIM3_CH2  ->  PA7  (输入捕获引脚，默认复用，无需重映射)
 *
 * 硬件连接：一根跳线把 PA1(PWM 输出) 连到 PA7(输入捕获输入)
 *           TIM3 对 PA7 上的 PWM 波形进行测量。
 *
 * 定时器时钟 = 72MHz，PSC = 72-1 -> 1MHz 计数 (1us / 计数)
 *
 * 测量原理（主从复位模式）：
 *   - 信号在 PA7 = TIM3_CH2 上
 *   - CH2 配置为 DirectTI(上升沿)：每个上升沿把"上一周期"计数值锁存到 CCR2 -> 周期
 *   - CH1 配置为 IndirectTI(交叉到 TI2, 下降沿)：锁存高电平宽度到 CCR1 -> 占空比
 *   - 触发源选 TI2FP2，从模式 Reset：每个上升沿计数器清零
 *   - 频率 = 1,000,000 / CCR2 (Hz)
 *   - 占空比 = CCR1 / CCR2 * 100 (%)
 *
 * 由于 PWM 与输入捕获使用同一系统时钟，两者同源，
 * 测量结果自洽，不依赖外部晶振的绝对精度。
 */

void IC_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* PA7 上拉输入，作为 TIM3_CH2 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_InternalClockConfig(TIM3);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period            = 65536 - 1;  // 最大量程
    TIM_TimeBaseInitStructure.TIM_Prescaler         = 72 - 1;     // 72MHz / 72 = 1MHz
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 0x00;   // 关闭数字滤波，排除高频被滤掉的可能

    /* CH2：上升沿，DirectTI(TI2)，用于测量周期 -> 锁存到 CCR2 */
    TIM_ICInitStructure.TIM_Channel     = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    /* CH1：下降沿，IndirectTI(交叉到 TI2)，用于测量高电平宽度 -> 锁存到 CCR1 */
    TIM_ICInitStructure.TIM_Channel     = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Falling;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    /* 主从触发：信号在 CH2 上，所以触发源选 TI2FP2，复位模式 */
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI2FP2);
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);

    TIM_Cmd(TIM3, ENABLE);
}

/**
 * 读取测得的频率 (Hz)。
 * 返回 0 表示尚未捕获到有效边沿。
 */
uint32_t IC_GetFreq(void)
{
    uint16_t ccr = TIM_GetCapture2(TIM3);   // 周期在 CCR2
    if (ccr == 0)
        return 0;
    return 1000000 / ccr;   // 1MHz 计数时钟
}

/**
 * 读取测得的占空比 (%)。
 * 返回 0 表示尚未捕获到有效边沿。
 */
uint8_t IC_GetDuty(void)
{
    uint16_t period = TIM_GetCapture2(TIM3);   // 周期
    uint16_t high   = TIM_GetCapture1(TIM3);   // 高电平宽度
    if (period == 0)
        return 0;
    return (uint8_t)((high * 100) / period);
}
