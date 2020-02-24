/*
 * GPIO.c
 *
 *  Created on: Feb 12, 2020
 *      Author: DELL
 */

#include "Std_types.h"
#include "BitOperations.h"
#include "Registers.h"
#include "GPIO.h"

static GPIO_BUS BusSelection[6] = {APB};
static void (*(interrupt_bits[8])) (void);
extern void PORTF_BIT4_Handler(void);

void GPIOBusSet(GPIO_PORT port, GPIO_BUS bus){
    if(bus != APB && bus != AHB) return;

    /*
    PORTA = 0x40004000 -> 0100 0000 0000 0000 0100 0000 0000 0000
    PORTB = 0x40005000 -> 0100 0000 0000 0000 0101 0000 0000 0000
    PORTC = 0x40006000 -> 0100 0000 0000 0000 0110 0000 0000 0000
    PORTD = 0x40007000 -> 0100 0000 0000 0000 0111 0000 0000 0000
    PORTE = 0x40024000 -> 0100 0000 0000 0010 0100 0000 0000 0000
    PORTF = 0x40025000 -> 0100 0000 0000 0010 0101 0000 0000 0000
     */
    uint8_t port_index = (((port >> 12) & 0xFF) - 4);
    if(port_index >= 32) port_index = port_index - 28;

    //Use APB Bus or AHB Bus
    bus == AHB? SetBit(SYSCTL_GPIOHBCTL_R,port_index) : ClearBit(SYSCTL_GPIOHBCTL_R,port_index);

    //Update the used bus for this particular port
    BusSelection[port_index] = bus;

}

GPIO_BUS GPIOBusGet(GPIO_PORT port){
    uint8_t port_index = (((port >> 12) & 0xFF) - 4);
    if(port_index >= 32) port_index = port_index - 28;
    return BusSelection[port_index];
}

void GPIODirectionModeSet(GPIO_PORT port, uint8_t pins, GPIO_MODE mode){

    uint8_t port_index = (((port >> 12) & 0xFF) - 4);
    if(port_index >= 32) port_index = port_index - 28;

    volatile uint32_t *reg_afsel = port + GPIOAFSEL;
    reg_afsel = port_index < 4? reg_afsel + BusSelection[port_index]*0x15000 : reg_afsel + BusSelection[port_index]*0xE000;
    uint32_t data_afsel = *reg_afsel;
    data_afsel &= ~(pins);

    volatile uint32_t *reg_dir = port + GPIODIR;
    reg_dir = port_index < 4? reg_dir + BusSelection[port_index]*0x15000 : reg_dir + BusSelection[port_index]*0xE000;
    uint32_t data_dir = *reg_dir;
    data_dir &= ~(pins);

    switch(mode){
    case MODE_AF:
        data_afsel |= (0xFF & pins);
        break;
    case MODE_OUT:
        data_dir |= (0xFF & pins);
        break;
    case MODE_IN:
        break;
    }

    *reg_afsel = data_afsel;
    *reg_dir = data_dir;
}

uint8_t GPIODirectionGet(GPIO_PORT port, uint8_t pins){
    uint8_t port_index = (((port >> 12) & 0xFF) - 4);
    if(port_index >= 32) port_index = port_index - 28;

    volatile uint32_t *reg_dir = port + GPIODIR;
    reg_dir = port_index < 4? reg_dir + BusSelection[port_index]*0x15000 : reg_dir + BusSelection[port_index]*0xE000;
    uint32_t data_dir = *reg_dir;
    data_dir = (uint8_t)(data_dir & (pins));
    return data_dir;
}

uint8_t GPIOModeGet(GPIO_PORT port, uint8_t pins){
    uint8_t port_index = (((port >> 12) & 0xFF) - 4);
    if(port_index >= 32) port_index = port_index - 28;

    volatile uint32_t *reg_afsel = port + GPIOAFSEL;
    reg_afsel = port_index < 4? reg_afsel + BusSelection[port_index]*0x15000 : reg_afsel + BusSelection[port_index]*0xE000;
    uint32_t data_afsel = *reg_afsel;
    data_afsel = (uint8_t)(data_afsel & (pins));
    return data_afsel;
}

