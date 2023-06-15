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
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#ifndef SYSCONFIG_H_
#define SYSCONFIG_H_


// Drivers configuration
// *********************

// Other hardware configuration
// ****************************

#define SYS_DBG_AUTO_START_TA4                    0

// Services configuration
// **********************

// files syslowpower.h, SysDefPowerModeHelper.c
#define SYS_CFG_USE_DEFAULT_PM_HELPER             0
#define SYS_CFG_DEF_PM_HELPER_STANDBY             0  ///< if defined to 1 then the MCU goes in STANDBY mode when the system enters in SLEEP_1.

// file SysTimestamp.c
#define SYS_TS_CFG_ENABLE_SERVICE                 1
/**
 * Configuration parameter for the timer used for the eLooM timestamp service.
 * Valid value are:
 * - SYS_TS_USE_SW_TSDRIVER to use the RTOS tick
 * - The configuration structure for an hardware timer. It must be compatible with SysTimestamp_t type.
 */
//#define SYS_TS_CFG_TSDRIVER_PARAMS                &MX_TIM17InitParams
#define SYS_TS_CFG_TSDRIVER_PARAMS                SYS_TS_USE_SW_TSDRIVER

//#define SYS_TS_CFG_TSDRIVER_FREQ_HZ               SystemCoreClock ///< hardware timer clock frequency in Hz
#define SYS_TS_CFG_TSDRIVER_FREQ_HZ             TX_TIMER_TICKS_PER_SECOND ///< ThreadX clock frequency in Hz


// Tasks configuration
// *******************

// file IManagedTask.h
#define MT_ALLOWED_ERROR_COUNT                    0x2

// file sysinit.c
#define INIT_TASK_CFG_ENABLE_BOOT_IF              0
#define INIT_TASK_CFG_STACK_SIZE                  (TX_MINIMUM_STACK*10)
#define INIT_TASK_CFG_HEAP_SIZE                   (100*1024)


// App configuration

// file HelloWorldTask.c
#define HW_TASK_CFG_STACK_DEPTH                   (TX_MINIMUM_STACK*7)
#define HW_TASK_CFG_PRIORITY                      (TX_MAX_PRIORITIES-2)

// file DprocessTask1.c
#define DPT1_TASK_CFG_STACK_DEPTH                 (TX_MINIMUM_STACK*15U)
#define DPT1_TASK_CFG_PRIORITY                    (TX_MAX_PRIORITIES-3U)

#endif /* SYSCONFIG_H_ */
