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
 * Driver:       Driver_USART1/2/3
 *
 * Configured:   via CubeMX
 * Project:      USART Driver for STMicroelectronics STM32U5xx
 * -------------------------------------------------------------------------- */

/*! \page stm32u5_usart USART (Synchronous mode)

# Revision History

- Version 1.2
  - Made variable status volatile (solved potential LTO problems)
- Version 1.1
  - Corrected GetStatus function and status flags handling
  - Corrected Control function to support 9 data bits in DMA mode
  - Corrected GetTxCount and GetRxCount functions in DMA mode
- Version 1.0
  - Initial release

# Capabilities

This driver supports **Synchronous Master** mode.

# Instances

Hardware resource relating to driver instance is shown in the table below:

Driver Instance | Hardware Resource
:---------------|:-----------------:
Driver_USART1   | USART1
Driver_USART2   | USART2
Driver_USART3   | USART3

\note
The **CMSIS Driver:USART (API):USART** component in the **Manage Run-Time Environment** dialog adds multiple C source files to the project.\n
The interface selection in STM32CubeMX selects the actual implementation that is compiled: IrDA, SmartCard, UART or USART.

# Limitations

STM32 HAL limitations:
 - Rx Overflow event can be detected only when send/receive/transfer operation is active

# Configuration

## STM32CubeMX

The USART driver requires:
  - **USART peripheral clock**
  - **USART** peripheral configured as **Synchronous Master**
  - **USART TX, Rx** and **CK pins**
  - optional **DMA** configuration for transfers

# B-U585I-IOT02A Board Configuration

These settings are relevant for this board, for different board please refer to the board
schematic for proper configuration.

Link to [B-U585I-IOT02A board schematic](https://www.st.com/resource/en/schematic_pack/mb1551-u585i-c02_schematic.pdf).

## STM32CubeMX

Required peripherals for the **B-U585I-IOT02A** board are listed in the table below:

Peripheral | Mode                         | Description
:---------:|:----------------------------:|:-----------
USART3     | Synchronous Master           | USART3 Controller
PD8        | Alternate Function Push Pull | USART3 TX Pin
PD9        | Alternate Function Push Pull | USART3 RX Pin
PB0        | Alternate Function Push Pull | USART3 CK Pin

\note All settings have to be configured as described in the procedure below. Important settings,
      typically the ones different from default, are emphasized in **bold**.

### Pinout & Configuration tab

  1. In the **Pinout view** window click on a pin and select it's functionality:
        Pin      | Functionality
        :--------|:-------------:
        PD8      | **USART3_TX**
        PD9      | **USART3_RX**
        PB0      | **USART3_CK**
       \n

  2. Under **Categories**: **Connectivity** select **USART3**:

     __Mode__:
       - Mode: **Synchronous Master**
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
            PB0      | USART3_CK     | n/a          | n/a       | n/a          | Alternate Function Push Pull  | No pull-up and no..| Low          | n/a       | .
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
         checked       | MX_USART3_Init              | USART3           | **checked**       | checked

### Generate Code

Generate source code by clicking on the **GENERATE CODE** button on the toolbar.
*/

/*! \cond */

#include "USART_STM32U5xx.h"
#ifdef USARTx_MODE_SYNC

#define ARM_USART_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,2)

// Driver Version
static const ARM_DRIVER_VERSION usart_driver_version = { ARM_USART_API_VERSION, ARM_USART_DRV_VERSION };

