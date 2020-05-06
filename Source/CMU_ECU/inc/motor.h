 /******************************************************************************
 *
 * Module: DC-Motor
 *
 * File Name: motor.h
 *
 * Description: A header file for DC-Motor driver
 *
 * Engineer: Hesham Khaled
 *
 *******************************************************************************/
#ifndef MOTOR_H_
#define MOTOR_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* Motor HW Pins */
#define MOTOR_DIR DDRB
#define MOTOR_PORT PORTB
#define MOTOR_PIN_A PB0
#define MOTOR_PIN_B PB1
#define CLOCKWISE 0
#define ANTICLOCKWISE 1

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void MOTOR_init (uint8);
void MOTOR_run (void);

#endif /* MOTOR_H_ */
