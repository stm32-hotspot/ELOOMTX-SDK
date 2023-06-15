/**
 ******************************************************************************
 * @file    HelloWorldTask_t.c
 * @author  SRA - GPM
 * 
 * 
 *
 * @brief
 *
 * <DESCRIPTIOM>
 *
 *********************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
 *********************************************************************************
 */

#include "HelloWorldTask.h"
#include "HelloWorldTask_vtbl.h"
#include "services/sysmem.h"
#include "services/sysdebug.h"
#include "services/SQuery.h"
#include "services/SysTimestamp.h"


#ifndef HW_TASK_CFG_STACK_DEPTH
#define HW_TASK_CFG_STACK_DEPTH                   TX_MINIMUM_STACK
#endif

#ifndef HW_TASK_CFG_PRIORITY
#define HW_TASK_CFG_PRIORITY                      (TX_MAX_PRIORITIES-2)
#endif

#define HW_TASK_ANTI_DEBOUNCH_PERIOD_TICK         (12U)

#define SYS_DEBUGF(level, message)      SYS_DEBUGF3(SYS_DBG_HW, level, message)


/* Imported function declaration */
/*********************************/

sys_error_code_t UsbCdcTask_Write(UCHAR *buffer, ULONG requested_length, ULONG *actual_length); //TODO: STF - must I import UsbCdcTask.h ??


/* Private member function declaration */
/***************************************/

/**
 * Execute one step of the task control loop while the system is in STATE1 or SENSOR_ACTIVE mode.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t HelloWorldTaskExecuteStep(AManagedTask *_this);

/**
 * Configure the sensors for the demo:
 * 1. disable all sensors
 * 2. enable and configure one sensor
 *
 * @param _this [IN] Specifies a pointer to a task object.
 */
static void HelloWorldTaskConfigureSensors(HelloWorldTask_t *_this);


/* Module object instances */
/***************************/

/**
 * The only instance of the task object.
 */
static HelloWorldTask_t *spTaskObj = NULL;


/**
 * Class object declaration
 */
typedef struct _HelloWorldClass {
  /**
   * HelloWorldTask_t class virtual table.
   */
  AManagedTask_vtbl vtbl;

  /**
   * HelloWorldTask_t (PM_STATE, ExecuteStepFunc) map.
   */
  pExecuteStepFunc_t m_pfPMState2FuncMap[];
} HelloWorldClass;

/**
 * The class object.
 */
static const HelloWorldClass sTheClass = {
    /* Class virtual table */
    {
        HelloWorldTask_vtblHardwareInit,
        HelloWorldTask_vtblOnCreateTask,
        HelloWorldTask_vtblDoEnterPowerMode,
        HelloWorldTask_vtblHandleError,
        HelloWorldTask_vtblOnEnterTaskControlLoop
    },

    /* class (PM_STATE, ExecuteStepFunc) map */
    {
        HelloWorldTaskExecuteStep,
        NULL,
        HelloWorldTaskExecuteStep,
    }
};


/* Public API definition */
/*************************/

AManagedTask *HelloWorldTaskAlloc(const void *pLEDConfigMX, const void *pUBConfigMX)
{
  if (spTaskObj == NULL)
  {
    HelloWorldTask_t *p_new_obj = SysAlloc(sizeof(HelloWorldTask_t));
    if (p_new_obj != NULL)
    {
      /* Initialize the super class */
      AMTInit(&p_new_obj->super);

      p_new_obj->super.vptr = &sTheClass.vtbl;

      p_new_obj->pLEDConfigMX = pLEDConfigMX;
      p_new_obj->pUBConfigMX = pUBConfigMX;
      spTaskObj = p_new_obj;
    }

  }

  return (AManagedTask*)spTaskObj;
}

