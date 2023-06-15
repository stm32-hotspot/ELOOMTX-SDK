/**
 ******************************************************************************
 * @file    ProcessTask_vtbl.h
 * @author  SRA - GPM
 * 
 * 
 *
 * @brief
 *
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics..
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 ******************************************************************************
 */
#ifndef PROCESSTASK_VTBL_H_
#define PROCESSTASK_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif


/* AManagedTask virtual functions */
sys_error_code_t ProcessTask_vtblHardwareInit(AManagedTask *_this, void *p_params); /*!< @sa AMTHardwareInit */
sys_error_code_t ProcessTask_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *pTaskCode, CHAR **pName, VOID **pStackStart, ULONG *pStackDepth, UINT *pPriority, UINT *pPreemptThreshold, ULONG *pTimeSlice, ULONG *pAutoStart, ULONG *pParams); ///< @sa AMTOnCreateTask
sys_error_code_t ProcessTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode); /*!< @sa AMTDoEnterPowerMode */
sys_error_code_t ProcessTask_vtblHandleError(AManagedTask *_this, SysEvent error); /*!< @sa AMTHandleError */
sys_error_code_t ProcessTask_vtblOnEnterTaskControlLoop(AManagedTask *this); ///< @sa AMTOnEnterTaskControlLoop

/* AManagedTaskEx virtual functions */
sys_error_code_t ProcessTask_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode active_power_mode); /*!< @sa AMTExForceExecuteStep */
sys_error_code_t ProcessTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode); /*!< @sa AMTExOnEnterPowerMode */

#ifdef __cplusplus
}
#endif

#endif /* PROCESSTASK_VTBL_H_ */
