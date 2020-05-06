 /******************************************************************************
 *
 * File Name: hmi_layer.c
 *
 * Description: HMI source file to interface with external components and peripherals
 *
 * Engineer: Hesham Khaled
 *
 ******************************************************************************/


#include "hmi_layer.h"

uint8 open_flag = 0;
uint8 close_flag = 0;
extern Timer_Config g_s_T1_Aconfig;
extern Timer_Config g_s_T1_Bconfig;



void MODULES_init (void)
{
	LCD_init ();
	LCD_clearScreen ();
	UART_init ();
	TIMER_config ('A',25000,&g_s_T1_Aconfig);
	TIMER_config ('B',25000,&g_s_T1_Bconfig);
	SREG |= (1<<7);
}


/******************************************************************************
 *
 * Function Name: SYSTEM_mainMenu
 *
 * Description: A function that's responsible for displaying the main menu screen
 * that has the system two main options
 * + : For Changing the system's password
 * - : To open the door
 *
 *****************************************************************************/

void SYSTEM_mainMenu (void)
{
	LCD_clearScreen ();
	LCD_goToRowColumn (0,0);
	LCD_displayString ("+: Change PW");
	LCD_goToRowColumn (1,0);
	LCD_displayString ("-: Open Door");
}


/******************************************************************************
 *
 * Function Name: SYSTEM_setPassword
 *
 * Description: A function that's responsible for sending each character in the
 * password to the Control machine unit byte by byte (2nd ECU)
 *
 *****************************************************************************/

void SYSTEM_setPassword (void)
{
	static uint8 visit = 0;
	uint8 password_1[password_length+2];
	uint8 counter;
	
	if (visit == 0)
	{
		visit++;
		LCD_clearScreen ();
		LCD_goToRowColumn (0,0);
		LCD_displayString ("  **WELCOME**");
		_delay_ms (500);
	}

	LCD_clearScreen ();
	LCD_goToRowColumn (0,0);
	LCD_displayString ("Enter Password:");
	LCD_goToRowColumn (1,0);

	/* Iterate 5 times to send a 5 characters password, each
	/* character is sent with each iteration */

	for (counter=0;counter<password_length;counter++)
	{
		password_1[counter]= KeyPad_getPressedKey (); /*Get and record the pressed key on the KeyPad */
		_delay_ms (250);
		LCD_integerToString (password_1[counter]);
		_delay_ms (150);
		LCD_goToRowColumn (1,counter);
		LCD_displayCharacter ('*');
	}
	password_1[counter] = '#';
	counter++;
	password_1[counter] = '\0';
	while ((UART_recieveByte ()) != READY);
	UART_sendString (password_1); /* Sending the password's character to the 2nd ECU */
	LCD_clearScreen ();
}

/******************************************************************************
 *
 * Function Name: SYSTEM_confirmPassword
 *
 * Description: A function that's responsible for sending a confirmation password
 * to the Control machine unit (2nd ECU) byte by byte to match it with the first.
 *
 *****************************************************************************/

void SYSTEM_confirmPassword (void)
{
	LCD_goToRowColumn (0,0);
	LCD_displayString ("Confirm Password:");
	LCD_goToRowColumn (1,0);

	/* Iterate 5 times to send a 5 characters password, each
	/* character is sent with each iteration */
	uint8 counter;
	uint8 password_2[password_length+2];

	for (counter=0;counter<password_length;counter++)
	{
		password_2[counter]= KeyPad_getPressedKey (); /*Get and record the pressed key on the KeyPad */
		_delay_ms (250);
		LCD_integerToString (password_2[counter]);
		_delay_ms (150);
		LCD_goToRowColumn (1,counter);
		LCD_displayCharacter ('*');
	}
	password_2[counter] = '#';
	counter++;
	password_2[counter] = '\0';
	/*while ((UART_recieveByte()) != READY);*/
	UART_sendString (password_2); /* Sending the password's character to the 2nd ECU */
	LCD_clearScreen ();
}

