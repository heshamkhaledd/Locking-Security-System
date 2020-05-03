 /******************************************************************************
 *
 * File Name: cmu_layer.h
 *
 * Description: cmu header file to interface with external components and peripherals
 *
 * Engineer: Hesham Khaled
 *
 ******************************************************************************/

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"
#include "external_eeprom.h"
#include "timer.h"
#include "uart.h"


#ifndef HMI_LAYER_H_
#define HMI_LAYER_H_

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/


#define password_length 3


/*******************************************************************************
 *                      Functions Prototypes                                   *
*******************************************************************************/

void CONTROL_receivePassword (void);



#endif /* HMI_LAYER_H_ */