AManagedTask *HelloWorldTaskStaticAlloc(void *p_mem_block, const void *pLEDConfigMX, const void *pUBConfigMX)
{
  if (spTaskObj == NULL)
  {
    HelloWorldTask_t *p_new_obj = (HelloWorldTask_t*)p_mem_block;

    if (p_new_obj != NULL)
    {
      /* Initialize the super class */
      AMTInit(&p_new_obj->super);

      p_new_obj->super.vptr = &sTheClass.vtbl;

      p_new_obj->pLEDConfigMX = pLEDConfigMX;
      p_new_obj->pUBConfigMX = pUBConfigMX;

      spTaskObj = p_new_obj;
    }
  }


  return (AManagedTask*)spTaskObj;
}

uint16_t HelloWorldTaskSelectSensor(HelloWorldTask_t *_this)
{
  assert_param(_this != NULL);
  uint16_t sensor_id = SI_NULL_SENSOR_ID;

  /*use the SensorManager query service to get the ID of the accelerometer. */
  SQuery_t query;
  SQInit(&query, SMGetSensorManager());
//  sensor_id = SQNextByNameAndType(&query, "ism330dhcx", COM_TYPE_ACC);
  sensor_id = SQNextByNameAndType(&query, "iis2dlpc", COM_TYPE_ACC);
//  sensor_id = SQNextByNameAndType(&query, "iis2mdc", COM_TYPE_MAG);

  /* Check the sensors configuration in the function HelloWorldTaskConfigureSensors().*/

  return sensor_id;
}

/* AManagedTask virtual functions definition */
/*********************************************/

sys_error_code_t HelloWorldTask_vtblHardwareInit(AManagedTask *_this, void *p_params)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  HelloWorldTask_t *p_obj = (HelloWorldTask_t*)_this;

  if (p_obj->pLEDConfigMX != NULL)
  {
    /*configure the LED*/
    MX_GPIOParams_t *p_led_params = (MX_GPIOParams_t*)p_obj->pLEDConfigMX;
    p_led_params->p_mx_init_f();
  }

  if (p_obj->pUBConfigMX != NULL)
  {
    /* configure the user button*/
    MX_GPIOParams_t *p_ub_params = (MX_GPIOParams_t*)p_obj->pUBConfigMX;
    p_ub_params->p_mx_init_f();
  }

  return res;
}

sys_error_code_t HelloWorldTask_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *p_task_code, CHAR **p_name,
    VOID **pvStackStart, ULONG *p_stack_size,
    UINT *p_priority, UINT *p_preempt_threshold,
    ULONG *p_time_slice, ULONG *p_auto_start,
    ULONG *p_params)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  HelloWorldTask_t *p_obj = (HelloWorldTask_t*)_this;

  p_obj->counter = 0;
  _this->m_pfPMState2FuncMap = sTheClass.m_pfPMState2FuncMap;

  *p_task_code = AMTRun;
  *p_name = "HW";
  *pvStackStart = NULL; /* allocate the task stack in the system memory pool. */
  *p_stack_size = HW_TASK_CFG_STACK_DEPTH;
  *p_params = (ULONG)_this;
  *p_priority = HW_TASK_CFG_PRIORITY;
  *p_preempt_threshold = HW_TASK_CFG_PRIORITY;
  *p_time_slice = TX_NO_TIME_SLICE;
  *p_auto_start = TX_AUTO_START;

  return res;
}

sys_error_code_t HelloWorldTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  HelloWorldTask_t *p_obj = (HelloWorldTask_t*)_this;
  MX_GPIOParams_t *p_led_params = (MX_GPIOParams_t*)p_obj->pLEDConfigMX;

  if (new_power_mode == E_POWER_MODE_SENSORS_ACTIVE)
  {
    SysTsStart(SysGetTimestampSrv(), true);
  }
  else if (new_power_mode == E_POWER_MODE_STATE1)
  {
    SysTsStop(SysGetTimestampSrv());
    if (p_led_params != NULL)
    {
      HAL_GPIO_WritePin(p_led_params->port, p_led_params->pin, GPIO_PIN_SET);
    }
  }

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HW: %d -> %d.\r\n", active_power_mode, new_power_mode));

  return res;
}

sys_error_code_t HelloWorldTask_vtblHandleError(AManagedTask *_this, SysEvent error) {
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  HelloWorldTask_t *p_obj = (HelloWorldTask_t*)_this; */

  return res;
}