/******************************************************************************
 *
 * Function Name: SYSTEM_checkMatching
 *
 * Description: A function that's responsible for checking if the first password
 * Entry matches the second entry (for confirmation)
 *
 *
 *****************************************************************************/

uint8 SYSTEM_checkMatching (void)
{
	if (UART_recieveByte == 0)
	{
		SYSTEM_errorMessage ();
		_delay_ms (50);
		return initial;
	}
	else
		return Idle;
}

/******************************************************************************
 *
 * Function Name: SYSTEM_enterPassword
 *
 * Description: A function that's responsible for entering the user's pre-setted
 * password to make a specific order
 *
 *
 *****************************************************************************/

void SYSTEM_enterPassword (void)
{
	LCD_clearScreen ();
	LCD_goToRowColumn (0,0);
	LCD_displayString ("Enter Password:");
	LCD_goToRowColumn (1,0);

	/* Iterate 5 times to send a 5 characters password, each
	/* character is sent with each iteration */
	uint8 counter;
	uint8 password[password_length+2];
	for (counter=0;counter<password_length;counter++)
	{
		password[counter] = KeyPad_getPressedKey (); /*Get and record the pressed key on the KeyPad */
		_delay_ms (250); /* To prevent pressing overlap */
		LCD_displayCharacter ('*');
	}
	password[counter] = '#';
	counter++;
	password[counter] = '\0';
	/*while ((UART_recieveByte ()) != READY);*/
	UART_sendString (password); /* Sending the password's character to the 2nd ECU */
	LCD_clearScreen ();
}

/******************************************************************************
 *
 * Function Name: SYSTEM_confirmMessage
 *
 * Description: A function that's responsible for displaying the door state on
 * the LCD, either it's opening or closing
 *
 *
 *****************************************************************************/

void SYSTEM_confirm_Open_Message (void)
{
		LCD_clearScreen ();
		LCD_displayString ("Door is now");
		LCD_goToRowColumn (1,0);
		LCD_displayString ("Opening...");
		while (open_flag == 0); /* Polling for 8 seconds until the door is opened */
		open_flag = 0;
}

void SYSTEM_confirm_Close_Message (void)
{	
	LCD_clearScreen ();
	LCD_displayString ("Door is now");
	LCD_goToRowColumn (1,0);
	LCD_displayString ("Closing...");
	while (close_flag == 0); /* Polling for 8 seconds until the door is closed */
	close_flag = 0;
}



/******************************************************************************
 *
 * Function Name: SYSTEM_errorMessage
 *
 * Description: A function that's responsible for displaying an error message
 * to the user if he pressed a wrong key on the main menu
 *
 *
 *****************************************************************************/

void SYSTEM_errorMessage (void)
{
	LCD_clearScreen ();
	LCD_displayString ("  **Invalid**");
	_delay_ms (500);
	LCD_sendCommand (0x08);
	_delay_ms (500);
	LCD_sendCommand (0x0C);
	_delay_ms (500);
	LCD_sendCommand (0x08);
	_delay_ms (500);
	LCD_sendCommand (0x0C);
	LCD_clearScreen ();
}

/******************************************************************************
 *
 * Function Name: SYSTEM_userChooseOption
 *
 * Description: A function that's responsible for taking only a valid input from
 * The user in the system's main menu.
 * Either,
 * '+' : Change Password
 * '-' : Open Door
 *
 *****************************************************************************/

uint8 SYSTEM_userChooseOption (void)
{
	uint8 pressed_key;
	pressed_key = KeyPad_getPressedKey ();
	_delay_ms (250);
	if ((pressed_key == '+') || (pressed_key == '-'))
	{
		if (pressed_key == '+')
		{
			UART_sendByte (CHG_PW);
			return CHG_PW;
		}
		else if (pressed_key == '-')
		{
			UART_sendByte (O_DOOR);
			return O_DOOR;
		}
	}
		return -1;
}