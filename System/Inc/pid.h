#ifndef PID_H
#define PID_H

// PID controller structure
typedef struct {
    float Kp;  // Proportional gain
    float Ki;  // Integral gain
    float Kd;  // Derivative gain
    float Target;  
    float Actual;
    float Out;
    float Error0;
    float Error1;
    float Errorint;
    float OutMax;  
    float OutMin;
    float Difout;
} PIDController;


// Function prototypes
void PID_Update(PIDController *pid);

#endif // PID_H