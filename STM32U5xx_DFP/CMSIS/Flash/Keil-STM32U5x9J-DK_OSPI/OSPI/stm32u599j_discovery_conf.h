/**
  ******************************************************************************
  * @file    stm32u599j_discovery_conf_template.h
  * @author  MCD Application Team
  * @brief   STM32U599J_DISCOVERY board configuration file.
  *          This file should be copied to the application folder and renamed
  *          to stm32u599j_discovery_conf.h .
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32U599J_DISCOVERY_CONF_H
#define STM32U599J_DISCOVERY_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"

/* Usage of STM32U599J_DISCOVERY board */
#define USE_STM32U599J_DISCOVERY      1U

  /* COM define */
#define USE_BSP_COM_FEATURE                  1U
#define USE_COM_LOG                          0U
  
/* I2C1 and I2C2 Frequencies in Hz  */
#define BUS_I2C1_FREQUENCY                   100000UL /* Frequency of I2C1 = 100 KHz*/
#define BUS_I2C2_FREQUENCY                   100000UL /* Frequency of I2C2 = 100 KHz*/
  
/* Button user interrupt priority */
#define BSP_BUTTON_USER_IT_PRIORITY          0x0FUL  /* Default is lowest priority level */

/* LCD interrupt priorities */
#define BSP_LCD_GFXMMU_IT_PRIORITY 0x0FUL  /* Default is lowest priority level */
#define BSP_LCD_LTDC_IT_PRIORITY   0x0FUL  /* Default is lowest priority level */
#define BSP_LCD_DSI_IT_PRIORITY    0x0FUL  /* Default is lowest priority level */
#define BSP_LCD_DMA2D_IT_PRIORITY  0x0FUL  /* Default is lowest priority level */

/* HSPI RAM interrupt priority */
#define BSP_HSPI_RAM_IT_PRIORITY     0x0FUL  /* Default is lowest priority level */
#define BSP_HSPI_RAM_DMA_IT_PRIORITY 0x0FUL  /* Default is lowest priority level */

#ifdef __cplusplus
}
#endif

#endif /* STM32U599J_DISCOVERY_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
