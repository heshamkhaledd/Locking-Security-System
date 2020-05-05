/******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.h
 *
 * Description: Header file for the timer driver
 *
 * Engineer: Hesham Khaled
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_
#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"
#include <avr/io.h>
#include <avr/interrupt.h>


/* Timer selection */
#define TIMER 1

/*******************************************************************************
 *                  Configuration Structures and Enumerators
 ******************************************************************************/
typedef enum{
	TIMER_normalMode , TIMER_compareMode
}TIMER_mode;

typedef enum{
	Prescaler_1 = 1 , Prescaler_8 = 2 , Prescaler_64 = 3 , Prescaler_256 = 4 , Prescaler_1024 = 5
}TIMER_prescaler;

typedef struct{
	TIMER_mode a_mode;
	uint8 a_channel;
	TIMER_prescaler a_prescaler;
	uint16 a_inital_value;
	uint16 a_compare_value;
}Timer_Config;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void TIMER_config (uint8,uint16,Timer_Config *);
void TIMER0_init (const Timer_Config *T0config_Ptr);
void TIMER1_init (const Timer_Config *T1config_Ptr);
void TIMER1_setCallBack (void(*Ptr2Func)(void));
void TIMER0_setCallBack (void(*Ptr2Func(void)));
void TIMER0_deinit (void);
void TIMER1_deinit (void);


#endif /* TIMER_H_ */