static const ARM_USART_CAPABILITIES Capabilities = {
    0,      ///< supports UART (Asynchronous) mode
    1,      ///< supports Synchronous Master mode
    0,      ///< supports Synchronous Slave mode
    0,      ///< supports UART Single-wire mode
    0,      ///< supports UART IrDA mode
    0,      ///< supports UART Smart Card mode
    0,      ///< Smart Card Clock generator available
    0,      ///< RTS Flow Control available
    0,      ///< CTS Flow Control available
    1,      ///< Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
    0,      ///< Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT
    0,      ///< RTS Line: 0=not available, 1=available
    0,      ///< CTS Line: 0=not available, 1=available
    0,      ///< DTR Line: 0=not available, 1=available
    0,      ///< DSR Line: 0=not available, 1=available
    0,      ///< DCD Line: 0=not available, 1=available
    0,      ///< RI Line: 0=not available, 1=available
    0,      ///< Signal CTS change event: \ref ARM_USART_EVENT_CTS
    0,      ///< Signal DSR change event: \ref ARM_USART_EVENT_DSR
    0,      ///< Signal DCD change event: \ref ARM_USART_EVENT_DCD
    0       ///< Signal RI change event: \ref ARM_USART_EVENT_RI
#if (defined(ARM_USART_API_VERSION) && (ARM_USART_API_VERSION >= 0x203U))
  , 0U  // Reserved bits
#endif
  };

// USART1
#ifdef USART1_MODE_SYNC
USARTx_RESOURCE_ALLOC(1);
#endif

// USART2
#ifdef USART2_MODE_SYNC
USARTx_RESOURCE_ALLOC(2);
#endif

// USART3
#ifdef USART3_MODE_SYNC
USARTx_RESOURCE_ALLOC(3);
#endif

/**
  \fn          const USART_RESOURCES USART_Resources (USART_HandleTypeDef *husart)
  \brief       Get USART_RESOURCES structure from USART_HandleTypeDef
*/
static const USART_RESOURCES * USART_Resources (USART_HandleTypeDef *husart) {
  const USART_RESOURCES *usart = NULL;

#ifdef USART1_MODE_SYNC
  if (husart->Instance == USART1)  { usart = &USART1_Resources; }
#endif
#ifdef USART2_MODE_SYNC
  if (husart->Instance == USART2)  { usart = &USART2_Resources; }
#endif
#ifdef USART3_MODE_SYNC
  if (husart->Instance == USART3)  { usart = &USART3_Resources; }
#endif

  return usart;
}

/**
  \fn          void USART_PeripheralReset (USART_TypeDef *usart)
  \brief       USART Reset
*/
static void USART_PeripheralReset (USART_TypeDef *usart) {

#ifdef USART1_MODE_SYNC
  if (usart == USART1)  { __HAL_RCC_USART1_FORCE_RESET(); }
#endif
#ifdef USART2_MODE_SYNC
  if (usart == USART2)  { __HAL_RCC_USART2_FORCE_RESET(); }
#endif
#ifdef USART3_MODE_SYNC
  if (usart == USART3)  { __HAL_RCC_USART3_FORCE_RESET(); }
#endif

      __NOP(); __NOP(); __NOP(); __NOP();

#ifdef USART1_MODE_SYNC
  if (usart == USART1)  { __HAL_RCC_USART1_RELEASE_RESET(); }
#endif
#ifdef USART2_MODE_SYNC
  if (usart == USART2)  { __HAL_RCC_USART2_RELEASE_RESET(); }
#endif
#ifdef USART3_MODE_SYNC
  if (usart == USART3)  { __HAL_RCC_USART3_RELEASE_RESET(); }
#endif
}

// USART Driver functions

/**
  \fn          ARM_DRIVER_VERSION USART_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRIVER_VERSION
*/
static ARM_DRIVER_VERSION USART_GetVersion (void) {
  return usart_driver_version;
}

/**
  \fn          ARM_USART_CAPABILITIES USART_GetCapabilities (void)
  \brief       Get driver capabilities
  \return      \ref ARM_USART_CAPABILITIES
*/
static ARM_USART_CAPABILITIES USART_GetCapabilities (void) {
  return Capabilities;
}

