/* -----------------------------------------------------------------------------
 * Copyright (c) 2023 Arm Limited (or its affiliates).
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * $Date:        14. February 2023
 * $Revision:    V1.1
 *
 * Project:      IrDA Driver definitions for STMicroelectronics STM32U5xx
 * -------------------------------------------------------------------------- */

#ifndef __IRDA_STM32U5XX_H
#define __IRDA_STM32U5XX_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "Driver_USART.h"
#include "stm32u5xx_hal.h"

#include "RTE_Components.h"
#include "MX_Device.h"

#define  VM_ASYNC                       (1UL)
#define  VM_SYNC                        (2UL)
#define  VM_IRDA                        (3UL)
#define  VM_SMARTCARD                   (4UL)
#define  Asynchronous                   VM_ASYNC
#define  IrDA                           VM_IRDA

#define IRDA_HAL_STATUS(stat)           ((stat == HAL_OK)      ? ARM_DRIVER_OK :            \
                                        ((stat == HAL_BUSY)    ? ARM_DRIVER_ERROR_BUSY :    \
                                        ((stat == HAL_TIMEOUT) ? ARM_DRIVER_ERROR_TIMEOUT : \
                                                                 ARM_DRIVER_ERROR)))



#define IRDAx_RESOURCE_ALLOC(x)         extern IRDA_HandleTypeDef    IRDA##x##_HANDLE;        \
                                        static IRDA_INFO             IRDA##x##_Info;          \
                                        static IRDA_TRANSFER_INFO    IRDA##x##_TransferInfo;  \
                                        static const IRDA_RESOURCES  IRDA##x##_Resources =  { \
                                                    &IRDA##x##_HANDLE,                        \
                                                     IRDA##x##_PERIPHERAL,                    \
                                                    &IRDA##x##_Info,                          \
                                                    &IRDA##x##_TransferInfo,                  \
                                                     IRDA##x##_DMA_USE_TX,                    \
                                                     IRDA##x##_DMA_USE_RX,                    \
                                                     0                                        \
                                                  }

#define IRDAx_EXPORT_DRIVER(x)                                                                                                                                                          \
static int32_t                USART##x##_Initialize      (ARM_USART_SignalEvent_t cb_event)                  { return IRDA_Initialize (cb_event, &IRDA##x##_Resources); }               \
static int32_t                USART##x##_Uninitialize    (void)                                              { return IRDA_Uninitialize (&IRDA##x##_Resources); }                       \
static int32_t                USART##x##_PowerControl    (ARM_POWER_STATE state)                             { return IRDA_PowerControl (state, &IRDA##x##_Resources); }                \
static int32_t                USART##x##_Send            (const void *data, uint32_t num)                    { return IRDA_Send (data, num, &IRDA##x##_Resources); }                    \
static int32_t                USART##x##_Receive         (void *data, uint32_t num)                          { return IRDA_Receive (data, num, &IRDA##x##_Resources); }                 \
static int32_t                USART##x##_Transfer        (const void *data_out, void *data_in, uint32_t num) { return IRDA_Transfer (data_out, data_in, num, &IRDA##x##_Resources); }   \
static uint32_t               USART##x##_GetGetTxCount   (void)                                              { return IRDA_GetTxCount (&IRDA##x##_Resources); }                         \
static uint32_t               USART##x##_GetGetRxCount   (void)                                              { return IRDA_GetRxCount (&IRDA##x##_Resources); }                         \
static int32_t                USART##x##_Control         (uint32_t control, uint32_t arg)                    { return IRDA_Control (control, arg, &IRDA##x##_Resources); }              \
static ARM_USART_STATUS       USART##x##_GetStatus       (void)                                              { return IRDA_GetStatus (&IRDA##x##_Resources); }                          \
                                                                                                                                                                                        \
ARM_DRIVER_USART Driver_USART##x = { \
  IRDA_GetVersion,                   \
  IRDA_GetCapabilities,              \
  USART##x##_Initialize,             \
  USART##x##_Uninitialize,           \
  USART##x##_PowerControl,           \
  USART##x##_Send,                   \
  USART##x##_Receive,                \
  USART##x##_Transfer,               \
  USART##x##_GetGetTxCount,          \
  USART##x##_GetGetRxCount,          \
  USART##x##_Control,                \
  USART##x##_GetStatus,              \
  IRDA_SetModemControl,              \
  IRDA_GetModemStatus                \
}

