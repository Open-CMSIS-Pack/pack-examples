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
 * $Date:        10. January 2023
 * $Revision:    V1.0
 *
 * Project:      OTG Full/Low-Speed Common Driver for
 *               STMicroelectronics STM32U5xx
 * -------------------------------------------------------------------------- */

/* History:
 *  Version 1.0
 *    Initial release
 */

#include "OTG_STM32U5xx.h"

#include "Driver_USBH.h"
#include "Driver_USBD.h"

#include "stm32u5xx_hal.h"


// Externally defined variables and functions
#ifdef HAL_HCD_MODULE_ENABLED
extern void USBH_FS_IRQ (uint32_t gintsts);
#endif

#ifdef HAL_PCD_MODULE_ENABLED
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;

extern void HAL_PCD_IRQHandler(PCD_HandleTypeDef *hpcd);
#endif

// Global variables
uint8_t otg_fs_role = ARM_USB_ROLE_NONE;


// OTG IRQ routine (common for Device and Host) ********************************

/**
  \fn          void OTG_FS_IRQHandler (void)
  \brief       USB Interrupt Routine (IRQ).
*/
void OTG_FS_IRQHandler (void) {

#if (defined(HAL_HCD_MODULE_ENABLED) && defined(HAL_PCD_MODULE_ENABLED))
  switch (otg_fs_role) {
    case ARM_USB_ROLE_HOST:
      USBH_FS_IRQ (USB_OTG_FS->GINTSTS & USB_OTG_FS->GINTMSK);
      break;
    case ARM_USB_ROLE_DEVICE:
      HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
      break;
    default:
      break;
  }
#elif defined(HAL_HCD_MODULE_ENABLED)
  USBH_FS_IRQ (USB_OTG_FS->GINTSTS & USB_OTG_FS->GINTMSK);
#elif defined(HAL_PCD_MODULE_ENABLED)
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
#endif
}
