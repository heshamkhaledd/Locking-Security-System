/******************************************************************************
 *
 * File Name: application.c
 *
 * Description: Application source file for Control Machine Unit Micro-Controller
 *
 * Engineer: Hesham Khaled
 *
 ******************************************************************************/


#include "uart.h"
#include "motor.h"
#include "timer.h"
#include "external_eeprom.h"


int main ()
{
	uint8 CONTROL_launchTimes = 0;
	UART_init ();
	EEPROM_init ();


	while (1)
	{
		if (CONTROL_launchTimes == 0)
		{
			CONTROL_receivePassword ();
			CONTROL_launchTimes++;
		}

	}
}