sys_error_code_t HelloWorldTask_vtblOnEnterTaskControlLoop(AManagedTask *_this) {
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  HelloWorldTask_t *p_obj = (HelloWorldTask_t*)_this;

  if (p_obj->pLEDConfigMX != NULL)
  {
    MX_GPIOParams_t *p_led_params = (MX_GPIOParams_t*)p_obj->pLEDConfigMX;
    HAL_GPIO_WritePin(p_led_params->port, p_led_params->pin, GPIO_PIN_SET);
  }

  HelloWorldTaskConfigureSensors(p_obj);

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HW: start.\r\n"));

  return res;
}


/* Private function definition */
/*******************************/

static sys_error_code_t HelloWorldTaskExecuteStep(AManagedTask *_this) {
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  HelloWorldTask_t *p_obj = (HelloWorldTask_t*)_this;
  static bool half_period = true;
  MX_GPIOParams_t *p_led_params = (MX_GPIOParams_t*)p_obj->pLEDConfigMX;

  static UCHAR usb_message[] = "ciao! xxxxxxx \r\n";

  tx_thread_sleep(AMT_MS_TO_TICKS(500));
  ULONG actual_length;

  if (!half_period)
  {
    p_obj->counter++;
    EPowerMode active_power_mode = SysGetPowerStatus().active_power_mode;
    if (active_power_mode == E_POWER_MODE_STATE1)
    {
      SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("Hello NUCLEO-U575ZI-Q %d!!\r\n", p_obj->counter));

      if (p_obj->counter > 5)
      {
        snprintf((char*)usb_message, sizeof(usb_message), "ciao! %ld \r\n", p_obj->counter);
        size_t usb_msg_size = strlen((char*)usb_message);
        UsbCdcTask_Write(usb_message, usb_msg_size, &actual_length);
      }
    }
    else if (active_power_mode == E_POWER_MODE_SENSORS_ACTIVE)
    {
      if (p_led_params != NULL)
      {
        HAL_GPIO_TogglePin(p_led_params->port, p_led_params->pin);
      }
    }
  }

  __NOP();
  half_period = !half_period;

  return res;
}

static void HelloWorldTaskConfigureSensors(HelloWorldTask_t *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  UNUSED(_this);

  uint16_t sensor_id = SI_NULL_SENSOR_ID;

  /*use the SensorManager iterator service to disable all sensors*/
  SIterator_t iterator;
  SIInit(&iterator, SMGetSensorManager());
  while (SIHasNext(&iterator))
  {
    SMSensorDisable(SINext(&iterator));
  }
  SYS_DEBUGF3(SYS_DBG_ON, SYS_DBG_LEVEL_VERBOSE, ("HW: disabled %d sensors.\r\n", SMGetNsensor()));

  sensor_id = HelloWorldTaskSelectSensor(_this);
  if (sensor_id != SI_NULL_SENSOR_ID)
  {
    SYS_DEBUGF3(SYS_DBG_ON, SYS_DBG_LEVEL_VERBOSE, ("DEMO: found sensor %d\r\n", sensor_id));

//    res = SMSensorSetODR(sensor_id, 52.0f); // for ism330dhcx
    res = SMSensorSetODR(sensor_id, 50.0f); // for iis2dlpc and iis2mdc
    assert_param(!SYS_IS_ERROR_CODE(res));
    res = SMSensorEnable(sensor_id);
    assert_param(!SYS_IS_ERROR_CODE(res));
  }
}

/* CubeMX integration */
/**********************/

void HelloWorld_USR_EXTI_Callback(uint16_t pin)
{
  /* anti debounch */
  static uint32_t t_start = 0;
  if(HAL_GetTick() - t_start > HW_TASK_ANTI_DEBOUNCH_PERIOD_TICK)
  {
    /* trigger the power mode transaction */
    SysEvent evt = {.nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_PB, SYS_PM_EVT_PARAM_SHORT_PRESS) };
    SysPostPowerModeEvent(evt);
  }
}

