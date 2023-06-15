/**
  ******************************************************************************
  * @file    mx.c
  * @author  SRA - GPM
  * 
  * 
  *
  * @brief Project peripherals configuration files.
  *
  * This files defines the peripherals configuration parameters that can be used
  * in the Low Level API of an eLooM driver - IDrvInit(). It creates a link
  * between the initialization code generated by CubeMX (see the .ioc file
  * in the tools folder) and the eLooM framework.
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
  ******************************************************************************
  */

#include "mx.h"
#include "tim.h"
#include "i2c.h"
#include "gpio.h"
#include "gpdma.h"
#include "usb_otg.h"


/**
  * TIM7 initialization parameters.
  */
const MX_TIMParams_t MX_TIM7InitParams =
{
  .p_mx_init_f = MX_TIM7_Init,
  .p_tim = &htim7,
  .irq_n = TIM7_IRQn
};

/**
  * I2C1 initialization parameters.
  */
const MX_I2CParams_t MX_I2C1InitParams =
{
  .p_mx_init_f = MX_I2C1_Init,
  .p_i2c_handle = &hi2c1,
  .p_mx_dma_init_f = MX_GPDMA1_InitCustom,
  .i2c_er_irq_n = I2C1_ER_IRQn,
  .i2c_ev_irq_n = I2C1_EV_IRQn,
  .i2c_dma_rx_irq_n = GPDMA1_Channel0_IRQn,
  .i2c_dma_tx_irq_n = GPDMA1_Channel1_IRQn
};

/**
  * (USER_BUTTON) Initialization parameters.
  */
const MX_GPIOParams_t MX_GPIO_UBInitParams =
{
  MX_GPIO_PC13_Init,
  EXTI13_IRQn,
  GPIO_PIN_13,
  GPIOC
};

/**
  * LED Red Initialization parameters.
  */
const MX_GPIOParams_t MX_GPIO_LEDRedInitParams =
{
  MX_GPIO_PG2_Init,
  UsageFault_IRQn, /*!< NOT USED */
  LED_RED_Pin,
  LED_RED_GPIO_Port
};

/**
  * LED Green Initialization parameters.
  */
const MX_GPIOParams_t MX_GPIO_LEDGreenInitParams =
{
  MX_GPIO_PC7_Init,
  UsageFault_IRQn, /*!< NOT USED */
  LED_GREEN_Pin,
  LED_GREEN_GPIO_Port
};

/**
  * LED Blue Initialization parameters.
  */
const MX_GPIOParams_t MX_GPIO_LEDBlueInitParams =
{
  MX_GPIO_PB7_Init,
  UsageFault_IRQn, /*!< NOT USED */
  LED_BLUE_Pin,
  LED_BLUE_GPIO_Port
};

/**
  * (INT1_DHCX) Initialization parameters.
  */
const MX_GPIOParams_t MX_GPIO_INT1_DHCXInitParams =
{
  MX_GPIO_PE9_Init,
  INT1_DHCX_EXTI_IRQn, /*!< NOT USED */
  INT1_DHCX_Pin,
  INT1_DHCX_GPIO_Port
};

/**
  * (INT2_DHCX) Initialization parameters.
  */
const MX_GPIOParams_t MX_GPIO_INT2_DHCXInitParams =
{
  MX_GPIO_PE11_Init,
  INT2_DHCX_EXTI_IRQn, /*!< NOT USED. Must be INT2_DHCX_EXTI_IRQn */
  INT2_DHCX_Pin,
  INT2_DHCX_GPIO_Port
};

/**
  * (INT1_DLPC) Initialization parameters.
  */
const MX_GPIOParams_t MX_GPIO_INT1_DLPCInitParams =
{
  MX_GPIO_PB0_Init,
  INT1_DLPC_EXTI_IRQn, /*!< NOT USED */
  INT1_DLPC_Pin,
  INT1_DLPC_GPIO_Port
};

/**
  * (INT2_DLPC) Initialization parameters.
  */
const MX_GPIOParams_t MX_GPIO_INT2_DLPCInitParams =
{
  MX_GPIO_PC1_Init,
  INT2_DLPC_EXTI_IRQn, /*!< NOT USED. Must be INT2_DHCX_EXTI_IRQn */
  INT2_DLPC_Pin,
  INT2_DLPC_GPIO_Port
};

/**
  * (DRDY_MDC) Initialization parameters.
  */
const MX_GPIOParams_t MX_GPIO_DRDY_MDCInitParams =
{
  MX_GPIO_PC3_Init,
  DRDY_MDC_EXTI_IRQn, /*!< NOT USED. Must be INT2_DHCX_EXTI_IRQn */
  DRDY_MDC_Pin,
  DRDY_MDC_GPIO_Port
};

const MX_PCDParams_t MX_PCDInitParams =
{
  .p_pcd = &hpcd_USB_OTG_FS,
  .p_mx_init_f = MX_USB_OTG_FS_PCD_Init,
  .irq_n = OTG_FS_IRQn
};
