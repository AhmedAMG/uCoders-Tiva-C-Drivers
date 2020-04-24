

/**
 * main.c
 */

#include "Std_types.h"
#include "Registers.h"
#include "GPIO.h"
#include "NVIC.h"
#include "GPTM.h"
#include "UART.h"

#define LED_RED PIN1
#define LED_BLUE PIN2
#define LED_GREEN PIN3
#define SW_2 PIN0
#define SW_1 PIN4

void UART_Handler(void);
UART_BUFFER buffer = {0,0};
uint8_t* string = "Hello to my fancy text editor\n\r";

int main(void)
{
    volatile uint32_t i = 0;

    GPIOBusSet(PORTE, APB);
    GPIOClockSet(PORTE);
    GPIO_PadSet(PORTE, PIN1, DRIVE_8m, PAD_NPU_NPD);
    GPIODirectionModeSet(PORTE,PIN1, MODE_AF_OUTPUT);
    GPIO_ConfigPCTLMux(PORTE, PIN1, 1);

    GPIOBusSet(PORTC, APB);
    GPIOClockSet(PORTC);
    GPIO_PadSet(PORTC, PIN6, DRIVE_8m, PAD_NPU_NPD);
    GPIODirectionModeSet(PORTC,PIN6, MODE_AF_INPUT);
    GPIO_ConfigPCTLMux(PORTC, PIN6, 1);

    GPIOBusSet(PORTF, APB);
    GPIOClockSet(PORTF);
    GPIO_PadSet(PORTF, LED_GREEN|LED_BLUE|LED_RED, DRIVE_8m, PAD_NPU_NPD);
    GPIODirectionModeSet(PORTF,LED_GREEN|LED_BLUE|LED_RED, MODE_OUT);

    UART_Init(UART7, BUADRATE_9600, WORD_8, STOP_1, PARITY_NONE);
    UART_ConfigEnableTx(UART7, ENABLE);
    UART_ConfigEnableFIFO(UART7, ENABLE); //Default: 8 bytes

    UART_Init(UART3, BUADRATE_9600, WORD_8, STOP_1, PARITY_NONE);
    UART_EnableAndSetIntISR(UART3, UART_INT_RECEIVE, UART_Handler);
    UART_ConfigEnableRx(UART3, ENABLE);

    UART_ConfigEnableModule(UART7, ENABLE);
    UART_ConfigEnableModule(UART3, ENABLE);
    UART_Send(UART7, string, 31);

    while(1){

    }
	return 0;
}

void UART_Handler(void){
    UART_Receive(UART3, &buffer);
    UART_Send(UART7, buffer.buffer, buffer.size);
    buffer.size = 0;
}


