#include "Std_types.h"
#include "Registers.h"
#include "BitOperations.h"
#include "GPIO.h"
#include "GPTM.h"
#include "NVIC.h"

#define LED_RED PIN1
#define LED_BLUE PIN2
#define LED_GREEN PIN3
#define SW_2 PIN0
#define SW_1 PIN4

void TIMER0_Handler(void);
void TIMER2_Handler(void);
uint8_t state = 0;

/**
 * main.c
 */
int main(void)
{
    GPIOBusSet(PORTF, APB);
    GPIOClockSet(PORTF);
    GPIO_PadSet(PORTF, LED_GREEN|LED_BLUE|LED_RED, DRIVE_8m, PAD_NPU_NPD);
    GPIODirectionModeSet(PORTF,LED_GREEN|LED_BLUE|LED_RED, MODE_OUT);

    GPIO_PadSet(PORTF, SW_1, DRIVE_8m, PAD_PU);
    GPIODirectionModeSet(PORTF,SW_1, MODE_AF_INPUT);
    GPIO_ConfigPCTLMux(PORTF, SW_1, 7);

    //CCP pin for timer0 for pwm
    //GPIO_PadSet(PORTF, LED_RED, DRIVE_8m, PAD_NPU_NPD);
    //GPIODirectionModeSet(PORTF,LED_RED, MODE_AF_OUTPUT);
    //GPIO_ConfigPCTLMux(PORTF, LED_RED, 7);

    //NOTE: Prescaler Value Register and Prescaler Snapshot registers are not updating AT ALL in the debugging session !!!
    //NOTE: IN COUNTING DOWN, PRESCALER IS A TRUE PRESCALER ONLY IN ONE-SHOT AND PERIODIC MODES
    //OTHER THAN THOSE, IT IS ALWAYS A TIME EXTENSION

    //FANCYTIMER_ONESHOT_SetTimeoutSeconds(TIM2_AB,1000,TIMER_Handler);
    //FANCYTIMER_ONESHOT_SetMatchSeconds(TIM2_AB,500,TIMER_Handler);
    //FANCYTIMER_PERIODIC_SetTimeoutSeconds(TIM2_AB,1000,TIMER_Handler);
    //FANCYTIMER_PERIODIC_SetMatchSeconds(TIM2_AB,500,TIMER_Handler);
    //FANCYTIMER_ECOUNT_SetEventsNumber(TIM2_A, 5, EDGE_FALLING, EDGECOUNT_CONTINUOUS, TIMER_Handler);
    FANCYTIMER_ETIME_Configure(TIM2_A, EDGE_FALLING, TIMER2_Handler);
    FANCYTIMER_PWM_SetDutyCycle(TIM0_B, 1, 50, CCP_INVERTED, TIMER0_Handler);
    NVIC_SetPriority(23, 3);
    NVIC_SetPriority(20, 5);
    TIMER_Start(TIM2_A);
    TIMER_Start(TIM0_B);

    while(1){

    }

    return 0;
}

void TIMER2_Handler(void){
    state++;
    if(state == 3) state = 0;
    GPIOWrite(PORTF, LED_GREEN, LOW);
    GPIOWrite(PORTF, LED_BLUE, LOW);
    GPIOWrite(PORTF, LED_RED, LOW);
}

void TIMER0_Handler(void){
    switch(state){
    case 0:
        GPIOWrite(PORTF, LED_GREEN, LOW);
        GPIOWrite(PORTF, LED_BLUE, LOW);
        if((GPIORead(PORTF, LED_RED)&LED_RED) == 0) GPIOWrite(PORTF, LED_RED, LED_RED);
        else GPIOWrite(PORTF, LED_RED, LOW);
        break;
    case 1:
        GPIOWrite(PORTF, LED_GREEN, LOW);
        GPIOWrite(PORTF, LED_RED, LOW);
        if((GPIORead(PORTF, LED_BLUE)&LED_BLUE) == 0) GPIOWrite(PORTF, LED_BLUE, LED_BLUE);
        else GPIOWrite(PORTF, LED_BLUE, LOW);
        break;
    case 2:
        GPIOWrite(PORTF, LED_RED, LOW);
        GPIOWrite(PORTF, LED_BLUE, LOW);
        if((GPIORead(PORTF, LED_GREEN)&LED_GREEN) == 0) GPIOWrite(PORTF, LED_GREEN, LED_GREEN);
        else GPIOWrite(PORTF, LED_GREEN, LOW);
        break;
    }
}

