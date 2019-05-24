/*
 * cube.h
 *
 *  Created on: 27 gru 2018
 *      Author: Daniel
 */

#ifndef CUBE_H_
#define CUBE_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define PORT_1_DDR DDRD		//Direction register for PORT_1
#define PORT_2_DDR DDRB		//Direction register for PORT_2
#define PORT_1 PORTD		//Port for 8 columns
#define PORT_2 PORTB		//Port for 1 remaining column and 3 layers

#define COL_1 1
#define COL_2 2
#define COL_3 4
#define COL_4 8
#define COL_5 16
#define COL_6 32
#define COL_7 64
#define COL_8 128
#define COL_9 256

void initCube();
void effect_upAndDown();
void effect_rotatingFrame();
void effect_rain();
void effect_random();
void turnOnWholeCube();
void turnOffWholeCube();
void setColumns(uint8_t layer, uint16_t cols);
void clearColumns(uint8_t layer, uint16_t cols);
void setColumnsInAllLayers(uint16_t cols);
void clearColumnsInAllLayers(uint16_t cols);
void clearAllColumns(uint8_t layer);
void setAllColumns(uint8_t layer);

#endif /* CUBE_H_ */
