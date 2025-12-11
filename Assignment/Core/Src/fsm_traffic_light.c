/*
 * fsm_traffic_light.c
 *
 */

#include "fsm_traffic_light.h"

int status = INIT;
int manual_step = 0;


static void setNormalLedsForMode1(){
    HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, RESET);
    HAL_GPIO_WritePin(A1_GPIO_Port, A1_Pin, SET);
    HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, SET);

    HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, SET);
    HAL_GPIO_WritePin(A2_GPIO_Port, A2_Pin, SET);
    HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, RESET);
}


static void enterManualStep(int step){
    if (step == 1){
        // Lane1 GREEN
        HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, SET);
        HAL_GPIO_WritePin(A1_GPIO_Port, A1_Pin, SET);
        HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, RESET);

        // Lane2 RED
        HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, RESET);
        HAL_GPIO_WritePin(A2_GPIO_Port, A2_Pin, SET);
        HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, SET);
    }
    else if (step == 2){
        // Lane1 RED
        HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, RESET);
        HAL_GPIO_WritePin(A1_GPIO_Port, A1_Pin, SET);
        HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, SET);

        // Lane2 GREEN
        HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, SET);
        HAL_GPIO_WritePin(A2_GPIO_Port, A2_Pin, SET);
        HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, RESET);
    }
    else if (step == 3){
        // Both AMBER (steady)
        HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, SET);
        HAL_GPIO_WritePin(A1_GPIO_Port, A1_Pin, RESET);
        HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, SET);

        HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, SET);
        HAL_GPIO_WritePin(A2_GPIO_Port, A2_Pin, RESET);
        HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, SET);
    }
}

void enterManualMode(){
    // previous semantics (Lane1 RED, Lane2 GREEN)
    HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, RESET);
    HAL_GPIO_WritePin(A1_GPIO_Port, A1_Pin, SET);
    HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, SET);

    HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, SET);
    HAL_GPIO_WritePin(A2_GPIO_Port, A2_Pin, SET);
    HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, RESET);
}

