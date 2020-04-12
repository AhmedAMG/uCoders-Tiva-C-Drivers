/*
 * GPTM.c
 *
 *  Created on: Feb 28, 2020
 *      Author: DELL
 */

#include "Std_types.h"
#include "Registers.h"
#include "NVIC.h"
#include "GPTM.h"

static void (*(TIMER0A_interrupt_sources[5])) (void);
static void (*(TIMER0B_interrupt_sources[5])) (void);
static void (*(TIMER1A_interrupt_sources[5])) (void);
static void (*(TIMER1B_interrupt_sources[5])) (void);
static void (*(TIMER2A_interrupt_sources[5])) (void);
static void (*(TIMER2B_interrupt_sources[5])) (void);
static void (*(TIMER3A_interrupt_sources[5])) (void);
static void (*(TIMER3B_interrupt_sources[5])) (void);
static void (*(TIMER4A_interrupt_sources[5])) (void);
static void (*(TIMER4B_interrupt_sources[5])) (void);
static void (*(TIMER5A_interrupt_sources[5])) (void);
static void (*(TIMER5B_interrupt_sources[5])) (void);

typedef void (*(fptr[5])) (void);
static fptr all_ptrs[24] = {TIMER0A_interrupt_sources,TIMER0B_interrupt_sources,
                            TIMER1A_interrupt_sources,TIMER1B_interrupt_sources,
                            TIMER2A_interrupt_sources,TIMER2B_interrupt_sources,
                            TIMER3A_interrupt_sources,TIMER3B_interrupt_sources,
                            TIMER4A_interrupt_sources,TIMER4B_interrupt_sources,
                            TIMER5A_interrupt_sources,TIMER5B_interrupt_sources
};

static uint8_t TIMER_NVICVectors[24] = {19,20,21,22,23,24,35,36,70,71,92,93};

//Initialize the timer module with the specified mode, direction of counting and debugging stalling feature
void TIMER_Init(GPTM_MODULE module, GPTM_MODE mode, GPTM_DIRECTION direction, GPTM_STALL stall){
    uint8_t AB = 0; //A=0,B=4
    uint8_t concatenation = 0;
    //Timer number and concatenated or individual/split modes
    if((module & 0x03) == 0){
        //Concatenated mode
        AB = 0;
        concatenation = 1;
    }else if((module & 0x01) == 0x01){
        //Timer A
        AB = 0;
        module = module - 1;
    }else if((module & 0x02) == 0x02){
        //Timer B
        AB = 4;
        module = module - 2;
    }else return;

    //Provide the clock to the specified timer
    SYSCTL_RCGCTIMER_R |= (1<<((module&(0xF<<0xC))>>0xC));

    volatile uint32_t *reg_cfg = (volatile uint32_t *)(module + (TIMERCFG));
    volatile uint32_t *reg_mr = (volatile uint32_t *)(module + (TIMERTAMR) + AB); //B is always the following register after A
    volatile uint32_t *reg_ctl = (volatile uint32_t *)(module + (TIMERCTL));
    uint32_t data_cfg = *reg_cfg;
    uint32_t data_mr = *reg_mr;
    uint32_t data_ctl = *reg_ctl;

    //Disable the timer
    data_ctl &= ~(1<<2*AB);
    *reg_ctl = data_ctl;

    //Timer mode: Concatenation or split/individual
    data_cfg &= ~(7<<0);
    data_cfg |= concatenation ? (0<<0) : (0x04<<0);
    *reg_cfg = data_cfg;

    //Timer mode:one-shot or periodic ...
    data_mr &= ~(3<<0);
    data_mr |= mode < 4 ? (mode<<0) : ((mode^0x07)<<0);

    //Timer mode: event count or event time or pwm
    data_mr &= ~(3<<2);
    if(mode == TIMER_MODE_PWM) data_mr |= (1<<3);
    else if(mode == TIMER_MODE_INPUT_EDGE_TIME) data_mr |= (1<<2);

    //Timer direction
    data_mr &= ~(1<<4);
    data_mr |= (direction<<4);
    *reg_mr = data_mr;

    //Timer Stall
   data_ctl &= ~(1<<(2*AB+1));
   data_ctl |= (stall<<(2*AB+1));
   *reg_ctl = data_ctl;
}

