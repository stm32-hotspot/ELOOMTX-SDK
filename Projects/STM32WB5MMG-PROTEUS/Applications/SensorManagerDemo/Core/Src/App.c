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
 * This is the most simple eLooM application. It shows how to setup an eLooM project.
 * It create one managed task (::HelloWorldTask_t). The task display
 * a greeting message in the debug log every second.
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
#include "SPIBusTask.h"
#include "ISM330DHCXTask.h"
#include "HelloWorldTask.h"
#include "mx.h"

/**
 * SPI bus task object.
 */
static AManagedTaskEx *spSPIBusObj = NULL;

/**
 * Sensor task object.
 */
static AManagedTaskEx *spISM330DHCXObj = NULL;

/**
 * Application managed task.
 */
static AManagedTask *spHelloWorldObj = NULL;


/* eLooM framework entry points definition */
/*******************************************/

sys_error_code_t SysLoadApplicationContext(ApplicationContext *pAppContext)
{
  assert_param(pAppContext != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* Allocate the task objects */
  spSPIBusObj = SPIBusTaskAlloc(&MX_SPI1InitParams);
  spISM330DHCXObj = ISM330DHCXTaskAlloc(&MX_GPIO_INT1_ISM330DHCX_InitParams, NULL, &MX_GPIO_CS_ISM330DHCX_InitParams);
  spHelloWorldObj = HelloWorldTaskAlloc(NULL, &MX_GPIO_UBInitParams);

  /* Add the task object to the context. */
  res = ACAddTask(pAppContext, (AManagedTask*)spSPIBusObj);
  res = ACAddTask(pAppContext, (AManagedTask*)spISM330DHCXObj);
  res = ACAddTask(pAppContext, spHelloWorldObj);

  return res;
}

sys_error_code_t SysOnStartApplication(ApplicationContext *pAppContext)
{
  UNUSED(pAppContext);

  if(spISM330DHCXObj)
  {
    SPIBusTaskConnectDevice((SPIBusTask *) spSPIBusObj, (SPIBusIF *)ISM330DHCXTaskGetSensorIF((ISM330DHCXTask *) spISM330DHCXObj));
  }

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

//UINT tx_thread_stack_error_notify(VOID (*error_handler)(TX_THREAD *))
//{
//  sys_error_handler();
//}

