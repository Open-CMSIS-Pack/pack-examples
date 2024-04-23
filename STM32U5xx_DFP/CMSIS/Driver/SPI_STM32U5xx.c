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
 * $Date:        10. March 2023
 * $Revision:    V1.1
 *
 * Driver:       Driver_SPI1/2/3
 *
 * Configured:   via CubeMX
 * Project:      SPI Driver for STMicroelectronics STM32U5xx
 * -------------------------------------------------------------------------- */

/*! \page stm32u5_spi SPI

# Revision History

- Version 1.1
  - Updated Receive function to properly support Default Tx Value transmission during reception
    (due to STM32 HAL changed functionality)
  - Corrected Control function to support more than 8 data bits in DMA mode
  - Corrected GetDataCount function in DMA mode
- Version 1.0
  - Initial release

# Capabilities

This driver supports **Master** and **Slave** modes.

# Instances

Hardware resource relating to driver instance is shown in the table below:

Driver Instance | Hardware Resource
:---------------|:-----------------:
Driver_SPI1     | SPI1
Driver_SPI2     | SPI2
Driver_SPI3     | SPI3

# Limitations

Hardware limitations:
 - Number of items to send, receive or transmit for SPI3 is limited to 1023

STM32 HAL limitations:
 - Number of items to send, receive or transmit is limited to 65535
 - Mode Fault and Data Lost events can be detected only when reception is active
 - Settings changes using Control function activate upon send/receive/transfer operation start

\note If SPI driver is used in IRQ mode, ensure that it has higher priority then
      any IRQ routine requiring longer processing time, because such IRQ routine
      can prevent SPI IRQ routine to correctly handle reception thus fail to
      receive all data correctly or fail to generate ARM_SPI_EVENT_TRANSFER_COMPLETE
      event.
\note The previous limitation can also be avoided by using SPI driver in DMA mode.

# Configuration

## STM32CubeMX

The SPI driver requires:
  - **SPI peripheral clock**
  - **SPI** peripheral configured as **Full-Duplex Master/Slave**
  - **SPI MISO, MOSI, SCK** and **NSS pins**
  - optional **DMA** configuration for transfers

# B-U585I-IOT02A Board Configuration

These settings are relevant for this board, for different board please refer to the board
schematic for proper configuration.

Link to [B-U585I-IOT02A board schematic](https://www.st.com/resource/en/schematic_pack/mb1551-u585i-c02_schematic.pdf).

## STM32CubeMX

Required peripherals for the **B-U585I-IOT02A** board are listed in the table below:

Peripheral | Mode                         | Description
:---------:|:----------------------------:|:-----------
SPI1       | Full-Duplex Master           | SPI1 Controller
PE12       | Alternate Function Push Pull | SPI1 NSS Pin
PE13       | Alternate Function Push Pull | SPI1 SCK Pin
PE14       | Alternate Function Push Pull | SPI1 MISO Pin
PE15       | Alternate Function Push Pull | SPI1 MOSI Pin

\note All settings have to be configured as described in the procedure below. Important settings,
      typically the ones different from default, are emphasized in **bold**.

### Pinout & Configuration tab

  1. In the **Pinout view** window click on a pin and select it's functionality:
        Pin      | Functionality
        :--------|:-------------:
        PE12     | **SPI1_NSS**
        PE13     | **SPI1_SCK**
        PE14     | **SPI1_MISO**
        PE15     | **SPI1_MOSI**
       \n

  2. Under **Categories**: **Connectivity** select **SPI1**:

     __Mode__:
       - Mode: **Full-Duplex Master**
       - Hardware NSS Signal: **Hardware NSS Output Signal**
       - Hardware RDY Signal: Disable

     __Configuration__:
       - Parameter Settings: not used
       - User Constants: not used
       - NVIC Settings: configured in later step (under Category: System Core: NVIC)
       - DMA Settings: configured in later step (under Category: System Core: GPDMA1)
       - GPIO Settings:
            Pin Name | Signal on Pin | Pin Context..| Pin Priv..| GPIO output..| GPIO mode                     | GPIO Pull-up/Pull..| Maximum out..| Fast Mode | User Label
            :--------|:-------------:|:------------:|:---------:|:------------:|:-----------------------------:|:------------------:|:------------:|:---------:|:----------:
            PE12     | SPI1_NSS      | n/a          | n/a       | n/a          | Alternate Function Push Pull  | No pull-up and no..| **Medium**   | n/a       | .
            PE13     | SPI1_SCK      | n/a          | n/a       | n/a          | Alternate Function Push Pull  | No pull-up and no..| **Medium**   | n/a       | .
            PE14     | SPI1_MISO     | n/a          | n/a       | n/a          | Alternate Function Push Pull  | No pull-up and no..| **Medium**   | n/a       | .
            PE15     | SPI1_MOSI     | n/a          | n/a       | n/a          | Alternate Function Push Pull  | No pull-up and no..| **Medium**   | n/a       | .
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
           - Request: **SPI1_TX**
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
           - Request: **SPI1_RX**
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
            SPI1 global interrupt             | **checked** | 0                   | 0
       - Code generation:
            Enabled interrupt table           | Select for..| Generate Enable in..| Generate IRQ h.. | Call HAL handler
            :---------------------------------|:-----------:|:-------------------:|:----------------:|:----------------:
            GPDMA1 Channel 0 global interrupt | unchecked   | checked             | checked          | checked
            GPDMA1 Channel 1 global interrupt | unchecked   | checked             | checked          | checked
            SPI1 global interrupt             | unchecked   | checked             | checked          | checked

### Clock Configuration tab

  1. Configure **To SPI1 (MHz)**: **160**

### Project Manager tab

  1. Under **Advanced Settings**:

     __Generated Function Calls__:
        Generate Code | Function Name               | Peripheral Inst..| Do not generate ..| Visibility (Static)
        :-------------|:---------------------------:|:----------------:|:-----------------:|:-------------------:
        checked       | MX_SPI1_Init                | SPI1             | **checked**       | checked
     \n

\note If you use HAL based driver in parallel with the CMSIS Driver (for different instances) and if the
      <b>HAL_SPI_RegisterCallback</b> function is called then select <b>ENABLE</b> under <b>Register Callback</b>
      for the <b>SPI</b> peripheral.

### Generate Code

Generate source code by clicking on the **GENERATE CODE** button on the toolbar.

# Validation

Results of the **CMSIS-Driver Validation** for this driver can be found in the [SPI_TestReport.txt](../../../CMSIS/Driver/Validation/SPI_TestReport.txt) file.
*/