//Enable the interrupt source from the 5 options and set ISR pointer
void TIMER_EnableAndSetIntISR(GPTM_MODULE module, GPTM_INT interrupt_ID, void (*function) (void)){
    uint8_t AB = 0; //A=0,B=4
    uint8_t module_index = 0;
    //Timer number and concatenated or individual/split modes
    if((module & 0x03) == 0){
        //Concatenated mode
        AB = 0;
    }else if((module & 0x01) == 0x01){
        //Timer A
        AB = 0;
        module = module - 1;
    }else if((module & 0x02) == 0x02){
        //Timer B
        AB = 4;
        module = module - 2;
    }else return;

    volatile uint32_t *reg_im = (volatile uint32_t *)(module + (TIMERIMR));
    volatile uint32_t *reg_mr = (volatile uint32_t *)(module + (TIMERTAMR) + AB); //B is always the following register after A

    if(interrupt_ID == INT_NON_CAPTURE_MATCH) *reg_mr = *reg_mr | (1<<5);
    else if(interrupt_ID == INT_CAPTURE_EVENT && (*reg_mr&(1<<3))) *reg_mr = *reg_mr | (1<<9);

    if(AB == 4 && interrupt_ID == 4) interrupt_ID = 3;
    else if(AB == 4 && interrupt_ID == 3) return;
    module_index = ((module&(0xF<<0xC))>>0xC) * 2 + (uint8_t)(AB*0.25);

    //TIMER0A_interrupt_sources[interrupt_ID] = function;
    //GENERAL INTERRUPT 2D ARRAY
    all_ptrs[module_index][interrupt_ID] = function;

    *reg_im = *reg_im | (1<<(interrupt_ID + (2*AB)));

    //Enable timer interrupt from the NVIC
    NVIC_Enable(TIMER_NVICVectors[module_index]);
}

//Disable the interrupt source from the 5 options
void TIMER_DisableInterrupt(GPTM_MODULE module, GPTM_INT interrupt_ID){
    uint8_t AB = 0; //A=0,B=4
    //Timer number and concatenated or individual/split modes
    if((module & 0x03) == 0){
        //Concatenated mode
        AB = 0;
    }else if((module & 0x01) == 0x01){
        //Timer A
        AB = 0;
        module = module - 1;
    }else if((module & 0x02) == 0x02){
        //Timer B
        AB = 4;
        module = module - 2;
    }else return;

    volatile uint32_t *reg_im = (volatile uint32_t *)(module + (TIMERIMR));
    uint32_t data_im = *reg_im;

    if(AB == 4 && interrupt_ID == 4) interrupt_ID = 3;
    else if(AB == 4 && interrupt_ID == 3) return;
    data_im &= ~(1<<(interrupt_ID + (2*AB)));
    *reg_im = data_im;

    //Disable timer interrupt from the NVIC
    NVIC_Disable(TIMER_NVICVectors[((module&(0xF<<0xC))>>0xC) * 2 + (uint8_t)(AB*0.25)]);
}

//Set the require time for the timeout and match timer events
uint8_t FANCYTIMER_ONESHOT_SetTimeoutSeconds(GPTM_MODULE module, uint32_t msec, void (*function) (void)){
    float finterval_load = (1.0/(float)(SYSCLK));
    finterval_load = (float)((float)msec/1000.0) / (float)finterval_load;
    uint32_t interval_load = (uint32_t)finterval_load;

    if(interval_load > 0xFFFF){
        if((module & 0x03) != 0){return 0;}
    }

    TIMER_Init(module, TIMER_MODE_ONE_SHOT, COUNT_UP, STALL_ENABLE);

    TIMER_SetIntervalLoad(module, (uint32_t)interval_load);
    TIMER_EnableAndSetIntISR(module,INT_TIMEOUT,function);
    return 1;
}

