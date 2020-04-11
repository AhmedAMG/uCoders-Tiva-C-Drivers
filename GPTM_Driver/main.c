#include "Std_types.h"
#include "Registers.h"
#include "BitOperations.h"
#include "GPIO.h"
#include "GPTM.h"
#include "NVIC.h"

#define LED_RED PIN1
#define LED_BLUE PIN2
#define LED_GREEN PIN3

void TIMER5_Handler(void);

/**
 * main.c
 */
int main(void)
{
    GPIOBusSet(PORTF, APB);
    GPIOClockSet(PORTF);
    GPIO_PadSet(PORTF, LED_RED, DRIVE_8m, PAD_NPU_NPD);
    GPIODirectionModeSet(PORTF,LED_RED, MODE_OUT);

    TIMER_Init(TIM5_B, TIMER_MODE_PERIODIC, COUNT_DOWN, STALL_ENABLE);

    //Load Initial value
    TIMER_SetIntervalLoad(TIM5_B, 0x00002400);

    //Load the prescaler
    TIMER_SetPrescalar(TIM5_B, 0xF4);

    //Load the match register
    //TIMER_SetMatch(TIM5_B, 0x0001200); //50% duty cycle if 1200

    TIMER_EnableAndSetIntISR(TIM5_B,INT_NON_CAPTURE_MATCH,TIMER5_Handler);
    TIMER_EnableAndSetIntISR(TIM5_B,INT_TIMEOUT,TIMER5_Handler);

    TIMER_EnableSnapshot(TIM5_B);

    TIMER_Start(TIM5_B);

    while(1){

    }

    return 0;
}

void TIMER5_Handler(void){
    if((GPIORead(PORTF, LED_RED)&LED_RED) == 0) GPIOWrite(PORTF, LED_RED, LED_RED);
    else GPIOWrite(PORTF, LED_RED, LOW);

}

