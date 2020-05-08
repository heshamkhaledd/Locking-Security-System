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

void MODULES_init (void)
{
	UART_init ();
	LCD_init ();
	EEPROM_init ();
	TIMER_config ('A',25000,&g_s_T1_Aconfig);
	TIMER_config ('B',25000,&g_s_T1_Bconfig);
	SREG |= (1<<7);
}





/******************************************************************************
 *
 * Function Name: CONTROL_recievePassword
 *
 * Description:
 *
 *
 *
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

void CONTROL_storePassword (uint8 *password_1_Ptr)
{
	for (uint16 Idix=0 ; Idix<password_length ; Idix++)
	{
		EEPROM_writeByte (Idix,password_1_Ptr[Idix]);
		_delay_ms (10);
	}
}


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

void MOTOR_open (uint8 dir)
{
	MOTOR_init (dir);
	while (CW_flag == 0);
}


void MOTOR_close (uint8 dir)
{
	MOTOR_init (dir);
	while (CCW_flag == 0);
}


void MOTOR_stop (void)
{
	MOTOR_deinit ();
}

uint8 get_state (void)
{
	return ((UART_recieveByte ()));
}