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
 * This demo add processing capability to the SensorManager demo by using
 * the Digital Processing Unit (DPU) eLooM component. In the entry point
 * SysOnStartApplication() a DPU is allocated, initialized and connect to the
 * selected sensor. An instance of DProcessTask1 allow to process teh data coming
 * from the sensor in a concurrent thread.
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
 * selected sensor start to produce data and feed the DPU. When there are
 * enough data the processing functoin is triggered and the result is displaied
 * in the log.
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
#include "DProcessTask1.h"
#include "mx.h"

/* SensorManager include*/
#include "SPIBusTask.h"
#include "ISM330DHCXTask.h"
#include "SensorManager.h"

/* DPU include*/
#include "Dummy_DPU2.h"
#include "Int16toFloatDataBuilder.h"


/**
 * Application managed task.
 */
static AManagedTask *spHelloWorldObj = NULL;

/**
 * SPI bus task object.
 */
static AManagedTaskEx *spSPIBusObj = NULL;

/**
 * Sensor task object: 3-axis accelerometer and 3-axis gyroscope
 */
static AManagedTaskEx *spISM330DHCXObj = NULL;

/**
 * Processing task. It provides an execution flow for the DPU used in this example.
 */
static DProcessTask1_t sProcessTask;


/* eLooM framework entry points definition */
/*******************************************/

sys_error_code_t SysLoadApplicationContext(ApplicationContext *pAppContext)
{
  assert_param(pAppContext != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* Allocate the task objects */
  spHelloWorldObj = HelloWorldTaskAlloc(NULL, &MX_GPIO_UBInitParams);
  spSPIBusObj = SPIBusTaskAlloc(&MX_SPI1InitParams);
  spISM330DHCXObj  = ISM330DHCXTaskAlloc(&MX_GPIO_INT1_ISM330DHCX_InitParams, NULL, &MX_GPIO_CS_ISM330DHCX_InitParams);

  /* allocate the process task. In this case we use the static allocation. In a embedded application
   * the memory management is critical. The DProcessTask_t support both static and dynamic allocation. */
  (void) DProcessTask1StaticAlloc(&sProcessTask);


  /* Add the task object to the context. */
  res = ACAddTask(pAppContext, spHelloWorldObj);
  res = ACAddTask(pAppContext, (AManagedTask*) spSPIBusObj);
  res = ACAddTask(pAppContext, (AManagedTask*) spISM330DHCXObj);
  res = ACAddTask(pAppContext, (AManagedTask*) &sProcessTask);

  return res;
}

sys_error_code_t SysOnStartApplication(ApplicationContext *pAppContext)
{
  UNUSED(pAppContext);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  SPIBusTaskConnectDevice((SPIBusTask *) spSPIBusObj, (SPIBusIF *)ISM330DHCXTaskGetSensorIF((ISM330DHCXTask *) spISM330DHCXObj));

  /*Select a sensor for the demo*/
  uint16_t sensor_id = HelloWorldTaskSelectSensor((HelloWorldTask_t*)spHelloWorldObj);
  /*The sensor is configured by the HelloWorkdTask::ConfigureSensors() before entering the task control loop*/

  /*get the ISourceObservable IF of the sensor*/
  ISourceObservable *p_data_src = SMGetSensorObserver(sensor_id);

  /*Step 1: allocate and initialize the DPU.*/
  /*this is a test DPU */
  ADPU2_t *p_dpu = (ADPU2_t*) Dummy_DPU2Alloc();
  (void)Dummy_DPU2Init((Dummy_DPU2_t*)p_dpu, 512, 3);
  (void)ADPU2_SetTag(p_dpu, 0x12); /* optional. A tag is useful to identify a DPU along the process*/

  /*Step 2 (optional): register the DPU with the processing task*/
  (void) DPT1AddDPU(&sProcessTask, p_dpu);

  /*Step 3: allocate the input data buffer for the DPU. The memory allocation is delegated to the application. */
  /* how many signal I want to handle in parallel?
   * If the ODR is not too high or the inference time is not too long, then a buffer for two signals should be enough:
   * - 1 buffer to acquire a data in the sensor task
   * - 1 buffer to process a data in the process task
   */
  const uint8_t max_signals_in_buffer = 2;
  size_t buff_size = ADPU2_GetInDataPayloadSize(p_dpu) * max_signals_in_buffer;
  /*allocate the input buffer in the framework heap*/
  uint8_t *p_buff = SysAlloc(buff_size);
  assert_param(p_buff != NULL);
  res = DPT1SetInDataBuffer(&sProcessTask, p_buff, buff_size);
  assert_param(!SYS_IS_ERROR_CODE(res));
  /*allocate the output data buffer for the DPU: it must be big enough to store 1 output data.*/
  buff_size = ADPU2_GetOutDataPayloadSize(p_dpu);
  /*allocate the output buffer in the framework heap*/
  p_buff = SysAlloc(buff_size);
  assert_param(p_buff != NULL);
  res = DPT1SetOutDataBuffer(&sProcessTask, p_buff, buff_size);
  assert_param(!SYS_IS_ERROR_CODE(res));

  /*Step4: connect the DPU of the Processing task with the data source (sensor).
   * We need a data builder to convert the data from the sensor format (int16_t) to the input data wanted by the DPU (float) */
  IDataBuilder_t *p_data_builder = Int16ToFloatDB_Alloc();
  if (p_data_builder != NULL)
  {
    res = DPT1AttachToDataSource(&sProcessTask, p_data_src, p_data_builder, E_IDB_NO_DATA_LOSS);
    assert_param(!SYS_IS_ERROR_CODE(res));
    /*I want to process the data in the process task execution flow and not inline in the sensor task*/
    (void) DPT1EnableAsyncDataProcessing(&sProcessTask, true);
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



