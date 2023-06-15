/**
 ******************************************************************************
 * @file    exti_pin_map.c
 * @author  SRA
 * 
 * 
 * @brief   Application level file. It defines the PIN to callback
 *          mapping function for the external interrupt.
 *
 * This files define a map (EXTI_N, function).
 *
 *********************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *********************************************************************************
 */

#include "drivers/EXTIPinMap.h"
#include "mx.h"

/* Forward function declaration. */
/*********************************/
void Def_EXTI_Callback(uint16_t pin) {};
void HW_PB_EXTI_Callback(uint16_t pin);
void INT2_DHCX_EXTI_Callback(uint16_t nPin);
void ISM330DHCXTask_EXTI_Callback(uint16_t nPin);

/* External variables */
/**********************/

EXTI_DECLARE_PIN2F_MAP()

/* Map definition */
/******************/

EXTI_BEGIN_P2F_MAP() 
EXTI_P2F_MAP_ENTRY(GPIO_PIN_0, HW_PB_EXTI_Callback)
EXTI_P2F_MAP_ENTRY(GPIO_PIN_8, ISM330DHCXTask_EXTI_Callback)
EXTI_END_P2F_MAP()
