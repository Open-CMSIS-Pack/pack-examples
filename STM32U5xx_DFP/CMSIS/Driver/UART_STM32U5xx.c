/* -----------------------------------------------------------------------------
 * Copyright (c) 2021-2023 Arm Limited (or its affiliates).
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
 * $Date:        15. February 2023
 * $Revision:    V1.2
 *
 * Driver:       Driver_USART1/2/3/4/5/6
 *
 * Configured:   via CubeMX
 * Project:      USART Driver for STMicroelectronics STM32U5xx
 * -------------------------------------------------------------------------- */

/*! \page stm32u5_uart USART (Asynchronous mode)

# Revision History

- Version 1.2
  - Made variable status volatile (solved potential LTO problems)
- Version 1.1
  - Corrected GetStatus function and status flags handling
  - Corrected Control function to support 9 data bits in DMA mode
  - Corrected GetTxCount and GetRxCount functions in DMA mode
  - Removed Rx Timeout capability and event signalization
- Version 1.0
  - Initial release

# Capabilities

This driver supports **Asynchronous** mode.

# Instances

Hardware resource relating to driver instance is shown in the table below:

Driver Instance | Hardware Resource
:---------------|:-----------------:
Driver_USART1   | USART1
Driver_USART2   | USART2
Driver_USART3   | USART3
Driver_USART4   | UART4
Driver_USART5   | UART5
Driver_USART6   | LPUART1

\note
The **CMSIS Driver:USART (API):USART** component in the **Manage Run-Time Environment** dialog adds multiple C source files to the project.\n
The interface selection in STM32CubeMX selects the actual implementation that is compiled: IrDA, SmartCard, UART or USART.

# Limitations

STM32 HAL limitations:
 - Rx Timeout event disables further reception
 - No callback available for Rx Idle event
 - Stop bits 0.5 and 1.5 are not supported
 - Manual control of modem lines is not supported
 - Break character is handled as framing error

# Configuration

## STM32CubeMX

The USART driver requires:
  - **UART/USART peripheral clock**
  - **UART/USART** peripheral configured as **Asynchronous**
  - optional **Hardware Flow Control**
  - **UART/USART TX, Rx** and optional **Hardware flow Control pins**
  - optional **DMA** configuration for transfers

# B-U585I-IOT02A Board Configuration

These settings are relevant for this board, for different board please refer to the board
schematic for proper configuration.

Link to [B-U585I-IOT02A board schematic](https://www.st.com/resource/en/schematic_pack/mb1551-u585i-c02_schematic.pdf).

## STM32CubeMX

Required peripherals for the **B-U585I-IOT02A** board are listed in the table below:

Peripheral | Mode                         | Description
:---------:|:----------------------------:|:-----------
USART3     | Asynchronous                 | USART3 Controller
PD8        | Alternate Function Push Pull | USART3 TX Pin
PD9        | Alternate Function Push Pull | USART3 RX Pin

\note All settings have to be configured as described in the procedure below. Important settings,
      typically the ones different from default, are emphasized in **bold**.

### Pinout & Configuration tab

  1. In the **Pinout view** window click on a pin and select it's functionality:
        Pin      | Functionality
        :--------|:-------------:
        PD8      | **USART3_TX**
        PD9      | **USART3_RX**
       \n

  2. Under **Categories**: **Connectivity** select **USART3**:

     __Mode__:
       - Mode: **Asynchronous**
       - Hardware Flow Control (RS232): Disable
       - Hardware Flow Control (RS485): unchecked
       - Software NSS Management: unchecked

     __Configuration__:
       - Parameter Settings: not used
       - User Constants: not used
       - NVIC Settings: configured in later step (under Category: System Core: NVIC)
       - DMA Settings: configured in later step (under Category: System Core: GPDMA1)
       - GPIO Settings:
            Pin Name | Signal on Pin | Pin Context..| Pin Priv..| GPIO output..| GPIO mode                     | GPIO Pull-up/Pull..| Maximum out..| Fast Mode | User Label
            :--------|:-------------:|:------------:|:---------:|:------------:|:-----------------------------:|:------------------:|:------------:|:---------:|:----------:
            PD8      | USART3_TX     | n/a          | n/a       | n/a          | Alternate Function Push Pull  | No pull-up and no..| Low          | n/a       | .
            PD9      | USART3_RX     | n/a          | n/a       | n/a          | Alternate Function Push Pull  | No pull-up and no..| Low          | n/a       | .
       \n

  3. Under **Categories**: **System Core** select **GPDMA1**:

     __Mode__:
       - Channel 0 - 2 Words Internal FIFO: **Standard Request Mode**
       - Channel 1 - 2 Words Internal FIFO: **Standard Request Mode**

     __Configuration__:
       - All Channels: not used
       - Security:
         - CH1:
           - Enable Channel as Privileged: NON PRIVILEGED
         - CH0:
           - Enable Channel as Privileged: NON PRIVILEGED
       - CH0:
         - Circular configuration:
           - Circular Mode: Disable
         - Request Configuration:
           - Request: **USART3_TX**
           - DMA Handle in IP Structure: hdmatx
           - Block HW request protocol: Single/Burst Level
         - Channel configuration:
           - Priority: Low
           - Transaction Mode: Normal
           - Direction: **Memory to Peripheral**
         - Source Data Setting:
           - Source Address Increment After Transfer: **Enabled**
           - Data Width: Byte
           - Burst Length: 1
           - Allocated Port for Transfer: Port 0
         - Destination Data Setting:
           - Destination Address Increment After Transfer: **Disabled**
           - Data Width: Byte
           - Burst Length: 1
           - Allocated Port for Transfer: Port 0
         - Data Handling:
           - Data Handling Configuration: Disable
         - Trigger:
           - Trigger Configuration: Disable
         - Transfer Event Configuration:
           - Transfer Event Generation: The TC (and the HT) ...
       - CH1:
         - Circular configuration:
           - Circular Mode: Disable
         - Request Configuration:
           - Request: **USART3_RX**
           - DMA Handle in IP Structure: hdmarx
           - Block HW request protocol: Single/Burst Level
         - Channel configuration:
           - Priority: Low
           - Transaction Mode: Normal
           - Direction: **Peripheral to Memory**
         - Source Data Setting:
           - Source Address Increment After Transfer: **Disabled**
           - Data Width: Byte
           - Burst Length: 1
           - Allocated Port for Transfer: **Port 1**
         - Destination Data Setting:
           - Destination Address Increment After Transfer: **Enabled**
           - Data Width: Byte
           - Burst Length: 1
           - Allocated Port for Transfer: **Port 1**
         - Data Handling:
           - Data Handling Configuration: Disable
         - Trigger:
           - Trigger Configuration: Disable
         - Transfer Event Configuration:
           - Transfer Event Generation: The TC (and the HT) ...
       - User Constants: not used
       \n\n

  4. Under **Categories**: **System Core** select **NVIC**:

     __Configuration__:
       - NVIC:
            NVIC Interrupt Table              | Enabled     | Preemption Priority | Sub Priority
            :---------------------------------|:-----------:|:-------------------:|:------------:
            GPDMA1 Channel 0 global interrupt | **checked** | 0                   | 0
            GPDMA1 Channel 1 global interrupt | **checked** | 0                   | 0
            USART3 global interrupt           | **checked** | 0                   | 0
       - Code generation:
            Enabled interrupt table           | Select for..| Generate Enable in..| Generate IRQ h.. | Call HAL handler
            :---------------------------------|:-----------:|:-------------------:|:----------------:|:----------------:
            GPDMA1 Channel 0 global interrupt | unchecked   | checked             | checked          | checked
            GPDMA1 Channel 1 global interrupt | unchecked   | checked             | checked          | checked
            USART3 global interrupt           | unchecked   | checked             | checked          | checked

### Clock Configuration tab

  1. Configure **To USART3 (MHz)**: **160**

### Project Manager tab

  1. Under **Advanced Settings**:

     __Generated Function Calls__:
        Generate Code | Function Name               | Peripheral Inst..| Do not generate ..| Visibility (Static)
        :-------------|:---------------------------:|:----------------:|:-----------------:|:-------------------:
        checked       | MX_USART3_UART_Init         | USART3           | **checked**       | checked

### Generate Code

Generate source code by clicking on the **GENERATE CODE** button on the toolbar.

# Validation

Results of the **CMSIS-Driver Validation** for this driver can be found in the [USART_TestReport.txt](../../../CMSIS/Driver/Validation/USART_TestReport.txt) file.
*/

