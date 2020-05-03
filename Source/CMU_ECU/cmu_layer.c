 /******************************************************************************
 *
 * File Name: cmu_layer.c
 *
 * Description: CMU source file that has the main ECU control functions
 *
 * Engineer: Hesham Khaled
 *
 ******************************************************************************/

#include "cmu_layer.h"


/******************************************************************************
 *
 * Function Name: CONTROL_recievePassword
 *
 * Description:
 *
 *
 *
 *
 *****************************************************************************/

void CONTROL_receivePassword (void)
{
	uint8 byte;
	uint8 counter;
	for (counter=0;counter<password_length;counter++)
	{
		byte = UART_recieveByte ();
	}

}