uint8_t FANCYTIMER_ONESHOT_SetMatchSeconds(GPTM_MODULE module, uint32_t msec, void (*function) (void)){
    float fmatch = (1.0/(float)(SYSCLK));
    fmatch = (float)((float)msec/1000.0) / (float)fmatch;
    uint32_t match = (uint32_t)fmatch;

    if(match > 0xFFFF){
        if((module & 0x03) != 0){return 0;}
    }

    TIMER_Init(module, TIMER_MODE_ONE_SHOT, COUNT_UP, STALL_ENABLE);

    TIMER_SetMatch(module, (uint32_t)match);
    TIMER_EnableAndSetIntISR(module,INT_NON_CAPTURE_MATCH,function);
    return 1;
}

uint8_t FANCYTIMER_PERIODIC_SetTimeoutSeconds(GPTM_MODULE module, uint32_t msec, void (*function) (void)){
    float finterval_load = (1.0/(float)(SYSCLK));
    finterval_load = (float)((float)msec/1000.0) / (float)finterval_load;
    uint32_t interval_load = (uint32_t)finterval_load;

    if(interval_load > 0xFFFF){
        if((module & 0x03) != 0){return 0;}
    }

    TIMER_Init(module, TIMER_MODE_PERIODIC, COUNT_UP, STALL_ENABLE);

    TIMER_SetIntervalLoad(module, (uint32_t)interval_load);
    TIMER_EnableAndSetIntISR(module,INT_TIMEOUT,function);
    return 1;
}

uint8_t FANCYTIMER_PERIODIC_SetMatchSeconds(GPTM_MODULE module, uint32_t msec, void (*function) (void)){
    float fmatch = (1.0/(float)(SYSCLK));
    fmatch = (float)((float)msec/1000.0) / (float)fmatch;
    uint32_t match = (uint32_t)fmatch;

    if(match > 0xFFFF){
        if((module & 0x03) != 0){return 0;}
    }

    TIMER_Init(module, TIMER_MODE_PERIODIC, COUNT_UP, STALL_ENABLE);

    TIMER_SetMatch(module, (uint32_t)match);
    TIMER_EnableAndSetIntISR(module,INT_NON_CAPTURE_MATCH,function);
    return 1;
}

uint8_t FANCYTIMER_ECOUNT_SetEventsNumber(GPTM_MODULE module, uint32_t events, GPTM_EDGE edge, GPTM_CONTINUITY continuity, void (*function) (void)){
    if((module & 0x03) == 0){return 0;}

    if(continuity == EDGECOUNT_CONTINUOUS){
        TIMER_Init(module, TIMER_MODE_INPUT_EDGE_COUNT, COUNT_UP, STALL_ENABLE);
        TIMER_SetIntervalLoad(module, 0x0000FFFF);
        TIMER_SetMatch(module, events);
        TIMER_SetPrescalar(module, 0xFF);
        if(events > 0xFFFF) TIMER_SetPrescalerMatch(module, events>>16);
    }else{
        TIMER_Init(module, TIMER_MODE_INPUT_EDGE_COUNT, COUNT_DOWN, STALL_ENABLE);
        TIMER_SetIntervalLoad(module, 0x0000FFFF);
        TIMER_SetMatch(module, 0x0000FFFF - events);
        if(events > 0xFFFF){
            TIMER_SetPrescalar(module, 0xFF);
            TIMER_SetPrescalerMatch(module,0xFF - (events>>16));
        }

    }

    TIMER_ConfigEdgeDetection(module,edge);
    TIMER_EnableAndSetIntISR(module,INT_CAPTURE_MATCH,function);
    TIMER_EnableSnapshot(module);
    return 1;
}

