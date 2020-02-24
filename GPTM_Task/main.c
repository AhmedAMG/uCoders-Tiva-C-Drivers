#include "Std_types.h"
#include "Registers.h"
#include "BitOperations.h"
#include "GPIO.h"

#define NVIC_EN0_R              (*((volatile uint32_t *)0xE000E100))

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
    GPIO_PadSet(PORTF, LED_GREEN, DRIVE_8m, PAD_NPU_NPD);
    GPIODirectionModeSet(PORTF,LED_GREEN, MODE_OUT);

    SYSCTL_RCGCTIMER_R = (1<<0);

    //Ensure that the timer is not enabled
    TIMER0_CTL_R &= ~((1<<0));

    //Enable timer interrupt from the NVIC
    NVIC_EN0_R |= (1<<19);

    //Write the GPTMCFG with 0
    TIMER0_CFG_R = 0x00000000;

    //Periodic mode = 0x02, Enable Match interrupt, Count up
    TIMER0_TAMR_R &= ~(0x03);
    TIMER0_TAMR_R |= (0x02);

    //Load Initial value
    TIMER0_TAILR_R = 0x00F42400;

    //Interrupt Masks (Overflow interrupt)
    TIMER0_IMR_R |= (1<<0);



    //Enable the timer
    TIMER0_CTL_R |= (1<<0);

    while(1){

    }

	return 0;
}

void TIMER0_Handler(void){
    if((GPIORead(PORTF, LED_GREEN)&LED_GREEN) == 0) GPIOWrite(PORTF, LED_GREEN, LED_GREEN);
    else GPIOWrite(PORTF, LED_GREEN, LOW);

    TIMER0_ICR_R |= (1<<0);
}
