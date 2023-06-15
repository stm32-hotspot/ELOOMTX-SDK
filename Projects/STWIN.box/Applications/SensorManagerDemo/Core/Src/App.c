/**
 ******************************************************************************
 * @file    App.c
 * @author  SRA - GPM
 * 
 * 
 *
 * @brief   Define the Application main entry points
 *
 * The framework `weak` function are redefined in this file and they link
 * the application specific code with the framework.
 *
 *********************************************************************************
 * @attention
 *
  * Copyright (c) 2023 STMicroelectronics..
 * All rights reserved.
 *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
 */

#include "services/sysdebug.h"
#include "services/ApplicationContext.h"
#include "events/SensorListener.h"
#include "events/SensorListener_vtbl.h"
#include "AppPowerModeHelper.h"
#include "HelloWorldTask.h"
#include "UtilTask.h"
#include "I2CBusTask.h"
#include "SPIBusTask.h"
#include "IIS2MDCTask.h"
#include "ISM330DHCXTask.h"
#include "mx.h"

/**
 * Application managed task.
 */
static AManagedTask *spHelloWorldObj = NULL;

/**
 * Utility task object.
 */
static AManagedTaskEx *spUtilObj = NULL;

/**
 * I2C bus task object.
 */
//static AManagedTaskEx *spI2CBusObj = NULL;

/**
 * SPI bus task object.
 */
static AManagedTaskEx *spSPIBusObj = NULL;

/**
 * Sensor task object.
 */
//static AManagedTaskEx *spIIS2MDCObj = NULL;
static AManagedTaskEx *spISM330DHCXObj = NULL;

/**
 * Sensor Listener used to print the data of the active sensor.
 */
static SensorListener_t sSensorListener;


/**
 * specifies the map (PM_APP, PM_SM). It re-map the state of the application into the state of the Sensor Manager.
 */
static EPowerMode spAppPMState2SMPMStateMap[] = {
    E_POWER_MODE_STATE1,
    E_POWER_MODE_SLEEP_1,
	E_POWER_MODE_SENSORS_ACTIVE,
	E_POWER_MODE_SENSORS_ACTIVE
};


/* Private functions declaration */
/*********************************/

/**
 * Re-map the PM State Machine of the Sensor Manager managed tasks used in the application according to the following map:
 *
 * | App State                      | SensorManager State          |
 * | :----------------------------- | ---------------------------: |
 * | E_POWER_MODE_STATE1            | E_POWER_MODE_STATE1          |
 * | E_POWER_MODE_SLEEP_1           | E_POWER_MODE_SLEEP_1         |
 * | E_POWER_MODE_SENSORS_ACTIVE    | E_POWER_MODE_SENSORS_ACTIVE  |
 * | E_POWER_MODE_TEST              | E_POWER_MODE_SENSORS_ACTIVE  |
 *
 * @param pPMState2PMStateMap [IN] specifies the map (PM_APP, PM_SM).
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
static sys_error_code_t SensorManagerStateMachineRemap(EPowerMode *pPMState2PMStateMap);


/* eLooM framework entry points definition */
/*******************************************/

sys_error_code_t SysLoadApplicationContext(ApplicationContext *pAppContext)
{
  assert_param(pAppContext);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;

  /* Allocate the task objects */
  spHelloWorldObj = HelloWorldTaskAlloc(&MX_GPIO_UBInitParams);
  spUtilObj = UtilTaskAlloc(NULL, NULL);

//  spI2CBusObj = I2CBusTaskAlloc(&MX_I2C2InitParams);
  spSPIBusObj = SPIBusTaskAlloc(&MX_SPI2InitParams);
//  spIIS2MDCObj = IIS2MDCTaskAlloc(&MX_GPIO_INT_MAGInitParams, NULL);
  spISM330DHCXObj = ISM330DHCXTaskAlloc(&MX_GPIO_INT1_DHCXInitParams, &MX_GPIO_INT2_DHCXInitParams, &MX_GPIO_CS_DHCXInitParams);

  /* Add the task object to the context. */
  xRes = ACAddTask(pAppContext, spHelloWorldObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)spUtilObj);
//  xRes = ACAddTask(pAppContext, (AManagedTask*)spI2CBusObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)spSPIBusObj);

//  xRes = ACAddTask(pAppContext, (AManagedTask*)spIIS2MDCObj);
  xRes = ACAddTask(pAppContext, (AManagedTask*)spISM330DHCXObj);

  // Allocate other application objects
  SensorListenerStaticAlloc(&sSensorListener);

  return xRes;
}

sys_error_code_t SysOnStartApplication(ApplicationContext *pAppContext)
{
  UNUSED(pAppContext);

  /* Re-map the state machine of the Sensor Manager tasks */
  SensorManagerStateMachineRemap(spAppPMState2SMPMStateMap);

  /* Disable the automatic low power mode timer */
  UtilTaskSetAutoLowPowerModePeriod((UtilTask_t*)spUtilObj, 0);
  /* Set the sensor listener used by the Util task.*/
  UtilTaskSetSensorListener((UtilTask_t*)spUtilObj, &sSensorListener);

  /* connect the sensors task to the I2C bus. */
//  I2CBusTaskConnectDevice((I2CBusTask*)spI2CBusObj, (I2CBusIF*)IIS2MDCTaskGetSensorIF((IIS2MDCTask*)spIIS2MDCObj));
  SPIBusTaskConnectDevice((SPIBusTask*)spSPIBusObj, (SPIBusIF*)ISM330DHCXTaskGetSensorIF((ISM330DHCXTask*)spISM330DHCXObj));

  return SYS_NO_ERROR_CODE;
}

/*IApplicationErrorDelegate *SysGetErrorDelegate(void)
 * {
  // Install the application error manager delegate.
  static IApplicationErrorDelegate *s_pxErrDelegate = NULL;
  if (s_pxErrDelegate == NULL)
  {
    s_pxErrDelegate = AEMAlloc();
  }

  return s_pxErrDelegate;
}*/

IAppPowerModeHelper *SysGetPowerModeHelper(void)
{
  // Install the application power mode helper.
  static IAppPowerModeHelper *s_pxPowerModeHelper = NULL;
  if (s_pxPowerModeHelper == NULL) {
    s_pxPowerModeHelper = AppPowerModeHelperAlloc();
  }

  return s_pxPowerModeHelper;
}

static sys_error_code_t SensorManagerStateMachineRemap(EPowerMode *pPMState2PMStateMap)
{
  assert_param(pPMState2PMStateMap != NULL);

//  AMTSetPMStateRemapFunc((AManagedTask*)spI2CBusObj, pPMState2PMStateMap);
  AMTSetPMStateRemapFunc((AManagedTask*)spSPIBusObj, pPMState2PMStateMap);
//  AMTSetPMStateRemapFunc((AManagedTask*)spIIS2MDCObj, pPMState2PMStateMap);
  AMTSetPMStateRemapFunc((AManagedTask*)spISM330DHCXObj, pPMState2PMStateMap);
  AMTSetPMStateRemapFunc((AManagedTask*)spUtilObj, pPMState2PMStateMap);

  return SYS_NO_ERROR_CODE;
}