uint8_t FANCYTIMER_ETIME_SetMaximumPeriodicity(GPTM_MODULE module, GPTM_EDGE edge, void (*function) (void)){
    if((module & 0x03) == 0){return 0;}

    TIMER_Init(module, TIMER_MODE_INPUT_EDGE_TIME, COUNT_UP, STALL_ENABLE);
    TIMER_ConfigEdgeDetection(module,edge);
    TIMER_SetIntervalLoad(module, 0x0000FFFF);
    TIMER_SetPrescalar(module, 0xFF);
    TIMER_EnableAndSetIntISR(module,INT_CAPTURE_EVENT,function);
    TIMER_EnableSnapshot(module);
    return 1;

}

uint8_t FANCYTIMER_PWM_SetDutyCycle(GPTM_MODULE module, float freq_Hz, uint8_t duty, GPTM_CCP ccp, void (*function) (void)){
    if((module & 0x03) == 0) return 0;
    uint32_t i = 0;
    float fsystime = (1.0/(float)(SYSCLK));
    float fperiod = (1.0/(freq_Hz));
    i = fperiod / fsystime;
    if(i > 0x00FFFFFF) return 0;

    TIMER_Init(module, TIMER_MODE_PWM, COUNT_DOWN, STALL_ENABLE);
    TIMER_SetIntervalLoad(module, i);
    TIMER_SetPrescalar(module, i>>16);
    i = (i*(duty/100.0));
    TIMER_SetMatch(module, i);
    TIMER_SetPrescalerMatch(module, i>>16);

    TIMER_ConfigCCP(module,ccp);

    if(function != 0) TIMER_EnableAndSetIntISR(module,INT_CAPTURE_EVENT,function);
    TIMER_EnableSnapshot(module);
    return 1;
}

void TIMER_EnableWaitForTrigger(GPTM_MODULE module){
    uint8_t AB = 0; //A=0,B=4
    //Timer number and concatenated or individual/split modes
    if((module & 0x03) == 0){
        //Concatenated mode
        AB = 0;
    }else if((module & 0x01) == 0x01){
        //Timer A
        AB = 0;
        module = module - 1;
    }else if((module & 0x02) == 0x02){
        //Timer B
        AB = 4;
        module = module - 2;
    }else return;

    volatile uint32_t *reg_mr = (volatile uint32_t *)(module + (TIMERTAMR) + AB); //B is always the following register after A
    *reg_mr = *reg_mr | (1<<6);
}

void TIMER_DisableWaitForTrigger(GPTM_MODULE module){
    uint8_t AB = 0; //A=0,B=4
    //Timer number and concatenated or individual/split modes
    if((module & 0x03) == 0){
        //Concatenated mode
        AB = 0;
    }else if((module & 0x01) == 0x01){
        //Timer A
        AB = 0;
        module = module - 1;
    }else if((module & 0x02) == 0x02){
        //Timer B
        AB = 4;
        module = module - 2;
    }else return;

    volatile uint32_t *reg_mr = (volatile uint32_t *)(module + (TIMERTAMR) + AB); //B is always the following register after A
    *reg_mr = *reg_mr & (~(1<<6));
}

void TIMER_ConfigEdgeDetection(GPTM_MODULE module, GPTM_EDGE edge){
    uint8_t AB = 0; //A=0,B=4
    //Timer number and concatenated or individual/split modes
    if((module & 0x03) == 0){
        //Concatenated mode
        AB = 0;
    }else if((module & 0x01) == 0x01){
        //Timer A
        AB = 0;
        module = module - 1;
    }else if((module & 0x02) == 0x02){
        //Timer B
        AB = 4;
        module = module - 2;
    }else return;

    volatile uint32_t *reg_ctl = (volatile uint32_t *)(module + (TIMERCTL));
    *reg_ctl = *reg_ctl & ~(3<<(2+2*AB));
    *reg_ctl = *reg_ctl | (edge<<(2+2*AB));
}

