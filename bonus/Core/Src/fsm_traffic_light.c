/*
 * fsm_traffic_light.c
 *
 *  Created on: Dec 11, 2025
 *      Author: LENOVO
 */


#include <stdbool.h>
#include "fsm_traffic_light.h"
#include "i2c-lcd.h"
#include <stdio.h>
int status = INIT;
int manual_step = 0;
bool perdestrian = false;
bool preper = false;
int timeperdes = 3;
int timepre = 1;


void fsm_perdes() {
    // Khi người dùng nhấn nút đi bộ
    if (isButtonPressed(3)) {
        lcd_clear_display();  // XÓA TRƯỚC
        lcd_goto_XY(0, 0);
        lcd_send_string(" PERDESTRIAN ");
        preper = true;
        timepre = 1;
        setTimer(4, 1000);
    }
    if (preper == true) {
        if (timer_flag[4] == 1) {
            timepre--;
            if (timepre <= 0) {
                timeperdes = 3;
                perdestrian = true;
                preper = false;
if(perdestrian){
	lcd_clear_display();  // XÓA TRƯỚC
	                	  lcd_goto_XY(0, 0);
	                	        lcd_send_string(" PERDESTRIAN ON ");
}
                setTimer(4, 3000);
            }

        }
    }
    if (perdestrian == true) {
        if (timer_flag[4] == 1) {
            timeperdes--;

                            	        lcd_goto_XY(1, 0);
                            	        lcd_send_string("a");
            if (timeperdes <= 0) {
                preper = false;
                perdestrian = false;
                if(!perdestrian) {
                	lcd_clear_display();  // XÓA TRƯỚC
                	        lcd_goto_XY(0, 0);
                	        lcd_send_string(" PERDESTRIAN OUT ");
                }
                clearTimer(4);
            }

        }
    }
}
void setRed1() {
    HAL_GPIO_WritePin(BIT1_GPIO_Port, BIT1_Pin, SET);
    HAL_GPIO_WritePin(BIT2_GPIO_Port, BIT2_Pin, SET);
}

void setGreen1() {
    HAL_GPIO_WritePin(BIT1_GPIO_Port, BIT1_Pin, RESET);
    HAL_GPIO_WritePin(BIT2_GPIO_Port, BIT2_Pin, SET);
}

void setAmber1() {
    HAL_GPIO_WritePin(BIT1_GPIO_Port, BIT1_Pin, SET);
    HAL_GPIO_WritePin(BIT2_GPIO_Port, BIT2_Pin, RESET);
}
void setRed2() {
    //    HAL_GPIO_WritePin(BIT1_GPIO_Port, BIT1_Pin, RESET);
    //    HAL_GPIO_WritePin(BIT2_GPIO_Port, BIT2_Pin, RESET);
}

void setGreen2() {
    //    HAL_GPIO_WritePin(BIT1_GPIO_Port, BIT1_Pin, RESET);
    //    HAL_GPIO_WritePin(BIT2_GPIO_Port, BIT2_Pin, SET);
}

void setAmber2() {
    //    HAL_GPIO_WritePin(BIT1_GPIO_Port, BIT1_Pin, SET);
    //    HAL_GPIO_WritePin(BIT2_GPIO_Port, BIT2_Pin, RESET);
}

static void setNormalLedsForMode1() {
    setRed1();

    setGreen2();
}


static void enterManualStep(int step) {
    if (step == 1) {
        // Lane1 GREEN
        if (!perdestrian) {
            setGreen1();
        }
        else {
            setRed1();
        }
        // Lane2 RED
        setRed2();
    }
    else if (step == 2) {
        // Lane1 RED

        setRed1();

        // Lane2 GREEN
        if (!perdestrian) {
            setGreen2();
        }
        else {
            setRed2();
        }
    }
    else if (step == 3) {
        // Both AMBER (steady)
        setAmber1();

        setAmber2();
    }
}

void enterManualMode() {
    // previous semantics (Lane1 RED, Lane2 GREEN)
    setRed1();

    setGreen2();
}

