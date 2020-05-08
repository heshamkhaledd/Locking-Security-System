 /******************************************************************************
 *
 * File Name: cmu_layer.h
 *
 * Description: cmu header file to interface with external components and peripherals
 *
 * Engineer: Hesham Khaled
 *
 ******************************************************************************/

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"
#include "external_eeprom.h"
#include "timer.h"
#include "uart.h"
#include "motor.h"
#include "lcd.h"


#ifndef HMI_LAYER_H_
#define HMI_LAYER_H_

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* PASS specs */
#define password_length 1
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

#define ADDRESS 10


/*******************************************************************************
 *                      Functions Prototypes                                   *
*******************************************************************************/

uint8 CONTROL_setReceivePassword (void);
uint8 CONTROL_checkMatch (void);
uint8 get_state (void);
void MODULES_init (void);
void CONTROL_storePassword (uint8 *password_1_Ptr);
void MOTOR_open (uint8);
void MOTOR_close (uint8);
void MOTOR_stop (void);


#endif /* HMI_LAYER_H_ */
