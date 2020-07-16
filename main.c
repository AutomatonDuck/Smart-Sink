#include <msp430.h> 

int count = 0;
/**
 * main.c
 */
int read_distance();

int main(void)
{
    int distance_cm;
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	BCSCTL1 = CALBC1_1MHZ;  // set basic clock for 1MHz operation
	DCOCTL = CALDCO_1MHZ; //set clock frequency for 1MHz operation
	TA1CTL = TASSEL_2 + ID_0 + MC_2; // sets timer control to up mode and 1 division
	P2OUT = 0x00; //P2.x will be output
	P1OUT = 0x00; //P1.0 will be input
	P2DIR = BIT3|BIT0|BIT4|BIT5; //set for led test
	P1DIR = 0x00;
	
	while(1){
	    distance_cm = read_distance();
	    if(distance_cm < 20 && distance_cm > 10){
	        P2OUT = BIT3;
	        __delay_cycles(1000000); // time for water to run
	    }
	    else if(distance_cm < 10 )
	    {
	        P2OUT = BIT4;
	        __delay_cycles(100000); //time for soap
	    }
	    else
	    {
	        P2OUT = BIT5;
	    }
	}


}

int read_distance()
{
    int echo_duration;
    int distance;

    P2OUT |= BIT0; // begin Trigger
    __delay_cycles(20);
    P2OUT &= ~BIT0; // End Trigger

    while((P1IN & BIT0)== 0); //wait for echo
    TA1R = 0; // reset timer
    while((P1IN & BIT0) > 0); // end echo
    echo_duration = TA1R; // test duration as timer
    distance = 0.017 * echo_duration; //conversion to cm
    //count++; // loop test
    return distance;


}
