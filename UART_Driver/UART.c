/*
 * UART.c
 *
 *  Created on: Apr 18, 2020
 *      Author: DELL
 */

#include "Std_types.h"
#include "Registers.h"
#include "NVIC.h"
#include "UART.h"

static uint32_t UART_BuadRates[8] = {0};
static void (*(UART0_interrupt_sources[8])) (void);
static void (*(UART1_interrupt_sources[8])) (void);
static void (*(UART2_interrupt_sources[8])) (void);
static void (*(UART3_interrupt_sources[8])) (void);
static void (*(UART4_interrupt_sources[8])) (void);
static void (*(UART5_interrupt_sources[8])) (void);
static void (*(UART6_interrupt_sources[8])) (void);
static void (*(UART7_interrupt_sources[8])) (void);

typedef void (*(fptr[8])) (void);
static fptr all_ptrs[8] = {UART0_interrupt_sources,
                           UART1_interrupt_sources,
                           UART2_interrupt_sources,
                           UART3_interrupt_sources,
                           UART4_interrupt_sources,
                           UART5_interrupt_sources,
                           UART6_interrupt_sources,
                           UART7_interrupt_sources
};

static uint8_t UART_NVICVectors[8] = {5,6,33,59,60,61,62,63};

void UART_Init(UART_MODULE module, UART_BUADRATE buad, UART_WORD word, UART_STOPBITS stop, UART_PARITY parity){
    //Provide the clock to the specified uart
    SYSCTL_RCGCUART_R |= (1<<(((module&(0xFF<<0xC))>>0xC) - 0x0C));

    UART_ConfigBuadRate(module, buad);
    UART_ConfigFrame(module, word, stop, parity);
}

void UART_ConfigBuadRate(UART_MODULE module, UART_BUADRATE buad){
    uint32_t UARTSysClk = 0;
    uint8_t ClkDiv = 0;
    uint16_t IBRD = 0;
    uint8_t FBRD = 0;
    float BRD = 0;

    volatile uint32_t *reg_ctl = (volatile uint32_t *)(module + (UARTCTL));
    volatile uint32_t *reg_cc = (volatile uint32_t *)(module + (UARTCC));
    volatile uint32_t *reg_ibrd = (volatile uint32_t *)(module + (UARTIBRD));
    volatile uint32_t *reg_fbrd = (volatile uint32_t *)(module + (UARTFBRD));
    volatile uint32_t *reg_lcrh = (volatile uint32_t *)(module + (UARTLCRH));

    *reg_ctl &= ~(1<<0); //Disable UART
    UART_BuadRates[(((module&(0xFF<<0xC))>>0xC) - 0x0C)] = buad;

    ClkDiv = (*reg_ctl & (1<<5)) ? 8 : 16; //High speed
    UARTSysClk = ((*reg_cc) & 0x0F) == 0x05 ? PIOSC : SYSCLK; //Used clock source for UART
    BRD = (float)UARTSysClk / (float)(ClkDiv*buad);
    IBRD = (uint16_t)BRD;
    FBRD = ((float)(BRD - IBRD) * (float)64.0) + 0.5;
    *reg_ibrd = IBRD;
    *reg_fbrd = FBRD;
    *reg_lcrh = *reg_lcrh;
}

void UART_ConfigFrame(UART_MODULE module, UART_WORD word, UART_STOPBITS stop, UART_PARITY parity){
    volatile uint32_t *reg_ctl = (volatile uint32_t *)(module + (UARTCTL));
    volatile uint32_t *reg_lcrh = (volatile uint32_t *)(module + (UARTLCRH));
    volatile uint32_t *reg_9bitaddr = (volatile uint32_t *)(module + (UART9BITADDR));
    *reg_ctl &= ~(1<<0); //Disable UART
    *reg_lcrh = *reg_lcrh & ~((7<<1)|(7<<5));
    if(word == WORD_9){
        *reg_lcrh = *reg_lcrh | (WORD_8);
        *reg_lcrh = *reg_lcrh | (stop);
        *reg_lcrh = *reg_lcrh | (parity);
        *reg_9bitaddr = *reg_9bitaddr | WORD_9;
    }else{
        *reg_9bitaddr &= (~(WORD_9));
        *reg_lcrh = *reg_lcrh | (word);
        *reg_lcrh = *reg_lcrh | (stop);
        *reg_lcrh = *reg_lcrh | (parity);
    }
}

