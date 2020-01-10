// Includes required for stm32f3
#include <stm32f30x.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>
#include <stm32f30x_tim.h>

//
void RCC_Configuration(void) {
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
}

void GPIO_Configuration(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_2);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_2);
}

void TIM4_Configuration(float d) {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  int Prescaler, Period;
  Prescaler = 1;
  Period = 1440;

  TIM_TimeBaseStructure.TIM_Period = Period / 2;
  TIM_TimeBaseStructure.TIM_Prescaler = Prescaler - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Asymmetric_PWM1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);
  int ontime = floor(d*Period);
  //int offtime = Period-ontime;
  int diff = ontime-Period/2;
  int reference;
  if(d<.5){
  reference=(Period/2)*1;
  }
  else{
  reference=(Period/2)*0.0; 
  } 

  int upPulse1 = reference;
  int downPulse1 = Period/2-(ontime-(Period/2-reference));
  TIM_SetCompare1(TIM4, upPulse1);
  TIM_SetCompare2(TIM4, downPulse1);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Asymmetric_PWM1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);
  int upPulse2 = reference+diff;
  int downPulse2 = downPulse1+diff;
  TIM_SetCompare3(TIM4, upPulse2);
  TIM_SetCompare4(TIM4, downPulse2);
  /* TIM4 enable counter */
  TIM_Cmd(TIM4, ENABLE);
}

int main(void) {
  RCC_Configuration();
  GPIO_Configuration();
  float dc=0.005;
  TIM4_Configuration(dc);
  while (1)
    ; /* Infinite loop */
}
