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

static uint8_t BusSelection[6] = {GPIO_APB};

/*
 * Function name: GPIO_Init
 * Description: Initialize the required port (A->F) and the required bus to use the port through
 * Input: Port_Number: GPIO_PORTx , x = A -> F
 *        Bus: GPIO_APB or GPIO_AHB
 * Return: None
 * Tested ? Yes and Verified
 */
void GPIO_Init(uint8_t Port_Number,uint8_t Bus){
    if(Bus != GPIO_APB && Bus != GPIO_AHB) return;

    //Use APB Bus or AHB Bus
    if(Bus == GPIO_AHB) SetBit(SYSCTL_GPIOHBCTL_R,Port_Number);
    else if(Bus == GPIO_APB) ClearBit(SYSCTL_GPIOHBCTL_R,Port_Number);

    //Provide Clock to the GPIO Port
    SetBit(SYSCTL_RCGCGPIO_R,Port_Number);

    //Update the used bus for this particular port
    BusSelection[Port_Number] = Bus;

}

/*
 * Function name: GPIO_SetPortDirection
 * Description: Initialize the required port (A->F) direction whether to be input or output
 * Input: Port_Number: GPIO_PORTx , x = A -> F
 *        Direction: GPIO_INPUT or GPIO_OUTPUT
 * Return: None
 * Tested ? Yes and Verified
 */
void GPIO_SetPortDirection(uint8_t Port_Number, uint8_t Direction){
    switch(Port_Number){
    case GPIO_PORTA:
        //BusSelesction = 0(APB) or BusSelesction = 1(AHB)
        //0x15000 = (Base address for PORTA on AHB - Base address for PORTA on APB) / 4
        //0xE000 = (Base address for PORTE on AHB - Base address for PORTE on APB) / 4
        //Division by 4 is done because pointer addition is done by incrementing the address by 4 bytes
        //PORTA addresses are handled by just the base address of APB
        //0x00 for input and 0xFF for output
        if(Direction == GPIO_INPUT) *(GPIO_PORTA_DIR + BusSelection[Port_Number]*0x15000) = 0x00;
        else if(Direction == GPIO_OUTPUT) *(GPIO_PORTA_DIR + BusSelection[Port_Number]*0x15000) = 0xFF;
        else; //ERROR
        *(GPIO_PORTA_DEN + BusSelection[Port_Number]*0x15000) = 0xFF;
        break;
    case GPIO_PORTB:
        if(Direction == GPIO_INPUT) *(GPIO_PORTB_DIR + BusSelection[Port_Number]*0x15000) = 0x00;
        else if(Direction == GPIO_OUTPUT) *(GPIO_PORTB_DIR + BusSelection[Port_Number]*0x15000) = 0xFF;
        else; //ERROR
        *(GPIO_PORTB_DEN + BusSelection[Port_Number]*0x15000) = 0xFF;
        break;
    case GPIO_PORTC:
        if(Direction == GPIO_INPUT) *(GPIO_PORTC_DIR + BusSelection[Port_Number]*0x15000) = 0x00;
        else if(Direction == GPIO_OUTPUT) *(GPIO_PORTC_DIR + BusSelection[Port_Number]*0x15000) = 0xFF;
        else; //ERROR
        *(GPIO_PORTC_DEN + BusSelection[Port_Number]*0x15000) = 0xFF;
        break;
    case GPIO_PORTD:
        if(Direction == GPIO_INPUT) *(GPIO_PORTD_DIR + BusSelection[Port_Number]*0x15000) = 0x00;
        else if(Direction == GPIO_OUTPUT) *(GPIO_PORTD_DIR + BusSelection[Port_Number]*0x15000) = 0xFF;
        else; //ERROR
        *(GPIO_PORTD_DEN + BusSelection[Port_Number]*0x15000) = 0xFF;
        break;
    case GPIO_PORTE:
        if(Direction == GPIO_INPUT) *(GPIO_PORTE_DIR + BusSelection[Port_Number]*0xE000) = 0x00;
        else if(Direction == GPIO_OUTPUT) *(GPIO_PORTE_DIR + BusSelection[Port_Number]*0xE000) = 0xFF;
        else; //ERROR
        *(GPIO_PORTE_DEN + BusSelection[Port_Number]*0xE000) = 0xFF;
        break;
    case GPIO_PORTF:
        if(Direction == GPIO_INPUT) *(GPIO_PORTF_DIR + BusSelection[Port_Number]*0xE000) = 0x00;
        else if(Direction == GPIO_OUTPUT) *(GPIO_PORTF_DIR + BusSelection[Port_Number]*0xE000) = 0xFF;
        else; //ERROR
        *(GPIO_PORTF_DEN + BusSelection[Port_Number]*0xE000) = 0xFF;
        break;
    }
}

