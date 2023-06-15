/**
 ******************************************************************************
 * @file    HelloWorldTask.c
 * @author  SRA - GPM
 * 
 * 
 *
 * @brief
 *
 * 
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

#include "HelloWorldTask.h"
#include "HelloWorldTask_vtbl.h"
#include "services/sysdebug.h"

#ifndef HW_TASK_CFG_STACK_DEPTH
#define HW_TASK_CFG_STACK_DEPTH                  TX_MINIMUM_STACK
#endif

#ifndef HW_TASK_CFG_PRIORITY
#define HW_TASK_CFG_PRIORITY                     (TX_MAX_PRIORITIES-2)
#endif

#define HW_TASK_ANTI_DEBOUNCH_PERIOD_TICK        7U

#define SYS_DEBUGF(level, message)               SYS_DEBUGF3(SYS_DBG_HW, level, message)

/**
 * The only instance of the task object.
 */
static HelloWorldTask s_xHwTaskObj;

// Private member function declaration
// ***********************************

/**
 * Execute one step of the task control loop while the system is in RUN mode.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t HelloWorldTaskExecuteStepRun(AManagedTask *_this);

/**
 * Class object declaration
 */
typedef struct _HelloWorldClass
{
  /**
   * HelloWorldTask class virtual table.
   */
  AManagedTask_vtbl m_xVTBL;


  /**
   * HelloWorldTask (PM_STATE, ExecuteStepFunc) map.
   */
  pExecuteStepFunc_t m_pfPMState2FuncMap[];
} HelloWorldClass;

/**
 * The class object.
 */
static const HelloWorldClass s_xTheClass = {
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
    HelloWorldTaskExecuteStepRun,
    NULL,
    NULL
  }
};

// Inline function forward declaration
// ***********************************

#if defined (__GNUC__)
#endif

// Public API definition
// *********************

AManagedTask* HelloWorldTaskAlloc()
{
  // In this application there is only one Keyboard task,
  // so this allocator implement the singleton design pattern.

  // Initialize the super class
  AMTInit(&s_xHwTaskObj.super);

  s_xHwTaskObj.super.vptr = &s_xTheClass.m_xVTBL;


  return (AManagedTask*) &s_xHwTaskObj;
}

// AManagedTask virtual functions definition
// ***********************************************

sys_error_code_t HelloWorldTask_vtblHardwareInit(AManagedTask *_this, void *pParams)
{
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;

  return xRes;
}

sys_error_code_t HelloWorldTask_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *pvTaskCode, CHAR **pcName,
	    VOID **pvStackStart, ULONG *pnStackSize,
	    UINT *pnPriority, UINT *pnPreemptThreshold,
	    ULONG *pnTimeSlice, ULONG *pnAutoStart,
	    ULONG *pnParams)
{
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
//  HelloWorldTask *pObj = (HelloWorldTask*)_this;

  _this->m_pfPMState2FuncMap = s_xTheClass.m_pfPMState2FuncMap;

//  *pvTaskCode = HelloWorldTaskRun;
  *pvTaskCode = AMTRun;
  *pcName = "HW";
  *pvStackStart = NULL; // allocate the task stack in the system memory pool.
  *pnStackSize = HW_TASK_CFG_STACK_DEPTH;
  *pnParams = (ULONG)_this;
  *pnPriority = HW_TASK_CFG_PRIORITY;
  *pnPreemptThreshold = HW_TASK_CFG_PRIORITY;
  *pnTimeSlice = TX_NO_TIME_SLICE;
  *pnAutoStart = TX_AUTO_START;

  return xRes;
}

sys_error_code_t HelloWorldTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode eActivePowerMode, const EPowerMode eNewPowerMode)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;



  return res;
}

sys_error_code_t HelloWorldTask_vtblHandleError(AManagedTask *_this, SysEvent xError)
{
  assert_param(_this);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;
//  HelloWorldTask *pObj = (HelloWorldTask*)_this;

  return xRes;
}

sys_error_code_t HelloWorldTask_vtblOnEnterTaskControlLoop(AManagedTask *_this)
{
  assert_param(_this);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;


  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HW: start.\r\n"));


  return xRes;
}



// Private function definition
// ***************************

static sys_error_code_t HelloWorldTaskExecuteStepRun(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t xRes = SYS_NO_ERROR_CODE;

  tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND);
  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("Hello World\r\n"));
  __NOP();
  __NOP();

  return xRes;
}

#if 0
/* CubeMX Integration */
/**********************/

void HW_PB_EXTI_Callback(uint16_t pin)
{
  /* anti debounch */
  static uint32_t t_start = 0;
  if(HAL_GetTick() - t_start > 10*HW_TASK_ANTI_DEBOUNCH_PERIOD_TICK)
  {
    if(pin == USER_BUTTON_Pin)
    {
      /* generate the system event to change the PM state*/
      SysEvent evt = {
          .nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_PB, SYS_PM_EVT_PARAM_SHORT_PRESS)
      };
      SysPostPowerModeEvent(evt);

      t_start = HAL_GetTick();
    }
  }

}
#endif
