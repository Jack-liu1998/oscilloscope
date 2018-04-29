#include "PWM.h"
#include "stm32f10x_tim.h"

void GPIO_PWM_config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=PWM_out_pin;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(PWM_GPIO,&GPIO_InitStructure);
}
void PWM_config()
{
	TIM_TimeBaseInitTypeDef TIM_struct;
	TIM_OCInitTypeDef TIMOC_config;
	
	GPIO_PWM_config();
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_struct.TIM_Prescaler=35999;
	TIM_struct.TIM_ClockDivision=0;
	TIM_struct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_struct.TIM_Period=40;
	
	TIM_TimeBaseInit(TIM4, &TIM_struct);
	
	TIMOC_config.TIM_OCMode =TIM_OCMode_PWM1 ;          
     TIMOC_config.TIM_OutputState = TIM_OutputState_Enable;                 

     TIMOC_config.TIM_Pulse =3;          
	//0.5ms--------------0?;
   //1.0ms------------45?;
   //1.5ms------------90?;
  // 2.0ms-----------135?; 
   //2.5ms-----------180?;

     TIMOC_config.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC1Init(TIM4, &TIMOC_config);  
		TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
		
		TIM_ARRPreloadConfig(TIM2, ENABLE);
	
    TIM_Cmd(TIM2,ENABLE);	
}