/*
 * Function name: GPIO_SetPinDirection
 * Description: Initialize the required pin (GPIO_PINx) direction in any port (A->F) whether to be input or output
 * Input: Port_Number: GPIO_PORTx , x = A -> F
 *        Pin_Number: GPIO_PINx , x = 0 -> 7
 *        Direction: GPIO_INPUT or GPIO_OUTPUT
 * Return: None
 * Tested ? Yes and Verified
 */
void GPIO_SetPinDirection(uint8_t Port_Number, uint8_t Pin_Number, uint8_t Direction){
    switch(Port_Number){
    case GPIO_PORTA:
        //BusSelesction = 0(APB) or BusSelesction = 1(AHB)
        //0x15000 = (Base address for PORTA on AHB - Base address for PORTA on APB) / 4
        //0xE000 = (Base address for PORTE on AHB - Base address for PORTE on APB) / 4
        //Division by 4 is done because pointer addition is done by incrementing the address by 4 bytes
        //PORTA addresses are handled by just the base address of APB
        //0 for input and 1 for output
        if(Direction == GPIO_INPUT) ClearBit(*(GPIO_PORTA_DIR + BusSelection[Port_Number]*0x15000),Pin_Number);
        else if(Direction == GPIO_OUTPUT) SetBit(*(GPIO_PORTA_DIR + BusSelection[Port_Number]*0x15000),Pin_Number);
        else; //ERROR
        //Enable the digital functionality for the pin
        SetBit(*(GPIO_PORTA_DEN + BusSelection[Port_Number]*0x15000),Pin_Number);
        break;
    case GPIO_PORTB:
        if(Direction == GPIO_INPUT) ClearBit(*(GPIO_PORTB_DIR + BusSelection[Port_Number]*0x15000),Pin_Number);
        else if(Direction == GPIO_OUTPUT) SetBit(*(GPIO_PORTB_DIR + BusSelection[Port_Number]*0x15000),Pin_Number);
        else; //ERROR
        SetBit(*(GPIO_PORTB_DEN + BusSelection[Port_Number]*0x15000),Pin_Number);
        break;
    case GPIO_PORTC:
        if(Direction == GPIO_INPUT) ClearBit(*(GPIO_PORTC_DIR + BusSelection[Port_Number]*0x15000),Pin_Number);
        else if(Direction == GPIO_OUTPUT) SetBit(*(GPIO_PORTC_DIR + BusSelection[Port_Number]*0x15000),Pin_Number);
        else; //ERROR
        SetBit(*(GPIO_PORTC_DEN + BusSelection[Port_Number]*0x15000),Pin_Number);
        break;
    case GPIO_PORTD:
        if(Direction == GPIO_INPUT) ClearBit(*(GPIO_PORTD_DIR + BusSelection[Port_Number]*0x15000),Pin_Number);
        else if(Direction == GPIO_OUTPUT) SetBit(*(GPIO_PORTD_DIR + BusSelection[Port_Number]*0x15000),Pin_Number);
        else; //ERROR
        SetBit(*(GPIO_PORTD_DEN + BusSelection[Port_Number]*0x15000),Pin_Number); //15
        break;
    case GPIO_PORTE:
        if(Direction == GPIO_INPUT) ClearBit(*(GPIO_PORTE_DIR + BusSelection[Port_Number]*0xE000),Pin_Number);
        else if(Direction == GPIO_OUTPUT) SetBit(*(GPIO_PORTE_DIR + BusSelection[Port_Number]*0xE000),Pin_Number);
        else; //ERROR
        SetBit(*(GPIO_PORTE_DEN + BusSelection[Port_Number]*0xE000),Pin_Number);
        break;
    case GPIO_PORTF:
        if(Direction == GPIO_INPUT) ClearBit(*(GPIO_PORTF_DIR + BusSelection[Port_Number]*0xE000),Pin_Number);
        else if(Direction == GPIO_OUTPUT) SetBit(*(GPIO_PORTF_DIR + BusSelection[Port_Number]*0xE000),Pin_Number);
        else; //ERROR
        SetBit(*(GPIO_PORTF_DEN + BusSelection[Port_Number]*0xE000),Pin_Number);
        break;
    }
}