/**
  \fn          int32_t USART_Initialize (      ARM_USART_SignalEvent_t  cb_event
                                         const USART_RESOURCES         *usart)
  \brief       Initialize USART Interface.
  \param[in]   cb_event  Pointer to \ref ARM_USART_SignalEvent
  \param[in]   usart     Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Initialize (      ARM_USART_SignalEvent_t  cb_event,
                                 const USART_RESOURCES         *usart) {

  if (usart->info->flags & USART_FLAG_INITIALIZED) {
    // Driver is already initialized
    return ARM_DRIVER_OK;
  }

  usart->h->Init.Mode = 0;

  // Initialize USART Run-time Resources
  usart->info->cb_event = cb_event;

  // Clear Status flags
  usart->info->status.tx_busy          = 0;
  usart->info->status.rx_busy          = 0;
  usart->info->status.tx_underflow     = 0;
  usart->info->status.rx_overflow      = 0;
  usart->info->status.rx_break         = 0;
  usart->info->status.rx_framing_error = 0;
  usart->info->status.rx_parity_error  = 0;

  // Clear transfer information
  memset(usart->xfer, 0, sizeof(USART_TRANSFER_INFO));

  usart->h->Instance = usart->reg;

  usart->info->flags = USART_FLAG_INITIALIZED;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_Uninitialize (const USART_RESOURCES *usart)
  \brief       De-initialize USART Interface.
  \param[in]   usart     Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Uninitialize (const USART_RESOURCES *usart) {

  // Reset USART status flags
  usart->info->flags = 0U;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_PowerControl (ARM_POWER_STATE state)
  \brief       Control USART Interface Power.
  \param[in]   state  Power state
  \param[in]   usart  Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_PowerControl (      ARM_POWER_STATE  state,
                                   const USART_RESOURCES *usart) {

  if ((usart->info->flags & USART_FLAG_INITIALIZED) == 0U) {
    return ARM_DRIVER_ERROR;
  }

  if ((state != ARM_POWER_OFF)  &&
      (state != ARM_POWER_FULL) &&
      (state != ARM_POWER_LOW)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  switch (state) {
    case ARM_POWER_OFF:

      // USART peripheral reset
      USART_PeripheralReset (usart->reg);

      if (usart->h->Instance != NULL) {
        HAL_USART_MspDeInit (usart->h);
      }

      // Clear Status flags
      usart->info->status.tx_busy          = 0;
      usart->info->status.rx_busy          = 0;
      usart->info->status.tx_underflow     = 0;
      usart->info->status.rx_overflow      = 0;
      usart->info->status.rx_break         = 0;
      usart->info->status.rx_framing_error = 0;
      usart->info->status.rx_parity_error  = 0;

      usart->info->flags &= ~USART_FLAG_POWERED;
      break;
    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;

    case ARM_POWER_FULL:
      if ((usart->info->flags & USART_FLAG_INITIALIZED) == 0U) {
        return ARM_DRIVER_ERROR;
      }
      if ((usart->info->flags & USART_FLAG_POWERED)     != 0U) {
        return ARM_DRIVER_OK;
      }

      usart->xfer->def_val = 0U;

      // Clear Status flags
      usart->info->status.tx_busy          = 0;
      usart->info->status.rx_busy          = 0;
      usart->info->status.tx_underflow     = 0;
      usart->info->status.rx_overflow      = 0;
      usart->info->status.rx_break         = 0;
      usart->info->status.rx_framing_error = 0;
      usart->info->status.rx_parity_error  = 0;

      usart->info->flags = USART_FLAG_POWERED | USART_FLAG_INITIALIZED;

      HAL_USART_MspInit (usart->h);

      // USART peripheral reset
      USART_PeripheralReset (usart->reg);

    break;
  }
  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_Send (const void            *data,
                                         uint32_t         num,
                                   const USART_RESOURCES *usart)
  \brief       Start sending data to USART transmitter.
  \param[in]   data  Pointer to buffer with data to send to USART transmitter
  \param[in]   num   Number of data items to send
  \param[in]   usart Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Send (const void            *data,
                                 uint32_t         num,
                           const USART_RESOURCES *usart) {

  HAL_StatusTypeDef stat;

  if ((data == NULL) || (num == 0U)) {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((usart->info->flags & USART_FLAG_CONFIGURED) == 0U) {
    // USART is not configured (mode not selected)
    return ARM_DRIVER_ERROR;
  }

  switch (HAL_USART_GetState (usart->h)) {
    case HAL_USART_STATE_RESET:
    case HAL_USART_STATE_ERROR:
      return ARM_DRIVER_ERROR;

    case HAL_USART_STATE_TIMEOUT:
      return ARM_DRIVER_ERROR_TIMEOUT;

    case HAL_USART_STATE_BUSY:
    case HAL_USART_STATE_BUSY_TX:
    case HAL_USART_STATE_BUSY_RX:
    case HAL_USART_STATE_BUSY_TX_RX:
      return ARM_DRIVER_ERROR_BUSY;

    case HAL_USART_STATE_READY:
      break;
  }

  // Clear ARM UART STATUS flags
  usart->info->status.tx_underflow = 0;

  // Save buffer info
  usart->xfer->tx_num = num;
  usart->xfer->tx_cnt = 0U;

  if (usart->dma_use != 0U) {
    // DMA mode
    stat = HAL_USART_Transmit_DMA (usart->h, (uint8_t *)(uint32_t)data, (uint16_t)num);
  } else {
    // Interrupt mode
    stat = HAL_USART_Transmit_IT (usart->h, (uint8_t *)(uint32_t)data, (uint16_t)num);
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
  \fn          int32_t USART_Receive (      void            *data,
                                            uint32_t         num,
                                      const USART_RESOURCES *usart)
  \brief       Start receiving data from USART receiver.
  \param[out]  data  Pointer to buffer for data to receive from USART receiver
  \param[in]   num   Number of data items to receive
  \param[in]   usart Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Receive (      void            *data,
                                    uint32_t         num,
                              const USART_RESOURCES *usart) {

  HAL_StatusTypeDef stat;

  if ((data == NULL) || (num == 0U)) {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((usart->info->flags & USART_FLAG_CONFIGURED) == 0U) {
    // USART is not configured (mode not selected)
    return ARM_DRIVER_ERROR;
  }

  switch (HAL_USART_GetState (usart->h)) {
    case HAL_USART_STATE_RESET:
    case HAL_USART_STATE_ERROR:
      return ARM_DRIVER_ERROR;

    case HAL_USART_STATE_TIMEOUT:
      return ARM_DRIVER_ERROR_TIMEOUT;

    case HAL_USART_STATE_BUSY:
    case HAL_USART_STATE_BUSY_TX:
    case HAL_USART_STATE_BUSY_RX:
    case HAL_USART_STATE_BUSY_TX_RX:
      return ARM_DRIVER_ERROR_BUSY;

    case HAL_USART_STATE_READY:
      break;
  }

  // Clear ARM UART STATUS flags
  usart->info->status.rx_overflow = 0;
  usart->info->status.rx_break = 0;
  usart->info->status.rx_framing_error = 0;
  usart->info->status.rx_parity_error = 0;

  // Save buffer info
  usart->xfer->rx_num = num;
  usart->xfer->rx_cnt = 0U;

  if (usart->dma_use != 0U) {
    // DMA mode
    stat = HAL_USART_Receive_DMA(usart->h, (uint8_t *)(uint32_t)data, (uint16_t)num);
  } else {
    // Interrupt mode
    stat = HAL_USART_Receive_IT (usart->h, (uint8_t *)(uint32_t)data, (uint16_t)num);
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
  \fn          int32_t USART_Transfer (const void             *data_out,
                                             void             *data_in,
                                             uint32_t          num,
                                       const USART_RESOURCES  *usart)
  \brief       Start sending/receiving data to/from USART transmitter/receiver.
  \param[in]   data_out  Pointer to buffer with data to send to USART transmitter
  \param[out]  data_in   Pointer to buffer for data to receive from USART receiver
  \param[in]   num       Number of data items to transfer
  \param[in]   usart     Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Transfer (const void             *data_out,
                                     void             *data_in,
                                     uint32_t          num,
                               const USART_RESOURCES  *usart) {

  HAL_StatusTypeDef stat;

  if ((data_out == NULL) || (data_in == NULL) || (num == 0U)) {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((usart->info->flags & USART_FLAG_CONFIGURED) == 0U) {
    // USART is not configured (mode not selected)
    return ARM_DRIVER_ERROR;
  }

  switch (HAL_USART_GetState (usart->h)) {
    case HAL_USART_STATE_RESET:
    case HAL_USART_STATE_ERROR:
      return ARM_DRIVER_ERROR;

    case HAL_USART_STATE_TIMEOUT:
      return ARM_DRIVER_ERROR_TIMEOUT;

    case HAL_USART_STATE_BUSY:
    case HAL_USART_STATE_BUSY_TX:
    case HAL_USART_STATE_BUSY_RX:
    case HAL_USART_STATE_BUSY_TX_RX:
      return ARM_DRIVER_ERROR_BUSY;

    case HAL_USART_STATE_READY:
      break;
  }

  // Save buffer info
  usart->xfer->tx_num = num;
  usart->xfer->rx_num = num;
  usart->xfer->tx_cnt = 0U;
  usart->xfer->rx_cnt = 0U;

  if (usart->dma_use != 0U) {
    // DMA mode
    stat = HAL_USART_TransmitReceive_DMA(usart->h, (uint8_t *)(uint32_t)data_out, (uint8_t *)(uint32_t)data_in, (uint16_t)num);
  } else {
    // Interrupt mode
    stat = HAL_USART_TransmitReceive_IT (usart->h, (uint8_t *)(uint32_t)data_out, (uint8_t *)(uint32_t)data_in, (uint16_t)num);
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
  \fn          uint32_t USART_GetTxCount (const USART_RESOURCES *usart)
  \brief       Get transmitted data count.
  \param[in]   usart     Pointer to USART resources
  \return      number of data items transmitted
*/
static uint32_t USART_GetTxCount (const USART_RESOURCES *usart) {
  uint32_t cnt;

  if ((usart->info->flags & USART_FLAG_POWERED) == 0U) {
    return 0U;
  }

  if (usart->dma_use != 0U) {
    cnt = usart->xfer->tx_num - __HAL_DMA_GET_COUNTER(usart->h->hdmatx);
  } else {
    cnt = usart->h->TxXferSize - usart->h->TxXferCount;
  }

  return cnt;
}

