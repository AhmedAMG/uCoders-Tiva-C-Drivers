/*
 * GPIO.h
 *
 *  Created on: Feb 12, 2020
 *      Author: DELL
 */

#ifndef GPIO_H_
#define GPIO_H_

typedef enum{
    PORTA = 0x40004000,
    PORTB = 0x40005000,
    PORTC = 0x40006000,
    PORTD = 0x40007000,
    PORTE = 0x40024000,
    PORTF = 0x40025000
}GPIO_PORT;

#define PIN0 (1<<0)
#define PIN1 (1<<1)
#define PIN2 (1<<2)
#define PIN3 (1<<3)
#define PIN4 (1<<4)
#define PIN5 (1<<5)
#define PIN6 (1<<6)
#define PIN7 (1<<7)

typedef enum{
    MODE_IN, MODE_OUT, MODE_AF
}GPIO_MODE;

typedef enum{
    LOW = 0, HIGH = 1
}GPIO_LEVEL;

typedef enum{
    APB = 0, AHB = 1
}GPIO_BUS;

typedef enum{
    DRIVE_2m = 0, DRIVE_4m = 1, DRIVE_8m = 2
}GPIO_CURRENT;

typedef enum{
    PAD_OD = 0, PAD_PU = 1, PAD_PD = 2, PAD_NPU_NPD = 3
}GPIO_PAD;

typedef enum{
    EDGE = 0, LEVEL = 1
}GPIO_SENSE;

typedef enum{
    RISING_EDGE = 0, FALLING_EDGE = 1, BOTH_EDGES = 2,
    LOW_LEVEL = 3, HIGH_LEVEL = 4
}GPIO_SPECS;

typedef struct{
    GPIO_MODE mode;
    GPIO_CURRENT current;
    GPIO_PAD pad;
    GPIO_SENSE sense;
    GPIO_SPECS specs;
}GPIO_SPECS_STRUCT;


void GPIODirectionModeSet(GPIO_PORT port, uint8_t pins, GPIO_MODE mode);
uint8_t GPIODirectionGet(GPIO_PORT port, uint8_t pins);
uint8_t GPIOModeGet(GPIO_PORT port, uint8_t pins);
void GPIOBusSet(GPIO_PORT port, GPIO_BUS bus);
GPIO_BUS GPIOBusGet(GPIO_PORT port);
void GPIOClockSet(GPIO_PORT port);
void GPIOClockReset(GPIO_PORT port);
uint8_t GPIOClockGet(GPIO_PORT port);
void GPIO_PadSet(GPIO_PORT port, uint8_t pins, GPIO_CURRENT strength, GPIO_PAD pad);
uint8_t GPIORead(GPIO_PORT port, uint8_t pins);
void GPIOWrite(GPIO_PORT port, uint8_t pins, uint8_t data);

void GPIO_ConfigInterrupt(GPIO_PORT port, uint8_t pins, GPIO_SENSE sense, GPIO_SPECS specs);
void GPIO_SetIntHandler(GPIO_PORT port, uint8_t pin, void (*function) (void));
void GPIOF_ISR(void);
void GPIOE_ISR(void);

#endif /* GPIO_H_ */