/*
 * Function name: GPIO_SetPortValue
 * Description: Write on any port (A->F) and set its values
 * Input: Port_Number: GPIO_PORTx , x = A -> F
 *        Direction: GPIO_LOW or GPIO_HIGH
 * Return: None
 * Tested ? Yes and Verified
 */
void GPIO_SetPortValue(uint8_t Port_Number, uint8_t Value){
    switch(Port_Number){
    case GPIO_PORTA:
        GPIO_PORTA_DATA_R = Value;
        break;
    case GPIO_PORTB:
        GPIO_PORTB_DATA_R = Value;
        break;
    case GPIO_PORTC:
        GPIO_PORTC_DATA_R = Value;
        break;
    case GPIO_PORTD:
        GPIO_PORTD_DATA_R = Value;
        break;
    case GPIO_PORTE:
        GPIO_PORTE_DATA_R = Value;
        break;
    case GPIO_PORTF:
        GPIO_PORTF_DATA_R = Value;
        break;
    }

}

/*
 * Function name: GPIO_SetPinValue
 * Description: Write on any pin (0->7) on any port (A->F) and set its value
 * Input: Port_Number: GPIO_PORTx , x = A -> F
 *        Pin_Number: GPIO_PINx , x = 0 -> 7
 *        Direction: GPIO_LOW or GPIO_HIGH
 * Return: None
 * Tested ? Yes and Verified
 */
void GPIO_SetPinValue(uint8_t Port_Number, uint8_t Pin_Number, uint8_t Value){
    if(Pin_Number > 7) return;
    uint8_t mask = 1, i = 0;
    for(i = 0; i < Pin_Number; i++) mask = mask * 2;

    switch(Port_Number){
    case GPIO_PORTA:
        //BusSelesction = 0(APB) or BusSelesction = 1(AHB)
        //0x15000 = (Base address for PORTA on AHB - Base address for PORTA on APB) / 4
        //0xE000 = (Base address for PORTE on AHB - Base address for PORTE on APB) / 4
        //Division by 4 is done because pointer addition is done by incrementing the address by 4 bytes
        //PORTA addresses are handled by just the base address of APB
        //mask is how many bytes should be incremented to mask the correct bit in the register address in the bit band area
        //mask is calculated to fit the correct mask in the address of DATA register [2:9]
        //mask = 2^Pin_Number
        *( (GPIO_PORTA_DATA_BITS_R + BusSelection[Port_Number]*0x15000) + mask) = Value << Pin_Number;
        break;
    case GPIO_PORTB:
        *( (GPIO_PORTB_DATA_BITS_R + BusSelection[Port_Number]*0x15000) + mask) = Value << Pin_Number;
        break;
    case GPIO_PORTC:
        *( (GPIO_PORTC_DATA_BITS_R + BusSelection[Port_Number]*0x15000) + mask) = Value << Pin_Number;
        break;
    case GPIO_PORTD:
        *( (GPIO_PORTD_DATA_BITS_R + BusSelection[Port_Number]*0x15000) + mask) = Value << Pin_Number;
        break;
    case GPIO_PORTE:
        *( (GPIO_PORTE_DATA_BITS_R + BusSelection[Port_Number]*0xE000) + mask) = Value << Pin_Number;
        break;
    case GPIO_PORTF:
        *( (GPIO_PORTF_DATA_BITS_R + BusSelection[Port_Number]*0xE000) + mask) = Value << Pin_Number;
        break;
    }

}

