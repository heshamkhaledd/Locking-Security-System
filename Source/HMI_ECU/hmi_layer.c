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
	LCD_init ();
	LCD_clearScreen ();
	UART_init ();
	TIMER_config ('A',65500,&g_s_T1_Aconfig);
	TIMER_config ('B',65500,&g_s_T1_Bconfig);
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
 * Input: void
 * Output: void
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
 * password to the Control machine unit (2nd ECU) as a string
 * 
 * Input: void
 * Output: void
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
		_delay_ms (1000);
	}

	LCD_clearScreen ();
	LCD_goToRowColumn (0,0);
	LCD_displayString ("Enter Password:");
	LCD_goToRowColumn (1,0);

	/* Iterate <password_length> times to send store the password,
	/* in a temporary array and then send it*/

	for (counter=0;counter<password_length;counter++)
	{
		password_1[counter]= KeyPad_getPressedKey (); /*Get and record the pressed key on the KeyPad */
		_delay_ms (500);							  /*delay for switch debouncing */
		if (password_1[counter] == 0)			   	 /* Redefining the zero with -1 to prevent confusion of null terminator '\0' */
		{
			password_1[counter] = -1;
			LCD_displayCharacter ('0');
			_delay_ms (150);
			LCD_goToRowColumn (1,counter);
			LCD_displayCharacter ('*');
		}
		else
		{
			LCD_integerToString (password_1[counter]);
			_delay_ms (150);
			LCD_goToRowColumn (1,counter);
			LCD_displayCharacter ('*');
		}
		
	}
	password_1[counter] = '#';				/*A conventional NULL Terminator for the UART_sendString */
	counter++;
	password_1[counter] = '\0';
	UART_sendByte (READY);		 			/* Sending UART ACK to the other ECU to make sure it's ready to receive */
	UART_sendString (password_1);			/* Sending the password to the 2nd ECU */
	LCD_clearScreen ();
}

/******************************************************************************
 *
 * Function Name: SYSTEM_confirmPassword
 *
 * Description: A function that's responsible for sending a confirmation password
 * to the Control machine unit (2nd ECU) to match it with the first.
 * 
 * Input: void
 * Output: void
 *
 *****************************************************************************/

void SYSTEM_confirmPassword (void)
{
	LCD_goToRowColumn (0,0);
	LCD_displayString ("Confirm Password:");
	LCD_goToRowColumn (1,0);

	/* Iterate <password_length> times to send store the password,
	/* in a temporary array and then send it*/
	uint8 counter;
	uint8 password_2[password_length+2];

	for (counter=0;counter<password_length;counter++)
	{
		password_2[counter]= KeyPad_getPressedKey (); /*Get and record the pressed key on the KeyPad */
		_delay_ms (500);							  /*delay for switch debouncing */
		if (password_2[counter] == 0)				 /* Redefining the zero with -1 to prevent confusion of null terminator '\0' */
		{
			password_2[counter] = -1;
			LCD_displayCharacter ('0');
			_delay_ms (150);
			LCD_goToRowColumn (1,counter);
			LCD_displayCharacter ('*');
		}
		else
		{
			LCD_integerToString (password_2[counter]);
			_delay_ms (150);
			LCD_goToRowColumn (1,counter);
			LCD_displayCharacter ('*');
		}
	}
	password_2[counter] = '#';			/*A conventional NULL Terminator for the UART_sendString */
	counter++;
	password_2[counter] = '\0';
	UART_sendByte (READY);				/* Sending UART ACK to the other ECU to make sure it's ready to receive */
	UART_sendString (password_2);		/* Sending the password's character to the 2nd ECU */
	LCD_clearScreen ();
}

/******************************************************************************
 *
 * Function Name: SYSTEM_checkMatching
 *
 * Description: Function responsible for waiting the 2nd ECU to give it the ACK
 * If the two passwords are identical or not. If yes proceed to the rest of the
 * Routine, if not, return WRONG and keep looping until there's a match.
 * 
 * Input:void
 * Output: uint8
 * 
 *
 *****************************************************************************/

