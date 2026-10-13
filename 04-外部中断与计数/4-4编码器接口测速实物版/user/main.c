#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Encoder.h"

/**
  * ================= 编码器测速 · 硬件/软件对照诊断 =================
  * 目的：定位"旋转时 CNT 只 0/1 跳、SPD 最大到 5"的根因。
  *
  * 同屏显示：
  *   第1行 HW <n> : TIM3 硬件编码器接口计数（实时）
  *   第2行 SW <n> : 软件读 PA6/PA7 自做正交解码计数（抗抖动查表）
  *   第3行 AB xy  : PA6/PA7 原始电平（x=PA6, y=PA7）
  *   第4行 dH<d> dS<d> : 每 200ms 内 HW 与 SW 的变化量
  *
  * 判读：
  *   - SW 大幅变化、HW 只 0/1 跳 -> TIM3 硬件编码器未正常计数，信号 OK，改用软件解码
  *   - HW 和 SW 都大幅变化        -> 信号与硬件都正常（之前是采样/显示误解）
  *   - 两者都不动                 -> 信号/接线/供电问题
  */

int16_t Speed;   /* 供 stm32f10x_it.c 引用 */

/* 正交解码查表（抗抖动）：索引 = (上一状态<<2)|当前状态，
   非法跳变（抖动）对应值为 0，自动忽略。 */
static const int8_t QTABLE[16] = {
     0,  1, -1,  0,
    -1,  0,  0,  1,
     1,  0,  0, -1,
     0, -1,  1,  0
};

int main(void)
{
    int32_t sw_count = 0;
    uint8_t prev = 0;
    uint8_t tick = 0;
    int16_t last_hw = 0, last_sw = 0;

    OLED_Init();
    Encoder_Init();     // PA6/PA7 输入 + TIM3 编码器接口模式

    OLED_ShowString(1, 1, "HW ");
    OLED_ShowString(2, 1, "SW ");
    OLED_ShowString(3, 1, "AB ");
    OLED_ShowString(4, 1, "dH    dS ");

    while (1)
    {
        int16_t hw = Encoder_Get();                       // TIM3 硬件计数
        uint8_t a = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
        uint8_t b = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);
        uint8_t cur = (a << 1) | b;
        sw_count += QTABLE[(prev << 2) | cur];            // 软件正交解码
        prev = cur;

        OLED_ShowSignedNum(1, 4, hw, 6);                  // 第1行：硬件计数
        OLED_ShowSignedNum(2, 4, (int16_t)sw_count, 6);   // 第2行：软件解码
        OLED_ShowChar(3, 4, a ? '1' : '0');
        OLED_ShowChar(3, 6, b ? '1' : '0');

        if (++tick >= 40)                                // 约 200ms
        {
            tick = 0;
            OLED_ShowSignedNum(4, 1, hw - last_hw, 5);            // dH
            OLED_ShowSignedNum(4, 7, (int16_t)(sw_count - last_sw), 5); // dS
            last_hw = hw;
            last_sw = sw_count;
        }

        Delay_ms(5);
    }
}
