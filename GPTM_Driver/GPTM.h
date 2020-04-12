/*
 * GPTM.h
 *
 *  Created on: Feb 28, 2020
 *      Author: DELL
 */

#ifndef GPTM_H_
#define GPTM_H_

#define SYSCLK 16000000

typedef enum{
    TIM0_AB = 0x40030000, TIM0_A = 0x40030001, TIM0_B = 0x40030002,
    TIM1_AB = 0x40031000, TIM1_A = 0x40031001, TIM1_B = 0x40031002,
    TIM2_AB = 0x40032000, TIM2_A = 0x40032001, TIM2_B = 0x40032002,
    TIM3_AB = 0x40033000, TIM3_A = 0x40033001, TIM3_B = 0x40033002,
    TIM4_AB = 0x40034000, TIM4_A = 0x40034001, TIM4_B = 0x40034002,
    TIM5_AB = 0x40035000, TIM5_A = 0x40035001, TIM5_B = 0x40035002,
    /*
    WTIM0_AB = 0x40036000, TIM0_A = 0x40036001, TIM0_B = 0x40036002,
    WTIM1_AB = 0x40037000, TIM1_A = 0x40037001, TIM1_B = 0x40037002,
    WTIM2_AB = 0x4004C000, TIM2_A = 0x4004C001, TIM2_B = 0x4004C002,
    WTIM3_AB = 0x4004D000, TIM3_A = 0x4004D001, TIM3_B = 0x4004D002,
    WTIM4_AB = 0x4004E000, TIM4_A = 0x4004E001, TIM4_B = 0x4004E002,
    WTIM5_AB = 0x4004F000, TIM5_A = 0x4004F001, TIM5_B = 0x4004F002
    */
}GPTM_MODULE;

typedef enum{
    TIMER_MODE_ONE_SHOT = 1,
    TIMER_MODE_PERIODIC = 2,
    TIMER_MODE_INPUT_EDGE_COUNT = 3,
    TIMER_MODE_INPUT_EDGE_TIME = 4,
    TIMER_MODE_PWM = 5,
    TIMER_MODE_RTC = 6
}GPTM_MODE;

typedef enum{
    COUNT_DOWN = 0,
    COUNT_UP = 1
}GPTM_DIRECTION;

typedef enum{
    STALL_DISABLE = 0,
    STALL_ENABLE = 1
}GPTM_STALL;

typedef enum{
    INT_TIMEOUT = 0, //For all modes
    INT_CAPTURE_MATCH = 1, //For edge count
    INT_CAPTURE_EVENT = 2, //For edge-time(input) and pwm(output) (9 in TIMER_MR)
    INT_RTC = 3, //For Real time clock
    INT_NON_CAPTURE_MATCH = 4 //For one shot and periodic (5 in TIMER_MR)
}GPTM_INT;

typedef enum{
    UPDATE_NEXT_CYCLE = 0,
    UPDATE_NEXT_TIMEOUT = 1
}GPTM_UPDATE;

typedef enum{
    REG_ILD, REG_PR, REG_MATCH
}GPTM_REGS;

typedef enum{
    EDGE_RISING = 0,
    EDGE_FALLING = 1,
    EDGE_BOTH = 3
}GPTM_EDGE;

typedef enum{
    EDGECOUNT_ONETIME = 0,
    EDGECOUNT_CONTINUOUS = 1
}GPTM_CONTINUITY;

typedef enum{
    CCP_NONINVERTED = 0,
    CCP_INVERTED = 1
}GPTM_CCP;

//Classy Functions :)
//You can use them for almost full control on the 32bit timers without the following section of functions
uint8_t FANCYTIMER_ONESHOT_SetTimeoutSeconds(GPTM_MODULE module, uint32_t msec, void (*function) (void));
uint8_t FANCYTIMER_ONESHOT_SetMatchSeconds(GPTM_MODULE module, uint32_t msec, void (*function) (void));
uint8_t FANCYTIMER_PERIODIC_SetTimeoutSeconds(GPTM_MODULE module, uint32_t msec, void (*function) (void));
uint8_t FANCYTIMER_PERIODIC_SetMatchSeconds(GPTM_MODULE module, uint32_t msec, void (*function) (void));
uint8_t FANCYTIMER_ECOUNT_SetEventsNumber(GPTM_MODULE module, uint32_t events, GPTM_EDGE edge, GPTM_CONTINUITY continuity, void (*function) (void));
uint8_t FANCYTIMER_ETIME_SetMaximumPeriodicity(GPTM_MODULE module, GPTM_EDGE edge, void (*function) (void));
uint8_t FANCYTIMER_PWM_SetDutyCycle(GPTM_MODULE module, float freq_KHz, uint8_t duty, GPTM_CCP ccp, void (*function) (void));

void TIMER_Start(GPTM_MODULE module);
void TIMER_Stop(GPTM_MODULE module);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Initialize the timer module with the specified mode, direction of counting and debugging stalling feature
void TIMER_Init(GPTM_MODULE module, GPTM_MODE mode, GPTM_DIRECTION direction, GPTM_STALL stall);

//Enable the interrupt source from the 5 options and set ISR pointer
void TIMER_EnableAndSetIntISR(GPTM_MODULE module, GPTM_INT interrupt_ID, void (*function) (void));

//Enable and Disable WOT bit
void TIMER_EnableWaitForTrigger(GPTM_MODULE module);
void TIMER_DisableWaitForTrigger(GPTM_MODULE module);

//Edge Detection Type
void TIMER_ConfigEdgeDetection(GPTM_MODULE module, GPTM_EDGE edge);

//CCP Configuration : Inverted or not
void TIMER_ConfigCCP(GPTM_MODULE module, GPTM_CCP ccp);

//Enable or Disable the snapshot feature
void TIMER_EnableSnapshot(GPTM_MODULE module);
void TIMER_DisableSnapshot(GPTM_MODULE module);

//Set the interval load and prescaler values
void TIMER_SetIntervalLoad(GPTM_MODULE module, uint32_t value);
void TIMER_SetPrescalar(GPTM_MODULE module, uint16_t value);

//Determine the update behaviour of ILD, PR and MATCH registers (combination of regs are allowed)
void TIMER_SetUpdateTime(GPTM_REGS regs, GPTM_UPDATE update);

//Set the values of the match register and the prescaler match register
void TIMER_SetMatch(GPTM_MODULE module, uint32_t value);
void TIMER_SetPrescalerMatch(GPTM_MODULE module, uint16_t value);

//It will return the snapshot of the timer and optionally Prescaler snapshot if it is used
uint32_t TIMER_GetSnapshot(GPTM_MODULE module);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TIMER0A_ISR(void);
void TIMER0B_ISR(void);
void TIMER2A_ISR(void);
void TIMER2B_ISR(void);
void TIMER5A_ISR(void);
void TIMER5B_ISR(void);


#endif /* GPTM_H_ */
