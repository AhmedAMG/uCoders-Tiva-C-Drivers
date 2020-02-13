//Date: 14/2/2020
//uCoders Stage: 3
//Instructor: Abdullrahman Ashraf
//Implemented by: Ahmed Gamal
//uCoders Task: GPIO

#include "Std_types.h"
#include "BitOperations.h"
#include "Registers.h"
#include "GPIO.h"

/**
 * main.c
 */
int main(void)
{

    GPIO_Init(GPIO_PORTF, GPIO_AHB);

    GPIO_SetPinDirection(GPIO_PORTF, GPIO_PIN1, GPIO_OUTPUT);
    GPIO_SetPinDirection(GPIO_PORTF, GPIO_PIN2, GPIO_OUTPUT);
    GPIO_SetPinDirection(GPIO_PORTF, GPIO_PIN3, GPIO_OUTPUT);

    volatile int state = 1;
    volatile int i = 0;

    while(1){
        switch(state){
        case 1:
            GPIO_SetPinValue(GPIO_PORTF, GPIO_PIN2, GPIO_LOW);
            GPIO_SetPinValue(GPIO_PORTF, GPIO_PIN3, GPIO_LOW);
            GPIO_SetPinValue(GPIO_PORTF, GPIO_PIN1, GPIO_HIGH);
            state = 2;
            break;
        case 2:
            GPIO_SetPinValue(GPIO_PORTF, GPIO_PIN1, GPIO_LOW);
            GPIO_SetPinValue(GPIO_PORTF, GPIO_PIN3, GPIO_LOW);
            GPIO_SetPinValue(GPIO_PORTF, GPIO_PIN2, GPIO_HIGH);
            state = 3;
            break;
        case 3:
            GPIO_SetPinValue(GPIO_PORTF, GPIO_PIN1, GPIO_LOW);
            GPIO_SetPinValue(GPIO_PORTF, GPIO_PIN2, GPIO_LOW);
            GPIO_SetPinValue(GPIO_PORTF, GPIO_PIN3, GPIO_HIGH);
            state = 1;
            break;
        }

        for(i = 0; i < 1000000; i++);
    }
	return 0;
}

