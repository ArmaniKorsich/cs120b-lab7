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
#include "io.h"

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

void writeVal(unsigned char val) {
	LCD_ClearScreen();
	LCD_WriteData(val + '0');
}

enum SM_States {START, SM_1, SM_2, SM_3 } state;
unsigned char count = 0x00;
unsigned char val = 0x00;

void TickFct() {
	switch(state) {
		case START:
			if ((~PINA & 0x03) == 0x01)
			{
				count = 10;
				state = SM_1;
			}
			else if ((~PINA & 0x03) == 0x02)
			{
				count = 10;
				state = SM_2;
			}
			else if ((~PINA & 0x03) == 0x03)
			{
				state = SM_3;
			}
			else
			{
				state = START;
			}
			break;
		case SM_1:
			if ((~PINA & 0x03) == 0x01)
			{
				state = SM_1;
			}
			else
			{
				state = START;
			}
			break;
		case SM_2:
			if ((~PINA & 0x03) == 0x02)
			{
				state = SM_2;
			}
			else
			{
				state = START;
			}
			break;
		case SM_3:
			state = START;
			break;
		default:
			state = START;
			break;
	}

	switch(state) {
		case START:
			writeVal(val);
			break;
		case SM_1:
			if (count == 10)
			{
				if (val < 9)
				{
					val++;
				}
				count = 0;
			}
			else
			{
				count++;
			}
			writeVal(val);
			break;
		case SM_2:
			if (count == 10)
			{
				if (val > 0)
				{
					val--;
				}
				count = 0;
			}
			else
			{
				count++;
			}
			writeVal(val);
			break;
		case SM_3:
			val = 0x00;
			writeVal(val);
			break;
		default:
			val = 0x00;
			writeVal(val);
			break;
	}

}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	TimerSet(100);
    	TimerOn();
	
	LCD_init();
	//LCD_DisplayString(1, "Hello World");
	while(1) {
		while (!TimerFlag);
        	TimerFlag = 0;
		TickFct();
	}

}
