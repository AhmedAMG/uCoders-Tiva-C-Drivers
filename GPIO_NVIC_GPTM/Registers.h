/*
 * Registers.h
 *
 *  Created on: Aug 30, 2019
 *      Author: DELL
 */

#ifndef REGISTERS_H_
#define REGISTERS_H_


//*****************************************************************************
//
// System Control registers (SYSCTL)
//
//*****************************************************************************
#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608))
#define SYSCTL_GPIOHBCTL_R      (*((volatile uint32_t *)0x400FE06C))
#define SYSCTL_RCGCTIMER_R      (*((volatile uint32_t *)0x400FE604))
#define SYSCTL_RCGCWTIMER_R      (*((volatile uint32_t *)0x400FE65C))

#define CorePeripheralBase 0xE000E000

//
// NVIC Offset
//
//*****************************************************************************
#define EN0 0x100
#define DIS0 0x180
#define PEND0 0x200
#define UNPEND0 0x280
#define ACTIVE0 0x300
#define PRI0 0x400
#define SWTRIG 0xF00



//
// Registers Offset
//
//*****************************************************************************
#define GPIODATA 0x000
#define GPIODIR 0x400
#define GPIOIS 0x404
#define GPIOIBE 0x408
#define GPIOIEV 0x40C
#define GPIOIM 0x410
#define GPIORIS 0x414
#define GPIOMIS 0x418
#define GPIOICR 0x41C
#define GPIOAFSEL 0x420
#define GPIODR2R 0x500
#define GPIODR4R 0x504
#define GPIODR8R 0x508
#define GPIOODR 0x50C
#define GPIOPUR 0x510
#define GPIOPDR 0x514
#define GPIOSLR 0x518
#define GPIODEN 0x51C
#define GPIOLOCK 0x520
#define GPIOCR 0x524
#define GPIOAMSEL 0x528
#define GPIOPCTL 0x52C

#define EN0 0x100

//*****************************************************************************
//
// GPIO registers (PORTA)
//
//*****************************************************************************
#define GPIO_PORTA_DATA_BITS_R  ((volatile uint32_t *)0x40004000)
#define GPIO_PORTA_DATA_R       (*((volatile uint32_t *)0x400043FC))
#define GPIO_PORTA_DIR_R        (*((volatile uint32_t *)0x40004400))
#define GPIO_PORTA_DEN_R        (*((volatile uint32_t *)0x4000451C))


//*****************************************************************************
//
// GPIO registers (PORTB)
//
//*****************************************************************************
#define GPIO_PORTB_DATA_BITS_R  ((volatile uint32_t *)0x40005000)
#define GPIO_PORTB_DATA_R       (*((volatile uint32_t *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile uint32_t *)0x40005400))
#define GPIO_PORTB_DEN_R        (*((volatile uint32_t *)0x4000551C))


//*****************************************************************************
//
// GPIO registers (PORTC)
//
//*****************************************************************************
#define GPIO_PORTC_DATA_BITS_R  ((volatile uint32_t *)0x40006000)
#define GPIO_PORTC_DATA_R       (*((volatile uint32_t *)0x400063FC))
#define GPIO_PORTC_DIR_R        (*((volatile uint32_t *)0x40006400))
#define GPIO_PORTC_DEN_R        (*((volatile uint32_t *)0x4000651C))


//*****************************************************************************
//
// GPIO registers (PORTD)
//
//*****************************************************************************
#define GPIO_PORTD_DATA_BITS_R  ((volatile uint32_t *)0x40007000)
#define GPIO_PORTD_DATA_R       (*((volatile uint32_t *)0x400073FC))
#define GPIO_PORTD_DIR_R        (*((volatile uint32_t *)0x40007400))
#define GPIO_PORTD_DEN_R        (*((volatile uint32_t *)0x4000751C))



//*****************************************************************************
//
// GPIO registers (PORTE)
//
//*****************************************************************************
#define GPIO_PORTE_DATA_BITS_R  ((volatile uint32_t *)0x40024000)
#define GPIO_PORTE_DATA_R       (*((volatile uint32_t *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile uint32_t *)0x40024400))
#define GPIO_PORTE_DEN_R        (*((volatile uint32_t *)0x4002451C))


//*****************************************************************************
//
// GPIO registers (PORTF)
//
//*****************************************************************************
#define GPIO_PORTF_DATA_BITS_R  ((volatile uint32_t *)0x40025000)
#define GPIO_PORTF_DATA_R       (*((volatile uint32_t *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile uint32_t *)0x40025400))
#define GPIO_PORTF_DEN_R        (*((volatile uint32_t *)0x4002551C))


//*****************************************************************************
//
// Timer registers offsets
//
//*****************************************************************************
#define TIMERCFG            (0x0000)
#define TIMERTAMR           (0x0004)
#define TIMERTBMR           (0x0008)
#define TIMERCTL            (0x000C)
#define TIMERSYNC           (0x0010)
#define TIMERIMR            (0x0018)
#define TIMERRIS            (0x001C)
#define TIMERMIS            (0x0020)
#define TIMERICR            (0x0024)
#define TIMERTAILR          (0x0028)
#define TIMERTBILR          (0x002C)
#define TIMERTAMATCHR       (0x0030)
#define TIMERTBMATCHR       (0x0034)
#define TIMERTAPR           (0x0038)
#define TIMERTBPR           (0x003C)
#define TIMERTAPMR          (0x0040)
#define TIMERTBPMR          (0x0044)
#define TIMERTAR            (0x0048)
#define TIMERTBR            (0x004C)
#define TIMERTAV            (0x0050)
#define TIMERTBV            (0x0054)
#define TIMERRTCPD          (0x0058)
#define TIMERTAPS           (0x005C)
#define TIMERTBPS           (0x0060)
#define TIMERTAPV           (0x0064)
#define TIMERTBPV           (0x0068)
#define TIMERPP             (0x0FC0)


#endif /* REGISTERS_H_ */
