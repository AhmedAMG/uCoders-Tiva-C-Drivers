/*
 * UART.h
 *
 *  Created on: Apr 18, 2020
 *      Author: DELL
 */

#ifndef UART_H_
#define UART_H_

#ifndef SYSCLK
#define SYSCLK 16000000
#endif
#ifndef PIOSC
#define PIOSC 16000000
#endif

typedef enum{
    UART0 = 0x4000C000,
    UART1 = 0x4000D000,
    UART2 = 0x4000E000,
    UART3 = 0x4000F000,
    UART4 = 0x40010000,
    UART5 = 0x40011000,
    UART6 = 0x40012000,
    UART7 = 0x40013000
}UART_MODULE;

typedef enum{
    BUADRATE_1200 = 1200,
    BUADRATE_2400 = 2400,
    BUADRATE_4800 = 4800,
    BUADRATE_9600 = 9600,
    BUADRATE_19200 = 19200,
    BUADRATE_38400 = 38400,
    BUADRATE_57600 = 57600,
    BUADRATE_115200 = 115200,
    BUADRATE_230400 = 230400,
    BUADRATE_460800 = 460800,
    BUADRATE_921600 = 921600
}UART_BUADRATE;

typedef enum{
    STOP_1 = (0<<3),
    STOP_2 = (1<<3)
}UART_STOPBITS;

typedef enum{
    PARITY_NONE = (0<<1),
    PARITY_ODD = (1<<1),
    PARITY_EVEN = (3<<1),
    PARITY_STICK0 = ((3<<1)|(1<<7)),
    PARITY_STICK1 = ((1<<1)|(1<<7))
}UART_PARITY;

typedef enum{
    WORD_5 = (0<<5),
    WORD_6 = (1<<5),
    WORD_7 = (2<<5),
    WORD_8 = (3<<5),
    WORD_9 = (1<<15)
}UART_WORD;

typedef enum{
    TX_14 = 0,
    TX_12 = 1,
    TX_8 = 2,
    TX_4 = 3,
    TX_2 = 4,
    TX_0 = (1<<4), //End-Of-Transmission (EOT) (1<<4)

    RX_2 = (0<<3), //Double check (0 is repeated) (Be careful)
    RX_4 = (1<<3),
    RX_8 = (2<<3),
    RX_12 = (3<<3),
    RX_14 = (4<<3)
}UART_TRIGGER;

typedef enum{
    UART_SYSCLK = 0x00,
    UART_PIOSC = 0x05
}UART_CLOCK;

typedef enum{
    DIV_8 = 0,
    DIV_16 = 1
}UART_HIGHSPEED;

typedef enum{
    DISABLE = 0,
    ENABLE = 1
}UART_ENABLE;

typedef enum{
    UART_INT_RECEIVE = (4),
    UART_INT_TRANSMIT = (5),
    UART_INT_RECEIVE_TIMEOUT = (6),
    UART_INT_FRAME_ERROR = (7),
    UART_INT_PARITY_ERROR = (8),
    UART_INT_BREAK_ERROR = (9),
    UART_INT_OVERRUN = (10),
    UART_INT_9BIT = (12)
}UART_INT;

typedef enum{
    FLAG_BUSY = (1<<3),
    FLAG_RX_EMPTY = (1<<4),
    FLAG_TX_FULL = (1<<5),
    FLAG_RX_FULL = (1<<6),
    FLAG_TX_EMPTY = (1<<7)
}UART_FLAG;

typedef struct{
    uint8_t buffer[100];
    uint8_t size;
}UART_BUFFER;

void UART_Init(UART_MODULE module, UART_BUADRATE buad, UART_WORD word, UART_STOPBITS stop, UART_PARITY parity);
void UART_ConfigBuadRate(UART_MODULE module, UART_BUADRATE buad);
void UART_ConfigFrame(UART_MODULE module, UART_WORD word, UART_STOPBITS stop, UART_PARITY parity);
void UART_ConfigEnableModule(UART_MODULE module, UART_ENABLE enable);
void UART_ConfigEnableTx(UART_MODULE module, UART_ENABLE enable);
void UART_ConfigEnableRx(UART_MODULE module, UART_ENABLE enable);
void UART_ConfigEnableFIFO(UART_MODULE module, UART_ENABLE enable);
void UART_ConfigEnableLoopBack(UART_MODULE module, UART_ENABLE enable);
void UART_ConfigEnableSIR(UART_MODULE module, UART_ENABLE enable);
void UART_ConfigEnableLowPowerModeSIR(UART_MODULE module, UART_ENABLE enable);
void UART_ConfigEnableHighSpeed(UART_MODULE module, UART_ENABLE enable); //Recalculate BuadRate (followed by UARTLCRH)
void UART_SendBreak(UART_MODULE module, UART_ENABLE enable);
void UART_SetTriggerFIFO(UART_MODULE module, UART_TRIGGER trigger);
void UART_SetSelfAddress(UART_MODULE module, uint8_t address);
void UART_SetSelfAddressMask(UART_MODULE module, uint8_t address);
void UART_SetClockSource(UART_MODULE module, UART_CLOCK clock); //Recalculate BuadRate (followed by UARTLCRH)
void UART_EnableAndSetIntISR(UART_MODULE module, UART_INT interrupt_ID, void (*function) (void));
void UART_DisableInterrupt(UART_MODULE module, UART_INT interrupt_ID);
uint8_t UART_GetFlag(UART_MODULE module, UART_FLAG flags);
uint8_t UART_GetEnableFIFO(UART_MODULE module);
void UART_Send(UART_MODULE module, uint8_t* ptr, uint32_t size);
void UART_Receive(UART_MODULE module, UART_BUFFER* buf);

void UART3_ISR(void);
void UART7_ISR(void);


#endif /* UART_H_ */
