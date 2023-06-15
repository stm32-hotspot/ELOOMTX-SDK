/**
 ******************************************************************************
 * @file    sysconfig.h
 * @author  SRA - GPM
 * @version 1.2.0
 * @date    Jan 5, 2017
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
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#ifndef SYSCONFIG_H_
#define SYSCONFIG_H_

//#include "SMPinConfig.h"

// Drivers configuration
// *********************

// file NucleoDriver.c
// uncomment the following line to change the drive common parameters
//#define NUCLEO_DRV_CFG_IRQ_PRIORITY             13

// Other hardware configuration
// ****************************

#define SYS_DBG_AUTO_START_TA4                    0

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
#define INIT_TASK_CFG_STACK_SIZE                  (TX_MINIMUM_STACK*6)
#define INIT_TASK_CFG_HEAP_SYZE                   (150*1024)


// HSDCore configuration

// file sensor_db.h
#define COM_MAX_SENSORS                           16

// file ISM330DHCXTask.c
#define ISM330DHCX_TASK_CFG_STACK_DEPTH           (TX_MINIMUM_STACK*5)
#define ISM330DHCX_TASK_CFG_PRIORITY              (4)

// file IIS3DWBTask.c
#define IIS3DWB_TASK_CFG_STACK_DEPTH              (TX_MINIMUM_STACK*4)
#define IIS3DWB_TASK_CFG_PRIORITY                 (4)

// file IIS2DHTask.c
#define IIS2DH_TASK_CFG_STACK_DEPTH              (TX_MINIMUM_STACK*4)
#define IIS2DH_TASK_CFG_PRIORITY                 (4)

// file IMP23ABSUTask.c
#define IMP23ABSU_TASK_CFG_STACK_DEPTH            (TX_MINIMUM_STACK*4)
#define IMP23ABSU_TASK_CFG_PRIORITY               (4)

// file HTS221Task.c
#define HTS221_TASK_CFG_STACK_DEPTH               (TX_MINIMUM_STACK*5)
#define HTS221_TASK_CFG_PRIORITY                  (4)

// file LPS22HHTask.c
#define LPS22HH_TASK_CFG_STACK_DEPTH              (TX_MINIMUM_STACK*5)
#define LPS22HH_TASK_CFG_PRIORITY                 (4)

// file IIS2MDCTask.c
#define IIS2MDC_TASK_CFG_STACK_DEPTH              (TX_MINIMUM_STACK*4)
#define IIS2MDC_TASK_CFG_PRIORITY                 (4)

// file IMP34DT05Task.c
#define IMP34DT05_TASK_CFG_STACK_DEPTH            (TX_MINIMUM_STACK*4)
#define IMP34DT05_TASK_CFG_PRIORITY               (4)

// file STTS751Task.c
#define STTS751_TASK_CFG_STACK_DEPTH            (TX_MINIMUM_STACK*4)
#define STTS751_TASK_CFG_PRIORITY               (4)

// file I2CBusTask.c
#define I2CBUS_TASK_CFG_STACK_DEPTH               (TX_MINIMUM_STACK*3)
#define I2CBUS_TASK_CFG_PRIORITY                  (4)

// file SPIBusTask.c
#define SPIBUS_TASK_CFG_STACK_DEPTH               (TX_MINIMUM_STACK*3)
#define SPIBUS_TASK_CFG_PRIORITY                  (4)

// file UtilTask.c
#define UTIL_TASK_CFG_STACK_DEPTH               (TX_MINIMUM_STACK*3)
#define UTIL_TASK_CFG_PRIORITY                  (7)

// App configuration

// file HelloWorldTask.c
// uncomment the following lines to change the task common parameters
#define HW_TASK_CFG_STACK_DEPTH                    (TX_MINIMUM_STACK*7)
#define HW_TASK_CFG_PRIORITY                       (TX_MAX_PRIORITIES-2)

#endif /* SYSCONFIG_H_ */
