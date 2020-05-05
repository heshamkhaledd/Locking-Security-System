/******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.c
 *
 * Description: Source file for the timer driver
 *
 * Engineer: Paravoid
 *
 *******************************************************************************/

#include "timer.h"
#include "lcd.h"

uint8 tick = 0;
extern uint8 open_flag;
extern uint8 close_flag;


#if (TIMER == 0)

static volatile void (*g_Timer0_Call_Back_Ptr)(void);


void TIMER0_deinit (void)
{
	TCCR0=0;
	CLEAR_BIT (TIMSK,OCIE0);
	CLEAR_BIT (TIMSK,TOIE0);
}

void TIMER0_init (const Timer_Config *T0config_Ptr)
{
	SET_BIT (TCCR0,FOC0);
	if (T0config_Ptr->a_mode == TIMER_normalMode)
	{

		TCCR0 = (TCCR0 & 0xF8) | (T0config_Ptr->a_prescaler & 0x7);
		TCNT0 = T0config_Ptr->a_inital_value;
		SET_BIT (TIMSK,TOIE0);
	}
	else if (T0config_Ptr->a_mode == TIMER_compareMode)
	{
		TCCR0 = (TCCR0 & 0xF8) | (T0config_Ptr->a_prescaler & 0x7);
		TCNT0 = T0config_Ptr->a_inital_value;
		OCR0 = T0config_Ptr->a_compare_value;
		SET_BIT (TCCR0,WGM01);
		SET_BIT (TIMSK,OCIE0);
	}

}
void TIMER0_setCallBack (void(*Ptr2Func(void)))
{
	g_Timer0_Call_Back_Ptr = Ptr2Func;
}

/* Interrupt service routines in case of Overflow mode
/* or Compare mode
 */

ISR (TIMER0_OVF_vect)
{

	(*g_Timer0_Call_Back_Ptr)();
}
ISR (TIMER0_COMP_vect)
{
	(*g_Timer0_Call_Back_Ptr)();
}

#elif (TIMER == 1)

static volatile void (*g_Timer1_Call_Back_Ptr) (void) = (void *) 0;

void TIMER1_deinit (void)
{
	TCCR1A = 0;
	TCCR1B = 0;
	OCR1A = 0;
	CLEAR_BIT (TIMSK,OCIE1A);
	CLEAR_BIT (TIMSK,TOIE1);
}

void TIMER1_init (const Timer_Config *T1config_Ptr)
{

	if (T1config_Ptr->a_mode == TIMER_normalMode)
	{
		SET_BIT (TCCR1A,FOC1A);
		TCCR1B = (TCCR1B & 0xF8) | ((T1config_Ptr->a_prescaler) & 0x7);
		TCNT1 = T1config_Ptr->a_inital_value;
		SET_BIT (TIMSK,TOIE1);
	}

	if (T1config_Ptr->a_mode == TIMER_compareMode)

	{
		if ((T1config_Ptr->a_channel) == 'A')
		{
			SET_BIT (TCCR1A,FOC1A);
			SET_BIT (TCCR1B,WGM12);
			TCCR1B = ((TCCR1B & 0xF8) | ((T1config_Ptr->a_prescaler) & 0x7));
			TCNT1 = T1config_Ptr->a_inital_value;
			OCR1A = T1config_Ptr->a_compare_value;
			SET_BIT (TIMSK,OCIE1A);
		}
		if ((T1config_Ptr->a_channel) == 'B')
		{
			SET_BIT (TCCR1A,FOC1B);
			SET_BIT (TCCR1B,WGM12);
			TCCR1B = ((TCCR1B & 0xF8) | (T1config_Ptr->a_prescaler) & 0x7);
			TCNT1 = T1config_Ptr->a_inital_value;
			OCR1B = T1config_Ptr->a_compare_value;
			SET_BIT (TIMSK,OCIE1B);
		}
	}
}
void TIMER1_setCallBack (void (* Ptr2Func) (void))
{
	g_Timer1_Call_Back_Ptr = Ptr2Func;
}

/* Interrupt service routines in case of Overflow mode
/* or Compare mode in both channels A and B */

ISR (TIMER1_OVF_vect)
{
	(*g_Timer1_Call_Back_Ptr)();
}
ISR (TIMER1_COMPA_vect)
{
	tick++;
	if (tick == 2)
	{
		tick = 0;
		close_flag = 1;
	}
}
ISR (TIMER1_COMPB_vect)
{
	SREG |= (1<<7);
	open_flag=1;
	(*g_Timer1_Call_Back_Ptr)();
}

#endif