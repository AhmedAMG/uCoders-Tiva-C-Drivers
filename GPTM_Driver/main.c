#include "Std_types.h"
#include "Registers.h"
#include "BitOperations.h"
#include "GPIO.h"
#include "GPTM.h"
#include "NVIC.h"

#define LED_RED PIN1
#define LED_BLUE PIN2
#define LED_GREEN PIN3

/**
 * main.c
 */
int main(void)
{
    GPIOBusSet(PORTF, APB);
    GPIOClockSet(PORTF);
    GPIO_PadSet(PORTF, LED_RED, DRIVE_8m, PAD_NPU_NPD);
    GPIODirectionModeSet(PORTF,LED_RED, MODE_OUT);

    //Provide the clock to the timer0
    SYSCTL_RCGCTIMER_R = (1<<0);

    //Ensure that the timer is not enabled
    TIMER0_CTL_R &= ~((1<<0));

    //Enable timer interrupt from the NVIC
    NVIC_Enable(19);

    //Write the GPTMCFG with 0
    TIMER0_CFG_R = 0x00000000;

    //16bit mode
    TIMER0_CFG_R = 0x04;

    //Periodic mode = 0x02, Count down
    TIMER0_TAMR_R &= ~(0x03);
    TIMER0_TAMR_R |= (0x02);

    //Count up
    //TIMER0_TAMR_R |= (1<<4); //Mystery: Why the true prescaler does NOT have the same meaning of the time extension !!!

    //Load Initial value
    TIMER0_TAILR_R = 0x00F42400; //In individual/split mode: it will ignore the F4 part

    //Load the prescaler
    TIMER0_TAPR_R = 0x6C8;

    //Interrupt Masks (Overflow interrupt)
    TIMER0_IMR_R |= (1<<0);

    //Freeze while debugging
    TIMER0_CTL_R |= (1<<1);

    //Enable the timer
    TIMER0_CTL_R |= (1<<0);

    while(1){

    }

	return 0;
}

void TIMER0_Handler(void){
    if((GPIORead(PORTF, LED_RED)&LED_RED) == 0) GPIOWrite(PORTF, LED_RED, LED_RED);
    else GPIOWrite(PORTF, LED_RED, LOW);

    TIMER0_ICR_R |= (1<<0);
}

