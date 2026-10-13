# 6-1 PWM 驱动 LED 呼吸灯
> 占空比从 0 平滑升到 100 再降回 0，LED 呈现渐亮渐灭的「呼吸」效果。PWM 是用数字信号模拟模拟量的核心手段。

---

## 学什么

- **PWM 两个参数**：频率（快慢）和占空比（高电平占比）
- 频率公式：`Freq = 72MHz / (PSC+1) / (ARR+1)`
- 占空比公式：`Duty = CCR / (ARR+1)`
- `TIM_OC1Init()` 输出比较通道配置 + `PWM_SetCompare1()` 运行时改占空比
- 人眼视觉暂留：频率够高时看不到闪烁，只感受到亮度变化

## 硬件连接

LED → PA15（TIM2_CH1，注意 PA15 默认是 JTAG 引脚，需要重映射）

## 关键代码

```c
for (i = 1; i <= 100; i++)        // 渐亮
{
    PWM_SetCompare1(i);
    Delay_ms(10);
}
for (i = 1; i <= 100; i++)        // 渐灭
{
    PWM_SetCompare1(100 - i);
    Delay_ms(10);
}
```

一个完整呼吸周期约 2 秒。想更快就减小 `Delay_ms`，想更细腻就增大循环上限。

## 注意与踩坑

- **PA15 是个特殊引脚**，上电默认是 JTAG 的 JTDI。要用它做普通 PWM 输出，必须先关闭 JTAG：`GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE)`（代码里已处理）
- 如果 LED 完全不亮，先确认是低电平点亮还是高电平点亮，以及是否用了 PA15 的重映射

---

[← 返回总目录](../../README.md)
