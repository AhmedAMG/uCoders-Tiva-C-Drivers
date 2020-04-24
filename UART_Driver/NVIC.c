/*
 * NVIC.c
 *
 *  Created on: Feb 28, 2020
 *      Author: DELL
 */

#include "Std_types.h"
#include "Registers.h"
#include "NVIC.h"

void NVIC_SetISR(void){

}

void NVIC_Enable(uint8_t ID){
    volatile uint32_t *reg_en = 0;

    uint8_t regNum = ID/32;
    uint32_t bitNum = ID%32;

    reg_en = (uint32_t*)((CorePeripheralBase + EN0)+(regNum*4));
    *reg_en = *reg_en | (1<<bitNum);
}

void NVIC_Disable(uint8_t ID){
    volatile uint32_t *reg_dis = 0;

    uint8_t regNum = ID/32;
    uint32_t bitNum = ID%32;

    reg_dis = (uint32_t*)((CorePeripheralBase + DIS0)+(regNum*4));
    *reg_dis = *reg_dis | (1<<bitNum);

}

void NVIC_SetPending(uint8_t ID){
    volatile uint32_t *reg_pend = 0;

    uint8_t regNum = ID/32;
    uint32_t bitNum = ID%32;

    reg_pend = (uint32_t*)((CorePeripheralBase + PEND0)+(regNum*4));
    *reg_pend = *reg_pend | (1<<bitNum);

}

void NVIC_ClearPending(uint8_t ID){
    volatile uint32_t *reg_unpend = 0;

    uint8_t regNum = ID/32;
    uint32_t bitNum = ID%32;

    reg_unpend = (uint32_t*)((CorePeripheralBase + UNPEND0)+(regNum*4));
    *reg_unpend = *reg_unpend | (1<<bitNum);

}

uint8_t NVIC_GetPending(uint8_t ID){
    volatile uint32_t *reg_pend = 0;

    uint8_t regNum = ID/32;
    uint32_t bitNum = ID%32;

    reg_pend = (uint32_t*)((CorePeripheralBase + PEND0)+(regNum*4));

    return (((*reg_pend)&(1<<bitNum)) >> bitNum);
}

uint8_t NVIC_GetActiveState(uint8_t ID){
    volatile uint32_t *reg_active = 0;

    uint8_t regNum = ID/32;
    uint32_t bitNum = ID%32;

    reg_active = (uint32_t*)((CorePeripheralBase + ACTIVE0)+(regNum*4));

    return (((*reg_active)&(1<<bitNum)) >> bitNum);

}

void NVIC_SetPriority(uint8_t ID,uint8_t priority){
    volatile uint32_t *reg_pri = 0;

    uint8_t regNum = ID/4;
    uint32_t bitNum = ID%4;
    bitNum = bitNum*8 + 5;

    reg_pri = (uint32_t*)((CorePeripheralBase + PRI0)+(regNum*4));
    *reg_pri = *reg_pri | (priority<<bitNum);
}

uint8_t NVIC_GetPriority(uint8_t ID){
    volatile uint32_t *reg_pri = 0;

    uint8_t regNum = ID/4;
    uint32_t bitNum = ID%4;
    bitNum = bitNum*8 + 5;

    reg_pri = (uint32_t*)((CorePeripheralBase + PRI0)+(regNum*4));

    return (((*reg_pri)&(7<<bitNum)) >> bitNum);
}


