 /******************************************************************************
 *
 * File Name: application.c
 *
 * Description: Application software layer soure code
 *
 * Engineer: Hesham Khaled
 *
 ******************************************************************************/



#include "hmi_layer.h"

Timer_Config g_s_T1_Aconfig;
Timer_Config g_s_T1_Bconfig;


void TIMER_config (uint8 channel,uint16 comp_var,Timer_Config *Member_Ptr)
{
	Timer_Config *config_Ptr = Member_Ptr;
	config_Ptr->a_mode = TIMER_compareMode;
	config_Ptr->a_channel = channel;
	config_Ptr->a_prescaler = Prescaler_1024;
	config_Ptr->a_inital_value = 0;
	config_Ptr->a_compare_value = comp_var;
}


int main ()
{
	uint8 SYSTEM_launchTimes = 0;
	uint8 state = initial;
	uint8 ERROR = 0;
	MODULES_init ();
	while (1)
	{
		/***********************************************************************
		 * If this is the system first launching, Display a Welcome message.
		 *  It's required from the user To Configure his security password for
		 * the first time, in order to Use it later
		 ***********************************************************************/

		while ((SYSTEM_launchTimes == 0) && state != Idle)
		{
			SYSTEM_setPassword ();
			SYSTEM_confirmPassword ();
			SYSTEM_checkMatching ();
			SYSTEM_launchTimes++; /* To Never get back in this block if the initial settings are made */
		}

		/**************************************************************************
		 * The default state of the system is to be at the main menu screen layout
		 * waiting for the user to choose one of the options available for him
		 ***************************************************************************/

		SYSTEM_mainMenu ();
		state = SYSTEM_userChooseOption ();

		if (state == CHG_PW)
		{
			SYSTEM_enterPassword ();
			while ((UART_recieveByte == 0) && ERROR != MAX_TRY )
				ERROR++;

			if (ERROR != MAX_TRY)
			{
				ERROR = 0;
				SYSTEM_setPassword ();
				SYSTEM_confirmPassword ();
				SYSTEM_checkMatching ();
			}
			else
			{
				SYSTEM_errorMessage ();
			}
			
		}
		else if (state == O_DOOR)
		{
			SYSTEM_enterPassword ();
			while ((UART_recieveByte == 0) && ERROR != MAX_TRY );
				ERROR++;

			if (ERROR != MAX_TRY)
			{
				ERROR = 0;
				TIMER1_setCallBack (SYSTEM_confirm_Close_Message);
				TIMER1_init (&g_s_T1_Aconfig);
				TIMER1_init (&g_s_T1_Bconfig);
				SYSTEM_confirm_Open_Message ();
				TIMER1_deinit ();
			}
			else
			{
				SYSTEM_errorMessage ();
			}
		}
		else
		{
			SYSTEM_errorMessage ();
		}
	}
}