// DMA Use
#define IRDA_DMA_USE_TX                 (1U << 0)
#define IRDA_DMA_USE_RX                 (1U << 1)
#define IRDA_DMA_USE_TX_RX              (USART_DMA_USE_TX | USART_DMA_USE_RX)

// IrDA1 Configuration
#ifdef MX_USART1
#if   (MX_USART1_VM == VM_IRDA)

// Peripheral: USART1
#define IRDA1_PERIPHERAL                USART1

// Handle
#define IRDA1_HANDLE                    hirda1

// USART1 used in IrDA mode
#define USART1_MODE_IRDA                1

// DMA Use
#ifdef MX_USART1_TX_DMA_Instance
  #define IRDA1_DMA_USE_TX              IRDA_DMA_USE_TX
#else
  #define IRDA1_DMA_USE_TX              0
#endif
#ifdef MX_USART1_RX_DMA_Instance
  #define IRDA1_DMA_USE_RX              IRDA_DMA_USE_RX
#else
  #define IRDA1_DMA_USE_RX              0
#endif
#define IRDA1_DMA_USE                   (IRDA1_DMA_USE_TX | IRDA1_DMA_USE_RX)
#endif
#endif

// IrDA2 Configuration
#ifdef MX_USART2
#if   (MX_USART2_VM == VM_IRDA)

// Peripheral: USART2
#define IRDA2_PERIPHERAL                USART2

// Handle
#define IRDA2_HANDLE                    hirda2

// USART2 used in IrDA mode
#define USART2_MODE_IRDA                1

// DMA Use
#ifdef MX_USART2_TX_DMA_Instance
  #define IRDA2_DMA_USE_TX              IRDA_DMA_USE_TX
#else
  #define IRDA2_DMA_USE_TX              0
#endif
#ifdef MX_USART2_RX_DMA_Instance
  #define IRDA2_DMA_USE_RX              IRDA_DMA_USE_RX
#else
  #define IRDA2_DMA_USE_RX              0
#endif
#define IRDA2_DMA_USE                   (IRDA2_DMA_USE_TX | IRDA2_DMA_USE_RX)
#endif
#endif

// IrDA3 Configuration
#ifdef MX_USART3
#if   (MX_USART3_VM == VM_IRDA)

// Peripheral: USART3
#define IRDA3_PERIPHERAL                USART3

// Handle
#define IRDA3_HANDLE                    hirda3

// USART3 used in IrDA mode
#define USART3_MODE_IRDA                1

// DMA Use
#ifdef MX_USART3_TX_DMA_Instance
  #define IRDA3_DMA_USE_TX              IRDA_DMA_USE_TX
#else
  #define IRDA3_DMA_USE_TX              0
#endif
#ifdef MX_USART3_RX_DMA_Instance
  #define IRDA3_DMA_USE_RX              IRDA_DMA_USE_RX
#else
  #define IRDA3_DMA_USE_RX              0
#endif
#define IRDA3_DMA_USE                   (IRDA3_DMA_USE_TX | IRDA3_DMA_USE_RX)
#endif
#endif

// IrDA4 Configuration
#ifdef MX_UART4
#if   (MX_UART4_VM == VM_IRDA)

// Peripheral: USART4
#define IRDA4_PERIPHERAL                UART4

// Handle
#define IRDA4_HANDLE                    hirda4

// USART4 used in IrDA mode
#define USART4_MODE_IRDA                1

// DMA Use
#ifdef MX_UART4_TX_DMA_Instance
  #define IRDA4_DMA_USE_TX              IRDA_DMA_USE_TX
#else
  #define IRDA4_DMA_USE_TX              0
#endif
#ifdef MX_UART4_RX_DMA_Instance
  #define IRDA4_DMA_USE_RX              IRDA_DMA_USE_RX
#else
  #define IRDA4_DMA_USE_RX              0
#endif
#define IRDA4_DMA_USE                   (IRDA4_DMA_USE_TX | IRDA4_DMA_USE_RX)
#endif
#endif

