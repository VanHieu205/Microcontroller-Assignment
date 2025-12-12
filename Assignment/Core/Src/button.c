/*
 * button.c
 *
 *  Created on: OCT 26, 2025
 *      Author: hanh
 */

#include "button.h"

int KeyReg0[NUM_OF_BUTTONS] = {NORMAL_STATE};
int KeyReg1[NUM_OF_BUTTONS] = {NORMAL_STATE};
int KeyReg2[NUM_OF_BUTTONS] = {NORMAL_STATE};
int KeyReg3[NUM_OF_BUTTONS] = {NORMAL_STATE};

// Create flag for normal press and long press
int normal_flag[NUM_OF_BUTTONS] = {0};
int long_flag[NUM_OF_BUTTONS] = {0};

int TimeOutForKeyPress[NUM_OF_BUTTONS] = {0};

int isButtonPressed(int index){
	if (normal_flag[index] == 1){
		normal_flag[index] = 0;
		return 1;
	}
	return 0;
}

void subKeyProcess(int index){
	normal_flag[index] = 1;
}

void getKeyInput() {
	for (int i = 0; i < NUM_OF_BUTTONS; i++) {
		KeyReg0[i] = KeyReg1[i];
		KeyReg1[i] = KeyReg2[i];

		// Read signals from buttons
		if (i == 0) {
			KeyReg2[i] = HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_Pin);
		}
		else if (i == 1) {
			KeyReg2[i] = HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin);
		}
		else if (i == 2) {
			KeyReg2[i] = HAL_GPIO_ReadPin(BTN3_GPIO_Port, BTN3_Pin);
		}
		else if (i == 3) {
		    KeyReg2[i] = HAL_GPIO_ReadPin(BTN4_GPIO_Port, BTN4_Pin);
		}

		if ((KeyReg1[i] == KeyReg0[i]) && (KeyReg1[i] == KeyReg2[i])){
			if (KeyReg3[i] != KeyReg2[i]){
				KeyReg3[i] = KeyReg2[i];
				if (KeyReg2[i] == PRESSED_STATE){
					subKeyProcess(i);	// Set timer_flag[i] = 1;
					if (long_flag[i] == 0) {
						TimeOutForKeyPress[i] = 2000 / TIME_CYCLE;
					}
					else {
						TimeOutForKeyPress[i] = 250 / TIME_CYCLE;
					}
				}
			} else {
				TimeOutForKeyPress[i]--;
				if (TimeOutForKeyPress[i] == 0){
					KeyReg3[i] = NORMAL_STATE;
					long_flag[i] = 1;
				}
			}
		}
		else {
			long_flag[i] = 0;
		}
	}
}

void toggleButton0() {

	KeyReg0[0] = PRESSED_STATE;
	    KeyReg1[0] = PRESSED_STATE;
	    KeyReg2[0] = PRESSED_STATE;

	    if (KeyReg3[0] != PRESSED_STATE) {
	        KeyReg3[0] = PRESSED_STATE;
	        subKeyProcess(0);
	        TimeOutForKeyPress[0] = 2000 / TIME_CYCLE;
	        long_flag[0] = 0;
	    }

	    KeyReg0[0] = NORMAL_STATE;
	    KeyReg1[0] = NORMAL_STATE;
	    KeyReg2[0] = NORMAL_STATE;

	    KeyReg3[0] = NORMAL_STATE;
}


void toggleButton1() {

	KeyReg0[1] = PRESSED_STATE;
	    KeyReg1[1] = PRESSED_STATE;
	    KeyReg2[1] = PRESSED_STATE;

	    if (KeyReg3[1] != PRESSED_STATE) {
	        KeyReg3[1] = PRESSED_STATE;
	        subKeyProcess(1);
	        TimeOutForKeyPress[1] = 2000 / TIME_CYCLE;
	        long_flag[1] = 0;
	    }

	    KeyReg0[1] = NORMAL_STATE;
	    KeyReg1[1] = NORMAL_STATE;
	    KeyReg2[1] = NORMAL_STATE;

	    KeyReg3[1] = NORMAL_STATE;
}

void toggleButton2() {

	KeyReg0[2] = PRESSED_STATE;
	    KeyReg1[2] = PRESSED_STATE;
	    KeyReg2[2] = PRESSED_STATE;

	    if (KeyReg3[2] != PRESSED_STATE) {
	        KeyReg3[2] = PRESSED_STATE;
	        subKeyProcess(2);
	        TimeOutForKeyPress[2] = 2000 / TIME_CYCLE;
	        long_flag[2] = 0;
	    }

	    KeyReg0[2] = NORMAL_STATE;
	    KeyReg1[2] = NORMAL_STATE;
	    KeyReg2[2] = NORMAL_STATE;

	    KeyReg3[2] = NORMAL_STATE;
}


void toggleButton3() {

	KeyReg0[3] = PRESSED_STATE;
	    KeyReg1[3] = PRESSED_STATE;
	    KeyReg2[3] = PRESSED_STATE;

	    if (KeyReg3[3] != PRESSED_STATE) {
	        KeyReg3[3] = PRESSED_STATE;
	        subKeyProcess(3);
	        TimeOutForKeyPress[3] = 2000 / TIME_CYCLE;
	        long_flag[3] = 0;
	    }

	    KeyReg0[3] = NORMAL_STATE;
	    KeyReg1[3] = NORMAL_STATE;
	    KeyReg2[3] = NORMAL_STATE;

	    KeyReg3[3] = NORMAL_STATE;
}
