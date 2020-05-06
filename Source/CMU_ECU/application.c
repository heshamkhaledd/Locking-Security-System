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
	uint8 CONTROL_launchTimes = 0;
	MODULES_init ();
	uint8 check = initial;
	uint8 state;
	uint8 ERROR_entry = 0;


	while (1)
	{
		while (CONTROL_launchTimes == 0 && check != Idle)
		{
			CONTROL_launchTimes++;
			check = CONTROL_setReceivePassword ();
		}

		check = initial;

		state = UART_recieveByte ();

		if (state == CHG_PW)
		{
			while (((CONTROL_checkMatch ()) == 0) && ERROR_entry != MAX_TRY)
				ERROR_entry++;

			if (ERROR_entry != MAX_TRY)
			{
				ERROR_entry = 0;
				while (check != Idle)
					check = CONTROL_setReceivePassword ();
			}
			
		}
		else if (state == O_DOOR)
		{
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
