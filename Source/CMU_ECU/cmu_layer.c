 /******************************************************************************
 *
 * File Name: cmu_layer.c
 *
 * Description: CMU source file that has the main ECU control functions
 *
 * Engineer: Hesham Khaled
 *
 ******************************************************************************/

#include "cmu_layer.h"
#include "string.h"


extern Timer_Config g_s_T1_Aconfig;
extern Timer_Config g_s_T1_Bconfig;
uint8 CW_flag = 0;
uint8 CCW_flag = 0;


/******************************************************************************
 *
 * Function Name: MODULES_init
 *
 * Description: A function that's responsible for initializing all the required
 * Modules that are used in the Human Machine Interface ECU
 * 
 * Input: void
 * Output: void
 * 
 *****************************************************************************/

void MODULES_init (void)
{
	UART_init ();
	EEPROM_init ();
	TIMER_config ('A',65500,&g_s_T1_Aconfig);
	TIMER_config ('B',65500,&g_s_T1_Bconfig);
	SREG |= (1<<7);
	SET_BIT (MOTOR_DIR,MOTOR_PIN_A);
	SET_BIT (MOTOR_DIR,MOTOR_PIN_B);
	CLEAR_BIT (MOTOR_PORT,MOTOR_PIN_A);
	CLEAR_BIT (MOTOR_PORT,MOTOR_PIN_B);
	SET_BIT (DDRD,PD7);
	CLEAR_BIT (PORTD,PD7);
}





/******************************************************************************
 *
 * Function Name: CONTROL_setRecievePassword
 *
 * Description: Function responsible for receiving both the initial and confirmatory
 * Passwords sent from the The 1st ECU and storing them in temporary strings and
 * Check for a match. If they are matched, Store them in the External EEPROM.
 * If not, Keep waiting until the user enters a valid initial/confirmatory passwords.
 * 
 * Input: void
 * Output: uint8
 * 
 *****************************************************************************/

uint8 CONTROL_setReceivePassword (void)
{
	uint8 password_1[password_length+1];
	uint8 password_2[password_length+1];
	uint8 check;
	uint8 state;
	while ((UART_recieveByte ()) != READY);
	UART_receiveString (password_1);
	while ((UART_recieveByte ()) != READY);
	UART_receiveString (password_2);
	check = strcmp (password_1,password_2);
	if (check == 0)
	{
		check = Idle;
		CONTROL_storePassword (password_1);
		while ((UART_recieveByte ()) != READY);
		UART_sendByte (Idle);
	}

	else
	{
		check = WRONG;
		while ((UART_recieveByte ()) != READY);
		UART_sendByte (WRONG);
	}
	return check;
}

/******************************************************************************
 *
 * Function Name: CONTROL_storePassword
 *
 * Description: Function responsible for storing the password in the external EEPROM
 * 
 * Input: uint8 *
 * Output: void
 * 
 *****************************************************************************/


void CONTROL_storePassword (uint8 *password_1_Ptr)
{
	for (uint16 Idix=0 ; Idix<password_length ; Idix++)
	{
		EEPROM_writeByte (Idix,password_1_Ptr[Idix]);
		_delay_ms (10);
	}
}


/******************************************************************************
 *
 * Function Name: CONTROL_checkMatch
 *
 * Description: Function responsible for checking matches with any entered password
 * And the stored password in the EEPROM.
 * 
 * Input: void
 * Output: uint8
 * 
 *****************************************************************************/

uint8 CONTROL_checkMatch (void)
{
	uint8 Idix;
	uint8 password[password_length+1];
	uint8 stored_password[password_length+1];
	while ((UART_recieveByte ()) != READY);
	UART_receiveString (password);
	for(Idix=0 ; Idix<password_length ; Idix++)
    {
     	EEPROM_readByte(Idix,&stored_password[Idix]);
     	_delay_ms(5);
    }
	stored_password[Idix] = '\0';

	if ((strcmp (password,stored_password)) == 0)
		{
			while ((UART_recieveByte ()) != READY);
			UART_sendByte (Idle);
			return Idle;
		}
	else
	{
		while ((UART_recieveByte ()) != READY);
		UART_sendByte (WRONG);
		return WRONG;
	}
}


/******************************************************************************
 *
 * Function Name: CONTROL_alert
 *
 * Description: Function responsible for toggling the buzzer in the case 
 * of a theft or breaking.
 * 
 * Input: void
 * Output: void
 * 
 *****************************************************************************/


void CONTROL_alert (void)
{
	while (1)
	{
		Toggle_buzzer ();
		_delay_ms (500);
	}
}

/******************************************************************************
 *
 * Function Name: MOTOR_<state>
 *
 * Description: Function responsible the motor mechanics, as it's either
 * Opening ---> CW Direction
 * Closing ---> CCW Direction
 * At Rest
 * 
 * Input: void
 * Output: void
 * 
 *****************************************************************************/

void MOTOR_open (void)
{
	MOTOR_init (CLOCKWISE);
	while (CW_flag == 0); /* Polling for 8 seconds until the door is opened */
}


void MOTOR_close (void)
{
	MOTOR_init (ANTICLOCKWISE);
	while (CCW_flag == 0); /* Polling for 8 seconds until the door is closed */
}


void MOTOR_stop (void)
{
	MOTOR_deinit ();
}


/******************************************************************************
 *
 * Function Name: CONTROL_getState
 *
 * Description: Function responsible for receiving the state at which the ECU
 * Must operate in.
 * 
 * Input: void
 * Output: uint8
 * 
 *****************************************************************************/

uint8 CONTROL_getState (void)
{
	return ((UART_recieveByte ()));
}