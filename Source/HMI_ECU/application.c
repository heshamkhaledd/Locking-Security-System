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
	MODULES_init ();
	uint8 state = initial;
	uint8 check= WRONG;
	uint8 ERROR_entry = 0;


	while (1)
	{
		
		check = WRONG; /* Initial state for entered password as wrong */
						/* Until proven the opposite */


		/***********************************************************************
		 * If this is the system first launching, Display a Welcome message.
		 * It's required from the user To Configure his security password for
		 * the first time, in order to Use it later
		 ***********************************************************************/
		while (state == initial && check == WRONG)
		{
			SYSTEM_setPassword ();
			SYSTEM_confirmPassword ();
			check = SYSTEM_checkMatching ();
		}

		/**************************************************************************
		 * The Idle state of the system is to be at the main menu screen layout
		 * waiting for the user to choose one of the options available for him
		 ***************************************************************************/
		SYSTEM_mainMenu ();
		state = SYSTEM_userChooseOption ();

		if (state == CHG_PW)
		{
			check= WRONG;
			while (check == WRONG && ERROR_entry != MAX_TRY)
			{
				check = SYSTEM_enterPassword ();
				if (check == WRONG)
				{
					ERROR_entry++;
				}
			}

			if (ERROR_entry != MAX_TRY)
			{
				ERROR_entry = 0;
				check = WRONG;
				while (check == WRONG)
				{
					SYSTEM_setPassword ();
					SYSTEM_confirmPassword ();
					check = SYSTEM_checkMatching ();
				}
			}

			else if (ERROR_entry == MAX_TRY)
			{
				while (1)
				{
					SYSTEM_errorMessage ();
				}
			}
			
		}
		else if (state == O_DOOR)
		{
			check = WRONG;
			while (check == WRONG && ERROR_entry != MAX_TRY)
			{
				check = SYSTEM_enterPassword ();
				if (check == WRONG)
				{
					ERROR_entry++;
				}
			}

			if (ERROR_entry != MAX_TRY)
			{
				ERROR_entry = 0;
				TIMER1_setCallBack (SYSTEM_confirm_Close_Message);
				TIMER1_init (&g_s_T1_Aconfig);
				TIMER1_init (&g_s_T1_Bconfig);
				SYSTEM_confirm_Open_Message ();
				TIMER1_deinit ();
			}
			else if (ERROR_entry == MAX_TRY)
			{
				while (1)
				{
					SYSTEM_errorMessage ();
				}
			}
		}
		else
		{
			SYSTEM_errorMessage ();
		}
	}
}