/*! \cond */

#include "SPI_STM32U5xx.h"

#define ARM_SPI_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,1)

// Driver Version
static const ARM_DRIVER_VERSION DriverVersion = { ARM_SPI_API_VERSION, ARM_SPI_DRV_VERSION };

// Driver Capabilities
static const ARM_SPI_CAPABILITIES DriverCapabilities = {
  0,  /* Simplex Mode (Master and Slave) */
  1,  /* TI Synchronous Serial Interface */
  0,  /* Microwire Interface */
  1   /* Signal Mode Fault event: \ref ARM_SPI_EVENT_MODE_FAULT */
#if (defined(ARM_SPI_API_VERSION) && (ARM_SPI_API_VERSION >= 0x202U))
, 0U  /* Reserved bits */
#endif
};

static ARM_SPI_CAPABILITIES SPIX_GetCapabilities (void);
static int32_t              SPI_Initialize       (ARM_SPI_SignalEvent_t cb_event, const SPI_RESOURCES *spi);
static int32_t              SPI_Uninitialize     (const SPI_RESOURCES *spi);
static int32_t              SPI_PowerControl     (ARM_POWER_STATE state, const SPI_RESOURCES *spi);
static int32_t              SPI_Send             (const void *data, uint32_t num, const SPI_RESOURCES *spi);
static int32_t              SPI_Receive          (void *data, uint32_t num, const SPI_RESOURCES *spi);
static int32_t              SPI_Transfer         (const void *data_out, void *data_in, uint32_t num, const SPI_RESOURCES *spi);
static uint32_t             SPI_GetDataCount     (const SPI_RESOURCES *spi);
static int32_t              SPI_Control          (uint32_t control, uint32_t arg, const SPI_RESOURCES *spi);
static ARM_SPI_STATUS       SPI_GetStatus        (const SPI_RESOURCES *spi);

// SPI1
#ifdef MX_SPI1
#ifdef  MX_SPI1_NSS_Pin
  SPIx_PIN_NSS_STRUCT_ALLOC(1);
#endif
SPIx_RESOURCE_ALLOC(1);
#endif

// SPI2
#ifdef MX_SPI2
#ifdef  MX_SPI2_NSS_Pin
  SPIx_PIN_NSS_STRUCT_ALLOC(2);
#endif
SPIx_RESOURCE_ALLOC(2);
#endif

// SPI3
#ifdef MX_SPI3
#ifdef  MX_SPI3_NSS_Pin
  SPIx_PIN_NSS_STRUCT_ALLOC(3);
#endif
SPIx_RESOURCE_ALLOC(3);
#endif

/**
  \fn          const SPI_RESOURCES SPI_Resources (SPI_HandleTypeDef *hspi)
  \brief       Get SPI_RESOURCES structure from SPI_HandleTypeDef
*/
static const SPI_RESOURCES * SPI_Resources (SPI_HandleTypeDef *hspi) {
  const SPI_RESOURCES *spi = NULL;

#ifdef MX_SPI1
  if (hspi->Instance == SPI1) { spi = &SPI1_Resources; }
#endif
#ifdef MX_SPI2
  if (hspi->Instance == SPI2) { spi = &SPI2_Resources; }
#endif
#ifdef MX_SPI3
  if (hspi->Instance == SPI3) { spi = &SPI3_Resources; }
#endif

  return spi;
}

/**
  \fn          uint32_t SPI_GetClk (const SPI_RESOURCES *spi)
  \brief       Get SPI peripheral clock
*/
static uint32_t SPI_GetClk (const SPI_RESOURCES *spi) {

#ifdef MX_SPI1
  if (spi->reg == SPI1) { return HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SPI1); }
#endif
#ifdef MX_SPI2
  if (spi->reg == SPI2) { return HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SPI2); }
#endif
#ifdef MX_SPI3
  if (spi->reg == SPI3) { return HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SPI3); }
#endif

  return 0;
}

/**
  \fn          void SPI_PeripheralReset (const SPI_TypeDef *spi)
  \brief       SPI Reset
*/
static void SPI_PeripheralReset (const SPI_TypeDef *spi) {

#ifdef SPI1
  if (spi == SPI1) { __HAL_RCC_SPI1_FORCE_RESET(); }
#endif
#ifdef SPI2
  if (spi == SPI2) { __HAL_RCC_SPI2_FORCE_RESET(); }
#endif
#ifdef SPI3
  if (spi == SPI3) { __HAL_RCC_SPI3_FORCE_RESET(); }
#endif

  __NOP(); __NOP(); __NOP(); __NOP();

#ifdef SPI1
  if (spi == SPI1) { __HAL_RCC_SPI1_RELEASE_RESET(); }
#endif
#ifdef SPI2
  if (spi == SPI2) { __HAL_RCC_SPI2_RELEASE_RESET(); }
#endif
#ifdef SPI3
  if (spi == SPI3) { __HAL_RCC_SPI3_RELEASE_RESET(); }
#endif
}

