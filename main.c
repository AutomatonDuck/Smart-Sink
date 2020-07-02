#include <msp430.h>
#include "led8bit.h"

int main()
{
     WDTCTL = WDTPW + WDTHOLD;   //stops watchdog
    init();
    send_string("Dominic Robbins"); //max 14 characters per line
    send_command(0xC0); // moves cursor to next line
    send_string("Smart sink");
    while(1){}


}
