/**
  ******************************************************************************
  * @file    Loader_Src.h
  * @author  MCD Tools Team
  * @date    November-2016
  * @brief   Loader Header file.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LOADER_SRC_H
#define __LOADER_SRC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"
#include "stm32u5xx_hal_ospi.h"
//#include "mx25lm51245g.h"
//#include "stm32u599j_discovery_ospi.h"

#define TIMEOUT 5000U

/* Private function prototypes -----------------------------------------------*/
int Init_OSPI(void);
int SectorErase (uint32_t EraseStartAddress ,uint32_t EraseEndAddress);
int Write (uint32_t Address, uint32_t Size, uint8_t* buffer);
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority);
int MassErase (uint32_t Parallelism );
void SystemClock_Config(void);
void HAL_MspInit(void);


#endif /* __LOADER_SRC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
