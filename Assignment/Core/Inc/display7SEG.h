/*
 * display7SEG.h
 *
*  Created on: OCT 26, 2025
 *      Author: hanh
 */

#ifndef INC_DISPLAY7SEG_H_
#define INC_DISPLAY7SEG_H_

#include "main.h"
#include "global.h"

extern int index_led;
extern int counter1, counter2;

void display7SEG(int num);
void update7SEG(int index);

void updateLedBufferMode1();
void updateLedBufferMode2();
void updateLedBufferMode3();
void updateLedBufferMode4();


#endif /* INC_DISPLAY7SEG_H_ */