/**
  \fn          uint32_t USART_GetRxCount (const USART_RESOURCES *usart)
  \brief       Get received data count.
  \param[in]   usart     Pointer to USART resources
  \return      number of data items received
*/
static uint32_t USART_GetRxCount (const USART_RESOURCES *usart) {
  uint32_t cnt;

  if ((usart->info->flags & USART_FLAG_POWERED) == 0U) {
    return 0U;
  }

  if (usart->dma_use != 0U) {
    cnt = usart->xfer->rx_num - __HAL_DMA_GET_COUNTER(usart->h->hdmarx);
  } else {
    cnt = usart->h->RxXferSize - usart->h->RxXferCount;
  }

  return cnt;
}

/**
  \fn          int32_t USART_Control (      uint32_t          control,
                                            uint32_t          arg,
                                      const USART_RESOURCES  *usart)
  \brief       Control USART Interface.
  \param[in]   control  Operation
  \param[in]   arg      Argument of operation (optional)
  \param[in]   usart    Pointer to USART resources
  \return      common \ref execution_status and driver specific \ref usart_execution_status
*/
static int32_t USART_Control (      uint32_t          control,
                                    uint32_t          arg,
                              const USART_RESOURCES  *usart) {

  HAL_StatusTypeDef status;

  if ((usart->info->flags & USART_FLAG_POWERED) == 0U) {
    // USART not powered
    return ARM_DRIVER_ERROR;
  }

  switch (control & ARM_USART_CONTROL_Msk) {
     // Control break
    case ARM_USART_CONTROL_BREAK:
      return ARM_DRIVER_ERROR_UNSUPPORTED;

    // Abort
    case ARM_USART_ABORT_SEND:
    case ARM_USART_ABORT_RECEIVE:
    case ARM_USART_ABORT_TRANSFER:
      HAL_USART_Abort(usart->h);
      usart->h->RxXferSize = 0U;
      usart->h->TxXferSize = 0U;
      return ARM_DRIVER_OK;

    // Control TX
    case ARM_USART_CONTROL_TX:
      if (arg) {
        // Transmitter enable
        usart->h->Init.Mode |=  USART_MODE_TX;
      } else {
        // Transmitter disable
        usart->h->Init.Mode &= ~USART_MODE_TX;
      }
      status = HAL_USART_Init(usart->h);
      return USART_HAL_STATUS(status);

    // Control RX
    case ARM_USART_CONTROL_RX:
      if (arg) {
        // Receiver enable
        usart->h->Init.Mode |= USART_MODE_RX;
      } else {
        // Receiver disable
        usart->h->Init.Mode &= ~USART_MODE_RX;
      }
      status = HAL_USART_Init(usart->h);
      return USART_HAL_STATUS(status);
    default: break;
  }

  switch (control & ARM_USART_CONTROL_Msk) {
    case ARM_USART_MODE_ASYNCHRONOUS:
    case ARM_USART_MODE_IRDA:
    case ARM_USART_MODE_SMART_CARD:
    case ARM_USART_MODE_SYNCHRONOUS_SLAVE:
    case ARM_USART_MODE_SINGLE_WIRE:
      return ARM_USART_ERROR_MODE;

    case ARM_USART_MODE_SYNCHRONOUS_MASTER:
      break;

    // Default TX value
    case ARM_USART_SET_DEFAULT_TX_VALUE:
      usart->xfer->def_val = (uint16_t)arg;
      return ARM_DRIVER_OK;

    default: { return ARM_DRIVER_ERROR_UNSUPPORTED; }
  }

  // USART Data bits
  switch (control & ARM_USART_DATA_BITS_Msk) {
    case ARM_USART_DATA_BITS_6:
      if ((control & ARM_USART_PARITY_Msk) != ARM_USART_PARITY_NONE) {
        usart->h->Init.WordLength = USART_WORDLENGTH_7B;
      } else {
        return ARM_USART_ERROR_DATA_BITS;
      }
      break;
    case ARM_USART_DATA_BITS_7:
      if ((control & ARM_USART_PARITY_Msk) == ARM_USART_PARITY_NONE) {
        usart->h->Init.WordLength = USART_WORDLENGTH_7B;
      } else {
        usart->h->Init.WordLength = USART_WORDLENGTH_8B;
      }
      break;
    case ARM_USART_DATA_BITS_8:
      if ((control & ARM_USART_PARITY_Msk) == ARM_USART_PARITY_NONE) {
        usart->h->Init.WordLength = USART_WORDLENGTH_8B;
      } else {
        usart->h->Init.WordLength = USART_WORDLENGTH_9B;
      }
      break;
    case ARM_USART_DATA_BITS_9:
      if ((control & ARM_USART_PARITY_Msk) == ARM_USART_PARITY_NONE) {
        usart->h->Init.WordLength = USART_WORDLENGTH_9B;
      } else {
        return ARM_USART_ERROR_DATA_BITS;
      }
      break;
    default: return ARM_USART_ERROR_DATA_BITS;
  }

  // USART Parity
  switch (control & ARM_USART_PARITY_Msk) {
    case ARM_USART_PARITY_NONE:
      usart->h->Init.Parity = USART_PARITY_NONE;
      break;
    case ARM_USART_PARITY_EVEN:
      usart->h->Init.Parity = USART_PARITY_EVEN;
      break;
    case ARM_USART_PARITY_ODD:
      usart->h->Init.Parity = USART_PARITY_ODD;
      break;
    default: return ARM_USART_ERROR_PARITY;
  }

  // USART Stop bits
  switch (control & ARM_USART_STOP_BITS_Msk) {
    case ARM_USART_STOP_BITS_1:
      usart->h->Init.StopBits = USART_STOPBITS_1;
      break;
    case ARM_USART_STOP_BITS_2:
      usart->h->Init.StopBits = USART_STOPBITS_2;
      break;
    case ARM_USART_STOP_BITS_1_5:
      usart->h->Init.StopBits = USART_STOPBITS_1_5;
      break;
    case ARM_USART_STOP_BITS_0_5:
      usart->h->Init.StopBits = USART_STOPBITS_0_5;
      break;
    default: return ARM_USART_ERROR_STOP_BITS;
  }

  // USART Flow control
  switch (control & ARM_USART_FLOW_CONTROL_Msk) {
    case ARM_USART_FLOW_CONTROL_NONE:
      break;
    case ARM_USART_FLOW_CONTROL_RTS:
    case ARM_USART_FLOW_CONTROL_CTS:
    case ARM_USART_FLOW_CONTROL_RTS_CTS:
    default: return ARM_USART_ERROR_FLOW_CONTROL;
  }

  // Clock setting for synchronous mode
  // Polarity
  switch (control & ARM_USART_CPOL_Msk) {
    case ARM_USART_CPOL0:
      usart->h->Init.CLKPolarity = USART_POLARITY_LOW;
      break;
    case ARM_USART_CPOL1:
      usart->h->Init.CLKPolarity = USART_POLARITY_HIGH;
      break;
    default: return ARM_USART_ERROR_CPOL;
  }

  // Phase
  switch (control & ARM_USART_CPHA_Msk) {
    case ARM_USART_CPHA0:
      usart->h->Init.CLKPhase = USART_PHASE_1EDGE;
      break;
    case ARM_USART_CPHA1:
      usart->h->Init.CLKPhase = USART_PHASE_2EDGE;
      break;
    default: return ARM_USART_ERROR_CPHA;
  }

  // USART Baudrate
  usart->h->Init.BaudRate = arg;

  // Set configured flag
  usart->info->flags |= USART_FLAG_CONFIGURED;

  // Initialize USART
  status = HAL_USART_Init(usart->h);

  // Reconfigure DMA
  if (usart->dma_use != 0U) {
    if (usart->h->hdmatx != NULL) {
      if ((control & ARM_USART_DATA_BITS_Msk) > ARM_USART_DATA_BITS_8) {
        usart->h->hdmatx->Init.SrcDataWidth  = DMA_SRC_DATAWIDTH_HALFWORD;
        usart->h->hdmatx->Init.DestDataWidth = DMA_DEST_DATAWIDTH_HALFWORD;
      } else {
        usart->h->hdmatx->Init.SrcDataWidth  = DMA_SRC_DATAWIDTH_BYTE;
        usart->h->hdmatx->Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
      }
      HAL_DMA_Init(usart->h->hdmatx);
    }

    if (usart->h->hdmarx != NULL) {
      if ((control & ARM_USART_DATA_BITS_Msk) > ARM_USART_DATA_BITS_8) {
        usart->h->hdmarx->Init.SrcDataWidth  = DMA_SRC_DATAWIDTH_HALFWORD;
        usart->h->hdmarx->Init.DestDataWidth = DMA_DEST_DATAWIDTH_HALFWORD;
      } else {
        usart->h->hdmarx->Init.SrcDataWidth  = DMA_SRC_DATAWIDTH_BYTE;
        usart->h->hdmarx->Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
      }
      HAL_DMA_Init(usart->h->hdmarx);
    }
  }
  return USART_HAL_STATUS(status);
}

