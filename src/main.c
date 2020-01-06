// Includes required for stm32f3
#include <stm32f30x.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>
#include <stm32f30x_tim.h>

// Configure Clock
void RCC_Configuration(void) {
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
}

// GPIO Configurations
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

// Timer Configuration
void TIM4_Configuration(void) {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  int Prescaler, Period;

	// Prescaler and period configuration
	// to set the frequency to 50 khz
	// and period to 20 micro-seconds
  Prescaler = 72;
  Period = 20;

	// Timer Base configurations
  TIM_TimeBaseStructure.TIM_Period = Period / 2;
  TIM_TimeBaseStructure.TIM_Prescaler = Prescaler - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;

	// Timer counter mode
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);


	// Set timer output configuration in asymmetric pwm 1
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Asymmetric_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);

	// d = duty cycle
	float d = 0.65;

	// Counter value when to pull the voltage to low
  int upPulse = floor(Period/2 - ((Period*d-1)/2));
	// counter value when to pull the voltage to high
  int downPulse = upPulse - 1;
  TIM_SetCompare1(TIM4, upPulse);
  TIM_SetCompare2(TIM4, downPulse);

	// Set timer output configuration in asymmetric pwm 1
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Asymmetric_PWM1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);
  TIM_SetCompare3(TIM4, Period / 2 - upPulse + 1);
  TIM_SetCompare4(TIM4, Period / 2 - upPulse);
  /* TIM4 enable counter */
  TIM_Cmd(TIM4, ENABLE);
}

int main(void) {
	// Call Configuration functions
  RCC_Configuration();
  GPIO_Configuration();
  TIM4_Configuration();
  while (1)
    ; /* Infinite loop */
}