void fsm_traffic_light() {

    switch (status) {

        // INIT
    case INIT:
        status = MODE1;
        lcd_clear_display();  // XÓA TRƯỚC
        break;

        // MODE1 entry - initialize normal cycle
    case MODE1:
        status = RED_GREEN;

        // Set up LED (normal)
        setNormalLedsForMode1();

        // Set up default value
        counter1 = counter2 = 1;
        index_led = 0;

        updateLedBufferMode1();
        update7SEG(index_led);

        // Set timers
        setTimer(0, GREEN * 1000);    // Set timer for RED_GREEN
        setTimer(1, 1000);            // Set timer for updateLedBufferMode1
        setTimer(2, 250);             // Set timer for 7SEG LED
        // keep timer 3 as before (blinking)
        clearTimer(3);

        break;

        // RED_GREEN normal flow
    case RED_GREEN:
        // Button2 -> enter MANUAL_MODE (start manual sequence)
        if (isButtonPressed(1) == 1) {
            status = MANUAL_MODE_1;

            manual_step = 1;
            // clear normal timers to avoid interference
            clearTimer(0);
            clearTimer(1);
            clearTimer(2);
            enterManualStep(manual_step);
            break;
        }

        setRed1();
        setGreen2();

        if (timer_flag[0] == 1) {
            status = RED_AMBER;
            setTimer(0, AMBER * 1000);
        }
        if (timer_flag[1] == 1) {
            updateLedBufferMode1();

            setTimer(1, 1000);
        }
        if (timer_flag[2] == 1) {
            setTimer(2, 250);
        }

        if (isButtonPressed(0) == 1) {
            status = MODE2;
        }
        break;

        // RED_AMBER
    case RED_AMBER:
        if (isButtonPressed(1) == 1) {
            status = MANUAL_MODE_1;

            manual_step = 1;
            clearTimer(0); clearTimer(1); clearTimer(2);
            enterManualStep(manual_step);
            break;
        }

        setAmber2();

        if (timer_flag[0] == 1) {
            status = GREEN_RED;
            setTimer(0, GREEN * 1000);
        }
        if (timer_flag[1] == 1) {

            setTimer(1, 1000);
        }
        if (timer_flag[2] == 1) {
            setTimer(2, 250);
        }

        if (isButtonPressed(0) == 1) {
            status = MODE2;
        }
        break;

        // GREEN_RED
    case GREEN_RED:
        // Button2 -> enter MANUAL_MODE
        if (isButtonPressed(1) == 1) {
            status = MANUAL_MODE_1;

            manual_step = 1;
            clearTimer(0); clearTimer(1); clearTimer(2);
            enterManualStep(manual_step);
            break;
        }

        // Set up LED
        if (!perdestrian && !preper) {
            setGreen1();
        }
        else {
            if (preper) {
                setAmber1();
            }
            if (perdestrian) {
                setRed1();
            }
        }
        setRed2();

        if (timer_flag[0] == 1) {
            status = AMBER_RED;
            setTimer(0, AMBER * 1000);
        }
        if (timer_flag[1] == 1) {


            setTimer(1, 1000);
        }
        if (timer_flag[2] == 1) {
            setTimer(2, 250);
        }

        if (isButtonPressed(0) == 1) {
            status = MODE2;
        }
        break;

        // AMBER_RED
    case AMBER_RED:
        // Button2 -> enter MANUAL_MODE
        if (isButtonPressed(1) == 1) {
            status = MANUAL_MODE_1;

            manual_step = 1;
            clearTimer(0); clearTimer(1); clearTimer(2);
            enterManualStep(manual_step);
            break;
        }

        // Set up LED
        if (!perdestrian) {
            setAmber1();
        }
        else {
            setRed1();
        }
        setGreen2();

        if (timer_flag[0] == 1) {
            status = RED_GREEN;
            setTimer(0, GREEN * 1000);
        }
        if (timer_flag[1] == 1) {

            setTimer(1, 1000);
        }
        if (timer_flag[2] == 1) {
            setTimer(2, 250);
        }

        if (isButtonPressed(0) == 1) {
            status = MODE2;
        }
        break;


    case MANUAL_MODE_1:
        // If manual_step == 0, initialize
        if (manual_step == 0) {
            manual_step = 1;
            // ensure normal timers are stopped
            clearTimer(0); clearTimer(1); clearTimer(2);
            enterManualStep(manual_step);
            break;
        }

        // On each press of Button2, advance step
        if (isButtonPressed(1) == 1) {
            manual_step++;
            if (manual_step >= 1 && manual_step <= 3) {
                enterManualStep(manual_step);
            }
            else {
                // manual_step == 4 exit back to MODE1 normal
                manual_step = 0;
                status = MODE1;
            }
        }
        // remain in manual until 4th press
        break;

        /* ----------------------------
         * MODE2 — setup RED (original tuning)
         * ---------------------------- */
    case MODE2:
        status = AUTO_RED;
        //Set up RED LED
        setRed1();
        setRed2();

        index_led = 0;
        update7SEG(index_led);

        setTimer(2, 250);            // Set timer for 7SEG LED
        setTimer(3, 500);            // Set timer for blinking LED
        clearTimer(0);                // Clear timer display LED in MODE1
        clearTimer(1);                // Clear timer update buffer
        break;

    case AUTO_RED:
        setRed1();
        setRed2();
        if (timer_flag[2] == 1) {
            update7SEG(index_led);
            setTimer(2, 250);
        }
        if (timer_flag[3] == 1) {
            //            HAL_GPIO_TogglePin(R1_GPIO_Port, R1_Pin);
            //            HAL_GPIO_TogglePin(R2_GPIO_Port, R2_Pin);
            setTimer(3, 250);
        }
        if (isButtonPressed(0) == 1) {
            status = MODE3;
        }
        if (isButtonPressed(1) == 1) {
            status = INC_RED;
            if (RED >= 99) RED = GREEN + 1;
            else RED++;
        }
        break;

    case INC_RED:
        if (timer_flag[2] == 1) {
            setTimer(2, 250);
        }
        if (timer_flag[3] == 1) {
            //            HAL_GPIO_TogglePin(R1_GPIO_Port, R1_Pin);
            //            HAL_GPIO_TogglePin(R2_GPIO_Port, R2_Pin);
            setTimer(3, 250);
        }
        if (isButtonPressed(1) == 1) {
            // stay in INC_RED and increment
            if (RED >= 99) RED = GREEN + 1;
            else RED++;
            break;
        }
        if (isButtonPressed(2) == 1) {
            status = AUTO_RED;
            // GREEN = RED - AMBER; // commented as in original
            break;
        }
        break;

        // MODE3
    case MODE3:
        status = AUTO_AMBER;
        //Set up AMBER LED
        setAmber1();
        setAmber2();

        setTimer(2, 250);            // Set timer for 7SEG LED
        setTimer(3, 500);            // Set timer for blinking LED
        clearTimer(0);                // Clear timer display LED in MODE1
        clearTimer(1);                // Clear timer update buffer
        break;

    case AUTO_AMBER:
        if (timer_flag[2] == 1) {
            setTimer(2, 250);
        }
        if (timer_flag[3] == 1) {
            //            HAL_GPIO_TogglePin(A1_GPIO_Port, A1_Pin);
            //            HAL_GPIO_TogglePin(A2_GPIO_Port, A2_Pin);
            setTimer(3, 250);
        }
        if (isButtonPressed(0) == 1) {
            status = MODE4;
        }
        if (isButtonPressed(1) == 1) {
            status = INC_AMBER;
            if (AMBER >= 4) AMBER = 1;
            else AMBER++;

        }
        break;

    case INC_AMBER:
        if (timer_flag[2] == 1) {
            setTimer(2, 250);
        }
        if (timer_flag[3] == 1) {
            //            HAL_GPIO_TogglePin(A1_GPIO_Port, A1_Pin);
            //            HAL_GPIO_TogglePin(A2_GPIO_Port, A2_Pin);
            setTimer(3, 250);
        }
        if (isButtonPressed(1) == 1) {
            // stay in INC_AMBER and increment
            if (AMBER >= 4) AMBER = 1;
            else AMBER++;
        }
        if (isButtonPressed(2) == 1) {
            status = AUTO_AMBER;
            // keep original commented lines
            break;
        }
        break;

        // MODE4
    case MODE4:
        status = AUTO_GREEN;
        //Set up GREEN LED
        setGreen1();
        setGreen2();

        setTimer(2, 250);            // Set timer for 7SEG LED
        setTimer(3, 500);            // Set timer for blinking LED
        clearTimer(0);                // Clear timer display LED in MODE1
        clearTimer(1);                // Clear timer update buffer
        break;

    case AUTO_GREEN:
        if (timer_flag[2] == 1) {
            setTimer(2, 250);
        }
        if (timer_flag[3] == 1) {
            //            HAL_GPIO_TogglePin(G1_GPIO_Port, G1_Pin);
            //            HAL_GPIO_TogglePin(G2_GPIO_Port, G2_Pin);
            setTimer(3, 250);
        }
        if (isButtonPressed(0) == 1) {
            status = MODE1;
            if (GREEN <= AMBER) GREEN = AMBER + 1;
            RED = GREEN + AMBER;
        }
         if (isButtonPressed(1) == 1) {
            status = INC_GREEN;
            if (GREEN >= 99) GREEN = RED - AMBER;
            else GREEN++;
        }
        break;

    case INC_GREEN:
        if (timer_flag[2] == 1) {
            setTimer(2, 250);
        }
        if (timer_flag[3] == 1) {
            //            HAL_GPIO_TogglePin(G1_GPIO_Port, G1_Pin);
            //            HAL_GPIO_TogglePin(G2_GPIO_Port, G2_Pin);
            setTimer(3, 250);
        }
        if (isButtonPressed(1) == 1) {
            // stay and increment
            if (GREEN >= 99) GREEN = RED - AMBER;
            else GREEN++;
            break;
        }
        if (isButtonPressed(2) == 1) {
            status = AUTO_GREEN;
            // RED = GREEN + AMBER; // left commented as original
            break;
        }
        break;

    default:
        break;
    }
}