void TIMER_ConfigCCP(GPTM_MODULE module, GPTM_CCP ccp){
    uint8_t AB = 0; //A=0,B=4
    //Timer number and concatenated or individual/split modes
    if((module & 0x03) == 0){
        //Concatenated mode
        AB = 0;
    }else if((module & 0x01) == 0x01){
        //Timer A
        AB = 0;
        module = module - 1;
    }else if((module & 0x02) == 0x02){
        //Timer B
        AB = 4;
        module = module - 2;
    }else return;

    volatile uint32_t *reg_ctl = (volatile uint32_t *)(module + (TIMERCTL));
    *reg_ctl = *reg_ctl & ~(1<<(6+2*AB));
    *reg_ctl = *reg_ctl | (ccp<<(6+2*AB));
}

//Start the timer and begin counting
void TIMER_Start(GPTM_MODULE module){
    uint8_t AB = 0; //A=0,B=4
    //Timer number and concatenated or individual/split modes
    if((module & 0x03) == 0){
        //Concatenated mode
        AB = 0;
    }else if((module & 0x01) == 0x01){
        //Timer A
        AB = 0;
        module = module - 1;
    }else if((module & 0x02) == 0x02){
        //Timer B
        AB = 4;
        module = module - 2;
    }else return;

    volatile uint32_t *reg_ctl = (volatile uint32_t *)(module + (TIMERCTL));
    uint32_t data_ctl = *reg_ctl;

    //Enable the timer
    data_ctl |= (1<<2*AB);
    *reg_ctl = data_ctl;
}

