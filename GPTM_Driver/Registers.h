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




//*****************************************************************************
//
// Timer registers (TIMER0)
//
//*****************************************************************************
#define TIMER0_CFG_R            (*((volatile uint32_t *)0x40030000))
#define TIMER0_TAMR_R           (*((volatile uint32_t *)0x40030004))
#define TIMER0_TBMR_R           (*((volatile uint32_t *)0x40030008))
#define TIMER0_CTL_R            (*((volatile uint32_t *)0x4003000C))
#define TIMER0_SYNC_R           (*((volatile uint32_t *)0x40030010))
#define TIMER0_IMR_R            (*((volatile uint32_t *)0x40030018))
#define TIMER0_RIS_R            (*((volatile uint32_t *)0x4003001C))
#define TIMER0_MIS_R            (*((volatile uint32_t *)0x40030020))
#define TIMER0_ICR_R            (*((volatile uint32_t *)0x40030024))
#define TIMER0_TAILR_R          (*((volatile uint32_t *)0x40030028))
#define TIMER0_TBILR_R          (*((volatile uint32_t *)0x4003002C))
#define TIMER0_TAMATCHR_R       (*((volatile uint32_t *)0x40030030))
#define TIMER0_TBMATCHR_R       (*((volatile uint32_t *)0x40030034))
#define TIMER0_TAPR_R           (*((volatile uint32_t *)0x40030038))
#define TIMER0_TBPR_R           (*((volatile uint32_t *)0x4003003C))
#define TIMER0_TAPMR_R          (*((volatile uint32_t *)0x40030040))
#define TIMER0_TBPMR_R          (*((volatile uint32_t *)0x40030044))
#define TIMER0_TAR_R            (*((volatile uint32_t *)0x40030048))
#define TIMER0_TBR_R            (*((volatile uint32_t *)0x4003004C))
#define TIMER0_TAV_R            (*((volatile uint32_t *)0x40030050))
#define TIMER0_TBV_R            (*((volatile uint32_t *)0x40030054))
#define TIMER0_RTCPD_R          (*((volatile uint32_t *)0x40030058))
#define TIMER0_TAPS_R           (*((volatile uint32_t *)0x4003005C))
#define TIMER0_TBPS_R           (*((volatile uint32_t *)0x40030060))
#define TIMER0_TAPV_R           (*((volatile uint32_t *)0x40030064))
#define TIMER0_TBPV_R           (*((volatile uint32_t *)0x40030068))
#define TIMER0_PP_R             (*((volatile uint32_t *)0x40030FC0))


//*****************************************************************************
//
// Timer registers (TIMER1)
//
//*****************************************************************************
#define TIMER1_CFG_R            (*((volatile uint32_t *)0x40031000))
#define TIMER1_TAMR_R           (*((volatile uint32_t *)0x40031004))
#define TIMER1_TBMR_R           (*((volatile uint32_t *)0x40031008))
#define TIMER1_CTL_R            (*((volatile uint32_t *)0x4003100C))
#define TIMER1_SYNC_R           (*((volatile uint32_t *)0x40031010))
#define TIMER1_IMR_R            (*((volatile uint32_t *)0x40031018))
#define TIMER1_RIS_R            (*((volatile uint32_t *)0x4003101C))
#define TIMER1_MIS_R            (*((volatile uint32_t *)0x40031020))
#define TIMER1_ICR_R            (*((volatile uint32_t *)0x40031024))
#define TIMER1_TAILR_R          (*((volatile uint32_t *)0x40031028))
#define TIMER1_TBILR_R          (*((volatile uint32_t *)0x4003102C))
#define TIMER1_TAMATCHR_R       (*((volatile uint32_t *)0x40031030))
#define TIMER1_TBMATCHR_R       (*((volatile uint32_t *)0x40031034))
#define TIMER1_TAPR_R           (*((volatile uint32_t *)0x40031038))
#define TIMER1_TBPR_R           (*((volatile uint32_t *)0x4003103C))
#define TIMER1_TAPMR_R          (*((volatile uint32_t *)0x40031040))
#define TIMER1_TBPMR_R          (*((volatile uint32_t *)0x40031044))
#define TIMER1_TAR_R            (*((volatile uint32_t *)0x40031048))
#define TIMER1_TBR_R            (*((volatile uint32_t *)0x4003104C))
#define TIMER1_TAV_R            (*((volatile uint32_t *)0x40031050))
#define TIMER1_TBV_R            (*((volatile uint32_t *)0x40031054))
#define TIMER1_RTCPD_R          (*((volatile uint32_t *)0x40031058))
#define TIMER1_TAPS_R           (*((volatile uint32_t *)0x4003105C))
#define TIMER1_TBPS_R           (*((volatile uint32_t *)0x40031060))
#define TIMER1_TAPV_R           (*((volatile uint32_t *)0x40031064))
#define TIMER1_TBPV_R           (*((volatile uint32_t *)0x40031068))
#define TIMER1_PP_R             (*((volatile uint32_t *)0x40031FC0))



//*****************************************************************************
//
// Timer registers (TIMER5)
//
//*****************************************************************************
#define TIMER5_CFG_R            (*((volatile uint32_t *)0x40035000))
#define TIMER5_TAMR_R           (*((volatile uint32_t *)0x40035004))
#define TIMER5_TBMR_R           (*((volatile uint32_t *)0x40035008))
#define TIMER5_CTL_R            (*((volatile uint32_t *)0x4003500C))
#define TIMER5_SYNC_R           (*((volatile uint32_t *)0x40035010))
#define TIMER5_IMR_R            (*((volatile uint32_t *)0x40035018))
#define TIMER5_RIS_R            (*((volatile uint32_t *)0x4003501C))
#define TIMER5_MIS_R            (*((volatile uint32_t *)0x40035020))
#define TIMER5_ICR_R            (*((volatile uint32_t *)0x40035024))
#define TIMER5_TAILR_R          (*((volatile uint32_t *)0x40035028))
#define TIMER5_TBILR_R          (*((volatile uint32_t *)0x4003502C))
#define TIMER5_TAMATCHR_R       (*((volatile uint32_t *)0x40035030))
#define TIMER5_TBMATCHR_R       (*((volatile uint32_t *)0x40035034))
#define TIMER5_TAPR_R           (*((volatile uint32_t *)0x40035038))
#define TIMER5_TBPR_R           (*((volatile uint32_t *)0x4003503C))
#define TIMER5_TAPMR_R          (*((volatile uint32_t *)0x40035040))
#define TIMER5_TBPMR_R          (*((volatile uint32_t *)0x40035044))
#define TIMER5_TAR_R            (*((volatile uint32_t *)0x40035048))
#define TIMER5_TBR_R            (*((volatile uint32_t *)0x4003504C))
#define TIMER5_TAV_R            (*((volatile uint32_t *)0x40035050))
#define TIMER5_TBV_R            (*((volatile uint32_t *)0x40035054))
#define TIMER5_RTCPD_R          (*((volatile uint32_t *)0x40035058))
#define TIMER5_TAPS_R           (*((volatile uint32_t *)0x4003505C))
#define TIMER5_TBPS_R           (*((volatile uint32_t *)0x40035060))
#define TIMER5_TAPV_R           (*((volatile uint32_t *)0x40035064))
#define TIMER5_TBPV_R           (*((volatile uint32_t *)0x40035068))
#define TIMER5_PP_R             (*((volatile uint32_t *)0x40035FC0))


#endif /* REGISTERS_H_ */
