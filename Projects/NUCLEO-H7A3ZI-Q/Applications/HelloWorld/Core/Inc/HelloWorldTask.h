/**
 ******************************************************************************
 * @file    HelloWorldTask.h
 * @author  SRA - GPM
 * 
 * 
 *
 * @brief Display a message in the debug log.
 *
 * This managed task is responsible to display a greeting message
 * in the debug log with a period of 1 second.
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
#ifndef HELLOWORLDTASK_H_
#define HELLOWORLDTASK_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "services/AManagedTask.h"
#include "services/AManagedTask_vtbl.h"


/**
 * Create  type name for _HelloWorldTask.
 */
typedef struct _HelloWorldTask HelloWorldTask_t;

/**
 *  HelloWorldTask internal structure.
 */
struct _HelloWorldTask {
  /**
   * Base class object.
   */
  AManagedTask super;

  // Task variables should be added here.

  /**
   * Simple counter displayed in the debug log message
   */
  uint32_t counter;

};


/* Public API declaration */
/**************************/

/**
 * Allocate an instance of HelloWorldTask in the framework heap.
 *
 * @return a pointer to the generic object ::AManagedTask if success,
 * or NULL if out of memory error occurs.
 */
AManagedTask *HelloWorldTaskAlloc();

/**
 * Allocate an instance of ::HelloWorldTask_t in a memory block specified by the application.
 * The size of the memory block must be greater or equal to `sizeof(HelloWorldTask_t)`.
 * This allocator allows the application to avoid the dynamic allocation.
 *
 * \code
 * HelloWorldTask_t my_task;
 * HelloWorldTaskStaticAlloc(&my_task);
 * \endcode
 *
 * @param p_mem_block [IN] specify a memory block allocated by the application.
 *        The size of the memory block must be greater or equal to `sizeof(HelloWorldTask_t)`.
 * @return a pointer to the generic object ::AManagedTaskEx_t if success,
 * or NULL if out of memory error occurs.
 */
AManagedTask *HelloWorldTaskStaticAlloc(void *p_mem_block);


/* Inline functions definition */
/*******************************/


#ifdef __cplusplus
}
#endif

#endif /* HELLOWORLDTASK_H_ */