/*
 * Function name: GPIO_GetPortValue
 * Description: Reads the whole required port (A->F)
 * Input: Port_Number: GPIO_PORTx , x = A -> F
 * Return: Port levels Values
 * Tested ? Yes and Verified
 */
uint8_t GPIO_GetPortValue(uint8_t Port_Number){
    uint8_t retVal = 0;
    switch(Port_Number){
    case GPIO_PORTA:
        retVal = GPIO_PORTA_DATA_R;
        break;
    case GPIO_PORTB:
        retVal = GPIO_PORTB_DATA_R;
        break;
    case GPIO_PORTC:
        retVal = GPIO_PORTC_DATA_R;
        break;
    case GPIO_PORTD:
        retVal = GPIO_PORTD_DATA_R;
        break;
    case GPIO_PORTE:
        retVal = GPIO_PORTE_DATA_R;
        break;
    case GPIO_PORTF:
        retVal = GPIO_PORTF_DATA_R;
        break;
    }
    return retVal;
}

/*
 * Function name: GPIO_GetPinValue
 * Description: Reads the required pin (0->7) on any port (A->F)
 * Input: Port_Number: GPIO_PORTx , x = A -> F
 *        Pin_Number: GPIO_PINx , x = 0 -> 7
 * Return: Pin level
 * Tested ? Yes and Verified
 */
uint8_t GPIO_GetPinValue(uint8_t Port_Number, uint8_t Pin_Number){
    if(Pin_Number > 7) return 0;

    uint8_t retVal = 0;
    uint8_t mask = 1, i = 0;
    for(i = 0; i < Pin_Number; i++) mask = mask * 2;

    switch(Port_Number){
    case GPIO_PORTA:
        //BusSelesction = 0(APB) or BusSelesction = 1(AHB)
        //0x15000 = (Base address for PORTA on AHB - Base address for PORTA on APB) / 4
        //0xE000 = (Base address for PORTE on AHB - Base address for PORTE on APB) / 4
        //Division by 4 is done because pointer addition is done by incrementing the address by 4 bytes
        //PORTA addresses are handled by just the base address of APB
        //mask is how many bytes should be incremented to mask the correct bit in the register address in the bit band area
        //mask is calculated to fit the correct mask in the address of DATA register [2:9]
        //mask = 2^Pin_Number
        //Right shifting is for returning 0 or 1 and nothing else
        retVal = *( (GPIO_PORTA_DATA_BITS_R + BusSelection[Port_Number]*0x15000) + mask) >> Pin_Number;
        break;
    case GPIO_PORTB:
        retVal = *( (GPIO_PORTB_DATA_BITS_R + BusSelection[Port_Number]*0x15000) + mask) >> Pin_Number;
        break;
    case GPIO_PORTC:
        retVal = *( (GPIO_PORTC_DATA_BITS_R + BusSelection[Port_Number]*0x15000) + mask) >> Pin_Number;
        break;
    case GPIO_PORTD:
        retVal = *( (GPIO_PORTD_DATA_BITS_R + BusSelection[Port_Number]*0x15000) + mask) >> Pin_Number;
        break;
    case GPIO_PORTE:
        retVal = *( (GPIO_PORTE_DATA_BITS_R + BusSelection[Port_Number]*0xE000) + mask) >> Pin_Number;
        break;
    case GPIO_PORTF:
        retVal = *( (GPIO_PORTF_DATA_BITS_R + BusSelection[Port_Number]*0xE000) + mask) >> Pin_Number;
        break;
    }
    return retVal;
}