/*! \cond */

#include "UART_STM32U5xx.h"
#ifdef USARTx_MODE_ASYNC

#define ARM_USART_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,2)

// Driver Version
static const ARM_DRIVER_VERSION usart_driver_version = { ARM_USART_API_VERSION, ARM_USART_DRV_VERSION };

// UART1 Configuration
#ifdef USART1_MODE_ASYNC
UARTx_RESOURCE_ALLOC(1);
#endif

// UART2 Configuration
#ifdef USART2_MODE_ASYNC
UARTx_RESOURCE_ALLOC(2);
#endif

// UART3 Configuration
#ifdef USART3_MODE_ASYNC
UARTx_RESOURCE_ALLOC(3);
#endif

// UART4 Configuration
#ifdef USART4_MODE_ASYNC
UARTx_RESOURCE_ALLOC(4);
#endif

// UART5 Configuration
#ifdef USART5_MODE_ASYNC
UARTx_RESOURCE_ALLOC(5);
#endif

// UART6 Configuration
#ifdef USART6_MODE_ASYNC
UARTx_RESOURCE_ALLOC(6);
#endif

/**
  \fn          const UART_RESOURCES UART_Resources (UART_HandleTypeDef *huart)
  \brief       Get UART_RESOURCES structure from UART_HandleTypeDef
*/
static const UART_RESOURCES * UART_Resources (UART_HandleTypeDef *huart) {
  const UART_RESOURCES *uart = NULL;

#ifdef USART1_MODE_ASYNC
  if (huart->Instance == USART1)  { uart = &UART1_Resources; }
#endif
#ifdef USART2_MODE_ASYNC
  if (huart->Instance == USART2)  { uart = &UART2_Resources; }
#endif
#ifdef USART3_MODE_ASYNC
  if (huart->Instance == USART3)  { uart = &UART3_Resources; }
#endif
#ifdef USART4_MODE_ASYNC
  if (huart->Instance == UART4)   { uart = &UART4_Resources; }
#endif
#ifdef USART5_MODE_ASYNC
  if (huart->Instance == UART5)   { uart = &UART5_Resources; }
#endif
#ifdef USART6_MODE_ASYNC
  if (huart->Instance == LPUART1) { uart = &UART6_Resources; }
#endif

  return uart;
}

