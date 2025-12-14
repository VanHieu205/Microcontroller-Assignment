#include "button.h"

#define LONGPRESS_DURATION   100   // 100 * 10ms = 1s
#define LONGPRESS_REPEAT     20    // 20 * 10ms = 200ms

static int KeyReg0[NUM_OF_BUTTONS];
static int KeyReg1[NUM_OF_BUTTONS];
static int KeyReg2[NUM_OF_BUTTONS];
static int KeyReg3[NUM_OF_BUTTONS];

static int button_flag[NUM_OF_BUTTONS] = {0};
static int longpress_timer[NUM_OF_BUTTONS] = {0};
static int longpress_active[NUM_OF_BUTTONS] = {0};

int isButtonPressed(int index){
    if (button_flag[index] == 1){
        button_flag[index] = 0;
        return 1;
    }
    return 0;
}

static void buttonGenerateEvent(int index){
    button_flag[index] = 1;
}

void getKeyInput(){
    for (int i = 0; i < NUM_OF_BUTTONS; i++){
        KeyReg2[i] = KeyReg1[i];
        KeyReg1[i] = KeyReg0[i];

        if (i == 0)
            KeyReg0[i] = HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_Pin);
        else if (i == 1)
            KeyReg0[i] = HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin);
        else if (i == 2)
            KeyReg0[i] = HAL_GPIO_ReadPin(BTN3_GPIO_Port, BTN3_Pin);
        else
            KeyReg0[i] = HAL_GPIO_ReadPin(BTN4_GPIO_Port, BTN4_Pin);

        // Debounce ổn định
        if (KeyReg0[i] == KeyReg1[i] && KeyReg1[i] == KeyReg2[i]){
            if (KeyReg3[i] != KeyReg2[i]){   // có sự thay đổi trạng thái
                KeyReg3[i] = KeyReg2[i];

                if (KeyReg3[i] == PRESSED_STATE){
                    buttonGenerateEvent(i);
                    longpress_timer[i] = LONGPRESS_DURATION;
                    longpress_active[i] = 0;
                }
            }
            else {
                // Nếu nút đang được giữ
                if (KeyReg3[i] == PRESSED_STATE){
                    if (longpress_timer[i] > 0){
                        longpress_timer[i]--;
                    }
                    else {
                        // long press lặp
                        buttonGenerateEvent(i);
                        longpress_timer[i] = LONGPRESS_REPEAT;
                        longpress_active[i] = 1;
                    }
                }
            }
        }
        else {
            // reset long press nếu nhả nút
            longpress_active[i] = 0;
        }
    }
}


static void forcePress(int i){
    KeyReg0[i] = KeyReg1[i] = KeyReg2[i] = PRESSED_STATE;
    if (KeyReg3[i] != PRESSED_STATE){
        KeyReg3[i] = PRESSED_STATE;
        buttonGenerateEvent(i);
        longpress_timer[i] = LONGPRESS_DURATION;
        longpress_active[i] = 0;
    }
}

static void forceRelease(int i){
    KeyReg0[i] = KeyReg1[i] = KeyReg2[i] = NORMAL_STATE;
    KeyReg3[i] = NORMAL_STATE;
    longpress_active[i] = 0;
}

void toggleButton0(){ forcePress(0); forceRelease(0); }
void toggleButton1(){ forcePress(1); forceRelease(1); }
void toggleButton2(){ forcePress(2); forceRelease(2); }
void toggleButton3(){ forcePress(3); forceRelease(3); }
