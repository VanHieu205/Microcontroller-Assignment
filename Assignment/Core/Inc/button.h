/*
 * button.h
 *
 *  Created on: OCT 26, 2025
 *      Author: hanh
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"
#include "software_timer.h"

#define NORMAL_STATE 			SET
#define PRESSED_STATE 			RESET
#define NUM_OF_BUTTONS 			4

int isButtonPressed(int index);
void getKeyInput();

#endif /* INC_BUTTON_H_ */
