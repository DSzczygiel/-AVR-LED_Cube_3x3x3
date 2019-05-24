/*
 * main.c
 *
 *  Created on: 27 gru 2018
 *      Author: Daniel
 *
 *      MCU: ATmega 328P
 */


#include <avr/io.h>
#include <avr/delay.h>
#include "cube.h"

#define SWITCH_DDR DDRC
#define SWITCH_PORT PORTC
#define SWITCH_PIN PINC
#define SWITCH PC4

uint8_t nr = 1;
volatile uint8_t isButtonPressed = 0;

int main(){
	SWITCH_DDR &= ~(1 << SWITCH);			//Switch pin as input
	SWITCH_PORT |= (1 << SWITCH); 			//Enable pull-ups on switch pin

	//Timer0
	TCCR0B |= (1 << CS00) | (1 << CS01);	//Prescler = 64
	TIMSK0 |= (1 << TOIE0);					//Enable overflow interrupt

	sei();
	initCube();

	while(1){
		if(isButtonPressed){
			isButtonPressed = 0;
			nr++;
			if(nr > 5)
				nr = 1;
			TIMSK0 &= ~(1 << TOIE0);			//Disable overflow interrupt
		}

		switch(nr){
			case 1:
				effect_upAndDown();
				break;
			case 2:
				effect_rotatingFrame();
				break;
			case 3:
				effect_rain();
				break;
			case 4:
				effect_random();
				break;
			case 5:
				turnOnWholeCube();
				break;
		}
		TIMSK0 |= (1 << TOIE0);					//Enable overflow interrupt
	}
}

ISR(TIMER0_OVF_vect){
	if(!(SWITCH_PIN & (1 << SWITCH)))
		isButtonPressed = 1;
}