void UART_ConfigEnableModule(UART_MODULE module, UART_ENABLE enable){
    volatile uint32_t *reg_ctl = (volatile uint32_t *)(module + (UARTCTL));
    *reg_ctl &= ~(1<<0); //Disable UART
    *reg_ctl = *reg_ctl | (enable<<0); //The only way the user can enable the uart is from here (More secure)
}

void UART_ConfigEnableTx(UART_MODULE module, UART_ENABLE enable){
    volatile uint32_t *reg_ctl = (volatile uint32_t *)(module + (UARTCTL));
    *reg_ctl &= ~(1<<0); //Disable UART
    *reg_ctl = *reg_ctl & (~(1<<8));
    *reg_ctl = *reg_ctl | (enable<<8);
}

void UART_ConfigEnableRx(UART_MODULE module, UART_ENABLE enable){
    volatile uint32_t *reg_ctl = (volatile uint32_t *)(module + (UARTCTL));
    *reg_ctl &= ~(1<<0); //Disable UART
    *reg_ctl = *reg_ctl & (~(1<<9));
    *reg_ctl = *reg_ctl | (enable<<9);
}

void UART_ConfigEnableFIFO(UART_MODULE module, UART_ENABLE enable){
    volatile uint32_t *reg_ctl = (volatile uint32_t *)(module + (UARTCTL));
    volatile uint32_t *reg_lcrh = (volatile uint32_t *)(module + (UARTLCRH));
    *reg_ctl &= ~(1<<0); //Disable UART
    *reg_lcrh = *reg_lcrh & (~(1<<4));
    *reg_lcrh = *reg_lcrh | (enable<<4);
}

void UART_ConfigEnableLoopBack(UART_MODULE module, UART_ENABLE enable){
    volatile uint32_t *reg_ctl = (volatile uint32_t *)(module + (UARTCTL));
    *reg_ctl &= ~(1<<0); //Disable UART
    *reg_ctl = *reg_ctl & (~(1<<7));
    *reg_ctl = *reg_ctl | (enable<<7);
}

void UART_ConfigEnableSIR(UART_MODULE module, UART_ENABLE enable){
    volatile uint32_t *reg_ctl = (volatile uint32_t *)(module + (UARTCTL));
    *reg_ctl &= ~(1<<0); //Disable UART
    *reg_ctl = *reg_ctl & (~(1<<1));
    *reg_ctl = *reg_ctl | (enable<<1);
}

void UART_ConfigEnableLowPowerModeSIR(UART_MODULE module, UART_ENABLE enable){
    volatile uint32_t *reg_ctl = (volatile uint32_t *)(module + (UARTCTL));
    *reg_ctl &= ~(1<<0); //Disable UART
    *reg_ctl = *reg_ctl & (~(1<<2));
    *reg_ctl = *reg_ctl | (enable<<2);
}

