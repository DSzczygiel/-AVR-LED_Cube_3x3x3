/*
 * cube.c
 *
 *  Created on: 27 gru 2018
 *      Author: Daniel
 */

#include "cube.h"

uint16_t layer1_cols = 0;
uint16_t layer2_cols = 0;
uint16_t layer3_cols = 0;
volatile uint8_t effectReady = 0;
volatile uint8_t counter = 2;
uint16_t bits[9] = {1, 2, 4, 8, 16, 32, 64, 128, 256};

void initCube() {
	//Timer1
	TCCR1B |= (1 << WGM12);		//CTC mode
	TCCR1B |= (1 << CS12);		//Prescler = 256
	OCR1A = 124;				//Value to send interrupt
	TIMSK1 |= (1 << OCIE1A);	//Enable compare match A interrupt

	//Ports as outputs
	PORT_1_DDR = 0xFF;
	PORT_2_DDR = 0xFF;

	//Low state on both ports
	PORT_1 = 0;
	PORT_2 = 0;
}

void effect_upAndDown() {
	setAllColumns(1);
	clearAllColumns(2);
	clearAllColumns(3);
	_delay_ms(150);
	setAllColumns(2);
	clearAllColumns(1);
	clearAllColumns(3);
	_delay_ms(150);
	setAllColumns(3);
	clearAllColumns(2);
	clearAllColumns(1);
	_delay_ms(150);
	setAllColumns(3);
	clearAllColumns(2);
	clearAllColumns(1);
	_delay_ms(150);
	setAllColumns(2);
	clearAllColumns(3);
	clearAllColumns(1);
	_delay_ms(150);
}

void effect_rotatingFrame(){
	turnOffWholeCube();
	setColumns(1, COL_2 | COL_5 | COL_8);
	setColumns(2, COL_2 | COL_8);
	setColumns(3, COL_2 | COL_5 | COL_8);
	_delay_ms(150);

	turnOffWholeCube();
	setColumns(1, COL_1 | COL_5 | COL_9);
	setColumns(2, COL_1 | COL_9);
	setColumns(3, COL_1 | COL_5 | COL_9);
	_delay_ms(150);

	turnOffWholeCube();
	setColumns(1, COL_4 | COL_5 | COL_6);
	setColumns(2, COL_4 | COL_6);
	setColumns(3, COL_6 | COL_5 | COL_4);
	_delay_ms(150);

	turnOffWholeCube();
	setColumns(1, COL_7 | COL_5 | COL_3);
	setColumns(2, COL_7 | COL_3);
	setColumns(3, COL_7 | COL_5 | COL_3);
	_delay_ms(150);
}

void effect_rain(){
	uint16_t drop1 = bits[rand()%9];
	uint16_t drop2;
	uint16_t drop3;

	do{
		drop2 = bits[rand()%9];
		drop3 = bits[rand()%9];
	}while(drop1 == drop2 || drop1 == drop3 || drop2 == drop3);

	turnOffWholeCube();
	setColumns(1, drop1);
	_delay_ms(150);

	turnOffWholeCube();
	setColumns(1, drop2);
	setColumns(2, drop1);
	_delay_ms(150);

	turnOffWholeCube();
	setColumns(1, drop3);
	setColumns(2, drop2);
	setColumns(3, drop1);
	_delay_ms(150);

	turnOffWholeCube();
	setColumns(2, drop3);
	setColumns(3, drop2);
	_delay_ms(150);

	turnOffWholeCube();
	setColumns(3, drop3);
	_delay_ms(150);
}

void effect_random(){
	uint16_t drop1 = bits[rand()%9];
	uint16_t drop2;
	uint16_t drop3;

	do{
		drop2 = bits[rand()%9];
		drop3 = bits[rand()%9];
	}while(drop1 == drop2 || drop1 == drop3 || drop2 == drop3);
	turnOffWholeCube();

	setColumns(1, drop1);
	setColumns(2, drop2);
	setColumns(3, drop3);
	_delay_ms(100);
}

void setColumns(uint8_t layer, uint16_t cols) {
	if (cols < 512 && cols > 0)
		switch (layer) {
		case 1:
			layer1_cols |= cols;
			break;
		case 2:
			layer2_cols |= cols;
			break;
		case 3:
			layer3_cols |= cols;
			break;
		}
}

void setColumnsInAllLayers(uint16_t cols){
	if (cols < 512 && cols > 0){
		layer1_cols |= cols;
		layer2_cols |= cols;
		layer3_cols |= cols;
	}
}

void clearColumns(uint8_t layer, uint16_t cols) {
	if (cols < 512 && cols > 0)
		switch (layer) {
		case 1:
			layer1_cols ^= cols;
			break;
		case 2:
			layer2_cols ^= cols;
			break;
		case 3:
			layer3_cols ^= cols;
			break;
		}
}

void clearColumnsInAllLayers(uint16_t cols){
	if (cols < 512 && cols > 0){
		layer1_cols ^= cols;
		layer2_cols ^= cols;
		layer3_cols ^= cols;
	}
}

void clearAllColumns(uint8_t layer) {
	switch (layer) {
	case 1:
		layer1_cols &= ~((uint16_t) 511);
		break;
	case 2:
		layer2_cols &= ~((uint16_t) 511);
		break;
	case 3:
		layer3_cols &= ~((uint16_t) 511);
		break;
	}
}

void setAllColumns(uint8_t layer) {
	switch (layer) {
	case 1:
		layer1_cols |= 511;
		break;
	case 2:
		layer2_cols |= 511;
		break;
	case 3:
		layer3_cols |= 511;
		break;
	}
}

void turnOnWholeCube() {
	setAllColumns(1);
	setAllColumns(2);
	setAllColumns(3);
}

void turnOffWholeCube() {
	clearAllColumns(1);
	clearAllColumns(2);
	clearAllColumns(3);
}

/*
 * Layers multiplexing
 * F_CPU / Prescaler / OCR value = Frequency
 * 8000000 / 256 / 124 = ~253Hz
 *
 * Each layer is displayed with frequency 253/3 = ~84Hz
 */
ISR(TIMER1_COMPA_vect) {

		if (counter > 8)
			counter = 2;

		if (counter == 2) {
			PORT_1 = layer1_cols;
			PORT_2 = (layer1_cols >> 8);
		}
		if (counter == 4) {
			PORT_1 = layer2_cols;
			PORT_2 = (layer2_cols >> 8);
		}
		if (counter == 8) {
			PORT_1 = layer3_cols;
			PORT_2 = (layer3_cols >> 8);
		}

		PORT_2 &= 0b00000001;
		PORT_2 |= counter;

		counter <<= 1;
}
