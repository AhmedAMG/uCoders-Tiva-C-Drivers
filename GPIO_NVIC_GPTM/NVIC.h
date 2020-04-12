/*
 * NVIC.h
 *
 *  Created on: Feb 28, 2020
 *      Author: DELL
 */

#ifndef NVIC_H_
#define NVIC_H_

#define PORTA_INT_ID 0
#define PORTB_INT_ID 1
#define PORTC_INT_ID 2
#define PORTD_INT_ID 3
#define PORTE_INT_ID 4
#define PORTF_INT_ID 30

void NVIC_Enable(uint8_t ID);
void NVIC_Disable(uint8_t ID);

void NVIC_SetPending(uint8_t ID);
void NVIC_ClearPending(uint8_t ID);
uint8_t NVIC_GetPending(uint8_t ID);

uint8_t NVIC_GetActiveState(uint8_t ID);
void NVIC_SetPriority(uint8_t ID,uint8_t priority);
uint8_t NVIC_GetPriority(uint8_t ID);

#endif /* NVIC_H_ */
