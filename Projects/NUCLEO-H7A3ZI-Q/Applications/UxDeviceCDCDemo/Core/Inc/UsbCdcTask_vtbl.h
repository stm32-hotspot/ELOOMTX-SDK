/**
  ******************************************************************************
  * @file    UsbCdcTask_vtbl.h
  * @author  SRA - GPM
  * 
  * 
  *
  * @brief
  *
  * <DESCRIPTIOM>
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USBCDCTASK_VTBL_H_
#define USBCDCTASK_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Exported functions --------------------------------------------------------*/
// IManagedTask virtual functions
sys_error_code_t UsbCdcTask_vtblHardwareInit(AManagedTask *this, void *pParams); ///< @sa AMTHardwareInit
sys_error_code_t UsbCdcTask_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *pTaskCode, CHAR **pName, VOID **pStackStart, ULONG *pStackDepth, UINT *pPriority, UINT *pPreemptThreshold, ULONG *pTimeSlice, ULONG *pAutoStart, ULONG *pParams); ///< @sa AMTOnCreateTask
sys_error_code_t UsbCdcTask_vtblDoEnterPowerMode(AManagedTask *this, const EPowerMode eActivePowerMode, const EPowerMode eNewPowerMode); ///< @sa AMTDoEnterPowerMode
sys_error_code_t UsbCdcTask_vtblHandleError(AManagedTask *this, SysEvent xError); ///< @sa AMTHandleError
sys_error_code_t UsbCdcTask_vtblOnEnterTaskControlLoop(AManagedTask *this); ///< @sa AMTOnEnterTaskControlLoop

/* AManagedTaskEx virtual functions */
sys_error_code_t UsbCdcTask_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode active_power_mode); /*!< @sa AMTExForceExecuteStep */
sys_error_code_t UsbCdcTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode); /*!< @sa AMTExOnEnterPowerMode */

#ifdef __cplusplus
}
#endif

#endif /* USBCDCTASK_VTBL_H_ */
