/**
  ******************************************************************************
  * @file    SensorManager_conf.h
  * @author  SRA
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
  *
  ******************************************************************************
  */

#ifndef SENSORMANAGERCONF_H_
#define SENSORMANAGERCONF_H_


#define IIS3DWB_FIFO_ENABLED 1
#define ISM330DHCX_FIFO_ENABLED 1


// file ISM330DHCXTask.c
#define ISM330DHCX_TASK_CFG_STACK_DEPTH           (TX_MINIMUM_STACK*8)
#define ISM330DHCX_TASK_CFG_PRIORITY              (8)

// file IIS3DWBTask.c
#define IIS3DWB_TASK_CFG_STACK_DEPTH              (TX_MINIMUM_STACK*8)
#define IIS3DWB_TASK_CFG_PRIORITY                 (8)

// file SPIBusTask.c
#define SPIBUS_TASK_CFG_STACK_DEPTH               (TX_MINIMUM_STACK*8)
#define SPIBUS_TASK_CFG_PRIORITY                  (4)


#endif /* SENSORMANAGERCONF_H_ */
