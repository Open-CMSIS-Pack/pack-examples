/**
  ******************************************************************************
  * @file    STM32U5OSPI.h
  * @author  MCD Application Team
  * @brief   Header file of STM32U5OSPI.c
  *
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

#ifndef STM32U5OSPI_H
#define STM32U5OSPI_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include <string.h>
#include "stm32u5xx_hal.h"
#include "stm32u575i_eval_ospi.h"
#include "mx25lm51245g.h"

#define TIMEOUT 5000U

/* Private function prototypes -----------------------------------------------*/
int Init_OSPI(void);
int Write (uint32_t Address, uint32_t Size, uint8_t* buffer);
int SectorErase (uint32_t EraseStartAddress ,uint32_t EraseEndAddress);
int MassErase (void);
int SystemClock_Config(void);

#endif /* STM32U5OSPI_H */