/**
  \fn          ARM_USART_STATUS USART_GetStatus (const USART_RESOURCES *usart)
  \brief       Get USART status.
  \param[in]   usart     Pointer to USART resources
  \return      USART status \ref ARM_USART_STATUS
*/
static ARM_USART_STATUS USART_GetStatus (const USART_RESOURCES *usart) {
  ARM_USART_STATUS  status = {0U};

  switch (HAL_USART_GetState (usart->h)) {
    case HAL_USART_STATE_BUSY:
    case HAL_USART_STATE_BUSY_TX_RX:
      status.rx_busy = 1;
      status.tx_busy = 1;
      break;
    case HAL_USART_STATE_BUSY_TX:
      status.tx_busy = 1;
      break;
    case HAL_USART_STATE_BUSY_RX:
      status.rx_busy = 1;
      break;

    case HAL_USART_STATE_TIMEOUT:
    case HAL_USART_STATE_READY:
    case HAL_USART_STATE_RESET:
    case HAL_USART_STATE_ERROR:
      break;
  }

  status.tx_underflow     = usart->info->status.tx_underflow;
  status.rx_overflow      = usart->info->status.rx_overflow;
  status.rx_break         = usart->info->status.rx_break;
  status.rx_framing_error = usart->info->status.rx_framing_error;
  status.rx_parity_error  = usart->info->status.rx_parity_error;

  return status;
}