/**
  \fn          void UART_PeripheralReset (USART_TypeDef *usart)
  \brief       UART Reset
*/
static void UART_PeripheralReset (USART_TypeDef *usart) {

#ifdef USART1_MODE_ASYNC
  if (usart == USART1)  { __HAL_RCC_USART1_FORCE_RESET(); }
#endif
#ifdef USART2_MODE_ASYNC
  if (usart == USART2)  { __HAL_RCC_USART2_FORCE_RESET(); }
#endif
#ifdef USART3_MODE_ASYNC
  if (usart == USART3)  { __HAL_RCC_USART3_FORCE_RESET(); }
#endif
#ifdef USART4_MODE_ASYNC
  if (usart == UART4)   { __HAL_RCC_UART4_FORCE_RESET(); }
#endif
#ifdef USART5_MODE_ASYNC
  if (usart == UART5)   { __HAL_RCC_UART5_FORCE_RESET(); }
#endif
#ifdef USART6_MODE_ASYNC
  if (usart == LPUART1) { __HAL_RCC_LPUART1_FORCE_RESET(); }
#endif

      __NOP(); __NOP(); __NOP(); __NOP();

#ifdef USART1_MODE_ASYNC
  if (usart == USART1)  { __HAL_RCC_USART1_RELEASE_RESET(); }
#endif
#ifdef USART2_MODE_ASYNC
  if (usart == USART2)  { __HAL_RCC_USART2_RELEASE_RESET(); }
#endif
#ifdef USART3_MODE_ASYNC
  if (usart == USART3)  { __HAL_RCC_USART3_RELEASE_RESET(); }
#endif
#ifdef USART4_MODE_ASYNC
  if (usart == UART4)   { __HAL_RCC_UART4_RELEASE_RESET(); }
#endif
#ifdef USART5_MODE_ASYNC
  if (usart == UART5)   { __HAL_RCC_UART5_RELEASE_RESET(); }
#endif
#ifdef USART6_MODE_ASYNC
  if (usart == LPUART1) { __HAL_RCC_LPUART1_RELEASE_RESET(); }
#endif
}

// UART Driver functions

/**
  \fn          ARM_DRIVER_VERSION UART_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRIVER_VERSION
*/
static ARM_DRIVER_VERSION UART_GetVersion (void) {
  return usart_driver_version;
}

/**
  \fn          ARM_USART_CAPABILITIES UART_GetCapabilities (const UART_RESOURCES *uart)
  \brief       Get driver capabilities
  \param[in]   uart     Pointer to UART resources
  \return      \ref ARM_USART_CAPABILITIES
*/
static ARM_USART_CAPABILITIES UART_GetCapabilities (const UART_RESOURCES *uart) {
  return uart->capabilities;
}

