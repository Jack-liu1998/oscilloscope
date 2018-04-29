#include "dac.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"

#define DAC_DHR12R1    (u32)&(DAC->DHR12R1)   //DAC DATA buff

extern DAC_WAVE_Typedef DAC_SETTING;

void DAC_HZ_Add(void)
{
	if(DAC_SETTING.HZ<281250)
	DAC_SETTING.HZ++;
	else{//可以在屏幕提醒
	
	}
	Wave_Init(&DAC_SETTING);
}

void DAC_HZ_Reduce(void)
{
	if(DAC_SETTING.HZ>5)
	DAC_SETTING.HZ--;
	else{//可以在屏幕提醒
	
	}
	Wave_Init(&DAC_SETTING);
}

void Wave_GPIO_Config(void)//DAC!-------PA5
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;      
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 ; 
    GPIO_SetBits(GPIOA,GPIO_Pin_4)  ;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);      
}

void Wave_DAC_Config( void)
{
    DAC_InitTypeDef            DAC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
     
    DAC_StructInit(&DAC_InitStructure);    
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;//不使用波形发生器
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable; 
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;//TIM2 Trigger
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    DAC_Cmd(DAC_Channel_1, ENABLE);   
    DAC_DMACmd(DAC_Channel_1, ENABLE); 
}

void Wave_TIM_Config(u32 Wave1_Fre)//TIM2 Init
{
    TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;   
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0; 
    TIM_TimeBaseStructure.TIM_Period = Wave1_Fre;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
}

void Wave_DMA_Config(uint16_t* wave)//DMA2
{                  
    DMA_InitTypeDef            DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
     
    DMA_StructInit( &DMA_InitStructure);      
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//peripherals  to memory
    DMA_InitStructure.DMA_BufferSize =256;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1;
     DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)wave;

     DMA_Init(DMA2_Channel3, &DMA_InitStructure);
     DMA_Cmd(DMA2_Channel3, ENABLE);

}

void Wave_Init(DAC_WAVE_Typedef* wave)
{
	uint32_t Tim_Period=72000000/((wave->HZ)*256);
	
  Wave_GPIO_Config();             
  Wave_TIM_Config(Tim_Period);            //72000000/3000=24000 points per second
  Wave_DAC_Config();              
  Wave_DMA_Config(wave->WAVE_P);              
  TIM_Cmd(TIM2, ENABLE);     
}

void Dac1_Set_Vol(u16 vol)
{
	float temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12位右对齐数据格式设置DAC值
}




















































