#ifndef _PWM_H_
#define _PWM_H_
#include "sys.h"//IO操作有关封装

#define PWM_out_pin GPIO_Pin_1
#define PWM_GPIO GPIOA

//TIM_SetCompare2(TIM3,pwmval);调节占空比
//TIM_SetAutoreload(TIM3,arr);调节周期

void PWM_config(void);


#endif
