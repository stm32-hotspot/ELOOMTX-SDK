/**
 ******************************************************************************
 * @file    mx.h
 * @author  SRA - GPM
 * 
 * @date    Jul 3, 2020
 *
 * @brief Project peripherals configuration files.
 *
 * This files declares the peripherals configuration parameters that can be used
 * in the Low Level API of an ODeV driver - IDrvInit(). It creates a link
 * between the initialization code generated by CubeMX (see the .ioc file
 * in the tools folder) and the ODeV framework.
 *
 * This example show how a managed task allocates an SPI driver that must be
 * connected to the SPI3 (that has been configured using CubeMX):
 * ~~~{.c}
 * p_task->p_driver = SPIMasterDriverAlloc();
 * if (p_task->p_driver != NULL)
 * {
 *   res = IDrvInit((IDriver*)p_task->p_driver, (void*)&MX_SPI3InitParams);
 *   if (SYS_IS_ERROR_CODE(res)) {
 *     SYS_DEBUGF(SYS_DBG_LEVEL_SEVERE, ("SPIBus task: error during driver initialization\r\n"));
 *   }
 * }
 * ~~~
 *
 *******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 *******************************************************************************
 */
#ifndef INCLUDE_MX_MX_H_
#define INCLUDE_MX_MX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"


/* Driver initialization parameters */
/************************************/

/**
 * ADC configuration parameters.
 */
//typedef struct _MX_ADC1Params_t
//{
//  ADC_HandleTypeDef *p_adc; /*!< HAL ADC handle */
//  void (*p_mx_init_f)(void); /*!< MX ADC initialization function */
//} MX_ADC1Params_t;

/**
 * SPI configuration parameters.
 */
typedef struct _MX_SPIParams_t
{
  SPI_HandleTypeDef *p_spi_handle; /*!< HAL SPI handle */
  IRQn_Type spi_dma_rx_irq_n; /*!< DMA channel rx interrupt number. */
  IRQn_Type spi_dma_tx_irq_n; /*!< DMA channel tx interrupt number. */
  void (*p_mx_init_f)(void); /*!< MX SPI initialization function */
  void (*p_mx_dma_init_f)(void); /*!< MX DMA initialization function */
} MX_SPIParams_t;

/**
  * I2C configuration parameters.
  */
//typedef struct _MX_I2CParams_t
//{
//  I2C_HandleTypeDef *p_i2c_handle; /*!< HAL I2C handle */
//  IRQn_Type i2c_ev_irq_n; /*!< I2C EV interrupt number. */
//  IRQn_Type i2c_er_irq_n; /*!< I2C ER interrupt number. */
//  IRQn_Type i2c_dma_rx_irq_n; /*!< DMA channel rx interrupt number. */
//  IRQn_Type i2c_dma_tx_irq_n; /*!< DMA channel tx interrupt number. */
//  void (*p_mx_init_f)(void); /*!< MX I2C initialization function */
//  void (*p_mx_dma_init_f)(void); /*!< MX DMA initialization function */
//} MX_I2CParams_t;

/**
  * CRC configuration parameters.
  */
typedef struct _MX_CRCParams_t
{
  CRC_HandleTypeDef *p_crc; /*!< HAL CRC handle */
  void (*p_mx_init_f)(void); /*!< MX CRC initialization function */
} MX_CRCParams_t;

/**
 * TIM configuration parameters.
 */
typedef struct _MX_TIMParams_t
{
  TIM_HandleTypeDef *p_tim; /*!< HAL TIM handle */
  IRQn_Type irq_n; /*!< External interrupt number. */
  void (*p_mx_init_f)(void); /*!< MX TIM initialization function */
} MX_TIMParams_t;

/**
 * GPIO Configuration parameters.
 */
typedef struct _MX_GPIOParams_t
{
  void (*p_mx_init_f)(void); /*!< MX GPIO initialization function */
  IRQn_Type irq_n; /*!< External interrupt number. */
  uint32_t pin;
  GPIO_TypeDef *port;
} MX_GPIOParams_t;

/**
 * ADC configuration parameters.
 */
//extern const MX_ADC1Params_t MX_ADC1InitParams;

/**
 * SPI1 configuration parameters.
 */
extern const MX_SPIParams_t MX_SPI1InitParams;

/**
 * CRC configuration parameters.
 */
extern const MX_CRCParams_t MX_CRCInitParams;

/**
 * PE3(INT1_IIS3W_Pin) configuration parameters.
 */
extern const MX_GPIOParams_t MX_GPIO_INT1_IIS3DWB_InitParams;

/**
 * PFA15 (CS_IIS3W_Pin) configuration parameters.
 */
extern const MX_GPIOParams_t MX_GPIO_CS_IIS3DWB_InitParams;

/**
 * PE4 (INT1_DHC_Pin) configuration parameters.
 */
extern const MX_GPIOParams_t MX_GPIO_INT1_ISM330DHCX_InitParams;

/**
 * PA4 (CS_DHC_Pin) configuration parameters.
 */
extern const MX_GPIOParams_t MX_GPIO_CS_ISM330DHCX_InitParams;

/**
 * TIM16 configuration parameters.
 */
extern const MX_TIMParams_t MX_TIM17InitParams;

/**
 * PE1 (STBC02_SW_SEL) configuration parameters.
 */
//extern const MX_GPIOParams_t MX_GPIO_PE1InitParams;

/**
 * PE0 (STBC02_CHG) configuration parameters.
 */
//extern const MX_GPIOParams_t MX_GPIO_PE0InitParams;

/**
 * PD12 (STBC02_CEN) configuration parameters.
 */
//extern const MX_GPIOParams_t MX_GPIO_PD12InitParams;

/**
 * User Button GPIO configuration parameters.
 */
extern const MX_GPIOParams_t MX_GPIO_UBInitParams;

/**
 * LED3 (Red) GPIO configuration parameters.
 */
extern const MX_GPIOParams_t MX_GPIO_LD3InitParams;

/**
 * LD1 (Green) GPIO configuration parameters.
 */
extern const MX_GPIOParams_t MX_GPIO_LD1InitParams;

/**
 * LD2 (Blue) GPIO configuration parameters.
 */
extern const MX_GPIOParams_t MX_GPIO_LD2InitParams;


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_MX_MX_H_ */
