/*
 * fsm_traffic_light.h
 *
*  Created on: OCT 26, 2025
 *      Author: hanh
 */

#ifndef INC_FSM_TRAFFIC_LIGHT_H_
#define INC_FSM_TRAFFIC_LIGHT_H_

#include "global.h"
#include "button.h"
#include "main.h"
#include "i2c-lcd.h"
#include "software_timer.h"
#include <stdio.h>
void fsm_traffic_light();
void updateLCD();
#endif /* INC_FSM_TRAFFIC_LIGHT_H_ */