void GPIOClockSet(GPIO_PORT port){
    uint8_t port_index = (((port >> 12) & 0xFF) - 4);
    if(port_index >= 32) port_index = port_index - 28;

    //Provide Clock to the GPIO Port
    SetBit(SYSCTL_RCGCGPIO_R,port_index);
}

void GPIOClockReset(GPIO_PORT port){
    uint8_t port_index = (((port >> 12) & 0xFF) - 4);
    if(port_index >= 32) port_index = port_index - 28;

    //Provide Clock to the GPIO Port
    ClearBit(SYSCTL_RCGCGPIO_R,port_index);
}

uint8_t GPIOClockGet(GPIO_PORT port){
    uint8_t port_index = (((port >> 12) & 0xFF) - 4);
    if(port_index >= 32) port_index = port_index - 28;

    //Provide Clock to the GPIO Port
    return GetBit(SYSCTL_RCGCGPIO_R,port_index);
}

void GPIO_PadSet(GPIO_PORT port, uint8_t pins, GPIO_CURRENT strength, GPIO_PAD pad){
    uint8_t port_index = (((port >> 12) & 0xFF) - 4);
    if(port_index >= 32) port_index = port_index - 28;

    volatile uint32_t *reg_strength = port + (GPIODR2R) + 4*(strength);
    volatile uint32_t *reg_pad = port + (GPIOODR) + 4*(pad);
    volatile uint32_t *reg_den = port + (GPIODEN);

    reg_strength = port_index < 4? reg_strength + BusSelection[port_index]*0x15000 : reg_strength + BusSelection[port_index]*0xE000;
    uint32_t data_strength = *reg_strength;
    data_strength &= ~(pins);
    data_strength |= (0xFF & pins);
    *reg_strength = data_strength;

    if(pad != PAD_NPU_NPD){
        reg_pad = port_index < 4? reg_pad + BusSelection[port_index]*0x15000 : reg_pad + BusSelection[port_index]*0xE000;
        uint32_t data_pad = *reg_pad;
        data_pad &= ~(pins);
        data_pad |= (0xFF & pins);
        *reg_pad = data_pad;
    }


    reg_den = port_index < 4? reg_den + BusSelection[port_index]*0x15000 : reg_den + BusSelection[port_index]*0xE000;
    uint32_t data_den = *reg_den;
    data_den &= ~(pins);
    data_den |= (0xFF & pins);
    *reg_den = data_den;
}

uint8_t GPIORead(GPIO_PORT port, uint8_t pins){
    uint8_t port_index = (((port >> 12) & 0xFF) - 4);
    if(port_index >= 32) port_index = port_index - 28;

    volatile uint32_t *reg_data = port + (GPIODATA) ;
    reg_data = port_index < 4? reg_data + BusSelection[port_index]*0x15000 : reg_data + BusSelection[port_index]*0xE000;
    reg_data += ((uint32_t)pins); //Shifting by 2 is done automatically because it is pointer to 32bits
    return *reg_data;
}

void GPIOWrite(GPIO_PORT port, uint8_t pins, uint8_t data){
    uint8_t port_index = (((port >> 12) & 0xFF) - 4);
    if(port_index >= 32) port_index = port_index - 28;

    volatile uint32_t *reg_data = port + (GPIODATA) ;
    reg_data = port_index < 4? reg_data + BusSelection[port_index]*0x15000 : reg_data + BusSelection[port_index]*0xE000;
    reg_data += ((uint32_t)pins); //Shifting by 2 is done automatically because it is pointer to 32bits
    *reg_data = (pins & data);
}

