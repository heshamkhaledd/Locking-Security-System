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
	uint8 check = 0;
	uint8 ERROR_entry = 0;
	

	while (1)
	{
		
		/***********************************************************************
		 * If this is the system first launching, keep the ECU in the initial
		 * Condition. Waitinf for the user to enter and confirm his new
		 * Password to store it in the EEPROM.
		 ***********************************************************************/

		while (state == initial)
		{
			state = CONTROL_setReceivePassword ();
		}

		/**************************************************************************
		 * The default state of the system is to be in the Idle state,
		 * waiting for the user to choose one of the options available for him
		 ***************************************************************************/

		state = get_state ();
		if (state == CHG_PW)
		{
			while ( !check && ERROR_entry != MAX_TRY)
			{
				check = CONTROL_checkMatch ();
				ERROR_entry++;
			}
			if (ERROR_entry != MAX_TRY)
			{
				LCD_displayString ("LOL");
				_delay_ms (1000);
				ERROR_entry = 0;
				while (state == CHG_PW)
					state = CONTROL_setReceivePassword ();
			}

			else if (ERROR_entry == MAX_TRY)
			{

			}
			
		}
		else if (state == O_DOOR)
		{
			LCD_displayCharacter ('-');
			while ((CONTROL_checkMatch () == 0) && ERROR_entry != MAX_TRY)
				ERROR_entry++;

			if (ERROR_entry != MAX_TRY)
			{
				ERROR_entry = 0;
				TIMER1_setCallBack (MOTOR_run);
				TIMER1_init (&g_s_T1_Aconfig);
				TIMER1_init (&g_s_T1_Bconfig);
				MOTOR_run ();
				TIMER1_deinit ();
			}
		}
		else
		{
			/* DO NOTHING */
		}
		
	}
}