/**
  \fn          int32_t USART_SetModemControl (ARM_USART_MODEM_CONTROL control)
  \brief       Set USART Modem Control line state.
  \param[in]   control   \ref ARM_USART_MODEM_CONTROL
  \return      \ref execution_status
*/
static int32_t USART_SetModemControl (ARM_USART_MODEM_CONTROL control) {

  (void) control;

  // No modem control in synchronous mode
  return ARM_DRIVER_ERROR;
}

/**
  \fn          ARM_USART_MODEM_STATUS USART_GetModemStatus (void)
  \brief       Get USART Modem Status lines state.
  \return      modem status \ref ARM_USART_MODEM_STATUS
*/
static ARM_USART_MODEM_STATUS USART_GetModemStatus (void) {
  ARM_USART_MODEM_STATUS modem_status;

  modem_status.cts = 0U;
  modem_status.dsr = 0U;
  modem_status.ri  = 0U;
  modem_status.dcd = 0U;

  return modem_status;
}

/**
  * @brief Tx Transfer completed callback.
  * @param husart: USART handle.
  * @retval None
  */
void HAL_USART_TxCpltCallback(USART_HandleTypeDef *husart) {
  const USART_RESOURCES * usart;

  usart = USART_Resources (husart);
  usart->xfer->tx_cnt = usart->xfer->tx_num;

  if (usart->info->cb_event != NULL) {
    usart->info->cb_event(ARM_USART_EVENT_TX_COMPLETE | ARM_USART_EVENT_SEND_COMPLETE);
  }
}

