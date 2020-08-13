#include <msp430.h>
#include <stdio.h>
//int count = 0;
/**
 * main.c
 */

#define OTROWS  P2OUT
#define ROW1    BIT3
#define ROW2    BIT4
#define ROW3    BIT5
#define ROW4    BIT7
#define ALLROWS (ROW4 | ROW3 |ROW2 |ROW1)

#define INCOLS  P1IN
#define COL1    BIT7
#define COL2    BIT3
#define COL3    BIT4
#define ALLCOLS (COL3 | COL2 |COL1)

// Declarations
int rownum, keynum = 1, ROWS;

//#define SERV BIT2                      // P1.2 SERV

#define SERVO_BIT BIT2

#define SM_clk 1000000

#define servo_freq 50

int PWM_period = SM_clk /servo_freq;

int read_distance();

int main(void)
{
    int distance_cm;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    BCSCTL1 = CALBC1_1MHZ;  // set basic clock for 1MHz operation
    DCOCTL = CALDCO_1MHZ; //set clock frequency for 1MHz operation
    TA1CTL = TASSEL_2 + ID_0 + MC_2; // sets timer control to up mode and 1 division

    P1DIR |= SERVO_BIT; //direction is set
    P1SEL |= SERVO_BIT;// port 1 function is set.
    TA0CCR0 = PWM_period-1; // pwm period
    TA0CCR1 = 0; //duty cycle = TA0CCR0/ TA0CCR1
    TA0CCTL1 = OUTMOD_7; // TACCR1 reset/set
    TA0CTL=TASSEL_2+MC_1; // starting timer by setting clock source SMCLK of timer and UP mode


    P2DIR  =  BIT0;            // Set port 2 all inputs
    P2DIR |=  ALLROWS | BIT2;         // Set all rows as output
    P1REN |=  ALLCOLS;         // Enable resistors for all columns
    P1OUT |=  ALLCOLS;         // Make thsoe resistors pullups
    P1IES |=  ALLCOLS;         // Interrupt edge selec, hi to lo
    P1IE  |=  ALLCOLS;         // Enable interrupt on all columns
    P1IFG &= ~ALLCOLS;         // Clear all interrupt flags on the columns

       __enable_interrupt();      //enable interrupts
       OTROWS &= ~(ALLROWS);      //Assert low on all rows low to detect any key stroke interrup
       __delay_cycles(200);

       while(1){

              distance_cm = read_distance();
              if(distance_cm < 20 && distance_cm > 7)
              {

                  if(keynum == 1){
                      TA0CCR1 = 2600; // 162 degrees
                      __delay_cycles(1000000);
                                   }
                  if(keynum == 2){
                      TA0CCR1 = 2373; // 144
                      __delay_cycles(1000000);
                                   }
                  if(keynum == 3){
                      TA0CCR1 = 2146; // 126 degrees
                      __delay_cycles(1000000);
                                   }
                  if(keynum == 4){
                      TA0CCR1 = 1919; // 108 degrees
                      __delay_cycles(1000000);
                                   }
                  if(keynum == 5){
                      TA0CCR1 = 1692; // 90 degrees
                      __delay_cycles(1000000);
                                   }
                  if(keynum == 6){
                      TA0CCR1 = 1456; // 72  degrees
                      __delay_cycles(1000000);
                                   }
                  if(keynum == 7){
                      TA0CCR1 = 1238; // 54 degrees
                      __delay_cycles(1000000);
                                   }
                  if(keynum == 8){
                      TA0CCR1 = 1011; // 36 degrees
                      __delay_cycles(1000000);
                                   }
                  if(keynum == 9){
                      TA0CCR1 = 784; // 18 degrees
                      __delay_cycles(1000000);
                                 }



              }
              else if(distance_cm < 7){
                  P2OUT |= BIT2;
                  __delay_cycles(200000);
                  P2OUT &= ~BIT2;
              }
              else{
                  TA0CCR1 = 550; // 0 degrees
                  __delay_cycles(1000000);
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
    distance = echo_duration / 58; //conversion to cm
    //count++; // loop test
    return distance;


}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    OTROWS |= ALLROWS; __delay_cycles(200);     //Make all rows high
    keynum=0;
    for(rownum=0; rownum<=3; rownum++)          //Start scanning the KP
        {
            //start a 1 in first roww and shift left rownum times, and XOR
            OTROWS = ((ROW1<<rownum) ^ ALLROWS); __delay_cycles(200);
            if(!(INCOLS & COL1)) {keynum=3*rownum +1;}
            if(!(INCOLS & COL2)) {keynum=3*rownum +2;}
            if(!(INCOLS & COL3)) {keynum=3*rownum +3;}
         }





    OTROWS &= ~ALLROWS;  __delay_cycles(200);   //Make all rows low to for for next key stroke interrup
    P1IFG  &= ~ALLCOLS;  __delay_cycles(200);   //Clear the interrupt flags
}

