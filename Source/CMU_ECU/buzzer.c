/******************************************************************************
 *
 * File Name: buzzer.c
 *
 * Description: Buzzer source file for Control Machine Unit Micro-Controller
 *
 * Engineer: Hesham Khaled
 *
 ******************************************************************************/

#include "buzzer.h"

void Toggle_buzzer (void)
{
    TOGGLE_BIT (PORTD,PD7);
}