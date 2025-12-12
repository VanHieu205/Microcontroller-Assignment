/*
 * display7SEG.c
 *
 *  Created on: OCT 26, 2025
 *      Author: hanh
 */

#include "display7SEG.h"

void display7SEG(int num){
  	char led7seg[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
  	for (int i=0; i < 7; i++){
  		HAL_GPIO_WritePin(GPIOB, SEG0_Pin<<i, (led7seg[num]>>i) & 1);
  	}
}

const int MAX_LED = 4;
int index_led = 0;
int led_buffer[4] = {1, 2, 3, 4};
void update7SEG(int index){
	display7SEG(led_buffer[index]);
	switch(index){
		case 0:
			// Display the first 7 SEG with led_buffer [0]
			index_led = 1;
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, RESET);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, SET);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, SET);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, SET);
			break;
		case 1:
			// Display the second 7 SEG with led_buffer [1]
			index_led = 2;
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, SET);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, RESET);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, SET);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, SET);
			break;
		case 2:
			// Display the third 7 SEG with led_buffer [2]
			index_led = 3;
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, SET);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, SET);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, RESET);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, SET);
			break;
		case 3:
			// Display the forth 7 SEG with led_buffer [3]
			index_led = 0;
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, SET);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, SET);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, SET);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, RESET);
			break;
		default:
			break;
	}
}

int counter1 = 1, counter2 = 1;
void updateLedBufferMode1(){
	// 2 7-SEG LED Landscape
	/* Turn on RED-Landscape (count down from RED -> 0) */
	if (counter1 <= RED){
		led_buffer[0] = (RED - counter1 + 1) / 10;
		led_buffer[1] = (RED - counter1 + 1) % 10;
	}
	/* Turn on GREEN-Landscape (count down from GREEN -> 0) */
	else if (counter1 <= (RED + GREEN)){
		led_buffer[0] = (RED + GREEN - counter1 + 1) / 10;
		led_buffer[1] = (RED + GREEN - counter1 + 1) % 10;
	}
	/* Turn on AMPER-Landscape (count down from AMPER -> 0) */
	else{
		led_buffer[0] = (RED + GREEN + AMBER - counter1 + 1) / 10;
		led_buffer[1] = (RED + GREEN + AMBER - counter1 + 1) % 10;
	}
	counter1++;
	/* Return to turn on RED-Landscape */
	if (counter1 > RED + AMBER + GREEN) counter1 = 1;


	// 2 7-SEG LED Portrait
	/* Turn on GREEN-Portrait (count down from GREEN -> 0) */
	if (counter2 <= GREEN){
		led_buffer[2] = (GREEN - counter2 + 1) / 10;
		led_buffer[3] = (GREEN - counter2 + 1) % 10;
	}
	/* Turn on AMPER-Portrait (count down from AMPER -> 0) */
	else if (counter2 <= (AMBER + GREEN)){
		led_buffer[2] = (AMBER + GREEN - counter2 + 1) / 10;
		led_buffer[3] = (AMBER + GREEN - counter2 + 1) % 10;
	}
	/* Turn on RED-Portrait (count down from RED -> 0) */
	else {
		led_buffer[2] = (RED + GREEN + AMBER - counter2 + 1) / 10;
		led_buffer[3] = (RED + GREEN + AMBER - counter2 + 1) % 10;
	}
	counter2++;
	/* Return to turn on GREEN-Portrait */
	if (counter2 > RED + AMBER + GREEN) counter2 = 1;
}

void updateLedBufferMode2(){
	// Display mode 2
	led_buffer[0] = 0;
	led_buffer[1] = 2;
	// Display value RED
	led_buffer[2] = RED / 10;
	led_buffer[3] = RED % 10;
}

void updateLedBufferMode3(){
	// Display mode 3
	led_buffer[0] = 0;
	led_buffer[1] = 3;
	// Display value AMBER
	led_buffer[2] = AMBER / 10;
	led_buffer[3] = AMBER % 10;
}

void updateLedBufferMode4(){
	// Display mode 4
	led_buffer[0] = 0;
	led_buffer[1] = 4;
	// Display value GREEN
	led_buffer[2] = GREEN / 10;
	led_buffer[3] = GREEN % 10;
}