/**
  \fn          int32_t UART_Initialize (      ARM_USART_SignalEvent_t  cb_event
                                         const UART_RESOURCES          *uart)
  \brief       Initialize USART Interface.
  \param[in]   cb_event  Pointer to \ref ARM_USART_SignalEvent
  \param[in]   uart      Pointer to UART resources
  \return      \ref execution_status
*/
static int32_t UART_Initialize (      ARM_USART_SignalEvent_t  cb_event,
                                 const UART_RESOURCES          *uart) {

  if (uart->info->flags & UART_FLAG_INITIALIZED) {
    // Driver is already initialized
    return ARM_DRIVER_OK;
  }

  uart->h->Init.Mode = 0;

  // Initialize UART Run-time Resources
  uart->info->cb_event = cb_event;

  // Clear Status flags
  uart->info->status.tx_busy          = 0;
  uart->info->status.rx_busy          = 0;
  uart->info->status.tx_underflow     = 0;
  uart->info->status.rx_overflow      = 0;
  uart->info->status.rx_break         = 0;
  uart->info->status.rx_framing_error = 0;
  uart->info->status.rx_parity_error  = 0;

  // Clear transfer information
  memset(uart->xfer, 0, sizeof(UART_TRANSFER_INFO));

  uart->h->Instance = (USART_TypeDef *)uart->reg;

  uart->info->flags = UART_FLAG_INITIALIZED;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t UART_Uninitialize (const UART_RESOURCES *uart)
  \brief       De-initialize UART Interface.
  \param[in]   usart     Pointer to UART resources
  \return      \ref execution_status
*/
static int32_t UART_Uninitialize (const UART_RESOURCES *uart) {

  // Reset UART status flags
  uart->info->flags = 0U;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t UART_PowerControl (ARM_POWER_STATE state)
  \brief       Control UART Interface Power.
  \param[in]   state  Power state
  \param[in]   usart  Pointer to UART resources
  \return      \ref execution_status
*/
static int32_t UART_PowerControl (      ARM_POWER_STATE  state,
                                   const UART_RESOURCES  *uart) {

  if ((uart->info->flags & UART_FLAG_INITIALIZED) == 0U) {
    return ARM_DRIVER_ERROR;
  }

  if ((state != ARM_POWER_OFF)  &&
      (state != ARM_POWER_FULL) &&
      (state != ARM_POWER_LOW)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  switch (state) {
    case ARM_POWER_OFF:

      // UART peripheral reset
      UART_PeripheralReset (uart->reg);

      if (uart->h->Instance != NULL) {
        HAL_UART_MspDeInit (uart->h);
      }

      // Clear Status flags
      uart->info->status.tx_busy          = 0;
      uart->info->status.rx_busy          = 0;
      uart->info->status.tx_underflow     = 0;
      uart->info->status.rx_overflow      = 0;
      uart->info->status.rx_break         = 0;
      uart->info->status.rx_framing_error = 0;
      uart->info->status.rx_parity_error  = 0;

      uart->info->flags &= ~UART_FLAG_POWERED;
      break;
    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;

    case ARM_POWER_FULL:
      if ((uart->info->flags & UART_FLAG_INITIALIZED) == 0U) {
        return ARM_DRIVER_ERROR;
      }
      if ((uart->info->flags & UART_FLAG_POWERED)     != 0U) {
        return ARM_DRIVER_OK;
      }

      uart->xfer->def_val = 0U;

      // Clear Status flags
      uart->info->status.tx_busy          = 0;
      uart->info->status.rx_busy          = 0;
      uart->info->status.tx_underflow     = 0;
      uart->info->status.rx_overflow      = 0;
      uart->info->status.rx_break         = 0;
      uart->info->status.rx_framing_error = 0;
      uart->info->status.rx_parity_error  = 0;

      uart->info->flags = UART_FLAG_POWERED | UART_FLAG_INITIALIZED;

      HAL_UART_MspInit (uart->h);

      // UART peripheral reset
      UART_PeripheralReset (uart->reg);

    break;
  }
  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t UART_Send (const void            *data,
                                        uint32_t         num,
                                  const UART_RESOURCES *uart)
  \brief       Start sending data to UART transmitter.
  \param[in]   data  Pointer to buffer with data to send to UART transmitter
  \param[in]   num   Number of data items to send
  \param[in]   usart Pointer to UART resources
  \return      \ref execution_status
*/
static int32_t UART_Send (const void            *data,
                                uint32_t         num,
                          const UART_RESOURCES  *uart) {

  HAL_StatusTypeDef stat;

  if ((data == NULL) || (num == 0U)) {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((uart->info->flags & UART_FLAG_CONFIGURED) == 0U) {
    // UART is not configured (mode not selected)
    return ARM_DRIVER_ERROR;
  }

  switch (HAL_UART_GetState (uart->h)) {
    case HAL_UART_STATE_RESET:
    case HAL_UART_STATE_ERROR:
      return ARM_DRIVER_ERROR;

    case HAL_UART_STATE_TIMEOUT:
      return ARM_DRIVER_ERROR_TIMEOUT;

    case HAL_UART_STATE_BUSY:
    case HAL_UART_STATE_BUSY_TX:
    case HAL_UART_STATE_BUSY_TX_RX:
      return ARM_DRIVER_ERROR_BUSY;

    case HAL_UART_STATE_BUSY_RX:
    case HAL_UART_STATE_READY:
      break;
  }

  // Clear ARM UART STATUS flags
  uart->info->status.tx_underflow = 0;

  // Save buffer info
  uart->xfer->tx_num = num;
  uart->xfer->tx_cnt = 0U;

  if (uart->dma_use_tx != 0U) {
    // DMA mode
    stat = HAL_UART_Transmit_DMA(uart->h, (uint8_t *)(uint32_t)data, (uint16_t)num);
  } else {
    // Interrupt mode
    stat = HAL_UART_Transmit_IT (uart->h, (uint8_t *)(uint32_t)data, (uint16_t)num);
  }

  switch (stat) {
    case HAL_ERROR:
    case HAL_TIMEOUT:
      return ARM_DRIVER_ERROR;

    case HAL_BUSY:
      return ARM_DRIVER_ERROR_BUSY;

    case HAL_OK:
      break;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t UART_Receive (      void            *data,
                                           uint32_t         num,
                                     const UART_RESOURCES  *uart)
  \brief       Start receiving data from UART receiver.
  \param[out]  data  Pointer to buffer for data to receive from UART receiver
  \param[in]   num   Number of data items to receive
  \param[in]   usart Pointer to UART resources
  \return      \ref execution_status
*/
static int32_t UART_Receive (      void           *data,
                                   uint32_t        num,
                             const UART_RESOURCES *uart) {

  HAL_StatusTypeDef stat;

  if ((data == NULL) || (num == 0U)) {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((uart->info->flags & UART_FLAG_CONFIGURED) == 0U) {
    // UART is not configured (mode not selected)
    return ARM_DRIVER_ERROR;
  }

  switch (HAL_UART_GetState (uart->h)) {
    case HAL_UART_STATE_RESET:
    case HAL_UART_STATE_ERROR:
      return ARM_DRIVER_ERROR;

    case HAL_UART_STATE_TIMEOUT:
      return ARM_DRIVER_ERROR_TIMEOUT;

    case HAL_UART_STATE_BUSY:
    case HAL_UART_STATE_BUSY_RX:
    case HAL_UART_STATE_BUSY_TX_RX:
      return ARM_DRIVER_ERROR_BUSY;

    case HAL_UART_STATE_BUSY_TX:
    case HAL_UART_STATE_READY:
      break;
  }

  // Clear ARM UART STATUS flags
  uart->info->status.rx_overflow = 0;
  uart->info->status.rx_break = 0;
  uart->info->status.rx_framing_error = 0;
  uart->info->status.rx_parity_error = 0;

  // Save buffer info
  uart->xfer->rx_num = num;
  uart->xfer->rx_cnt = 0U;

  if (uart->dma_use_rx != 0U) {
    // DMA mode
    stat = HAL_UART_Receive_DMA(uart->h, (uint8_t *)(uint32_t)data, (uint16_t)num);
  } else {
    // Interrupt mode
    stat = HAL_UART_Receive_IT (uart->h, (uint8_t *)(uint32_t)data, (uint16_t)num);
  }

  switch (stat) {
    case HAL_ERROR:
    case HAL_TIMEOUT:
      return ARM_DRIVER_ERROR;

    case HAL_BUSY:
      return ARM_DRIVER_ERROR_BUSY;

    case HAL_OK:
      break;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t UART_Transfer (const void            *data_out,
                                            void            *data_in,
                                            uint32_t         num,
                                      const UART_RESOURCES  *uart)
  \brief       Start sending/receiving data to/from UART transmitter/receiver.
  \param[in]   data_out  Pointer to buffer with data to send to UART transmitter
  \param[out]  data_in   Pointer to buffer for data to receive from UART receiver
  \param[in]   num       Number of data items to transfer
  \param[in]   uart      Pointer to UART resources
  \return      \ref execution_status
*/
static int32_t UART_Transfer (const void             *data_out,
                                    void             *data_in,
                                    uint32_t          num,
                              const UART_RESOURCES   *uart) {

  UNUSED(data_out);
  UNUSED(data_in);
  UNUSED(num);
  UNUSED(uart);

  // Supported only in Synchronous mode
  return ARM_DRIVER_ERROR;
}

/**
  \fn          uint32_t UART_GetTxCount (const UART_RESOURCES *uart)
  \brief       Get transmitted data count.
  \param[in]   uart     Pointer to UART resources
  \return      number of data items transmitted
*/
static uint32_t UART_GetTxCount (const UART_RESOURCES *uart) {
  uint32_t cnt;

  if ((uart->info->flags & UART_FLAG_POWERED) == 0U) {
    return 0U;
  }

  if (uart->dma_use_tx != 0U) {
    cnt = uart->xfer->tx_num - __HAL_DMA_GET_COUNTER(uart->h->hdmatx);
  } else {
    cnt = uart->h->TxXferSize - uart->h->TxXferCount;
  }

  return cnt;
}

/**
  \fn          uint32_t UART_GetRxCount (const UART_RESOURCES *uart)
  \brief       Get received data count.
  \param[in]   uart     Pointer to UART resources
  \return      number of data items received
*/
static uint32_t UART_GetRxCount (const UART_RESOURCES *uart) {
  uint32_t cnt;

  if ((uart->info->flags & UART_FLAG_POWERED) == 0U) {
    return 0U;
  }

  if (uart->dma_use_rx != 0U) {
    cnt = uart->xfer->rx_num - __HAL_DMA_GET_COUNTER(uart->h->hdmarx);
  } else {
    cnt = uart->h->RxXferSize - uart->h->RxXferCount;
  }

  return cnt;
}

/**
  \fn          int32_t UART_Control (      uint32_t          control,
                                           uint32_t          arg,
                                      const UART_RESOURCES  *uart)
  \brief       Control UART Interface.
  \param[in]   control  Operation
  \param[in]   arg      Argument of operation (optional)
  \param[in]   uart    Pointer to UART resources
  \return      common \ref execution_status and driver specific \ref uart_execution_status
*/
static int32_t UART_Control (      uint32_t         control,
                                   uint32_t         arg,
                             const UART_RESOURCES  *uart) {

  HAL_StatusTypeDef status;

  if ((uart->info->flags & UART_FLAG_POWERED) == 0U) {
    // UART not powered
    return ARM_DRIVER_ERROR;
  }

  switch (control & ARM_USART_CONTROL_Msk) {
     // Control break
    case ARM_USART_CONTROL_BREAK:
      return ARM_DRIVER_ERROR_UNSUPPORTED;

    // Abort
    case ARM_USART_ABORT_SEND:
      HAL_UART_AbortTransmit(uart->h);
      uart->h->TxXferSize = 0U;
      return ARM_DRIVER_OK;
    case ARM_USART_ABORT_RECEIVE:
      HAL_UART_AbortReceive(uart->h);
      uart->h->RxXferSize = 0U;
      return ARM_DRIVER_OK;
    case ARM_USART_ABORT_TRANSFER:
      HAL_UART_Abort(uart->h);
      uart->h->RxXferSize = 0U;
      uart->h->TxXferSize = 0U;
      return ARM_DRIVER_OK;

    // Control TX
    case ARM_USART_CONTROL_TX:
      if (arg) {
        // Transmitter enable
        uart->h->Init.Mode |=  UART_MODE_TX;
      } else {
        // Transmitter disable
        uart->h->Init.Mode &= ~UART_MODE_TX;
      }
      status = HAL_UART_Init(uart->h);
      return UART_HAL_STATUS(status);

    // Control RX
    case ARM_USART_CONTROL_RX:
      if (arg) {
        // Receiver enable
        uart->h->Init.Mode |= UART_MODE_RX;
      } else {
        // Receiver disable
        uart->h->Init.Mode &= ~UART_MODE_RX;
      }
      status = HAL_UART_Init(uart->h);
      return UART_HAL_STATUS(status);
    default: break;
  }

  switch (control & ARM_USART_CONTROL_Msk) {
    case ARM_USART_MODE_IRDA:
    case ARM_USART_MODE_SMART_CARD:
    case ARM_USART_MODE_SYNCHRONOUS_MASTER:
    case ARM_USART_MODE_SYNCHRONOUS_SLAVE:
    case ARM_USART_MODE_SINGLE_WIRE:
      return ARM_USART_ERROR_MODE;

    case ARM_USART_MODE_ASYNCHRONOUS:
      break;

    // Default TX value
    case ARM_USART_SET_DEFAULT_TX_VALUE:
      uart->xfer->def_val = (uint16_t)arg;
      return ARM_DRIVER_OK;

    default: { return ARM_DRIVER_ERROR_UNSUPPORTED; }
  }

  // USART Data bits
  switch (control & ARM_USART_DATA_BITS_Msk) {
    case ARM_USART_DATA_BITS_6:
      if ((control & ARM_USART_PARITY_Msk) != ARM_USART_PARITY_NONE) {
        uart->h->Init.WordLength = UART_WORDLENGTH_7B;
      } else {
        return ARM_USART_ERROR_DATA_BITS;
      }
      break;
    case ARM_USART_DATA_BITS_7:
      if ((control & ARM_USART_PARITY_Msk) == ARM_USART_PARITY_NONE) {
        uart->h->Init.WordLength = UART_WORDLENGTH_7B;
      } else {
        uart->h->Init.WordLength = UART_WORDLENGTH_8B;
      }
      break;
    case ARM_USART_DATA_BITS_8:
      if ((control & ARM_USART_PARITY_Msk) == ARM_USART_PARITY_NONE) {
        uart->h->Init.WordLength = UART_WORDLENGTH_8B;
      } else {
        uart->h->Init.WordLength = UART_WORDLENGTH_9B;
      }
      break;
    case ARM_USART_DATA_BITS_9:
      if ((control & ARM_USART_PARITY_Msk) == ARM_USART_PARITY_NONE) {
        uart->h->Init.WordLength = UART_WORDLENGTH_9B;
      } else {
        return ARM_USART_ERROR_DATA_BITS;
      }
      break;
    default: return ARM_USART_ERROR_DATA_BITS;
  }

  // USART Parity
  switch (control & ARM_USART_PARITY_Msk) {
    case ARM_USART_PARITY_NONE:
      uart->h->Init.Parity = UART_PARITY_NONE;
      break;
    case ARM_USART_PARITY_EVEN:
      uart->h->Init.Parity = UART_PARITY_EVEN;
      break;
    case ARM_USART_PARITY_ODD:
      uart->h->Init.Parity = UART_PARITY_ODD;
      break;
    default: return ARM_USART_ERROR_PARITY;
  }

  // USART Stop bits
  switch (control & ARM_USART_STOP_BITS_Msk) {
    case ARM_USART_STOP_BITS_1:
      uart->h->Init.StopBits = UART_STOPBITS_1;
      break;
    case ARM_USART_STOP_BITS_2:
      uart->h->Init.StopBits = UART_STOPBITS_2;
      break;
    case ARM_USART_STOP_BITS_1_5:
    case ARM_USART_STOP_BITS_0_5:
    default: return ARM_USART_ERROR_STOP_BITS;
  }

  // USART Flow control
  switch (control & ARM_USART_FLOW_CONTROL_Msk) {
    case ARM_USART_FLOW_CONTROL_NONE:
      uart->h->Init.HwFlowCtl = UART_HWCONTROL_NONE;
      break;
    case ARM_USART_FLOW_CONTROL_RTS:
      uart->h->Init.HwFlowCtl = UART_HWCONTROL_RTS;
      break;
    case ARM_USART_FLOW_CONTROL_CTS:
      uart->h->Init.HwFlowCtl = UART_HWCONTROL_CTS;
      break;
    case ARM_USART_FLOW_CONTROL_RTS_CTS:
      uart->h->Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
      break;
    default: return ARM_USART_ERROR_FLOW_CONTROL;
  }

  // USART Baudrate
  uart->h->Init.BaudRate = arg;

  // Set configured flag
  uart->info->flags |= UART_FLAG_CONFIGURED;

  // Initialize UART
  status = HAL_UART_Init(uart->h);

  // Reconfigure DMA
  if ((uart->dma_use_tx != 0U) && (uart->h->hdmatx !=NULL)) {
    if ((control & ARM_USART_DATA_BITS_Msk) > ARM_USART_DATA_BITS_8) {
      uart->h->hdmatx->Init.SrcDataWidth  = DMA_SRC_DATAWIDTH_HALFWORD;
      uart->h->hdmatx->Init.DestDataWidth = DMA_DEST_DATAWIDTH_HALFWORD;
    } else {
      uart->h->hdmatx->Init.SrcDataWidth  = DMA_SRC_DATAWIDTH_BYTE;
      uart->h->hdmatx->Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
    }
    HAL_DMA_Init(uart->h->hdmatx);
  }

  if ((uart->dma_use_rx != 0U) && (uart->h->hdmarx !=NULL)) {
    if ((control & ARM_USART_DATA_BITS_Msk) > ARM_USART_DATA_BITS_8) {
      uart->h->hdmarx->Init.SrcDataWidth  = DMA_SRC_DATAWIDTH_HALFWORD;
      uart->h->hdmarx->Init.DestDataWidth = DMA_DEST_DATAWIDTH_HALFWORD;
    } else {
      uart->h->hdmarx->Init.SrcDataWidth  = DMA_SRC_DATAWIDTH_BYTE;
      uart->h->hdmarx->Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
    }
    HAL_DMA_Init(uart->h->hdmarx);
  }

  return UART_HAL_STATUS(status);
}

/**
  \fn          ARM_USART_STATUS UART_GetStatus (const UART_RESOURCES *uart)
  \brief       Get UART status.
  \param[in]   uart     Pointer to UART resources
  \return      UART status \ref ARM_UART_STATUS
*/
static ARM_USART_STATUS UART_GetStatus (const UART_RESOURCES *uart) {
  ARM_USART_STATUS  status = {0U};

  switch (HAL_UART_GetState (uart->h)) {
    case HAL_UART_STATE_BUSY:
    case HAL_UART_STATE_BUSY_TX_RX:
      status.rx_busy = 1;
      status.tx_busy = 1;
      break;
    case HAL_UART_STATE_BUSY_TX:
      status.tx_busy = 1;
      break;
    case HAL_UART_STATE_BUSY_RX:
      status.rx_busy = 1;
      break;

    case HAL_UART_STATE_TIMEOUT:
    case HAL_UART_STATE_READY:
    case HAL_UART_STATE_RESET:
    case HAL_UART_STATE_ERROR:
      break;
  }

  status.tx_underflow     = uart->info->status.tx_underflow;
  status.rx_overflow      = uart->info->status.rx_overflow;
  status.rx_break         = uart->info->status.rx_break;
  status.rx_framing_error = uart->info->status.rx_framing_error;
  status.rx_parity_error  = uart->info->status.rx_parity_error;

  return status;
}

/**
  \fn          int32_t UART_SetModemControl (ARM_USART_MODEM_CONTROL control)
  \brief       Set USART Modem Control line state.
  \param[in]   control   \ref ARM_USART_MODEM_CONTROL
  \return      \ref execution_status
*/
static int32_t UART_SetModemControl (ARM_USART_MODEM_CONTROL control) {

  (void) control;

  // No manual control of modem control lines
  return ARM_DRIVER_ERROR_UNSUPPORTED;
}

/**
  \fn          ARM_USART_MODEM_STATUS UART_GetModemStatus (void)
  \brief       Get UART Modem Status lines state.
  \return      modem status \ref ARM_UART_MODEM_STATUS
*/
static ARM_USART_MODEM_STATUS UART_GetModemStatus (void) {
  ARM_USART_MODEM_STATUS modem_status;

  modem_status.cts = 0U;
  modem_status.dsr = 0U;
  modem_status.ri  = 0U;
  modem_status.dcd = 0U;

  return modem_status;
}

/**
  * @brief Tx Transfer completed callback.
  * @param husart: UART handle.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
  const UART_RESOURCES * uart;

  uart = UART_Resources (huart);
  uart->xfer->tx_cnt = uart->xfer->tx_num;

  if (uart->info->cb_event != NULL) {
    uart->info->cb_event(ARM_USART_EVENT_TX_COMPLETE | ARM_USART_EVENT_SEND_COMPLETE);
  }
}

/**
  * @brief  Rx Transfer completed callback.
  * @param husart: UART handle.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  const UART_RESOURCES * uart;

  uart = UART_Resources (huart);
  uart->xfer->rx_cnt = uart->xfer->rx_num;

  if (uart->info->cb_event != NULL) {
    uart->info->cb_event(ARM_USART_EVENT_RECEIVE_COMPLETE);
  }
}

/**
  * @brief UART error callback.
  * @param huart: UART handle.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
  const UART_RESOURCES * uart;
        uint32_t         error;
        uint32_t         event;

  uart = UART_Resources (huart);

  error = HAL_UART_GetError (uart->h);
  event = 0;

  if (error & HAL_UART_ERROR_PE) {
    event |= ARM_USART_EVENT_RX_PARITY_ERROR;
    uart->info->status.rx_parity_error = 1;
  }
  if (error & HAL_UART_ERROR_FE) {
    event |= ARM_USART_EVENT_RX_FRAMING_ERROR;
    uart->info->status.rx_framing_error = 1;
  }
  if (error & HAL_UART_ERROR_ORE) {
    event |= ARM_USART_EVENT_RX_OVERFLOW;
    uart->info->status.rx_overflow = 1;
  }

  if ((event != 0) && (uart->info->cb_event != NULL)) {
    uart->info->cb_event(event);
  }
}

#ifdef USART1_MODE_ASYNC
UARTx_EXPORT_DRIVER(1);
#endif

#ifdef USART2_MODE_ASYNC
UARTx_EXPORT_DRIVER(2);
#endif

#ifdef USART3_MODE_ASYNC
UARTx_EXPORT_DRIVER(3);
#endif

#ifdef USART4_MODE_ASYNC
UARTx_EXPORT_DRIVER(4);
#endif

#ifdef USART5_MODE_ASYNC
UARTx_EXPORT_DRIVER(5);
#endif

#ifdef USART6_MODE_ASYNC
UARTx_EXPORT_DRIVER(6);
#endif

#endif

/*! \endcond */
