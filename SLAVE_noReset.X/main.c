/*
 * File:   main.c
 * Author: Sloba
 *
 * Created on 13.08.2019., 13.17
 */

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = ON      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF     // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = OFF         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define _XTAL_FREQ 500000
#include <xc.h>
//#include<inttypes.h>
void changeClock(unsigned long int receivedValue);
void managePins(unsigned long int receivedValue);
unsigned int read(unsigned int value);

volatile unsigned char enabled;

void main(void) {
__delay_ms(200);
#undef _XTAL_FREQ
    OSCCON = 0b01111000;
   
    ANSELA = 0;
    ANSELC = 0;

    TRISC = 0b00000011; // RC0 i RC1 ulaz sck i SDI, ostalo izlazi za ledove
    LATC = 0;
    
    NCO1SEL = 1; // NCO1 output on RA4
    TRISA &= 0b11101111; // RA4 output
    // CKE = 0, bit se sempluje na padajucu
    // CPOL = 0, clock je 0 idle stanje
    SSPBUF = 0;
    BF = 0;
    SSP1CON1 = 0;
    SSP1CON1 |= 0b00000101;
    SSP1CON1 |= 0b00100101;
     
   unsigned long int value = 0;
    
   enabled = 0;
    
   unsigned char u[4] = {0, 0, 0, 0};
    
   NCO1CLK |= 0b00000001; // FOSC clock for the NCO1
        
    value = 0;
    while(1)
    {
        if(BF)
        {
            u[0] = SSPBUF;
            while(!BF);
            u[1] = SSPBUF;
            while(!BF);
            u[2] = SSPBUF;
            while(!BF);
            u[3] = SSPBUF;
            
           
            value = u[0];
            value = value << 8;
            value |= u[1];
            value = value << 8;
            value |= u[2];
            value = value << 8;
            value |= u[3];
            
            if(value < 65536) // <= 65535
            {
               
                if(!enabled)
                {
                    enabled = 1;
                    NCO1CLK = 0b00000001; // FOSC clock for the NCO1
                    NCO1CON = 0b11000000; // Enable module enable output
                }
                NCO1INCH = u[2];
                NCO1INCL = u[3];
            }
            else if(value <= 70003)
            {
                changeClock(value);
            }
            else
            {
                managePins(value);
            }

        }

    }
    return;
}


void changeClock(unsigned long int receivedValue)
{
    enabled = 0;
    NCO1INCH = 0;
    NCO1INCL = 0;
    NCO1CON &= 0b00111111;
    NCO1CLK = 0b00000011; // FOSC clock for the NCO1
    switch(receivedValue)
                {
                    case(70000):
                        OSCCON = 0b01111000; //16MHz
                        break;
                    case(70001):
                        OSCCON = 0b01110000; //8MHz
                        break;
                    case(70002):
                        OSCCON = 0b01101000; //4MHz
                        break;
                    case(70003):
                        OSCCON = 0b01100000; //2MHz
                        break;
                  
                }
   
}

void managePins(unsigned long receivedValue)
{
    if(receivedValue == 70004)
        LATC |= 0b00100000;
    else if(receivedValue == 70005)
        LATC &= 0b11011111;
}

