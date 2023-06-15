/**
 ******************************************************************************
 * @file    sysconfig.h
 * @author  SRA - GPM
 * 
 * 
 * @brief   Global System configuration file
 *
 * This file include some configuration parameters grouped here for user
 * convenience. This file override the default configuration value, and it is
 * used in the "Preinclude file" section of the "compiler > prepocessor"
 * options.
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

#ifndef SYSCONFIG_H_
#define SYSCONFIG_H_

#include "SensorManager_conf.h"

// Drivers configuration
// *********************

// file NucleoDriver.c
// uncomment the following line to change the drive common parameters
//#define NUCLEO_DRV_CFG_IRQ_PRIORITY             13

// Other hardware configuration
// ****************************

#define SYS_DBG_AUTO_START_TA4                    0

// file SysTimestamp.c
#define SYS_TS_CFG_ENABLE_SERVICE                 1
/**
�* Configuration parameter for the timer used for the eLooM timestamp service.
�* Valid value are:
�* - SYS_TS_USE_SW_TSDRIVER to use the RTOS tick
�* - The configuration structure for an hardware timer. It must be compatible with SysTimestamp_t type.
�*/
//#define SYS_TS_CFG_TSDRIVER_PARAMS � � �&MX_TIM3InitParams
#define SYS_TS_CFG_TSDRIVER_PARAMS                SYS_TS_USE_SW_TSDRIVER
//#define SYS_TS_CFG_TSDRIVER_FREQ_HZ � � SystemCoreClock ///< hardware timer clock frequency in Hz
#define SYS_TS_CFG_TSDRIVER_FREQ_HZ               TX_TIMER_TICKS_PER_SECOND ///< ThreadX clock frequency in Hz



// Services configuration
// **********************

// files syslowpower.h, SysDefPowerModeHelper.c
#define SYS_CFG_USE_DEFAULT_PM_HELPER             0
#define SYS_CFG_DEF_PM_HELPER_STANDBY             0  ///< if defined to 1 then the MCU goes in STANDBY mode when the system enters in SLEEP_1.


// Tasks configuration
// *******************

// file IManagedTask.h
#define MT_ALLOWED_ERROR_COUNT                    0x2

// file sysinit.c
#define INIT_TASK_CFG_ENABLE_BOOT_IF              0
#define INIT_TASK_CFG_STACK_SIZE                  (TX_MINIMUM_STACK*10)
#define INIT_TASK_CFG_HEAP_SIZE                   (50*1024)

// file UtilTask.c
#define UTIL_TASK_CFG_STACK_DEPTH                 (TX_MINIMUM_STACK*3)
#define UTIL_TASK_CFG_PRIORITY                    (7)

// file ProcessTask.c
#define PRC_TASK_CFG_STACK_DEPTH                  (TX_MINIMUM_STACK*5)
#define PRC_TASK_CFG_PRIORITY                     (7)

// App configuration

// file HelloWorldTask.c
// uncomment the following lines to change the task common parameters
#define HW_TASK_CFG_STACK_DEPTH                   (TX_MINIMUM_STACK*7)
#define HW_TASK_CFG_PRIORITY                      (TX_MAX_PRIORITIES-2)

#endif /* SYSCONFIG_H_ */