void UART_ConfigEnableHighSpeed(UART_MODULE module, UART_ENABLE enable){ //Recalculate BuadRate (followed by UARTLCRH)
    volatile uint32_t *reg_ctl = (volatile uint32_t *)(module + (UARTCTL));
    *reg_ctl &= ~(1<<0); //Disable UART
    *reg_ctl = *reg_ctl & (~(1<<5));
    *reg_ctl = *reg_ctl | (enable<<5);
    UART_ConfigBuadRate(module, UART_BuadRates[(((module&(0xFF<<0xC))>>0xC) - 0x0C)]);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Send Break for UNKOWN TIME (It is should be for 2 character periods) so do NOT use it now
//Probable Solution: Start timer in one shot mode using the FANCYTIMER functions but with some calculations regarding
//System clock and required Buadrate
void UART_SendBreak(UART_MODULE module, UART_ENABLE enable){
    volatile uint32_t *reg_ctl = (volatile uint32_t *)(module + (UARTCTL));
    volatile uint32_t *reg_lcrh = (volatile uint32_t *)(module + (UARTLCRH));
    uint32_t i = 0;
    *reg_lcrh = *reg_lcrh | (enable<<0);
    for(i = 0; i < 100000; i++);
    *reg_lcrh = *reg_lcrh & (~(1<<0));
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UART_SetTriggerFIFO(UART_MODULE module, UART_TRIGGER trigger){
    volatile uint32_t *reg_ctl = (volatile uint32_t *)(module + (UARTCTL));
    volatile uint32_t *reg_ifls = (volatile uint32_t *)(module + (UARTIFLS));
    *reg_ctl &= ~(1<<0); //Disable UART
    if(trigger != TX_0){
        *reg_ctl = *reg_ctl | (1<<4); //EOT
    }else if(trigger <= TX_2){
        *reg_ifls = *reg_ifls & (~(7<<0));
        reg_ifls = *reg_ifls | trigger;
    }else if(trigger >= RX_4){
        *reg_ifls = *reg_ifls & (~(7<<3));
        reg_ifls = *reg_ifls | trigger;
    }
}

void UART_SetSelfAddress(UART_MODULE module, uint8_t address){
    volatile uint32_t *reg_ctl = (volatile uint32_t *)(module + (UARTCTL));
    volatile uint32_t *reg_9bitaddr = (volatile uint32_t *)(module + (UART9BITADDR));
    *reg_ctl &= ~(1<<0); //Disable UART
    *reg_9bitaddr = (*reg_9bitaddr&(~(0xFF))) | (address&0xFF);
}

void UART_SetSelfAddressMask(UART_MODULE module, uint8_t address){
    volatile uint32_t *reg_ctl = (volatile uint32_t *)(module + (UARTCTL));
    volatile uint32_t *reg_9bitamask = (volatile uint32_t *)(module + (UART9BITAMASK));
    *reg_ctl &= ~(1<<0); //Disable UART
    *reg_9bitamask = (*reg_9bitamask&(~(0xFF))) | (address&0xFF);
}

void UART_SetClockSource(UART_MODULE module, UART_CLOCK clock){ //Recalculate BuadRate (followed by UARTLCRH)
    volatile uint32_t *reg_ctl = (volatile uint32_t *)(module + (UARTCTL));
    volatile uint32_t *reg_cc = (volatile uint32_t *)(module + (UARTCC));
    *reg_ctl &= ~(1<<0); //Disable UART
    *reg_cc = *reg_cc & (~(0x0F));
    if(clock == UART_PIOSC) *reg_cc = *reg_cc | UART_PIOSC;
    UART_ConfigBuadRate(module, UART_BuadRates[(((module&(0xFF<<0xC))>>0xC) - 0x0C)]);
}

void UART_EnableAndSetIntISR(UART_MODULE module, UART_INT interrupt_ID, void (*function) (void)){
    volatile uint32_t *reg_ctl = (volatile uint32_t *)(module + (UARTCTL));
    volatile uint32_t *reg_im = (volatile uint32_t *)(module + (UARTIM));
    uint8_t module_index = (((module&(0xFF<<0xC))>>0xC) - 0x0C);
    *reg_ctl &= ~(1<<0); //Disable UART

    if(interrupt_ID != UART_INT_9BIT) all_ptrs[module_index][interrupt_ID-4] = function;
    else all_ptrs[module_index][7] = function;

    *reg_im = *reg_im | (1<<interrupt_ID);

    //Enable timer interrupt from the NVIC
    NVIC_Enable(UART_NVICVectors[module_index]);
}

void UART_DisableInterrupt(UART_MODULE module, UART_INT interrupt_ID){
    volatile uint32_t *reg_ctl = (volatile uint32_t *)(module + (UARTCTL));
    volatile uint32_t *reg_im = (volatile uint32_t *)(module + (UARTIM));
    uint8_t module_index = (((module&(0xFF<<0xC))>>0xC) - 0x0C);
    //*reg_ctl &= ~(1<<0); //Disable UART

    //Disable timer interrupt from the NVIC
    NVIC_Enable(UART_NVICVectors[module_index]);

    *reg_im = *reg_im & (~(1<<interrupt_ID));
}

uint8_t UART_GetFlag(UART_MODULE module, UART_FLAG flags){
    volatile uint32_t *reg_fr = (volatile uint32_t *)(module + (UARTFR));
    return (*reg_fr & flags); //Return all required bits in its position
}

uint8_t UART_GetEnableFIFO(UART_MODULE module){
    volatile uint32_t *reg_lcrh = (volatile uint32_t *)(module + (UARTLCRH));
    return *reg_lcrh & (1<<4);
}

//It should be improved to handle characters more than 16 without delay like Eng.Abd-Alrahman said by changing the ISR function
void UART_Send(UART_MODULE module, uint8_t* ptr, uint32_t size){
    volatile uint32_t *reg_dr = (volatile uint32_t *)(module + (UARTDR));
    uint32_t i = 0;

    while(i < size){
        if(UART_GetFlag(module, FLAG_TX_FULL) == 0){
            *reg_dr = *(ptr+i);
            i++;
        }
    }
}

void UART_Receive(UART_MODULE module, UART_BUFFER* buf){
    volatile uint32_t *reg_dr = (volatile uint32_t *)(module + (UARTDR));
    uint32_t i = 0;

    while(UART_GetFlag(module, FLAG_RX_EMPTY) == 0){
        buf->buffer[buf->size] = *reg_dr;
        buf->size++;
    }
}


void UART3_ISR(void){
    uint8_t i = UART_INT_RECEIVE;
    uint8_t bit_position = 0;
    volatile uint32_t *reg_icr = (volatile uint32_t *)(UART3 + (UARTICR));
    volatile uint32_t *reg_mis = (volatile uint32_t *)(UART3 + (UARTMIS));

    while(((*reg_mis) & (1<<i)) == 0){
        i = i + 1;
        bit_position++;
        if(i == 11){
            i = i + 1;
            continue;
        }
        if(i>UART_INT_9BIT) return;
    }

    all_ptrs[3][bit_position]();

    //NEED TO BE REVISE AND UNDERSTOOD FOR AUTOMATIC CLEARING OR MANUAL AND THE MECHANISM IN HARDWARE
    if(bit_position + 4 != UART_INT_TRANSMIT && bit_position + 4 != UART_INT_RECEIVE){
        if(bit_position == 7) *reg_icr = (*reg_icr) | (1<<12);
        else *reg_icr = (*reg_icr) | (1<<(bit_position + 4));
    }
}

void UART7_ISR(void){
    uint8_t i = UART_INT_RECEIVE;
    uint8_t bit_position = 0;
    volatile uint32_t *reg_icr = (volatile uint32_t *)(UART7 + (UARTICR));
    volatile uint32_t *reg_mis = (volatile uint32_t *)(UART7 + (UARTMIS));

    while(((*reg_mis) & (1<<i)) == 0){
        i = i + 1;
        bit_position++;
        if(i == 11){
            i = i + 1;
            continue;
        }
        if(i>UART_INT_9BIT) return;
    }

    all_ptrs[7][bit_position]();

    //NEED TO BE REVISE AND UNDERSTOOD FOR AUTOMATIC CLEARING OR MANUAL AND THE MECHANISM IN HARDWARE
    if(bit_position + 4 != UART_INT_TRANSMIT && bit_position + 4 != UART_INT_RECEIVE){
        if(bit_position == 7) *reg_icr = (*reg_icr) | (1<<12);
        else *reg_icr = (*reg_icr) | (1<<(bit_position + 4));
    }
}
