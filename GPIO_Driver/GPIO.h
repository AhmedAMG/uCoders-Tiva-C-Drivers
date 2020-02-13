/*
 * GPIO.h
 *
 *  Created on: Feb 12, 2020
 *      Author: DELL
 */

#ifndef GPIO_H_
#define GPIO_H_

#define GPIO_PORTA 0
#define GPIO_PORTB 1
#define GPIO_PORTC 2
#define GPIO_PORTD 3
#define GPIO_PORTE 4
#define GPIO_PORTF 5

#define GPIO_PIN0 0
#define GPIO_PIN1 1
#define GPIO_PIN2 2
#define GPIO_PIN3 3
#define GPIO_PIN4 4
#define GPIO_PIN5 5
#define GPIO_PIN6 6
#define GPIO_PIN7 7

#define GPIO_INPUT 0
#define GPIO_OUTPUT 1

#define GPIO_LOW 0
#define GPIO_HIGH 1

#define GPIO_APB 0
#define GPIO_AHB 1


void GPIO_Init(uint8_t Port_Number,uint8_t Bus);

void GPIO_SetPortValue(uint8_t Port_Number, uint8_t Value);
void GPIO_SetPinValue(uint8_t Port_Number, uint8_t Pin_Number, uint8_t Value);

void GPIO_SetPortDirection(uint8_t Port_Number, uint8_t Direction);
void GPIO_SetPinDirection(uint8_t Port_Number, uint8_t Pin_Number, uint8_t Direction);

//void GPIO_SetPinPullUp(uint8_t Port_Number, uint8_t Pin_Number);
//void GPIO_SetPinPullDown(uint8_t Port_Number, uint8_t Pin_Number);

//void GPIO_SetPinAF(uint8_t Port_Number, uint8_t Pin_Number, uint8_t AlternateFunction);

uint8_t GPIO_GetPortValue(uint8_t Port_Number);
uint8_t GPIO_GetPinValue(uint8_t Port_Number, uint8_t Pin_Number);




#endif /* GPIO_H_ */
