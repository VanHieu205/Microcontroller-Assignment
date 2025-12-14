/*
 * fsm_traffic_light.c
 */

#include "fsm_traffic_light.h"

int status = INIT;
int manual_step = 0;

void updateLCD() {
    char str_buff[16]; // Buffer để chứa chuỗi hiển thị

    switch (status) {
        // --- TRƯỜNG HỢP MODE 1: CHẠY TỰ ĐỘNG ---
        case MODE1:
        case RED_GREEN:
        case RED_AMBER:
        case GREEN_RED:
        case AMBER_RED:
            lcd_goto_XY(0, 0);
            lcd_send_string("MODE 1: AUTO    "); // Khoảng trắng để xóa chữ cũ

            lcd_goto_XY(1, 0);
            // Hiển thị trạng thái đèn hiện tại
            if (status == RED_GREEN)      lcd_send_string("L1:RED  L2:GREEN");
            else if (status == RED_AMBER) lcd_send_string("L1:RED  L2:AMBER");
            else if (status == GREEN_RED) lcd_send_string("L1:GREEN  L2:RED");
            else if (status == AMBER_RED) lcd_send_string("L1:AMBER  L2:RED");
            else                          lcd_send_string("    STARTING    ");
            break;

        // --- TRƯỜNG HỢP MODE 2: CHỈNH TIME ĐÈN ĐỎ ---
        case MODE2:
        case AUTO_RED:
        case INC_RED:
            lcd_goto_XY(0, 0);
            lcd_send_string("MODE 2: SET RED ");

            lcd_goto_XY(1, 0);
            sprintf(str_buff, "Time: %d s      ", RED); // Hiển thị giá trị RED
            lcd_send_string(str_buff);
            break;

        // --- TRƯỜNG HỢP MODE 3: CHỈNH TIME ĐÈN VÀNG ---
        case MODE3:
        case AUTO_AMBER:
        case INC_AMBER:
            lcd_goto_XY(0, 0);
            lcd_send_string("MODE 3: SET AMBER");

            lcd_goto_XY(1, 0);
            sprintf(str_buff, "Time: %d s      ", AMBER); // Hiển thị giá trị AMBER
            lcd_send_string(str_buff);
            break;

        // --- TRƯỜNG HỢP MODE 4: CHỈNH TIME ĐÈN XANH ---
        case MODE4:
        case AUTO_GREEN:
        case INC_GREEN:
            lcd_goto_XY(0, 0);
            lcd_send_string("MODE 4: SET GREEN");

            lcd_goto_XY(1, 0);
            sprintf(str_buff, "Time: %d s      ", GREEN); // Hiển thị giá trị GREEN
            lcd_send_string(str_buff);
            break;

        // --- TRƯỜNG HỢP MANUAL MODE ---
        case MANUAL_MODE_1:
        case MANUAL_MODE_2:
        case MANUAL_MODE_3:
            lcd_goto_XY(0, 0);
            lcd_send_string("  MANUAL MODE   ");

            lcd_goto_XY(1, 0);
            if(manual_step == 1)      lcd_send_string("Step: GRN - RED ");
            else if(manual_step == 2) lcd_send_string("Step: RED - GRN ");
            else if(manual_step == 3) lcd_send_string("Step: AMB - AMB ");
            break;

        default:
            break;
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
	HAL_GPIO_WritePin(BIT3_GPIO_Port, BIT3_Pin, SET);
	HAL_GPIO_WritePin(BIT4_GPIO_Port, BIT4_Pin, SET);
}
void setGreen2() {
	HAL_GPIO_WritePin(BIT3_GPIO_Port, BIT3_Pin, RESET);
	HAL_GPIO_WritePin(BIT4_GPIO_Port, BIT4_Pin, SET);
}
void setAmber2() {
	HAL_GPIO_WritePin(BIT3_GPIO_Port, BIT3_Pin, SET);
	HAL_GPIO_WritePin(BIT4_GPIO_Port, BIT4_Pin, RESET);
}

static void setNormalLedsForMode1(){
    setRed1();
    setGreen2();
}

static void enterManualStep(int step){
    if (step == 1){
        setGreen1();
        setRed2();
    }
    else if (step == 2){
        setRed1();
        setGreen2();
    }
    else if (step == 3){
        setAmber1();
        setAmber2();
    }
}

void enterManualMode(){
    setRed1();
    setGreen2();
}

void fsm_traffic_light(){
    switch(status){

    case INIT:
        status = MODE1;
        break;

    // MODE1
    case MODE1:
        status = RED_GREEN;
        setNormalLedsForMode1();

        setTimer(0, GREEN * 1000);   // only timer left
        break;

    // RED_GREEN
    case RED_GREEN:

        if (isButtonPressed(1) == 1){
            status = MANUAL_MODE_1;
            manual_step = 1;
            clearTimer(0);
            enterManualStep(manual_step);
            break;
        }

        if (isButtonPressed(3) == 1){
            status = RED_AMBER;
            setTimer(0, AMBER * 1000);
        }

        setRed1();
        setGreen2();

        if (timer_flag[0] == 1){
            status = RED_AMBER;
            setTimer(0, AMBER * 1000);
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
            clearTimer(0);
            enterManualStep(manual_step);
            break;
        }

        setAmber2();

        if (timer_flag[0] == 1){
            status = GREEN_RED;
            setTimer(0, GREEN * 1000);
        }

        if (isButtonPressed(0) == 1){
            status = MODE2;
        }

        break;

    // GREEN_RED
    case GREEN_RED:

        if (isButtonPressed(1) == 1){
            status = MANUAL_MODE_1;
            manual_step = 1;
            clearTimer(0);
            enterManualStep(manual_step);
            break;
        }

        if (isButtonPressed(3) == 1){
            status = AMBER_RED;
            setTimer(0, AMBER * 1000);
        }

        setGreen1();
        setRed2();

        if (timer_flag[0] == 1){
            status = AMBER_RED;
            setTimer(0, AMBER * 1000);
        }

        if (isButtonPressed(0) == 1){
            status = MODE2;
        }
        break;

    // AMBER_RED
    case AMBER_RED:

        if (isButtonPressed(1) == 1){
            status = MANUAL_MODE_1;
            manual_step = 1;
            clearTimer(0);
            enterManualStep(manual_step);
            break;
        }

        setAmber1();
        setRed2();

        if (timer_flag[0] == 1){
            status = RED_GREEN;
            setTimer(0, GREEN * 1000);
        }

        if (isButtonPressed(0) == 1){
            status = MODE2;
        }
        break;

    // MANUAL_MODE_1
    case MANUAL_MODE_1:

        if (manual_step == 0){
            manual_step = 1;
            clearTimer(0);
            enterManualStep(manual_step);
            break;
        }

        if (isButtonPressed(1) == 1){
            manual_step++;
            if (manual_step >= 1 && manual_step <= 3){
                enterManualStep(manual_step);
            }
            else {
                manual_step = 0;
                status = MODE1;
            }
        }
        break;

    /* MODE2 */
    case MODE2:
        status = AUTO_RED;
        setRed1();
        setRed2();
        break;

    case AUTO_RED:

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


        // đọc nút liên tục
        if (isButtonPressed(2) == 1){
            status = AUTO_RED;
            break;
        }

        if (isButtonPressed(1) == 1){
            if (RED >= 99) RED = GREEN + 1;
            else RED++;
        }

        break;


    /* MODE3 */
    case MODE3:
        status = AUTO_AMBER;
        setAmber1();
        setAmber2();
        break;

    case AUTO_AMBER:

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
        if (isButtonPressed(2) == 1){
            status = AUTO_AMBER;
            break;
        }
        if (isButtonPressed(1) == 1){
            if (AMBER >= 4) AMBER = 1;
            else AMBER++;
        }


        break;

    /* MODE4 */
    case MODE4:
        status = AUTO_GREEN;
        setGreen1();
        setGreen2();
        break;

    case AUTO_GREEN:

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
        if (isButtonPressed(2) == 1){
            status = AUTO_GREEN;
            break;
        }
        if (isButtonPressed(1) == 1){
            if (GREEN >= 99) GREEN = RED - AMBER;
            else GREEN++;
        }


        break;

    default:
        break;
    }
}
