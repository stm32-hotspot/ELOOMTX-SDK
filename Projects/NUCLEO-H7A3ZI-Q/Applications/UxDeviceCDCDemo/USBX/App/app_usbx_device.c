/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_usbx_device.c
  * @author  MCD Application Team
  * 
  * 
  * @brief   USBX Device applicative file
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_usbx_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ux_system.h"
#include "ux_utility.h"
#include "ux_device_stack.h"
#include "ux_dcd_stm32.h"
#include "ux_device_descriptors.h"
#include "services/sysmem.h"
#include "ux_device_class_cdc_acm.h"

#include "usb_otg.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern PCD_HandleTypeDef            hpcd_USB_OTG_HS;

/* CDC Class Calling Parameter structure */
UX_SLAVE_CLASS_CDC_ACM_PARAMETER    cdc_acm_parameter;

/* Define constants.  */
#define USBX_APP_STACK_SIZE         1024
#define USBX_MEMORY_SIZE            (7 * 1024)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
VOID USBD_CDC_ACM_Activate(VOID *cdc_acm_instance);
VOID USBD_CDC_ACM_Deactivate(VOID *cdc_acm_instance);
/* USER CODE END PFP */
/**
  * @brief  Application USBX Device Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT MX_USBX_Device_Init(VOID *memory_ptr)
{
  UINT ret = UX_SUCCESS;

  UCHAR *p_mem;
  /* Device framework FS length*/
  ULONG device_framework_fs_length;
  /* Device framework HS length*/
  ULONG device_framework_hs_length;
  /* Device String framework length*/
  ULONG string_framework_length;
  /* Device language id framework length*/
  ULONG languge_id_framework_length;
  /* Device Framework Full Speed */
  UCHAR *device_framework_full_speed;
  /* Device Framework High Speed */
  UCHAR *device_framework_high_speed;
  /* String Framework*/
  UCHAR *string_framework;
  /* Language_Id_Framework*/
  UCHAR *p_language_id;

  /* Allocate USBX_MEMORY_SIZE. */
  p_mem = (UCHAR*)SysAlloc(USBX_MEMORY_SIZE);

  /* Initialize USBX Memory */
  if (ux_system_initialize(p_mem, USBX_MEMORY_SIZE, UX_NULL, 0) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Get Device Framework High Speed and get the length */
  device_framework_high_speed = USBD_Get_Device_Framework_Speed(USBD_HIGH_SPEED, &device_framework_hs_length);

  /* Get_Device_Framework_Full_Speed and get the length */
  device_framework_full_speed = USBD_Get_Device_Framework_Speed(USBD_FULL_SPEED, &device_framework_fs_length);

  /* Get_String_Framework and get the length */
  string_framework = USBD_Get_String_Framework(&string_framework_length);

  /* Get_Language_Id_Framework and get the length */
  p_language_id = USBD_Get_Language_Id_Framework(&languge_id_framework_length);

  /* The code below is required for installing the device portion of USBX.
  In this application */
  ret =  ux_device_stack_initialize(device_framework_high_speed,
                                    device_framework_hs_length,
                                    device_framework_full_speed,
                                    device_framework_fs_length,
                                    string_framework,
                                    string_framework_length,
                                    p_language_id,
                                    languge_id_framework_length, UX_NULL);

  /* Check the device stack class status */
  if (ret != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /*
   * Don't need to control VCP Parameters in this implementation
   */
  /* Initialize the CDC class parameters for the device. */
  cdc_acm_parameter.ux_slave_class_cdc_acm_instance_activate = USBD_CDC_ACM_Activate;

  /* Deinitialize the CDC class parameters for the device. */
  cdc_acm_parameter.ux_slave_class_cdc_acm_instance_deactivate = USBD_CDC_ACM_Deactivate;

  /* Manage the CDC class requests */
  cdc_acm_parameter.ux_slave_class_cdc_acm_parameter_change = UX_NULL;

  /* registers a slave class to the slave stack. The class is connected with
     interface 0 */
  ret = ux_device_stack_class_register(_ux_system_slave_class_cdc_acm_name,
                                       ux_device_class_cdc_acm_entry, 1, 0,
                                       (VOID *)&cdc_acm_parameter);

  /* Check the device stack class status */
  if (ret != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  return ret;
}

/* USER CODE BEGIN 1 */

/**
  * @brief MX_USB_Device_Init
  *        Initialization of USB device.
  * Init USB device Library, add supported class and start the library
  * @retval None
  */
void MX_USB_Device_Init(void)
{
  MX_USB_OTG_HS_PCD_Init();

//  HAL_PCDEx_SetRxFiFo(&HPCD_USB_OTG_FS, 0x100);
//  HAL_PCDEx_SetTxFiFo(&HPCD_USB_OTG_FS, 0, 0x10);
//  HAL_PCDEx_SetTxFiFo(&HPCD_USB_OTG_FS, 1, 0x20);
//  HAL_PCDEx_SetTxFiFo(&HPCD_USB_OTG_FS, 2, 0x10);

  HAL_PCDEx_SetRxFiFo(&hpcd_USB_OTG_HS, 0x200);
  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_HS, 0, 0x10);
  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_HS, 1, 0x10);
  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_HS, 2, 0x20);

  /* initialize the device controller driver*/
  _ux_dcd_stm32_initialize((ULONG)USB_OTG_HS, (ULONG)&hpcd_USB_OTG_HS);
}

uint32_t CDC_ACM_ActivateCounter = 0;
VOID USBD_CDC_ACM_Activate(VOID *cdc_acm_instance)
{
  CDC_ACM_ActivateCounter++;
}

uint32_t CDC_ACM_DeactivateCounter = 0;
VOID USBD_CDC_ACM_Deactivate(VOID *cdc_acm_instance)
{
  CDC_ACM_DeactivateCounter++;
}

/* USER CODE END 1 */
