#include <msp430.h>
/*
 * led8bit.h
 *
 *  Created on: Jun 30, 2020
 *  Author: Dominic Robbins
 */
#ifndef LED8BIT_H_
#define LED8BIT_H_

#define DR P2OUT = P2OUT|BIT0 //defines RS High (data register)
#define CCR P2OUT = P2OUT&(~BIT0) //defines RS low(Command code register)
#define READ P2OUT = P2OUT|BIT1 //defines RW as high to read from LED
#define WRITE P2OUT = P2OUT&(~BIT1) //defines RW as low to write to LED
#define EH P2OUT = P2OUT|BIT2   //defines enable as high
#define EL P2OUT = P2OUT&(~BIT2) //defines enable as low

//changes trigger edge from high to low for data write
void input_data()
{
   EH;
   __delay_cycles(2000);
   EL;

}

//changes trigger edge from low to high for data read
void read_data()
{
    EL;
    __delay_cycles(2000);
    EH;
}

//checks if led is busy
void busy()
{
    P1DIR &= ~(BIT7); // BIT7 is busy flag
    while((P1IN & BIT7) == 1)
    {
        read_data();

    }
    P1DIR |= BIT7;
}

void send_command(char cmd)
{
    busy();
    WRITE;
    CCR;
    P1OUT = (P1OUT & 0x00)|(cmd);
    input_data(); //enable trigger
}

void send_data(char data)
{
    busy();
    WRITE;
    DR;
    P1OUT = (P1OUT & 0x00)|(data);
    input_data(); //enable trigger
}

void send_string(char *x)
{
    while(*x)
    {
        send_data(*x);
        x++;
    }
}

void init()
{
    P2DIR |= 0xFF;
    P1DIR |= 0xFF;
    P2OUT &= 0x00;
    P1OUT &= 0x00;
    send_command(0x38); //8 bit mode
    send_command(0x0C); //Display on
    send_command(0x01);//display clear
    send_command(0x06); //Entry mode set
}


#endif /* LED8BIT_H_ */
