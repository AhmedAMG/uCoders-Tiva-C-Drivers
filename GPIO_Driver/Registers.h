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
// GPIO registers (PORTA)
//
//*****************************************************************************
#define GPIO_PORTA_DATA        ((volatile uint32_t *)0x400043FC)
#define GPIO_PORTA_DIR         ((volatile uint32_t *)0x40004400)
#define GPIO_PORTA_DEN         ((volatile uint32_t *)0x4000451C)

//*****************************************************************************
//
// GPIO registers (PORTB)
//
//*****************************************************************************
#define GPIO_PORTB_DATA        ((volatile uint32_t *)0x400053FC)
#define GPIO_PORTB_DIR         ((volatile uint32_t *)0x40005400)
#define GPIO_PORTB_DEN         ((volatile uint32_t *)0x4000551C)

//*****************************************************************************
//
// GPIO registers (PORTC)
//
//*****************************************************************************
#define GPIO_PORTC_DATA        ((volatile uint32_t *)0x400063FC)
#define GPIO_PORTC_DIR         ((volatile uint32_t *)0x40006400)
#define GPIO_PORTC_DEN         ((volatile uint32_t *)0x4000651C)


//*****************************************************************************
//
// GPIO registers (PORTD)
//
//*****************************************************************************
#define GPIO_PORTD_DATA        ((volatile uint32_t *)0x400073FC)
#define GPIO_PORTD_DIR         ((volatile uint32_t *)0x40007400)
#define GPIO_PORTD_DEN         ((volatile uint32_t *)0x4000751C)



//*****************************************************************************
//
// GPIO registers (PORTE)
//
//*****************************************************************************
#define GPIO_PORTE_DATA        ((volatile uint32_t *)0x400243FC)
#define GPIO_PORTE_DIR         ((volatile uint32_t *)0x40024400)
#define GPIO_PORTE_DEN         ((volatile uint32_t *)0x4002451C)


//*****************************************************************************
//
// GPIO registers (PORTF)
//
//*****************************************************************************
#define GPIO_PORTF_DATA       ((volatile uint32_t *)0x400253FC)
#define GPIO_PORTF_DIR        ((volatile uint32_t *)0x40025400)
#define GPIO_PORTF_DEN        ((volatile uint32_t *)0x4002551C)


#endif /* REGISTERS_H_ */