// IrDA5 Configuration
#ifdef MX_UART5
#if   (MX_UART5_VM == VM_IRDA)

// Peripheral: USART5
#define IRDA5_PERIPHERAL                UART5

// Handle
#define IRDA5_HANDLE                    hirda5

// USART5 used in IrDA mode
#define USART5_MODE_IRDA                1

// DMA Use
#ifdef MX_UART5_TX_DMA_Instance
  #define IRDA5_DMA_USE_TX              IRDA_DMA_USE_TX
#else
  #define IRDA5_DMA_USE_TX              0
#endif
#ifdef MX_UART5_RX_DMA_Instance
  #define IRDA5_DMA_USE_RX              IRDA_DMA_USE_RX
#else
  #define IRDA5_DMA_USE_RX              0
#endif
#define IRDA5_DMA_USE                   (IRDA5_DMA_USE_TX | IRDA5_DMA_USE_RX)
#endif
#endif

#if defined(USART1_MODE_IRDA)  || \
    defined(USART2_MODE_IRDA)  || \
    defined(USART3_MODE_IRDA)  || \
    defined(USART4_MODE_IRDA)  || \
    defined(USART5_MODE_IRDA)

#define USARTx_MODE_IRDA                1

// Status flags
#define IRDA_FLAG_INITIALIZED           ((uint8_t)(1U))
#define IRDA_FLAG_POWERED               ((uint8_t)(1U << 1))
#define IRDA_FLAG_CONFIGURED            ((uint8_t)(1U << 2))
#define IRDA_FLAG_TX_ENABLED            ((uint8_t)(1U << 3))
#define IRDA_FLAG_RX_ENABLED            ((uint8_t)(1U << 4))

// Transfer Information (Run-Time)
typedef struct {
  uint32_t  rx_num;                     // Total number of receive data
  uint32_t  tx_num;                     // Total number of transmit data
  uint32_t  rx_cnt;                     // Number of data received
  uint32_t  tx_cnt;                     // Number of data sent
  uint16_t  def_val;                    // Default transfer value
  uint16_t  reserved;
} IRDA_TRANSFER_INFO;

// Status Information (Run-time)
typedef struct {
  uint8_t   tx_busy;                    // Transmitter busy flag
  uint8_t   rx_busy;                    // Receiver busy flag
  uint8_t   tx_underflow;               // Transmit data underflow detected (cleared on start of next send operation)
  uint8_t   rx_overflow;                // Receive data overflow detected (cleared on start of next receive operation)
  uint8_t   rx_break;                   // Break detected on receive (cleared on start of next receive operation)
  uint8_t   rx_framing_error;           // Framing error detected on receive (cleared on start of next receive operation)
  uint8_t   rx_parity_error;            // Parity error detected on receive (cleared on start of next receive operation)
  uint8_t   reserved;
} IRDA_STATUS;

// Information (Run-time)
typedef struct {
  ARM_USART_SignalEvent_t  cb_event;    // Event Callback
  volatile IRDA_STATUS     status;      // Status flags
  uint8_t                  flags;       // Current USART flags
  uint8_t                  reserved[3];
} IRDA_INFO;

// Resources definition
typedef const struct {
  IRDA_HandleTypeDef      *h;           // Handle
  void                    *reg;         // Peripheral pointer
  IRDA_INFO               *info;        // Run-Time Information
  IRDA_TRANSFER_INFO      *xfer;        // Transfer information
  uint8_t                  dma_use_tx;
  uint8_t                  dma_use_rx;
  uint16_t                 reserved;
} IRDA_RESOURCES;

// Global functions and variables exported by driver .c module
#ifdef USART1_MODE_IRDA
extern ARM_DRIVER_USART Driver_USART1;
#endif

#ifdef USART2_MODE_IRDA
extern ARM_DRIVER_USART Driver_USART2;
#endif

#ifdef USART3_MODE_IRDA
extern ARM_DRIVER_USART Driver_USART3;
#endif

#ifdef USART4_MODE_IRDA
extern ARM_DRIVER_USART Driver_USART4;
#endif

#ifdef USART5_MODE_IRDA
extern ARM_DRIVER_USART Driver_USART5;
#endif

#endif
#endif /* __IRDA_STM32U5XX_H */
