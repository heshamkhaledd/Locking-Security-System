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
	UART_sendByte (READY);
	UART_receiveString (password_1);
	UART_sendByte (READY);
	UART_receiveString (password_2);
	check = strcmp (password_1,password_2);
	if (check == 0)
	{
		check = Idle;
		CONTROL_storePassword (password_1);
		UART_sendByte (1);
	}

	else
	{
		check = initial;
		UART_sendByte (0);
	}
	
	return check;
}

void CONTROL_storePassword (const uint8 *password_1_Ptr)
{
	uint16 Idix = 0;
	while ((*password_1_Ptr) != '\0' )
	{
		EEPROM_writeByte (Idix,password_1_Ptr[Idix]);
		Idix++;
	}
}


uint8 CONTROL_checkMatch (void)
{
	uint8 password[password_length+1];
	uint8 stored_password[password_length+1];
	UART_receiveString (password);

	for (uint16 Idix = 0 ; Idix < password_length ; Idix++)
		EEPROM_readByte (Idix,stored_password);

	if ((strcmp (password,stored_password)) == 0)
		{
			UART_sendByte (1);	
			return 1;
		}
	else
	{
		UART_sendByte (0);
		return 0;
	}
}

void MOTOR_run (void)
{
	static uint8 entry = 0;
	if (entry == 0)
	{
		entry++;
		MOTOR_init (CLOCKWISE);
		while (CW_flag == 0);
		CW_flag = 0;
	}
	else if (entry == 1)
	{
		entry = 0;
		MOTOR_init (ANTICLOCKWISE);
		while (CCW_flag == 0);
		CCW_flag = 0;
	}
}