void GPIO_ConfigInterrupt(GPIO_PORT port, uint8_t pins, GPIO_SENSE sense, GPIO_SPECS specs){
    uint8_t port_index = (((port >> 12) & 0xFF) - 4);
    if(port_index >= 32) port_index = port_index - 28;

    volatile uint32_t *reg_en0 = CorePeripheralBase + EN0;

    volatile uint32_t *reg_im = port + GPIOIM;
    volatile uint32_t *reg_is = port + GPIOIS;
    volatile uint32_t *reg_ibe = port + GPIOIBE;
    volatile uint32_t *reg_iev = port + GPIOIEV;
    volatile uint32_t *reg_icr = port + GPIOICR;

    uint32_t data_en0 = 0;
    uint32_t data_im = 0;
    uint32_t data_is = 0;
    uint32_t data_ibe = 0;
    uint32_t data_iev = 0;
    uint32_t data_icr = 0;

    reg_im = port_index < 4? reg_im + BusSelection[port_index]*0x15000 : reg_im + BusSelection[port_index]*0xE000;
    data_im = *reg_im;
    data_im &= ~(pins);
    *reg_im = data_im;

    reg_is = port_index < 4? reg_is + BusSelection[port_index]*0x15000 : reg_is + BusSelection[port_index]*0xE000;
    data_is = *reg_is;
    data_is &= ~(pins);
    //EDGE = 0, Level = 1
    data_is |= sense? 0xFF&pins : 0x00;
    *reg_is = data_is;

    switch(specs){
    case BOTH_EDGES:
        reg_ibe = port_index < 4? reg_ibe + BusSelection[port_index]*0x15000 : reg_ibe + BusSelection[port_index]*0xE000;
        data_ibe = *reg_ibe;
        data_ibe |= pins;
        *reg_ibe = data_ibe;
        break;
    case RISING_EDGE:
    case HIGH_LEVEL:
        reg_iev = port_index < 4? reg_iev + BusSelection[port_index]*0x15000 : reg_iev + BusSelection[port_index]*0xE000;
        data_iev = *reg_iev;
        data_iev |= pins;
        *reg_iev = data_iev;
        break;
    case FALLING_EDGE:
    case LOW_LEVEL:
        reg_iev = port_index < 4? reg_iev + BusSelection[port_index]*0x15000 : reg_iev + BusSelection[port_index]*0xE000;
        data_iev = *reg_iev;
        data_iev &= ~(pins);
        *reg_iev = data_iev;
        break;

    }

    reg_icr = port_index < 4? reg_icr + BusSelection[port_index]*0x15000 : reg_icr + BusSelection[port_index]*0xE000;
    data_icr = *reg_icr;
    data_icr |= pins;
    *reg_icr = data_icr;

    reg_im = port + GPIOIM;
    reg_im = port_index < 4? reg_im + BusSelection[port_index]*0x15000 : reg_im + BusSelection[port_index]*0xE000;
    data_im = *reg_im;
    data_im |= pins;
    *reg_im = data_im;

    data_en0 = *reg_en0;
    data_en0 |= (1<<30);
    *reg_en0 = data_en0;

}


void GPIO_SetIntHandler(uint8_t pin, void (*function) (void)){
    uint8_t temp = 1;
    uint8_t bit_position = 0;
    while(temp != pin){
        temp = temp << 1;
        bit_position++;
    }

    interrupt_bits[bit_position] = function;
}

void GPIOF_ISR(void){

    volatile uint32_t *reg_icr = PORTF + GPIOICR;
    volatile uint32_t *reg_mis = PORTF + GPIOMIS; //USE MIS NOT RIS (interference happens from your finger charge)
    volatile int temp = 0;
    uint8_t i = 1;
    uint16_t count = 0;
    uint8_t bit_position = 0;

    reg_mis = reg_mis + BusSelection[5]*0xE000;
    uint32_t data_mis = *reg_mis;

    while((data_mis & i) == 0){
        i = i << 1;
        bit_position++;
    }

    for(temp = 0; temp < 1000; temp++){
        if((GPIORead(PORTF, (i))&(i)) == 0){
            count++;
        }
    }

    if(count>900){
        interrupt_bits[bit_position]();
    }

    reg_icr = reg_icr + BusSelection[5]*0xE000;
    uint32_t data_icr = *reg_icr;
    data_icr |= (i);
    *reg_icr = data_icr;

}

