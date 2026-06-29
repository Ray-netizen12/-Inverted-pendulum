#include "stm32f1xx_hal.h"
#include "pid.h"
#include "math.h"

void PID_Update(PIDController *pid) 
{
    int16_t ErrIntThreshold = 70; //误差阈值，超过该值不进行积分运算

	pid->Error1 = pid->Error0;
    pid->Error0 = pid->Target - pid->Actual;
	float K = 0.5; // 假设K的值为0.5，可以根据需要进行调整
	pid->Difout = (1 - K) * pid->Kd * (pid->Error0 - pid->Error1) + K * pid->Difout;
	
    if (pid->Ki != 0)
	{
		if (ErrIntThreshold == 0)
		{
			pid->Errorint += pid->Error0;
		}
		else if (fabs(pid->Error0) < ErrIntThreshold)
		{
			pid->Errorint += pid->Error0;
		}
		else
		{
			pid->Errorint = 0;
		}
	}
	else
	{
		pid->Errorint = 0;
	}

    pid->Out = pid->Kp * pid->Error0 + pid->Ki * pid->Errorint + pid->Difout;

    if (pid->Out > pid->OutMax) { pid->Out = pid->OutMax; }
    if (pid->Out < pid->OutMin) { pid->Out = pid->OutMin; }

}