/**
  * @brief  Rx Transfer completed callback.
  * @param husart: USART handle.
  * @retval None
  */
void HAL_USART_RxCpltCallback(USART_HandleTypeDef *husart) {
  const USART_RESOURCES * usart;

  usart = USART_Resources (husart);
  usart->xfer->rx_cnt = usart->xfer->rx_num;

  if (usart->info->cb_event != NULL) {
    usart->info->cb_event(ARM_USART_EVENT_RECEIVE_COMPLETE);
  }
}

/**
  * @brief Tx/Rx Transfers completed callback for the non-blocking process.
  * @param husart: USART handle.
  * @retval None
  */
void HAL_USART_TxRxCpltCallback(USART_HandleTypeDef *husart) {
  const USART_RESOURCES * usart;

  usart = USART_Resources (husart);
  usart->xfer->tx_cnt = usart->xfer->tx_num;
  usart->xfer->rx_cnt = usart->xfer->rx_num;

  if (usart->info->cb_event != NULL) {
    usart->info->cb_event(ARM_USART_EVENT_TRANSFER_COMPLETE);
  }
}

/**
  * @brief USART error callback.
  * @param husart: USART handle.
  * @retval None
  */
void HAL_USART_ErrorCallback(USART_HandleTypeDef *husart) {
  const USART_RESOURCES * usart;
        uint32_t          error;
        uint32_t          event;

  usart = USART_Resources (husart);

  error = HAL_USART_GetError (usart->h);
  event = 0;

  if (error & HAL_USART_ERROR_PE) {
    event |= ARM_USART_EVENT_RX_PARITY_ERROR;
    usart->info->status.rx_parity_error = 1;
  }
  if (error & HAL_USART_ERROR_FE) {
    event |= ARM_USART_EVENT_RX_FRAMING_ERROR;
    usart->info->status.rx_framing_error = 1;
  }
  if (error & HAL_USART_ERROR_ORE) {
    event |= ARM_USART_EVENT_RX_OVERFLOW;
    usart->info->status.rx_overflow = 1;
  }

  if ((event != 0) && (usart->info->cb_event != NULL)) {
    usart->info->cb_event(event);
  }
}


#ifdef USART1_MODE_SYNC
USARTx_EXPORT_DRIVER(1);
#endif

#ifdef USART2_MODE_SYNC
USARTx_EXPORT_DRIVER(2);
#endif

#ifdef USART3_MODE_SYNC
USARTx_EXPORT_DRIVER(3);
#endif

#endif

/*! \endcond */
