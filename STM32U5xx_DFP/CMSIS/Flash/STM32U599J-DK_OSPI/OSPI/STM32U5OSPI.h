/**
  ******************************************************************************
  * @file    STM32U5OSPI.h
  * @author  MCD Application Team
  * @brief   Header file of STM32U5OSPI.c
  ******************************************************************************
  */

#ifndef STM32U5OSPI_H
#define STM32U5OSPI_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include <string.h>
#include "stm32u5xx_hal.h"
#include "stm32u599j_discovery_ospi.h"
#include "mx25lm51245g.h"

#define TIMEOUT 5000U

/* Private function prototypes -----------------------------------------------*/
int Init_OSPI(void);
int Write (uint32_t Address, uint32_t Size, uint8_t* buffer);
int SectorErase (uint32_t EraseStartAddress ,uint32_t EraseEndAddress);
int MassErase (void);
int SystemClock_Config(void);

#endif /* STM32U5OSPI_H */