//Stop the timer
void TIMER_Stop(GPTM_MODULE module){
    uint8_t AB = 0; //A=0,B=4
    //Timer number and concatenated or individual/split modes
    if((module & 0x03) == 0){
        //Concatenated mode
        AB = 0;
    }else if((module & 0x01) == 0x01){
        //Timer A
        AB = 0;
        module = module - 1;
    }else if((module & 0x02) == 0x02){
        //Timer B
        AB = 4;
        module = module - 2;
    }else return;

    volatile uint32_t *reg_ctl = (volatile uint32_t *)(module + (TIMERCTL));
    uint32_t data_ctl = *reg_ctl;

    //Disable the timer
    data_ctl &= ~(1<<2*AB);
    *reg_ctl = data_ctl;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Enable or Disable the snapshot feature
void TIMER_EnableSnapshot(GPTM_MODULE module){
    uint8_t AB = 0; //A=0,B=4
    //Timer number and concatenated or individual/split modes
    if((module & 0x03) == 0){
        //Concatenated mode
        AB = 0;
    }else if((module & 0x01) == 0x01){
        //Timer A
        AB = 0;
        module = module - 1;
    }else if((module & 0x02) == 0x02){
        //Timer B
        AB = 4;
        module = module - 2;
    }else return;

    volatile uint32_t *reg_mr = (volatile uint32_t *)(module + (TIMERTAMR) + AB); //B is always the following register after A
    *reg_mr = *reg_mr | (1<<7);
}

void TIMER_DisableSnapshot(GPTM_MODULE module){
    uint8_t AB = 0; //A=0,B=4
    //Timer number and concatenated or individual/split modes
    if((module & 0x03) == 0){
        //Concatenated mode
        AB = 0;
    }else if((module & 0x01) == 0x01){
        //Timer A
        AB = 0;
        module = module - 1;
    }else if((module & 0x02) == 0x02){
        //Timer B
        AB = 4;
        module = module - 2;
    }else return;

    volatile uint32_t *reg_mr = (volatile uint32_t *)(module + (TIMERTAMR) + AB); //B is always the following register after A
    *reg_mr = *reg_mr & (~(1<<7));
}


//Set the interval load and prescaler values
void TIMER_SetIntervalLoad(GPTM_MODULE module, uint32_t value){
    uint8_t AB = 0; //A=0,B=4
    //Timer number and concatenated or individual/split modes
    if((module & 0x03) == 0){
        //Concatenated mode
        AB = 0;
    }else if((module & 0x01) == 0x01){
        //Timer A
        AB = 0;
        module = module - 1;
    }else if((module & 0x02) == 0x02){
        //Timer B
        AB = 4;
        module = module - 2;
    }else return;

    volatile uint32_t *reg_ilr = (volatile uint32_t *)(module + (TIMERTAILR) + AB); //B is always the following register after A
    *reg_ilr = value;
}

void TIMER_SetPrescalar(GPTM_MODULE module, uint16_t value){
    uint8_t AB = 0; //A=0,B=4
    //Timer number and concatenated or individual/split modes
    if((module & 0x03) == 0){
        //Concatenated mode
        AB = 0;
    }else if((module & 0x01) == 0x01){
        //Timer A
        AB = 0;
        module = module - 1;
    }else if((module & 0x02) == 0x02){
        //Timer B
        AB = 4;
        module = module - 2;
    }else return;

    volatile uint32_t *reg_pr = (volatile uint32_t *)(module + (TIMERTAPR) + AB); //B is always the following register after A
    *reg_pr = value;
}


//Determine the update behaviour of ILD, PR and MATCH registers (combination of regs are allowed)
void TIMER_SetUpdateTime(GPTM_REGS regs, GPTM_UPDATE update){

}

//Set the values of the match register and the prescaler match register
void TIMER_SetMatch(GPTM_MODULE module, uint32_t value){
    uint8_t AB = 0; //A=0,B=4
    //Timer number and concatenated or individual/split modes
    if((module & 0x03) == 0){
        //Concatenated mode
        AB = 0;
    }else if((module & 0x01) == 0x01){
        //Timer A
        AB = 0;
        module = module - 1;
    }else if((module & 0x02) == 0x02){
        //Timer B
        AB = 4;
        module = module - 2;
    }else return;

    volatile uint32_t *reg_matchr = (volatile uint32_t *)(module + (TIMERTAMATCHR) + AB); //B is always the following register after A
    *reg_matchr = value;
}

void TIMER_SetPrescalerMatch(GPTM_MODULE module, uint16_t value){
    uint8_t AB = 0; //A=0,B=4
    //Timer number and concatenated or individual/split modes
    if((module & 0x03) == 0){
        //Concatenated mode
        AB = 0;
    }else if((module & 0x01) == 0x01){
        //Timer A
        AB = 0;
        module = module - 1;
    }else if((module & 0x02) == 0x02){
        //Timer B
        AB = 4;
        module = module - 2;
    }else return;

    volatile uint32_t *reg_pmr = (volatile uint32_t *)(module + (TIMERTAPMR) + AB); //B is always the following register after A
    *reg_pmr = value;
}

//It will return the snapshot of the timer and optionally Prescaler snapshot if it is used
uint32_t TIMER_GetSnapshot(GPTM_MODULE module){
    uint8_t AB = 0; //A=0,B=4
    //Timer number and concatenated or individual/split modes
    if((module & 0x03) == 0){
        //Concatenated mode
        AB = 0;
    }else if((module & 0x01) == 0x01){
        //Timer A
        AB = 0;
        module = module - 1;
    }else if((module & 0x02) == 0x02){
        //Timer B
        AB = 4;
        module = module - 2;
    }else return;

    volatile uint32_t *reg_mr = (volatile uint32_t *)(module + (TIMERTAMR) + AB); //B is always the following register after A
    volatile uint32_t *reg_r = (volatile uint32_t *)(module + (TIMERTAR) + AB); //B is always the following register after A
    volatile uint32_t *reg_ps = (volatile uint32_t *)(module + (TIMERTAPS) + AB); //B is always the following register after A
    uint32_t snapshot = 0;

    //Prescaler is always a time extension HERE
    //MYSTERY: PRESCALER SNAPSHOT IS ZERO ALL THE TIME IN ALL MODES
    //But the function is working well as reg_r has the whole value with the value the prescaler snapshot should have
    snapshot = (*reg_r);
    snapshot = snapshot | ((*reg_ps)<<16);
    return snapshot;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TIMER0A_ISR(void){
    uint16_t i = 1<<0;
    uint8_t bit_position = 0;
    volatile uint32_t *reg_icr = (volatile uint32_t *)(TIM0_AB + (TIMERICR));
    volatile uint32_t *reg_mis = (volatile uint32_t *)(TIM0_AB + (TIMERMIS));

    while(((*reg_mis) & i) == 0){
        i = i << 1;
        bit_position++;
        if(i>(1<<4)) return;
    }

    //TIMER0A_interrupt_sources[bit_position]();
    all_ptrs[0][bit_position]();

    *(reg_icr) = (*(reg_icr)) | (1<<bit_position);
}

void TIMER0B_ISR(void){
    uint16_t i = 1<<8;
    uint8_t bit_position = 0;
    volatile uint32_t *reg_icr = (volatile uint32_t *)(TIM0_AB + (TIMERICR));
    volatile uint32_t *reg_mis = (volatile uint32_t *)(TIM0_AB + (TIMERMIS));

    while(((*reg_mis) & i) == 0){
        i = i << 1;
        bit_position++;
        if(i>(1<<11)) return;
    }

    //TIMER0B_interrupt_sources[bit_position]();
    all_ptrs[1][bit_position]();

    *(reg_icr) = (*(reg_icr)) | (1<<(bit_position+8));
}

void TIMER2A_ISR(void){
    uint16_t i = 1<<0;
    uint8_t bit_position = 0;
    volatile uint32_t *reg_icr = (volatile uint32_t *)(TIM2_AB + (TIMERICR));
    volatile uint32_t *reg_mis = (volatile uint32_t *)(TIM2_AB + (TIMERMIS));

    while(((*reg_mis) & i) == 0){
        i = i << 1;
        bit_position++;
        if(i>(1<<4)) return;
    }

    //TIMER0A_interrupt_sources[bit_position]();
    all_ptrs[4][bit_position]();

    *(reg_icr) = (*(reg_icr)) | (1<<bit_position);
}

void TIMER2B_ISR(void){
    uint16_t i = 1<<8;
    uint8_t bit_position = 0;
    volatile uint32_t *reg_icr = (volatile uint32_t *)(TIM2_AB + (TIMERICR));
    volatile uint32_t *reg_mis = (volatile uint32_t *)(TIM2_AB + (TIMERMIS));

    while(((*reg_mis) & i) == 0){
        i = i << 1;
        bit_position++;
        if(i>(1<<11)) return;
    }

    //TIMER0B_interrupt_sources[bit_position]();
    all_ptrs[5][bit_position]();

    *(reg_icr) = (*(reg_icr)) | (1<<(bit_position+8));
}

void TIMER5A_ISR(void){
    uint16_t i = 1<<0;
    uint8_t bit_position = 0;
    volatile uint32_t *reg_icr = (volatile uint32_t *)(TIM5_AB + (TIMERICR));
    volatile uint32_t *reg_mis = (volatile uint32_t *)(TIM5_AB + (TIMERMIS));

    while(((*reg_mis) & i) == 0){
        i = i << 1;
        bit_position++;
        if(i>(1<<4)) return;
    }

    //TIMER0A_interrupt_sources[bit_position]();
    all_ptrs[10][bit_position]();

    *(reg_icr) = (*(reg_icr)) | (1<<bit_position);
}

void TIMER5B_ISR(void){
    uint16_t i = 1<<8;
    uint8_t bit_position = 0;
    volatile uint32_t *reg_icr = (volatile uint32_t *)(TIM5_AB + (TIMERICR));
    volatile uint32_t *reg_mis = (volatile uint32_t *)(TIM5_AB + (TIMERMIS));

    while(((*reg_mis) & i) == 0){
        i = i << 1;
        bit_position++;
        if(i>(1<<11)) return;
    }

    //TIMER0B_interrupt_sources[bit_position]();
    all_ptrs[11][bit_position]();

    *(reg_icr) = (*(reg_icr)) | (1<<(bit_position+8));
}
