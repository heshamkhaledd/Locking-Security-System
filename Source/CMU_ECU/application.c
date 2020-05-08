/******************************************************************************
 *
 * File Name: application.c
 *
 * Description: Application source file for Control Machine Unit Micro-Controller
 *
 * Engineer: Hesham Khaled
 *
 ******************************************************************************/

#include "cmu_layer.h"


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
	uint8 state=initial;
	uint8 check=WRONG;
	uint8 ERROR_entry = 0;
	

	while (1)
	{
		
		/***********************************************************************
		 * If this is the system first launching, keep the ECU in the initial
		 * Condition. Waitinf for the user to enter and confirm his new
		 * Password to store it in the EEPROM.
		 ***********************************************************************/

		while (state == initial && check == WRONG)
		{
			check = CONTROL_setReceivePassword ();
		}

		/**************************************************************************
		 * The default state of the system is to be in the Idle state,
		 * waiting for the user to choose one of the options available for him
		 ***************************************************************************/
		state = get_state ();

		if (state == CHG_PW)
		{
			check=WRONG;
			while (check == WRONG && ERROR_entry != MAX_TRY)
			{
				check = CONTROL_checkMatch ();
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
					check = CONTROL_setReceivePassword ();
			}

			else if (ERROR_entry == MAX_TRY)
			{
				while (1);  /* MAKE IT A BUZZER */
			}
			
		}
		else if (state == O_DOOR)
		{
			check = WRONG;
			while (check == WRONG && ERROR_entry != MAX_TRY)
			{
				check = CONTROL_checkMatch ();
				if (check == WRONG)
				{
					ERROR_entry++;
				}
			}

			if (ERROR_entry != MAX_TRY)
			{
				ERROR_entry = 0;
				TIMER1_setCallBack (MOTOR_close);
				TIMER1_init (&g_s_T1_Aconfig);
				TIMER1_init (&g_s_T1_Bconfig);
				MOTOR_open (CLOCKWISE);
				TIMER1_deinit ();
				MOTOR_stop ();
			}
		}
		else
		{
			/* DO NOTHING AND RETURN TO IDEL STATE */
		}
		
	}
}