void fsm_traffic_light(){
    switch(status){

    // INIT
    case INIT:
        status = MODE1;
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
        if (isButtonPressed(1) == 1){
            status = MANUAL_MODE_1;;
            manual_step = 1;
            // clear normal timers to avoid interference
            clearTimer(0);
            clearTimer(1);
            clearTimer(2);
            enterManualStep(manual_step);
            break;
        }

        if (isButtonPressed(3) == 1){
             status = RED_AMBER;        // Chuyển trạng thái kế tiếp
             setTimer(0, AMBER * 1000); // Cài đặt thời gian cho đèn Vàng
             updateLedBufferMode1();    // Cập nhật hiển thị (tùy chọn)
             setTimer(1, 1000);         // Reset bộ đếm giây
        }
        HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, RESET);
        HAL_GPIO_WritePin(A1_GPIO_Port, A1_Pin, SET);
        HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, SET);
        HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, RESET);

        if (timer_flag[0] == 1){
            status = RED_AMBER;
            setTimer(0, AMBER * 1000);
        }
        if (timer_flag[1] == 1){
            updateLedBufferMode1();
            setTimer(1, 1000);
        }
        if (timer_flag[2] == 1){
            update7SEG(index_led);
            setTimer(2, 250);
        }

        if (isButtonPressed(0) == 1){
            status = MODE2;
        }
        break;

    // RED_AMBER
    case RED_AMBER:
        if (isButtonPressed(1) == 1){
            status = MANUAL_MODE_1;
            manual_step = 1;
            clearTimer(0); clearTimer(1); clearTimer(2);
            enterManualStep(manual_step);
            break;
        }

        HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, SET);
        HAL_GPIO_WritePin(A2_GPIO_Port, A2_Pin, RESET);

        if (timer_flag[0] == 1){
            status = GREEN_RED;
            setTimer(0, GREEN * 1000);
        }
        if (timer_flag[1] == 1){
            updateLedBufferMode1();
            setTimer(1, 1000);
        }
        if (timer_flag[2] == 1){
            update7SEG(index_led);
            setTimer(2, 250);
        }

        if (isButtonPressed(0) == 1){
            status = MODE2;
        }
        break;

    // GREEN_RED
    case GREEN_RED:
        // Button2 -> enter MANUAL_MODE
        if (isButtonPressed(1) == 1){
            status = MANUAL_MODE_1;
            manual_step = 1;
            clearTimer(0); clearTimer(1); clearTimer(2);
            enterManualStep(manual_step);
            break;
        }

        // Nếu bấm nút đi bộ khi đang Xanh, chuyển ngay sang Vàng
        if (isButtonPressed(3) == 1){
             status = AMBER_RED;        // Chuyển trạng thái kế tiếp
             setTimer(0, AMBER * 1000); // Cài đặt thời gian cho đèn Vàng
             updateLedBufferMode1();
             setTimer(1, 1000);
        }
        // Set up LED
        HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, SET);
        HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, RESET);
        HAL_GPIO_WritePin(A2_GPIO_Port, A2_Pin, SET);
        HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, RESET);

        if (timer_flag[0] == 1){
            status = AMBER_RED;
            setTimer(0, AMBER * 1000);
        }
        if (timer_flag[1] == 1){
            updateLedBufferMode1();
            setTimer(1, 1000);
        }
        if (timer_flag[2] == 1){
            update7SEG(index_led);
            setTimer(2, 250);
        }

        if (isButtonPressed(0) == 1){
            status = MODE2;
        }
        break;

    // AMBER_RED
    case AMBER_RED:
        // Button2 -> enter MANUAL_MODE
        if (isButtonPressed(1) == 1){
            status = MANUAL_MODE_1;
            manual_step = 1;
            clearTimer(0); clearTimer(1); clearTimer(2);
            enterManualStep(manual_step);
            break;
        }

        // Set up LED
        HAL_GPIO_WritePin(A1_GPIO_Port, A1_Pin, RESET);
        HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, SET);

        if (timer_flag[0] == 1){
            status = RED_GREEN;
            setTimer(0, GREEN * 1000);
        }
        if (timer_flag[1] == 1){
            updateLedBufferMode1();
            setTimer(1, 1000);
        }
        if (timer_flag[2] == 1){
            update7SEG(index_led);
            setTimer(2, 250);
        }

        if (isButtonPressed(0) == 1){
            status = MODE2;
        }
        break;


    case MANUAL_MODE_1:
        // If manual_step == 0, initialize
        if (manual_step == 0){
            manual_step = 1;
            // ensure normal timers are stopped
            clearTimer(0); clearTimer(1); clearTimer(2);
            enterManualStep(manual_step);
            break;
        }

        // On each press of Button2, advance step
        if (isButtonPressed(1) == 1){
            manual_step++;
            if (manual_step >= 1 && manual_step <= 3){
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
        HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, RESET);
        HAL_GPIO_WritePin(A1_GPIO_Port, A1_Pin, SET);
        HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, SET);
        HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, RESET);
        HAL_GPIO_WritePin(A2_GPIO_Port, A2_Pin, SET);
        HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, SET);

        index_led = 0;
        updateLedBufferMode2();
        update7SEG(index_led);

        setTimer(2, 250);            // Set timer for 7SEG LED
        setTimer(3, 500);            // Set timer for blinking LED
        clearTimer(0);                // Clear timer display LED in MODE1
        clearTimer(1);                // Clear timer update buffer
        break;

    case AUTO_RED:
        if (timer_flag[2] == 1){
            updateLedBufferMode2();
            update7SEG(index_led);
            setTimer(2, 250);
        }
        if (timer_flag[3] == 1){
            HAL_GPIO_TogglePin(R1_GPIO_Port, R1_Pin);
            HAL_GPIO_TogglePin(R2_GPIO_Port, R2_Pin);
            setTimer(3, 250);
        }
        if (isButtonPressed(0) == 1){
            status = MODE3;
        }
        if (isButtonPressed(1) == 1){
            status = INC_RED;
            if (RED >= 99) RED = GREEN + 1;
            else RED++;
        }
        break;

    case INC_RED:
        if (timer_flag[2] == 1){
            updateLedBufferMode2();
            update7SEG(index_led);
            setTimer(2, 250);
        }
        if (timer_flag[3] == 1){
            HAL_GPIO_TogglePin(R1_GPIO_Port, R1_Pin);
            HAL_GPIO_TogglePin(R2_GPIO_Port, R2_Pin);
            setTimer(3, 250);
        }
        if (isButtonPressed(1) == 1){
            // stay in INC_RED and increment
            if (RED >= 99) RED = GREEN + 1;
            else RED++;
        }
        if (isButtonPressed(2) == 1){
            status = AUTO_RED;
            // GREEN = RED - AMBER; // commented as in original
        }
        break;

    // MODE3
    case MODE3:
        status = AUTO_AMBER;
        //Set up AMBER LED
        HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, SET);
        HAL_GPIO_WritePin(A1_GPIO_Port, A1_Pin, RESET);
        HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, SET);
        HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, SET);
        HAL_GPIO_WritePin(A2_GPIO_Port, A2_Pin, RESET);
        HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, SET);

        index_led = 0;
        updateLedBufferMode3();
        update7SEG(index_led);

        setTimer(2, 250);            // Set timer for 7SEG LED
        setTimer(3, 500);            // Set timer for blinking LED
        clearTimer(0);                // Clear timer display LED in MODE1
        clearTimer(1);                // Clear timer update buffer
        break;

    case AUTO_AMBER:
        if (timer_flag[2] == 1){
            updateLedBufferMode3();
            update7SEG(index_led);
            setTimer(2, 250);
        }
        if (timer_flag[3] == 1){
            HAL_GPIO_TogglePin(A1_GPIO_Port, A1_Pin);
            HAL_GPIO_TogglePin(A2_GPIO_Port, A2_Pin);
            setTimer(3, 250);
        }
        if (isButtonPressed(0) == 1){
            status = MODE4;
        }
        if (isButtonPressed(1) == 1){
            status = INC_AMBER;
            if (AMBER >= 4) AMBER = 1;
            else AMBER++;
        }
        break;

    case INC_AMBER:
        if (timer_flag[2] == 1){
            updateLedBufferMode3();
            update7SEG(index_led);
            setTimer(2, 250);
        }
        if (timer_flag[3] == 1){
            HAL_GPIO_TogglePin(A1_GPIO_Port, A1_Pin);
            HAL_GPIO_TogglePin(A2_GPIO_Port, A2_Pin);
            setTimer(3, 250);
        }
        if (isButtonPressed(1) == 1){
            // stay in INC_AMBER and increment
            if (AMBER >= 4) AMBER = 1;
            else AMBER++;
        }
        if (isButtonPressed(2) == 1){
            status = AUTO_AMBER;
            // keep original commented lines
        }
        break;

    // MODE4
    case MODE4:
        status = AUTO_GREEN;
        //Set up GREEN LED
        HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, SET);
        HAL_GPIO_WritePin(A1_GPIO_Port, A1_Pin, SET);
        HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, RESET);
        HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, SET);
        HAL_GPIO_WritePin(A2_GPIO_Port, A2_Pin, SET);
        HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, RESET);

        index_led = 0;
        updateLedBufferMode4();
        update7SEG(index_led);

        setTimer(2, 250);            // Set timer for 7SEG LED
        setTimer(3, 500);            // Set timer for blinking LED
        clearTimer(0);                // Clear timer display LED in MODE1
        clearTimer(1);                // Clear timer update buffer
        break;

    case AUTO_GREEN:
        if (timer_flag[2] == 1){
            updateLedBufferMode4();
            update7SEG(index_led);
            setTimer(2, 250);
        }
        if (timer_flag[3] == 1){
            HAL_GPIO_TogglePin(G1_GPIO_Port, G1_Pin);
            HAL_GPIO_TogglePin(G2_GPIO_Port, G2_Pin);
            setTimer(3, 250);
        }
        if (isButtonPressed(0) == 1){
            status = MODE1;
            if (GREEN <= AMBER) GREEN = AMBER + 1;
            RED = GREEN + AMBER;
        }
        if (isButtonPressed(1) == 1){
            status = INC_GREEN;
            if (GREEN >= 99) GREEN = RED - AMBER;
            else GREEN++;
        }
        break;

    case INC_GREEN:
        if (timer_flag[2] == 1){
            updateLedBufferMode4();
            update7SEG(index_led);
            setTimer(2, 250);
        }
        if (timer_flag[3] == 1){
            HAL_GPIO_TogglePin(G1_GPIO_Port, G1_Pin);
            HAL_GPIO_TogglePin(G2_GPIO_Port, G2_Pin);
            setTimer(3, 250);
        }
        if (isButtonPressed(1) == 1){
            // stay and increment
            if (GREEN >= 99) GREEN = RED - AMBER;
            else GREEN++;
        }
        if (isButtonPressed(2) == 1){
            status = AUTO_GREEN;
            // RED = GREEN + AMBER; // left commented as original
        }
        break;

    default:
        break;
    }
}
