 /******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: motor.c
 *
 * Description: Source file for DC-Motor driver that initiate motor in the required
 * Direction, Clockwise or Anti-Clockwise
 *
 * Engineer: Hesham Khaled
 *
 *******************************************************************************/


#include "motor.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void MOTOR_init (uint8 direction)
{
	/* Motor control pins are set to output */

	SET_BIT (MOTOR_DIR,MOTOR_PIN_A);
	SET_BIT (MOTOR_DIR,MOTOR_PIN_A);

	/* Configure Motor to rotate in the required direction */
	if (direction == CLOCKWISE)
	{
		CLEAR_BIT (MOTOR_PORT,MOTOR_PIN_A);
		SET_BIT (MOTOR_PORT,MOTOR_PIN_B);
	}
	else if (direction == ANTICLOCKWISE)
	{
		SET_BIT (MOTOR_PORT,MOTOR_PIN_A);
		CLEAR_BIT (MOTOR_PORT,MOTOR_PIN_B);
	}
}
