/**
  ******************************************************************************
  * @file    UsbCdcTask.h
  * @author  SRA - GPM
  * 
  * 
  *
  * @brief USB_CDC Class
  *
  * This managed task is responsible to manage the USB_CDC communication
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
#ifndef USBCDCTASK_H_
#define USBCDCTASK_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "services/systp.h"
#include "services/syserror.h"
#include "services/AManagedTaskEx.h"
#include "services/AManagedTaskEx_vtbl.h"
#include "IUsbDelegate.h"
#include "IUsbDelegate_vtbl.h"

/* Exported constants --------------------------------------------------------*/
#define APP_RX_DATA_SIZE (2048)

/* Exported types ------------------------------------------------------------*/
/**
 * Create  type name for _UsbCdcTask.
 */
typedef struct _UsbCdcTask UsbCdcTask_t;

/**
 *  UsbCdcTask internal structure.
 */
struct _UsbCdcTask {
  /**
   * Base class object.
   */
  AManagedTaskEx super;

  /**
   * Task input message queue. The task receives message of type struct AIMessage_t in this queue.
   * This is one of the way the task expose its services at application level.
   */
  TX_QUEUE in_queue;

  /**
   * Mutex to protect USB Write function
   */
  TX_MUTEX write_mutex;

  /**
   * Delegate interface for received USB data.
   */
  IUsbDelegate *p_delegate;

  /**
   * Data received over USB is stored in this buffer
   */
  uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

};

/* Exported functions --------------------------------------------------------*/

// Public API declaration
//***********************

/**
 * Allocate an instance of UsbCdcTask.
 *
 * @return a pointer to the generic obeject ::AManagedTask if success,
 * or NULL if out of memory error occurs.
 */
AManagedTaskEx *UsbCdcTaskAlloc(void);

/**
 * Static function to send data to the USB interface.
 * If the buffer is too big to be sent in a single shot, the function will take care of
 * splitting it into smaller chunks.
 *
 * @param buffer [IN] Pointer to data to write.
 * @param requested_length [IN] Length of bytes to write.
 * @param actual_length [OUT] Pointer to save the actual number of bytes written. It is set to zero if a problem in the transmission occurs.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
sys_error_code_t UsbCdcTask_Write(UCHAR *buffer, ULONG requested_length, ULONG *actual_length);

/**
 * Set the USB delegate to manage received USB data
 *
 * @param _this [IN] specifies a pointer to the object.
 * @param delegate [IN] specifies a pointer to the delegate instance.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise.
 */
sys_error_code_t UsbCdcTask_SetDelegate(UsbCdcTask_t *_this, IUsbDelegate *delegate);


#ifdef __cplusplus
}
#endif

#endif /* USBCDCTASK_H_ */
