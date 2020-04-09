#include "Std_types.h"
#include "Registers.h"
#include "BitOperations.h"
#include "GPIO.h"

void PORTF_BIT4_Handler(void);
void PORTE_BIT3_Handler(void);

/**
 * main.c
 */
int main(void)
{
    GPIOBusSet(PORTF, AHB);
    GPIOClockSet(PORTF);
    GPIOBusSet(PORTE, AHB);
    GPIOClockSet(PORTE);

    GPIO_PadSet(PORTF, PIN3|PIN2, DRIVE_8m, PAD_NPU_NPD);
    GPIODirectionModeSet(PORTF,PIN3|PIN2, MODE_OUT);

    GPIO_PadSet(PORTF, PIN4, DRIVE_8m, PAD_PU);
    GPIO_PadSet(PORTE, PIN3, DRIVE_8m, PAD_PD);
    GPIODirectionModeSet(PORTF,PIN4, MODE_IN);
    GPIODirectionModeSet(PORTE,PIN3, MODE_IN);

    GPIO_SetIntHandler(PORTF,PIN4,PORTF_BIT4_Handler);
    GPIO_SetIntHandler(PORTE,PIN3,PORTE_BIT3_Handler);
    GPIO_ConfigInterrupt(PORTF, PIN4, EDGE, FALLING_EDGE);
    GPIO_ConfigInterrupt(PORTE, PIN3, LEVEL, HIGH_LEVEL);


    while(1){

    }

	return 0;
}

void PORTF_BIT4_Handler(void){
    if((GPIORead(PORTF, PIN2)&PIN2) == 0) GPIOWrite(PORTF, PIN3|PIN2, PIN3|PIN2);
    else GPIOWrite(PORTF, PIN3|PIN2, LOW);

}

void PORTE_BIT3_Handler(void){
    if((GPIORead(PORTF, PIN2)&PIN2) == 0) GPIOWrite(PORTF, PIN3|PIN2, PIN3|PIN2);
    else GPIOWrite(PORTF, PIN3|PIN2, LOW);

}

