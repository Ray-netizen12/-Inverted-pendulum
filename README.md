# -Inverted-pendulum
基于STM32HAL库实现的旋转倒立摆
本项目根据B站江协科技的视频教程开发实现，运用了江协科技的部分标准库驱动代码，并将其在HAL库中实现。
本项目分成三大块，分别为Core（main，STM32初始化配置）、Hardware（硬件驱动）、System（PID算法，定时中断），最终完整实现一个可以自动起摆的倒立摆系统。
