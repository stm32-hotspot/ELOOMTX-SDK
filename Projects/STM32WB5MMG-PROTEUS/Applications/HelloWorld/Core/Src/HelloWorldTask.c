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


#ifndef HW_TASK_CFG_STACK_DEPTH
#define HW_TASK_CFG_STACK_DEPTH         TX_MINIMUM_STACK
#endif

#ifndef HW_TASK_CFG_PRIORITY
#define HW_TASK_CFG_PRIORITY            (TX_MAX_PRIORITIES-2)
#endif

#define SYS_DEBUGF(level, message)      SYS_DEBUGF3(SYS_DBG_HW, level, message)


/* Private member function declaration */
/***************************************/

/**
 * Execute one step of the task control loop while the system is in STATE1 mode.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t HelloWorldTaskExecuteStepState1(AManagedTask *_this);

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
        HelloWorldTaskExecuteStepState1,
        NULL,
        NULL,
    }
};


/* Public API definition */
/*************************/

AManagedTask *HelloWorldTaskAlloc()
{
  HelloWorldTask_t *p_new_obj = SysAlloc(sizeof(HelloWorldTask_t));
  /* Initialize the super class */
  AMTInit(&p_new_obj->super);

  p_new_obj->super.vptr = &sTheClass.vtbl;

  return (AManagedTask*)p_new_obj;
}

AManagedTask *HelloWorldTaskStaticAlloc(void *p_mem_block)
{
  HelloWorldTask_t *p_new_obj = (HelloWorldTask_t*)p_mem_block;

  if (p_new_obj != NULL)
  {
    /* Initialize the super class */
    AMTInit(&p_new_obj->super);

    p_new_obj->super.vptr = &sTheClass.vtbl;
  }

  return (AManagedTask*)p_new_obj;
}


/* AManagedTask virtual functions definition */
/*********************************************/

sys_error_code_t HelloWorldTask_vtblHardwareInit(AManagedTask *_this, void *p_params)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  HelloWorldTask_t *p_obj = (HelloWorldTask_t*)_this; */

  /* for this demo we disable the user button IRQ*/
  HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);

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
/*  HelloWorldTask_t *p_obj = (HelloWorldTask_t*)_this; */

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
/*  HelloWorldTask_t *p_obj = (HelloWorldTask_t*)_this; */

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HW: start.\r\n"));

  return res;
}


/* Private function definition */
/*******************************/

static sys_error_code_t HelloWorldTaskExecuteStepState1(AManagedTask *_this) {
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  HelloWorldTask_t *p_obj = (HelloWorldTask_t*)_this;

  tx_thread_sleep(AMT_MS_TO_TICKS(1000));

  p_obj->counter++;
#ifdef SYS_DEBUG
  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("Hello STM32WB5MMG-PROTEUS %d!!\r\n", p_obj->counter));
#endif
  __NOP();

  return res;
}

