#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "IC.h"

int main(void)
{
    /* PC13 板载 LED：用于指示程序正在运行（证明固件已正确烧录并跑起来） */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    OLED_Init();
    PWM_Init();   // PA1 输出 PWM
    IC_Init();    // PA7 输入捕获

    OLED_ShowString(1, 1, "Freq Meter");
    OLED_ShowString(2, 1, "F:");
    OLED_ShowString(3, 1, "D:");
    OLED_ShowString(4, 1, "P7:");   // 第4行显示 PA7 是否收到了跳变信号

    /* 被测信号频率表，循环切换，便于直观看到测量值随输入变化 */
    uint32_t freq_table[] = {1000, 2000, 5000, 10000};
    uint8_t idx = 0;

    while (1)
    {
        PWM_SetFrequency(freq_table[idx]);  // 改变 PA1 输出的 PWM 频率
        Delay_ms(1500);                      // 等待信号稳定后再读取

        /* 诊断：在约几十毫秒内反复读 PA7 电平，看它有没有翻转。
           JUMP = PA7 上有方波（跳线连通、信号已到达 PA7）
           STAT = PA7 电平静止（PWM 没输出 或 跳线没连通 / A1排针不是PA1） */
        uint8_t p7_last = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);
        uint8_t p7_chg  = 0;
        for (volatile uint32_t i = 0; i < 300000; i++)
        {
            uint8_t p7_now = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);
            if (p7_now != p7_last) { p7_chg = 1; p7_last = p7_now; }
        }

        uint32_t freq = IC_GetFreq();         // 输入捕获测得的频率
        uint8_t  duty = IC_GetDuty();         // 输入捕获测得的占空比

        OLED_ShowNum(2, 3, freq, 6);
        OLED_ShowString(2, 9, "Hz");
        OLED_ShowNum(3, 3, duty, 3);
        OLED_ShowString(3, 6, "%");
        OLED_ShowString(4, 4, p7_chg ? "JUMP" : "STAT");

        /* LED 翻转，指示主循环在运行 */
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)(idx & 1));

        idx = (idx + 1) % 4;
    }
}
