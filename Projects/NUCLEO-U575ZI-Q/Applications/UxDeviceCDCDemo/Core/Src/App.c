/**
 ******************************************************************************
 * @file    App.c
 * @author  SRA - GPM
 * 
 * 
 *
 * @brief   Define the Application main entry points
 *
 * ## Introduction
 *
 * This file is the main entry point for the user code.
 *
 * The framework `weak` functions are redefined in this file and they link
 * the application specific code with the framework:
 * - SysLoadApplicationContext(): it is the first application defined function
 *   called by the framework. Here we define all managed tasks. A managed task
 *   implements one or more application specific feature.
 * - SysOnStartApplication(): this function is called by the framework
 *   when the system is initialized (all managed task objects have been
 *   initialized), and before the INIT task release the control. Here we
 *   link the application objects according to the application design.
 *
 * The execution time  between the two above functions is called
 * *system initialization*. During this period only the INIT task is running.
 *
 * Each managed task will be activated in turn to initialize its hardware
 * resources, if any - MyTask_vtblHardwareInit() - and its software
 * resources - MyTask_vtblOnCreateTask().
 *
 * ## About this demo
 *
 * This demo demonstrates how to use the Sensor Manager eLooM component to operate the sensors
 * available in the X-NUCLEO-IKSO2A1 expansion board. The HelloWorld task select one sensor
 * and enable and configure it.
 *
 * ## How to use the demo
 *
 * connect the board through the ST-Link. Open a terminal
 * like [Tera Term](http://www.teraterm.org) to display the debug log using these parameters:
 * - Speed       : 115200
 * - Data        : 8 bit
 * - Parity      : none
 * - Stop bits   : 1
 * - Flow control: none
 *
 * Build the project and program the board.
 * At the beginning the application is in power mode E_PM_MODE_STATE1 and a
 * greetings message is displayed in the log.
 * To change state to E_POWER_MODE_SENSOR_ACTIVE press the user button. Now the
 * selected sensor start to produce data.
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

#include "services/sysdebug.h"
#include "services/ApplicationContext.h"
#include "AppPowerModeHelper.h"
#include "HelloWorldTask.h"
#include "UsbCdcTask.h"
#include "mx.h"

/* SensorManager include*/
#include "I2CBusTask.h"
#include "ISM330DHCXTask.h"
#include "IIS2DLPCTask.h"
#include "IIS2MDCTask.h"


/**
 * Application managed task.
 */
static AManagedTask *spHelloWorldObj = NULL;

/**
 * USB_CDC task object.
 */
static AManagedTaskEx *spUsbCdcObj = NULL;

/**
 * I2C bus task object.
 */
static AManagedTaskEx *spI2C2BusObj = NULL;

/**
 * Sensor task object: 3-axis accelerometer and 3-axis gyroscope
 */
static AManagedTaskEx *spISM330DHCXObj = NULL;

/**
 * Sensor task object: 3-axis accelerometer.
 */
static AManagedTaskEx *spIIS2DLPCObj = NULL;

/**
 * Sensor task object: 3-axis magnetometer
 */
static AManagedTaskEx *spIIS2MDCObj = NULL;


/* eLooM framework entry points definition */
/*******************************************/

sys_error_code_t SysLoadApplicationContext(ApplicationContext *pAppContext)
{
  assert_param(pAppContext != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* Allocate the task objects */
  spHelloWorldObj = HelloWorldTaskAlloc(&MX_GPIO_LEDBlueInitParams, &MX_GPIO_UBInitParams);
  spUsbCdcObj      = UsbCdcTaskAlloc();
  spI2C2BusObj     = I2CBusTaskAlloc(&MX_I2C1InitParams);
  spISM330DHCXObj  = ISM330DHCXTaskAlloc(&MX_GPIO_INT1_DHCXInitParams, NULL, NULL);
  spIIS2DLPCObj    = IIS2DLPCTaskAlloc(&MX_GPIO_INT2_DLPCInitParams, NULL);
  spIIS2MDCObj     = IIS2MDCTaskAlloc(&MX_GPIO_DRDY_MDCInitParams, NULL);


  /* Add the task object to the context. */
  res = ACAddTask(pAppContext, spHelloWorldObj);
  res = ACAddTask(pAppContext, (AManagedTask*) spUsbCdcObj);
  res = ACAddTask(pAppContext, (AManagedTask*) spI2C2BusObj);
  res = ACAddTask(pAppContext, (AManagedTask*) spISM330DHCXObj);
  res = ACAddTask(pAppContext, (AManagedTask*) spIIS2DLPCObj);
  res = ACAddTask(pAppContext, (AManagedTask*) spIIS2MDCObj);

  return res;
}

sys_error_code_t SysOnStartApplication(ApplicationContext *pAppContext)
{
  UNUSED(pAppContext);

  /* connect the sensor tasks to the bus. */
  (void) I2CBusTaskConnectDevice((I2CBusTask*) spI2C2BusObj, (I2CBusIF*)ISM330DHCXTaskGetSensorIF((ISM330DHCXTask*) spISM330DHCXObj));
  (void) I2CBusTaskConnectDevice((I2CBusTask*) spI2C2BusObj, (I2CBusIF*)IIS2DLPCTaskGetSensorIF((IIS2DLPCTask*) spIIS2DLPCObj));
  (void) I2CBusTaskConnectDevice((I2CBusTask*) spI2C2BusObj, (I2CBusIF*)IIS2MDCTaskGetSensorIF((IIS2MDCTask*) spIIS2MDCObj));

  return SYS_NO_ERROR_CODE;
}

/*
IApplicationErrorDelegate *SysGetErrorDelegate(void)
{
  // Install the application error manager delegate.
  static IApplicationErrorDelegate *s_pxErrDelegate = NULL;
  if (s_pxErrDelegate == NULL)
  {
    s_pxErrDelegate = AEMAlloc();
  }

  return s_pxErrDelegate;
}
*/

IAppPowerModeHelper *SysGetPowerModeHelper(void)
{
  /* Install the application power mode helper. */
  static IAppPowerModeHelper *s_pxPowerModeHelper = NULL;
  if (s_pxPowerModeHelper == NULL) {
    s_pxPowerModeHelper = AppPowerModeHelperAlloc();
  }

  return s_pxPowerModeHelper;
}



