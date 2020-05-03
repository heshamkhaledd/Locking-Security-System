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

Timer_Config g_s_T1config;
void TIMER_config (void)
{
	Timer_Config *config_Ptr = &g_s_T1config;
	config_Ptr->a_mode = TIMER_compareMode;
	config_Ptr->a_channel = 'A';
	config_Ptr->a_prescaler = Prescaler_1024;
	config_Ptr->a_inital_value = 0;
	config_Ptr->a_compare_value = 65500;
}
int main ()
{
	uint8 SYSTEM_launchTimes = 0;
	uint8 order;
	LCD_init ();
	LCD_clearScreen ();
	UART_init ();
	TIMER_config ();
	SREG |= (1<<7);
	while (1)
	{
		/***********************************************************************
		 * If this is the system first launching, It's required from the user
		 * To Configure his security password for the first time, in order to
		 * Use it later
		 ***********************************************************************/

		if (SYSTEM_launchTimes == 0)
		{
			SYSTEM_setPassword ();
			SYSTEM_confirmPassword ();
			/*SYSTEM_checkMatching ();*/
			SYSTEM_launchTimes++; /* To Never get back in this block if the initial settings are made */
		}

		/**************************************************************************
		 * The default state of the system is to be at the main menu screen layout
		 * waiting for the user to choose one of the options available for him
		 ***************************************************************************/

		SYSTEM_mainMenu ();
		order = SYSTEM_userChooseOption ();
		if (order == 1)
		{
			SYSTEM_enterPassword ();
			SYSTEM_setPassword ();
			SYSTEM_confirmPassword ();
			/*SYSTEM_checkMatching ();*/
		}
		else if (order == 0)
		{
			SYSTEM_enterPassword ();
			TIMER1_setCallBack (SYSTEM_confirm_Close_Message);
			TIMER1_init (&g_s_T1config);
			SYSTEM_confirm_Open_Message ();
		}
		else
		{
			SYSTEM_errorMessage ();
		}
	}
}
