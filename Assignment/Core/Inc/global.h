/*
 * global.h
 *
 *  Created on: OCT 26, 2025
 *
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "stdint.h"
//========================
// SYSTEM STATES
//========================
#define INIT                0
#define MODE1               1
#define MODE2               2
#define MODE3               3
#define MODE4               4

// Manual Mode (4-step cycle when pressing Button2)
#define MANUAL_MODE_1       50     // Lane1 RED – Lane2 GREEN
#define MANUAL_MODE_2       51     // Lane1 GREEN – Lane2 RED
#define MANUAL_MODE_3       52     // All AMBER
#define MANUAL_MODE_EXIT    53     // Return to MODE1

//========================
// NORMAL TRAFFIC STATES
//========================
#define RED_GREEN           11
#define RED_AMBER           12
#define GREEN_RED           13
#define AMBER_RED           14

//========================
// MODE2 – MODE4 STATES (Setup RED / AMBER / GREEN)
//========================
#define AUTO_RED            21
#define INC_RED             22

#define AUTO_AMBER          31
#define INC_AMBER           32

#define AUTO_GREEN          41
#define INC_GREEN           42

//========================
// GLOBAL VARIABLES
//========================
extern int status;

extern int RED;
extern int AMBER;
extern int GREEN;
extern int pedestrian_request;

extern uint32_t timestamp;
#endif /* INC_GLOBAL_H_ */