// SPI Driver functions

/**
  \fn          ARM_DRIVER_VERSION SPIX_GetVersion (void)
  \brief       Get SPI driver version.
  \return      \ref ARM_DRV_VERSION
*/
static ARM_DRIVER_VERSION SPIX_GetVersion (void) {
  return DriverVersion;
}

/**
  \fn          ARM_SPI_CAPABILITIES SPI_GetCapabilities (void)
  \brief       Get driver capabilities.
  \return      \ref ARM_SPI_CAPABILITIES
*/
static ARM_SPI_CAPABILITIES SPIX_GetCapabilities (void) {
  return DriverCapabilities;
}

/**
  \fn          int32_t SPI_Initialize (ARM_SPI_SignalEvent_t cb_event, const SPI_RESOURCES *spi)
  \brief       Initialize SPI Interface.
  \param[in]   cb_event  Pointer to \ref ARM_SPI_SignalEvent
  \param[in]   spi       Pointer to SPI resources
  \return      \ref execution_status
*/
static int32_t SPI_Initialize (ARM_SPI_SignalEvent_t cb_event, const SPI_RESOURCES *spi) {

  if (spi->info->state & SPI_INITIALIZED) { return ARM_DRIVER_OK; }

  // Initialize SPI Run-Time Resources
  spi->info->cb_event = cb_event;

  // Clear transfer information
  memset(spi->xfer, 0, sizeof(SPI_TRANSFER_INFO));

  if (spi->h != NULL) {
    spi->h->Instance    = spi->reg;
    spi->h->RxXferCount = 0U;
    spi->h->RxXferSize  = 0U;
    spi->h->TxXferCount = 0U;
    spi->h->TxXferSize  = 0U;
  }

  spi->info->state = SPI_INITIALIZED;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t SPI_Uninitialize (const SPI_RESOURCES *spi)
  \brief       De-initialize SPI Interface.
  \param[in]   spi  Pointer to SPI resources
  \return      \ref execution_status
*/
static int32_t SPI_Uninitialize (const SPI_RESOURCES *spi) {

  if ((spi->info->state & SPI_POWERED) != 0U) {
    // If peripheral is powered, power off the peripheral
    (void)SPI_PowerControl(ARM_POWER_OFF, spi);
  }
  spi->h->Instance = NULL;

  // Clear SPI state
  spi->info->state = 0U;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t SPI_PowerControl (ARM_POWER_STATE state, const SPI_RESOURCES *spi)
  \brief       Control SPI Interface Power.
  \param[in]   state  Power state
  \param[in]   spi    Pointer to SPI resources
  \return      \ref execution_status
*/
static int32_t SPI_PowerControl (ARM_POWER_STATE state, const SPI_RESOURCES *spi) {

  if ((spi->info->state & SPI_INITIALIZED) == 0U) {
    return ARM_DRIVER_ERROR;
  }

  if ((state != ARM_POWER_OFF)  &&
      (state != ARM_POWER_FULL) &&
      (state != ARM_POWER_LOW)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  switch (state) {
    case ARM_POWER_OFF:
      if (SPI_GetStatus(spi).busy != 0U) {
        // If operation is in progress, abort it
        (void)SPI_Control(ARM_SPI_ABORT_TRANSFER, 0U, spi);
      }

      // SPI peripheral reset
      SPI_PeripheralReset (spi->reg);

      //Deinitialize SPI
      (void)HAL_SPI_DeInit (spi->h);

      // Clear powered flag
      spi->info->state &= ~((uint8_t)SPI_POWERED);
      break;

    case ARM_POWER_FULL:
      if ((spi->info->state & SPI_INITIALIZED) == 0U) {
        return ARM_DRIVER_ERROR;
      }
      if ((spi->info->state & SPI_POWERED)     != 0U) {
        return ARM_DRIVER_OK;
      }

      spi->xfer->def_val = 0U;

      // Ready for operation - set powered flag
      spi->info->state |= SPI_POWERED;

      HAL_SPI_MspInit (spi->h);

      // SPI peripheral reset
      SPI_PeripheralReset (spi->reg);
      break;

    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t SPI_Send (const void *data, uint32_t num, const SPI_RESOURCES *spi)
  \brief       Start sending data to SPI transmitter.
  \param[in]   data  Pointer to buffer with data to send to SPI transmitter
  \param[in]   num   Number of data items to send
  \param[in]   spi   Pointer to SPI resources
  \return      \ref execution_status
*/
static int32_t SPI_Send (const void *data, uint32_t num, const SPI_RESOURCES *spi) {
  HAL_StatusTypeDef stat;

  if ((data == NULL) || (num == 0U)) { return ARM_DRIVER_ERROR_PARAMETER; }

  switch (HAL_SPI_GetState (spi->h)) {
    case HAL_SPI_STATE_ABORT:
    case HAL_SPI_STATE_RESET:
    case HAL_SPI_STATE_ERROR:
      return ARM_DRIVER_ERROR;

    case HAL_SPI_STATE_BUSY:
    case HAL_SPI_STATE_BUSY_TX:
    case HAL_SPI_STATE_BUSY_RX:
    case HAL_SPI_STATE_BUSY_TX_RX:
      return ARM_DRIVER_ERROR_BUSY;

    case HAL_SPI_STATE_READY:
      break;
  }

#ifdef __SPI_DMA_TX
  if ((spi->dma_use & SPI_DMA_USE_TX) != 0U) {
    // DMA mode
    stat = HAL_SPI_Transmit_DMA(spi->h, (uint8_t *)(uint32_t)data, (uint16_t)num);
  } else
#endif
  {
    // Interrupt mode
    stat = HAL_SPI_Transmit_IT (spi->h, (uint8_t *)(uint32_t)data, (uint16_t)num);
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
  \fn          int32_t SPI_Receive (void *data, uint32_t num, const SPI_RESOURCES *spi)
  \brief       Start receiving data from SPI receiver.
  \param[out]  data  Pointer to buffer for data to receive from SPI receiver
  \param[in]   num   Number of data items to receive
  \param[in]   spi   Pointer to SPI resources
  \return      \ref execution_status
*/
static int32_t SPI_Receive (void *data, uint32_t num, const SPI_RESOURCES *spi) {
  uint32_t          i;
  uint8_t          *data_ptr8;
  uint16_t         *data_ptr16;
  HAL_StatusTypeDef stat;

  if ((data == NULL) || (num == 0U)) { return ARM_DRIVER_ERROR_PARAMETER; }

  switch (HAL_SPI_GetState (spi->h)) {
    case HAL_SPI_STATE_ABORT:
    case HAL_SPI_STATE_RESET:
    case HAL_SPI_STATE_ERROR:
      return ARM_DRIVER_ERROR;

    case HAL_SPI_STATE_BUSY:
    case HAL_SPI_STATE_BUSY_TX:
    case HAL_SPI_STATE_BUSY_RX:
    case HAL_SPI_STATE_BUSY_TX_RX:
      return ARM_DRIVER_ERROR_BUSY;

    case HAL_SPI_STATE_READY:
     break;
  }

  // Since HAL does not support default value for Transmission during Reception,
  // this is emulated by loading receive buffer with default values and providing it 
  // to TransmitReceive function as tramsmit buffer also

  // Fill buffer with default transmit value
  if (spi->h->Init.DataSize <= SPI_DATASIZE_8BIT) {
    data_ptr8 = data;
    for (i = 0; i < num; i++) {
      *data_ptr8++ = (uint8_t)spi->xfer->def_val;
    }
  } else {
    data_ptr16 = data;
    for (i = 0; i < num; i++) {
      *data_ptr16++ = spi->xfer->def_val;
    }
  }

#ifdef __SPI_DMA
  if ((spi->dma_use & SPI_DMA_USE_TX_RX) == SPI_DMA_USE_TX_RX) {
    // DMA mode
    stat = HAL_SPI_TransmitReceive_DMA(spi->h, (uint8_t *)(uint32_t)data, (uint8_t *)(uint32_t)data, (uint16_t)num);
  } else
#endif
  {
    // Interrupt mode
    stat = HAL_SPI_TransmitReceive_IT (spi->h, (uint8_t *)(uint32_t)data, (uint8_t *)(uint32_t)data, (uint16_t)num);
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
  \fn          int32_t SPI_Transfer (const void *data_out, void *data_in, uint32_t num, const SPI_RESOURCES *spi)
  \brief       Start sending/receiving data to/from SPI transmitter/receiver.
  \param[in]   data_out  Pointer to buffer with data to send to SPI transmitter
  \param[out]  data_in   Pointer to buffer for data to receive from SPI receiver
  \param[in]   num       Number of data items to transfer
  \param[in]   spi       Pointer to SPI resources
  \return      \ref execution_status
*/
static int32_t SPI_Transfer (const void *data_out, void *data_in, uint32_t num, const SPI_RESOURCES *spi) {
  HAL_StatusTypeDef stat;

  if ((data_out == NULL) || (data_in == NULL) || (num == 0U)) { return ARM_DRIVER_ERROR_PARAMETER; }

  switch (HAL_SPI_GetState (spi->h)) {
    case HAL_SPI_STATE_ABORT:
    case HAL_SPI_STATE_RESET:
    case HAL_SPI_STATE_ERROR:
      return ARM_DRIVER_ERROR;

    case HAL_SPI_STATE_BUSY:
    case HAL_SPI_STATE_BUSY_TX:
    case HAL_SPI_STATE_BUSY_RX:
    case HAL_SPI_STATE_BUSY_TX_RX:
      return ARM_DRIVER_ERROR_BUSY;

    case HAL_SPI_STATE_READY:
      break;
  }

#ifdef __SPI_DMA
  if (spi->dma_use == SPI_DMA_USE_TX_RX) {
    // DMA mode
    stat = HAL_SPI_TransmitReceive_DMA(spi->h, (uint8_t *)(uint32_t)data_out, (uint8_t *)(uint32_t)data_in, (uint16_t)num);
  } else
#endif
  {
    // Interrupt mode
    stat = HAL_SPI_TransmitReceive_IT (spi->h, (uint8_t *)(uint32_t)data_out, (uint8_t *)(uint32_t)data_in, (uint16_t)num);
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
  \fn          uint32_t SPI_GetDataCount (const SPI_RESOURCES *spi)
  \brief       Get transferred data count.
  \param[in]   spi  Pointer to SPI resources
  \return      number of data items transferred
*/
static uint32_t SPI_GetDataCount (const SPI_RESOURCES *spi) {

  if ((spi->info->state & SPI_INITIALIZED) == 0U) {
    return 0U;
  }

#ifdef __SPI_DMA
  if (((spi->dma_use & SPI_DMA_USE_RX) != 0U) && (spi->h->hdmarx != NULL)) {
    if (spi->h->RxXferSize != 0U) {
      // If reception is active
      if (spi->h->RxXferSize >= __HAL_DMA_GET_COUNTER(spi->h->hdmarx)) {
        return (spi->h->RxXferSize - __HAL_DMA_GET_COUNTER(spi->h->hdmarx));
      }
    }
  }
  if (((spi->dma_use & SPI_DMA_USE_TX) != 0U) && (spi->h->hdmatx != NULL)) {
    if (spi->h->TxXferSize != 0U) {
      if (spi->h->TxXferSize >= __HAL_DMA_GET_COUNTER(spi->h->hdmatx)) {
        return (spi->h->TxXferSize - __HAL_DMA_GET_COUNTER(spi->h->hdmatx));
      }
    }
  }
#endif
  if (spi->h->RxXferSize != 0U) {
    // If reception is active
    return (spi->h->RxXferSize - spi->h->RxXferCount);
  }

  if (spi->h->TxXferSize != 0U) {
    return (spi->h->TxXferSize - spi->h->TxXferCount);
  }

  return 0;
}

/**
  \fn          int32_t SPI_Control (uint32_t control, uint32_t arg, const SPI_RESOURCES *spi)
  \brief       Control SPI Interface.
  \param[in]   control  operation
  \param[in]   arg      argument of operation (optional)
  \param[in]   spi      Pointer to SPI resources
  \return      \ref execution_status
*/
static int32_t SPI_Control (uint32_t control, uint32_t arg, const SPI_RESOURCES *spi) {
  GPIO_InitTypeDef GPIO_InitStruct;
  uint32_t         pclk, val;
  bool             reconfigure_nss_pin = false;

  if ((spi->info->state & SPI_POWERED) == 0U) { return ARM_DRIVER_ERROR; }

  if ((control & ARM_SPI_CONTROL_Msk) == ARM_SPI_ABORT_TRANSFER) {
    (void)HAL_SPI_Abort(spi->h);
    spi->h->RxXferSize = 0U;
    spi->h->TxXferSize = 0U;
    return ARM_DRIVER_OK;
  }

  // Check for busy flag
  switch (HAL_SPI_GetState (spi->h)) {
    case HAL_SPI_STATE_ABORT:
    case HAL_SPI_STATE_BUSY:
    case HAL_SPI_STATE_BUSY_TX:
    case HAL_SPI_STATE_BUSY_RX:
    case HAL_SPI_STATE_BUSY_TX_RX:
      return ARM_DRIVER_ERROR_BUSY;

    case HAL_SPI_STATE_READY:
    case HAL_SPI_STATE_RESET:
    case HAL_SPI_STATE_ERROR:
      break;
  }

  switch (control & ARM_SPI_CONTROL_Msk) {
    case ARM_SPI_MODE_INACTIVE:
      __HAL_SPI_DISABLE (spi->h);
      return ARM_DRIVER_OK;

    case ARM_SPI_MODE_MASTER:
      spi->h->Init.Mode      = SPI_MODE_MASTER;
      spi->h->Init.Direction = SPI_DIRECTION_2LINES;
      break;

    case ARM_SPI_MODE_SLAVE:
      spi->h->Init.Mode      = SPI_MODE_SLAVE;
      spi->h->Init.Direction = SPI_DIRECTION_2LINES;
      break;

    case ARM_SPI_MODE_MASTER_SIMPLEX:
      spi->h->Init.Mode      = SPI_MODE_MASTER;
      spi->h->Init.Direction = SPI_DIRECTION_1LINE;
      break;

    case ARM_SPI_MODE_SLAVE_SIMPLEX:
      spi->h->Init.Mode      = SPI_MODE_SLAVE;
      spi->h->Init.Direction = SPI_DIRECTION_1LINE;
      break;

    case ARM_SPI_SET_BUS_SPEED:
      // Set SPI Bus Speed
      pclk = SPI_GetClk(spi);

      for (val = 0U; val < 8U; val++) {
        if (arg >= (pclk >> (val + 1U))) { break; }
      }
      if ((val == 8U) || (arg < (pclk >> (val + 1U)))) {
        // Requested Bus Speed can not be configured
        return ARM_DRIVER_ERROR;
      }

      // Save prescaler value
      spi->h->Init.BaudRatePrescaler = _VAL2FLD(SPI_CFG1_MBR, val);

      if (HAL_SPI_Init (spi->h) != HAL_OK) {
        return ARM_DRIVER_ERROR;
      }
      return ARM_DRIVER_OK;

    case ARM_SPI_GET_BUS_SPEED:
      // Return current bus speed
      pclk = SPI_GetClk(spi);
      return ((int32_t)(pclk >> (_FLD2VAL(SPI_CFG1_MBR, spi->reg->CFG1) + 1U)));

    case ARM_SPI_SET_DEFAULT_TX_VALUE:
      spi->xfer->def_val = (uint16_t)(arg & 0xFFFFU);
      return ARM_DRIVER_OK;

    case ARM_SPI_CONTROL_SS:
      val = (spi->info->mode & ARM_SPI_CONTROL_Msk);
      // Master modes
      if (val == ARM_SPI_MODE_MASTER) {
        val = spi->info->mode & ARM_SPI_SS_MASTER_MODE_Msk;
        // Check if NSS pin is available and
        // software slave select master is selected
        if ((spi->nss != NULL) && (val == ARM_SPI_SS_MASTER_SW)) {
          // Set/Clear NSS pin
          if (arg == ARM_SPI_SS_INACTIVE) {
            // Inactive High
            HAL_GPIO_WritePin (spi->nss->port, (uint16_t)spi->nss->pin, GPIO_PIN_SET);
          } else {
            // Active Low
            HAL_GPIO_WritePin (spi->nss->port, (uint16_t)spi->nss->pin, GPIO_PIN_RESET);
          }
        } else return ARM_DRIVER_ERROR;
        return ARM_DRIVER_OK;
      }
      // Slave modes
      else if (val == ARM_SPI_MODE_SLAVE) {
        val = spi->info->mode & ARM_SPI_SS_SLAVE_MODE_Msk;
        // Check if slave select slave mode is selected
        if (val == ARM_SPI_SS_SLAVE_SW) {
          if (arg == ARM_SPI_SS_ACTIVE) {
            // Active Low
            spi->reg->CR1 &= ~SPI_CR1_SSI;
          }
          else {
            spi->reg->CR1 |= SPI_CR1_SSI;
          }
          return ARM_DRIVER_OK;
        } else { return ARM_DRIVER_ERROR; }
      } else { return ARM_DRIVER_ERROR; }

    default:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  // Frame format:
  spi->h->Init.TIMode = SPI_TIMODE_DISABLE;
  switch (control & ARM_SPI_FRAME_FORMAT_Msk) {
    case ARM_SPI_CPOL0_CPHA0:
      spi->h->Init.CLKPhase    = SPI_PHASE_1EDGE;
      spi->h->Init.CLKPolarity = SPI_POLARITY_LOW;
      break;
    case ARM_SPI_CPOL0_CPHA1:
      spi->h->Init.CLKPhase    = SPI_PHASE_2EDGE;
      spi->h->Init.CLKPolarity = SPI_POLARITY_LOW;
      break;
    case ARM_SPI_CPOL1_CPHA0:
      spi->h->Init.CLKPhase    = SPI_PHASE_1EDGE;
      spi->h->Init.CLKPolarity = SPI_POLARITY_HIGH;
      break;
    case ARM_SPI_CPOL1_CPHA1:
      spi->h->Init.CLKPhase    = SPI_PHASE_2EDGE;
      spi->h->Init.CLKPolarity = SPI_POLARITY_HIGH;
      break;
    case ARM_SPI_TI_SSI:
      spi->h->Init.TIMode      = SPI_TIMODE_ENABLE;
      break;
    case ARM_SPI_MICROWIRE:
      return ARM_SPI_ERROR_FRAME_FORMAT;
    default: return ARM_SPI_ERROR_FRAME_FORMAT;
  }

  // Data Bits
  switch (control & ARM_SPI_DATA_BITS_Msk) {
    case ARM_SPI_DATA_BITS(4U):   spi->h->Init.DataSize = SPI_DATASIZE_4BIT;  break;
    case ARM_SPI_DATA_BITS(5U):   spi->h->Init.DataSize = SPI_DATASIZE_5BIT;  break;
    case ARM_SPI_DATA_BITS(6U):   spi->h->Init.DataSize = SPI_DATASIZE_6BIT;  break;
    case ARM_SPI_DATA_BITS(7U):   spi->h->Init.DataSize = SPI_DATASIZE_7BIT;  break;
    case ARM_SPI_DATA_BITS(8U):   spi->h->Init.DataSize = SPI_DATASIZE_8BIT;  break;
    case ARM_SPI_DATA_BITS(9U):   spi->h->Init.DataSize = SPI_DATASIZE_9BIT;  break;
    case ARM_SPI_DATA_BITS(10U):  spi->h->Init.DataSize = SPI_DATASIZE_10BIT; break;
    case ARM_SPI_DATA_BITS(11U):  spi->h->Init.DataSize = SPI_DATASIZE_11BIT; break;
    case ARM_SPI_DATA_BITS(12U):  spi->h->Init.DataSize = SPI_DATASIZE_12BIT; break;
    case ARM_SPI_DATA_BITS(13U):  spi->h->Init.DataSize = SPI_DATASIZE_13BIT; break;
    case ARM_SPI_DATA_BITS(14U):  spi->h->Init.DataSize = SPI_DATASIZE_14BIT; break;
    case ARM_SPI_DATA_BITS(15U):  spi->h->Init.DataSize = SPI_DATASIZE_15BIT; break;
    case ARM_SPI_DATA_BITS(16U):  spi->h->Init.DataSize = SPI_DATASIZE_16BIT; break;
    case ARM_SPI_DATA_BITS(17U):  spi->h->Init.DataSize = SPI_DATASIZE_17BIT; break;
    case ARM_SPI_DATA_BITS(18U):  spi->h->Init.DataSize = SPI_DATASIZE_18BIT; break;
    case ARM_SPI_DATA_BITS(19U):  spi->h->Init.DataSize = SPI_DATASIZE_19BIT; break;
    case ARM_SPI_DATA_BITS(20U):  spi->h->Init.DataSize = SPI_DATASIZE_20BIT; break;
    case ARM_SPI_DATA_BITS(21U):  spi->h->Init.DataSize = SPI_DATASIZE_21BIT; break;
    case ARM_SPI_DATA_BITS(22U):  spi->h->Init.DataSize = SPI_DATASIZE_22BIT; break;
    case ARM_SPI_DATA_BITS(23U):  spi->h->Init.DataSize = SPI_DATASIZE_23BIT; break;
    case ARM_SPI_DATA_BITS(24U):  spi->h->Init.DataSize = SPI_DATASIZE_24BIT; break;
    case ARM_SPI_DATA_BITS(25U):  spi->h->Init.DataSize = SPI_DATASIZE_25BIT; break;
    case ARM_SPI_DATA_BITS(26U):  spi->h->Init.DataSize = SPI_DATASIZE_26BIT; break;
    case ARM_SPI_DATA_BITS(27U):  spi->h->Init.DataSize = SPI_DATASIZE_27BIT; break;
    case ARM_SPI_DATA_BITS(28U):  spi->h->Init.DataSize = SPI_DATASIZE_28BIT; break;
    case ARM_SPI_DATA_BITS(29U):  spi->h->Init.DataSize = SPI_DATASIZE_29BIT; break;
    case ARM_SPI_DATA_BITS(30U):  spi->h->Init.DataSize = SPI_DATASIZE_30BIT; break;
    case ARM_SPI_DATA_BITS(31U):  spi->h->Init.DataSize = SPI_DATASIZE_31BIT; break;
    case ARM_SPI_DATA_BITS(32U):  spi->h->Init.DataSize = SPI_DATASIZE_32BIT; break;
    default: return ARM_SPI_ERROR_DATA_BITS;
  }

  // Bit order
  if ((control & ARM_SPI_BIT_ORDER_Msk) == ARM_SPI_LSB_MSB) {
    spi->h->Init.FirstBit = SPI_FIRSTBIT_LSB;
  } else {
    spi->h->Init.FirstBit = SPI_FIRSTBIT_MSB;
  }

  // Slave select master modes
  spi->h->Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (spi->h->Init.Mode == SPI_MODE_MASTER) {
    switch (control & ARM_SPI_SS_MASTER_MODE_Msk) {
      case ARM_SPI_SS_MASTER_UNUSED:
        spi->h->Init.NSS = SPI_NSS_SOFT;
        break;

      case ARM_SPI_SS_MASTER_HW_INPUT:
        spi->h->Init.NSS = SPI_NSS_HARD_INPUT;
        if (spi->nss != NULL) {
          // Configure NSS pin
          GPIO_InitStruct.Pin       = spi->nss->pin;
          GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
          GPIO_InitStruct.Pull      = GPIO_NOPULL;
          GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
          GPIO_InitStruct.Alternate = spi->nss->af;
          reconfigure_nss_pin = true;
        } else {
          // NSS pin is not available
          return ARM_SPI_ERROR_SS_MODE;
        }
        break;

      case ARM_SPI_SS_MASTER_SW:
        spi->h->Init.NSS      = SPI_NSS_SOFT;
        if (spi->nss != NULL) {
          // Configure NSS pin as GPIO output
          GPIO_InitStruct.Pin       = spi->nss->pin;
          GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
          GPIO_InitStruct.Pull      = GPIO_NOPULL;
          GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
          reconfigure_nss_pin = true;
        } else {
          // NSS pin is not available
          return ARM_SPI_ERROR_SS_MODE;
        }
        break;

      case ARM_SPI_SS_MASTER_HW_OUTPUT:
        spi->h->Init.NSS      = SPI_NSS_HARD_OUTPUT;
        if (spi->nss != NULL) {
          // Configure NSS pin - SPI NSS alternative function
          GPIO_InitStruct.Pin       = spi->nss->pin;
          GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
          GPIO_InitStruct.Pull      = GPIO_NOPULL;
          GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
          GPIO_InitStruct.Alternate = spi->nss->af;
          reconfigure_nss_pin = true;
        } else {
          // NSS pin is not available
          return ARM_SPI_ERROR_SS_MODE;
        }
        break;
      default: return ARM_SPI_ERROR_SS_MODE;
    }
  }

  // Slave select slave modes
  if (spi->h->Init.Mode ==  SPI_MODE_SLAVE) {
    switch (control & ARM_SPI_SS_SLAVE_MODE_Msk) {
      case ARM_SPI_SS_SLAVE_HW:
        spi->h->Init.NSS = SPI_NSS_HARD_INPUT;
        if (spi->nss != NULL) {
          // Configure NSS pin - SPI NSS alternative function
          GPIO_InitStruct.Pin       = spi->nss->pin;
          GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
          GPIO_InitStruct.Pull      = GPIO_NOPULL;
          GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
          GPIO_InitStruct.Alternate = spi->nss->af;
          reconfigure_nss_pin = true;
        } else {
          // NSS pin is not available
          return ARM_SPI_ERROR_SS_MODE;
        }
        break;

      case ARM_SPI_SS_SLAVE_SW:
        spi->h->Init.NSS = SPI_NSS_SOFT;
        if (spi->nss != NULL) {
          // Unconfigure NSS pin
          HAL_GPIO_DeInit (spi->nss->port, spi->nss->pin);
        }
        break;
      default: return ARM_SPI_ERROR_SS_MODE;
    }
  }

  // Set SPI Bus Speed, only for master mode
  if (spi->h->Init.Mode == SPI_MODE_MASTER) {
    pclk = SPI_GetClk(spi);
    for (val = 0U; val < 8U; val++) {
      if (arg >= (pclk >> (val + 1U))) {
        break;
      }
    }
    if ((val == 8U) || (arg < (pclk >> (val + 1U)))) {
      // Requested Bus Speed can not be configured
      return ARM_DRIVER_ERROR;
    }
    // Save prescaler value
    switch (val) {
      case 0: spi->h->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;   break;
      case 1: spi->h->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;   break;
      case 2: spi->h->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;   break;
      case 3: spi->h->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;  break;
      case 4: spi->h->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;  break;
      case 5: spi->h->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;  break;
      case 6: spi->h->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128; break;
      case 7: spi->h->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; break;
    }
  }

  spi->h->Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;
  if (HAL_SPI_Init (spi->h) != HAL_OK) {
    return ARM_DRIVER_ERROR;
  }

  // Reconfigure nss pin
  if (reconfigure_nss_pin == true) {
    HAL_GPIO_Init(spi->nss->port, &GPIO_InitStruct);
  }

  // Reconfigure DMA
#ifdef __SPI_DMA_RX
  if (((spi->dma_use & SPI_DMA_USE_RX) != 0U) && (spi->h->hdmarx != NULL)) {
    if ((control & ARM_SPI_DATA_BITS_Msk) > ARM_SPI_DATA_BITS(16U)) {
      spi->h->hdmarx->Init.SrcDataWidth  = DMA_SRC_DATAWIDTH_WORD;
      spi->h->hdmarx->Init.DestDataWidth = DMA_DEST_DATAWIDTH_WORD;
    } else if ((control & ARM_SPI_DATA_BITS_Msk) > ARM_SPI_DATA_BITS(8U)) {
      spi->h->hdmarx->Init.SrcDataWidth  = DMA_SRC_DATAWIDTH_HALFWORD;
      spi->h->hdmarx->Init.DestDataWidth = DMA_DEST_DATAWIDTH_HALFWORD;
    } else {
      spi->h->hdmarx->Init.SrcDataWidth  = DMA_SRC_DATAWIDTH_BYTE;
      spi->h->hdmarx->Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
    }
    HAL_DMA_Init(spi->h->hdmarx);
  }
#endif

#ifdef __SPI_DMA_TX
  if (((spi->dma_use & SPI_DMA_USE_TX) != 0U) && (spi->h->hdmatx != NULL)) {
    if ((control & ARM_SPI_DATA_BITS_Msk) > ARM_SPI_DATA_BITS(16U)) {
      spi->h->hdmatx->Init.SrcDataWidth  = DMA_SRC_DATAWIDTH_WORD;
      spi->h->hdmatx->Init.DestDataWidth = DMA_DEST_DATAWIDTH_WORD;
    } else if ((control & ARM_SPI_DATA_BITS_Msk) > ARM_SPI_DATA_BITS(8U)) {
      spi->h->hdmatx->Init.SrcDataWidth  = DMA_SRC_DATAWIDTH_HALFWORD;
      spi->h->hdmatx->Init.DestDataWidth = DMA_DEST_DATAWIDTH_HALFWORD;
    } else {
      spi->h->hdmatx->Init.SrcDataWidth  = DMA_SRC_DATAWIDTH_BYTE;
      spi->h->hdmatx->Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
    }
    HAL_DMA_Init(spi->h->hdmatx);
  }
#endif

  spi->info->mode   = control;
  spi->info->state |= SPI_CONFIGURED;

  return ARM_DRIVER_OK;
}

/**
  \fn          ARM_SPI_STATUS SPI_GetStatus (const SPI_RESOURCES *spi)
  \brief       Get SPI status.
  \param[in]   spi  Pointer to SPI resources
  \return      SPI status \ref ARM_SPI_STATUS
*/
static ARM_SPI_STATUS SPI_GetStatus (const SPI_RESOURCES *spi) {
  ARM_SPI_STATUS status = {0U};
  uint32_t       error;

  error = HAL_SPI_GetError (spi->h);

  switch (HAL_SPI_GetState (spi->h)) {
    case HAL_SPI_STATE_ABORT:
    case HAL_SPI_STATE_BUSY:
    case HAL_SPI_STATE_BUSY_TX:
    case HAL_SPI_STATE_BUSY_RX:
    case HAL_SPI_STATE_BUSY_TX_RX:
      status.busy = 1U;
      break;

    case HAL_SPI_STATE_RESET:
    case HAL_SPI_STATE_ERROR:
    case HAL_SPI_STATE_READY:
      status.busy = 0U;
      break;
  }

  if (error & HAL_SPI_ERROR_OVR)  { status.data_lost  = 1U; }
  else                            { status.data_lost  = 0U; }
  if (error & HAL_SPI_ERROR_MODF) { status.mode_fault = 1U; }
  else                            { status.mode_fault = 0U; }

  return status;
}

/**
  \fn          void SPI_TransferComplete (SPI_HandleTypeDef *hspi)
  \brief       Transfer Complete Callback
*/
static void SPI_TransferComplete (SPI_HandleTypeDef *hspi) {
  const SPI_RESOURCES * spi;
  spi = SPI_Resources (hspi);

  if (spi->info->cb_event != NULL) {
    spi->info->cb_event(ARM_SPI_EVENT_TRANSFER_COMPLETE);
  }
}

/**
  * @brief Tx Transfer completed callback.
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
  const SPI_RESOURCES * spi;
  spi = SPI_Resources (hspi);

  HAL_SPIEx_FlushRxFifo(spi->h);
  SPI_TransferComplete (hspi);
}

/**
  * @brief Rx Transfer completed callback.
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {
  SPI_TransferComplete (hspi);
}

/**
  * @brief Tx and Rx Transfer completed callback.
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
  SPI_TransferComplete (hspi);
}

/**
  * @brief SPI error callback.
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi) {
  uint32_t  error, event;
  const SPI_RESOURCES * spi;

  spi = SPI_Resources (hspi);
  error = HAL_SPI_GetError (hspi);

  event = 0;
  if (error & HAL_SPI_ERROR_MODF) {
    event |= ARM_SPI_EVENT_MODE_FAULT;
  }
  if (error & HAL_SPI_ERROR_OVR) {
    event |= ARM_SPI_EVENT_DATA_LOST;
  }

  if ((spi->info->cb_event != NULL) && (event != 0)) {
    spi->info->cb_event(event);
  }
}

// SPI1
#ifdef MX_SPI1
SPIx_EXPORT_DRIVER(1);
#endif

// SPI2
#ifdef MX_SPI2
SPIx_EXPORT_DRIVER(2);
#endif

// SPI3
#ifdef MX_SPI3
SPIx_EXPORT_DRIVER(3);
#endif

/*! \endcond */