uint8 SYSTEM_checkMatching (void)
{
	UART_sendByte (READY);
	if (((UART_recieveByte ()) != Idle))
	{
		SYSTEM_errorMessage ();
		return WRONG;
	}
	else
		return Idle;
}

/******************************************************************************
 *
 * Function Name: SYSTEM_enterPassword
 *
 * Description: A function that's responsible for taking the user's password
 * storing it, and sending it to the 2nd ECU to check if there's a match or not.
 *
 * Input: void
 * Output: uint8
 *
 *****************************************************************************/

uint8 SYSTEM_enterPassword (void)
{
	LCD_clearScreen ();
	LCD_goToRowColumn (0,0);
	LCD_displayString ("Enter Password:");
	LCD_goToRowColumn (1,0);

	/* Iterate <password_length> times to send store the password,
	/* in a temporary array and then send it*/
	uint8 counter;
	uint8 password[password_length+2];
	for (counter=0;counter<password_length;counter++)
	{
		password[counter] = KeyPad_getPressedKey (); /*Get and record the pressed key on the KeyPad */
		_delay_ms (500);							 /* To prevent switch debouncing */
		if (password[counter] == 0)					/* Redefining the zero with -1 to prevent confusion of null terminator '\0' */
		{
			password[counter] = -1;
			LCD_displayCharacter ('*');
		}
		else
			LCD_displayCharacter ('*');
	}
	password[counter] = '#';			/*A conventional NULL Terminator for the UART_sendString */
	counter++;
	password[counter] = '\0';
	LCD_clearScreen ();
	UART_sendByte (READY);				/* Sending UART ACK to the other ECU to make sure it's ready to receive */
	UART_sendString (password); 		/* Sending the password's character to the 2nd ECU */
	UART_sendByte (READY);
	return ((UART_recieveByte ()));
}

/******************************************************************************
 *
 * Function Name: SYSTEM_confirm_<DoorState>Message
 *
 * Description: A function that's responsible for displaying the door state on
 * the LCD, either it's opening or closing
 * 
 * Input: void
 * Output: void
 *
 *****************************************************************************/

void SYSTEM_confirm_Open_Message (void)
{
		LCD_clearScreen ();
		LCD_displayString ("Door is now");
		LCD_goToRowColumn (1,0);
		LCD_displayString ("Opening...");
		while (open_flag == 0);			/* Polling for 8 seconds until the door is opened */
}

void SYSTEM_confirm_Close_Message (void)
{	
	LCD_clearScreen ();
	LCD_displayString ("Door is now");
	LCD_goToRowColumn (1,0);
	LCD_displayString ("Closing...");
	while (close_flag == 0);			/* Polling for 8 seconds until the door is closed */
}

/******************************************************************************
 *
 * Function Name: SYSTEM_confirmSave
 *
 * Description: A function that's responsible for displaying an confirm message
 * to the user that his new password is saved successfully.
 * 
 * Input: void
 * Output: void
 *
 *****************************************************************************/


void SYSTEM_confirmSave (void)
{
	LCD_clearScreen ();
	LCD_displayString ("Password");
	LCD_goToRowColumn (1,0);
	LCD_displayString ("Saved.");
	_delay_ms (1000);
	LCD_clearScreen ();
}

/******************************************************************************
 *
 * Function Name: SYSTEM_errorMessage
 *
 * Description: A function that's responsible for displaying an error message
 * to the user if he pressed a wrong key on the main menu or entered a wrong
 * Password.
 * 
 * Input: void
 * Output: void
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
 * Input: void
 * Output: uint8
 *
 *****************************************************************************/

uint8 SYSTEM_userChooseOption (void)
{
	uint8 pressed_key;
	pressed_key = KeyPad_getPressedKey ();
	_delay_ms (500);
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