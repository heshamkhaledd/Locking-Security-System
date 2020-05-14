 /******************************************************************************
 *
 * File Name: hmi_layer.h
 *
 * Description: HMI header file to interface with external components and peripherals
 *
 * Engineer: Hesham Khaled
 *
 ******************************************************************************/


#ifndef HMI_LAYER_H_
#define HMI_LAYER_H_
#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"
#include "keypad.h"
#include "lcd.h"
#include "uart.h"
#include "timer.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/


/* PASS specs */
#define password_length 5
#define MAX_TRY         3


/* SYSTEM states */
#define initial         1
#define Idle            2
#define CHG_PW          3
#define O_DOOR          4
#define WRONG           0


/* UART states */
#define READY           1
#define NOT_READY       0


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

uint8 SYSTEM_checkMatching (void);
uint8 SYSTEM_userChooseOption (void);
uint8 SYSTEM_enterPassword (void);
void MODULES_init (void);
void SYSTEM_mainMenu (void);
void SYSTEM_setPassword (void);
void SYSTEM_confirmPassword (void);
void SYSTEM_confirm_Open_Message (void);
void SYSTEM_confirm_Close_Message (void);
void SYSTEM_errorMessage (void);
void SYSTEM_confirmSave (void);

#endif /* HMI_LAYER_H_ */