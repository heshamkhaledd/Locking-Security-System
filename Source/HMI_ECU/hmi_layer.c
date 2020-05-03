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
	LCD_clearScreen ();
	LCD_goToRowColumn (0,0);
	LCD_displayString ("Enter Password:");
	LCD_goToRowColumn (1,0);

	/* Iterate 5 times to send a 5 characters password, each
	/* character is sent with each iteration */
	uint8 counter;
	for (counter=0;counter<password_length;counter++)
	{
		uint8 pressed_key;
		pressed_key = KeyPad_getPressedKey (); /*Get and record the pressed key on the KeyPad */
		_delay_ms (200);
		LCD_displayCharacter ('*');
		/*UART_sendByte (pressed_key); */ /* Sending the password's character to the 2nd ECU */
	}
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
	LCD_clearScreen ();
	LCD_goToRowColumn (0,0);
	LCD_displayString ("Confirm Password:");
	LCD_goToRowColumn (1,0);

	/* Iterate 5 times to send a 5 characters password, each
	 * character is sent with each iteration */
	uint8 counter;
	for (counter=0;counter<password_length;counter++)
	{
		uint8 pressed_key;
		pressed_key = KeyPad_getPressedKey (); /*Get and record the pressed key on the KeyPad */
		_delay_ms (200);
		LCD_displayCharacter ('*');
		_delay_ms (50);
		/*UART_sendByte (pressed_key); */ /* Sending the password's character to the 2nd ECU */
	}
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

void SYSTEM_checkMatching (void)
{
	while (!UART_recieveByte ())
	{
		SYSTEM_errorMessage ();
		_delay_ms (50);
		SYSTEM_setPassword ();
		SYSTEM_confirmPassword ();
	}
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
	for (counter=0;counter<password_length;counter++)
	{
		uint8 pressed_key;
		pressed_key = KeyPad_getPressedKey (); /*Get and record the pressed key on the KeyPad */
		_delay_ms (200); /* To prevent pressing overlap */
		LCD_displayCharacter ('*');
		/*UART_sendByte (pressed_key); */ /* Sending the password's character to the 2nd ECU */
		}
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
}

void SYSTEM_confirm_Close_Message (void)
{
	LCD_clearScreen ();
	LCD_displayString ("Door is now");
	LCD_goToRowColumn (1,0);
	LCD_displayString ("Closing...");
	while (BIT_IS_CLEAR (TIFR,OCF1A)); /* Polling for 15 seconds until the door is closed */
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
	_delay_ms (2000);
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
	_delay_ms (200);
	if ((pressed_key == '+') || (pressed_key == '-'))
	{
		if (pressed_key == '+')
			return 1;
		else if (pressed_key == '-')
			return 0;
	}
		return -1;
}