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
 * Project:      USBH Driver Header File for STMicroelectronics STM32U5xx
 * -------------------------------------------------------------------------- */

#ifndef __USBH_STM32U5XX_H
#define __USBH_STM32U5XX_H

#include <stdint.h>

#include "Driver_USBH.h"

// Global functions and variables exported by driver .c module
extern void USBH_FS_IRQ (uint32_t gintsts);
extern ARM_DRIVER_USBH Driver_USBH0;

#endif /* __USBH_STM32U5XX_H */
