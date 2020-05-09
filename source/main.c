/*	Author: armanikorsich
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>

#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
    
    TCCR1B = 0x0b;
    OCR1A = 125;
    TIMSK1 = 0x02;
    TCNT1 = 0;
    
    _avr_timer_cntcurr = _avr_timer_M;
    SREG |= 0x80;
}

void Timerff() {
    TCCR1B = 0x00;
}

void TimerISR() {
    TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
    _avr_timer_cntcurr--;
    if(_avr_timer_cntcurr == 0) {
        TimerISR();
        _avr_timer_cntcurr = _avr_timer_M;
    }
}

void TimerSet(unsigned long M) {
    _avr_timer_M = M;
    _avr_timer_cntcurr = _avr_timer_M;
}


enum SM_States {START, SM_0, SM_1, SM_2, SM_3, SM_4, SM_5 } state;

void TickFct() {
	switch(state) {
		case START:
			if ((~PINA & 0x01) == 0x01)
			{
				state = START;
			}
			else
			{
				state = SM_0;
			}
			break;
		case SM_0:
			if ((~PINA & 0x01) == 0x01)
			{
				state = SM_4;
			}
			else
			{
				state = SM_1;
			}
			break;
		case SM_1:
			if ((~PINA & 0x01) == 0x01)
			{
				state = SM_4;
			}
			else
			{
				state = SM_2;
			}
			break;
		case SM_2:
			if ((~PINA & 0x01) == 0x01)
			{
				state = SM_4;
			}
			else
			{
				state = SM_3;
			}
			break;
		case SM_3:
			if ((~PINA & 0x01) == 0x01)
			{
				state = SM_4;
			}
			else
			{	
				state = SM_0;
			}
			break;
		case SM_4:
			if ((~PINA & 0x01) == 0x01)
			{
				state = SM_4;
			}
			else
			{
				state = SM_5;
			}
			break;
		case SM_5:
			if ((~PINA & 0x01) == 0x01)
			{
				state = START;
			}
			else
			{
				state = SM_5;
			}
			break;
		default:
			state = START;
			break;
	}

	switch(state) {
		case START:
			PORTC = 0x01;
			break;
		case SM_0:
			PORTC = 0x01;
			break;
		case SM_1:
			PORTC = 0x02;
			break;
		case SM_2:
			PORTC = 0x04;
			break;
		case SM_3:
			PORTC = 0x02;
			break;
		case SM_4:
		case SM_5:
			break;
		default:
			PORTC = 0x01;
			break;
	}

}


int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    TimerSet(300);
    TimerOn();
    PORTB = 0x01;
    /* Insert your solution below */
    while (1) {
        while (!TimerFlag);
        TimerFlag = 0;
	TickFct();
    }
    return 1;    

}
