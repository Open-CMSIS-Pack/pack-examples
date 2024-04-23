/* -----------------------------------------------------------------------------
 * Copyright (c) 2021 - 2023 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software. Permission is granted to anyone to use this
 * software for any purpose, including commercial applications, and to alter
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *
 * $Date:        31. October 2023
 * $Revision:    V1.3.0
 *
 * Project:      Flash Device Description for ST STM32U5xx Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.3.0
 *    Added more algorithms
 *  Version 1.2.0
 *    Added additional algorithms
 *  Version 1.1.0
 *    Reworked algorithms
 *  Version 1.0.0
 *    Initial release
 */

#include "..\FlashOS.h"                /* FlashOS Structures */


#ifdef FLASH_MEM
#ifdef STM32U5Fx_4096_Secure
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32U5Fx 4M Secure Flash",       /* Device Name */
    ONCHIP,                            /* Device Type */
    0x0C000000,                        /* Device Start Address */
    0x00400000,                        /* Device Size in Bytes (4MB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size 8kB */
    SECTOR_END
  };
#endif

#ifdef STM32U5Fx_4096_NSecure
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32U5Fx 4M NSecure Flash",      /* Device Name */
    ONCHIP,                            /* Device Typev */
    0x08000000,                        /* Device Start Address */
    0x00400000,                        /* Device Size in Bytes (4MB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size 8kB */
    SECTOR_END
  };
#endif

#ifdef STM32U5Fx_2048_NSecure
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32U5Fx 2M NSecure Flash",      /* Device Name */
    ONCHIP,                            /* Device Type */
    0x08000000,                        /* Device Start Address */
    0x00200000,                        // Device Size in Bytes (2MB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size 8kB */
    SECTOR_END
  };
#endif

#ifdef STM32U5Fx_2048_Secure
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32U5Fx 2M NSecure Flash",      /* Device Name */
    ONCHIP,                            /* Device Type */
    0x0C000000,                        /* Device Start Address */
    0x00200000,                        /* Device Size in Bytes (2MB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size 8kB */
    SECTOR_END
  };
#endif

#ifdef STM32U59x_4096K_NSecure
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32U5xx 4M NSecure Flash",      /* Device Name */
    ONCHIP,                            /* Device Type */
    0x08000000,                        /* Device Start Address */
    0x00400000,                        /* Device Size in Bytes (4MB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size  8kB (512 Sectore) */
    SECTOR_END
  };
#endif

#ifdef STM32U59x_4096K_Secure
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32U5xx 4M Secure Flash",       /* Device Name */
    ONCHIP,                            /* Device Type */
    0x0C000000,                        /* Device Start Address */
    0x00400000,                        /* Device Size in Bytes (4MB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size  8kB (512 Sectore) */
    SECTOR_END
  };
#endif

#ifdef STM32U59x_2048K_NSecure
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32U59x 2M NSecure Flash",      /* Device Name */
    ONCHIP,                            /* Device Type */
    0x08000000,                        /* Device Start Address */
    0x00200000,                        // Device Size in Bytes (2MB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size  8kB (256 Sectore) */
    SECTOR_END
  };
#endif

#ifdef STM32U59x_2048K_Secure
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32U59x 2M NSecure Flash",      /* Device Name */
    ONCHIP,                            /* Device Type */
    0x0C000000,                        /* Device Start Address */
    0x00200000,                        /* Device Size in Bytes (2MB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size  8kB (256 Sectore) */
    SECTOR_END
  };
#endif

#ifdef STM32U5xx_2048K_NSecure
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32U5xx 2M NSecure Flash",      /* Device Name */
    ONCHIP,                            /* Device Type */
    0x08000000,                        /* Device Start Address */
    0x00200000,                        /* Device Size in Bytes (2048 kB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size  8kB (256 Sectore) */
    SECTOR_END
  };
#endif

#ifdef STM32U5xx_2048K_Secure
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32U5xx 2M Secure Flash",       /* Device Name */
    ONCHIP,                            /* Device Type */
    0x0C000000,                        /* Device Start Address */
    0x00200000,                        /* Device Size in Bytes (2048 kB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size  8kB (256 Sectore) */
    SECTOR_END
  };
#endif

#ifdef STM32U5xx_1024K_NSecure
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32U5xx 1M NSecure Flash",      /* Device Name */
    ONCHIP,                            /* Device Type */
    0x08000000,                        /* Device Start Address */
    0x00100000,                        /* Device Size in Bytes (1024 kB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size  8kB (128 Sectore) */
    SECTOR_END
  };
#endif

#ifdef STM32U5xx_1024K_Secure
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32U5xx 1M Secure Flash",       /* Device Name */
    ONCHIP,                            /* Device Type */
    0x0C000000,                        /* Device Start Address */
    0x00100000,                        /* Device Size in Bytes (1024 kB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size  8kB (128 Sectore) */
    SECTOR_END
  };
#endif

#ifdef STM32U5xx_512K_NSecure
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32U5xx 512K NSecure Flash",    /* Device Name */
    ONCHIP,                            /* Device Type */
    0x08000000,                        /* Device Start Address */
    0x00080000,                        /* Device Size in Bytes (512KB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size  8kB (64 Sectore) */
    SECTOR_END
  };
#endif
#ifdef STM32U5xx_512K_Secure
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32U5xx 512K Secure Flash",     /* Device Name */
    ONCHIP,                            /* Device Type */
    0x0C000000,                        /* Device Start Address */
    0x00080000,                        /* Device Size in Bytes (512KB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size  8kB (64 Sectore) */
    SECTOR_END
  };
#endif
	#ifdef STM32U5xx_256K_NSecure
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32U5xx 256K NSecure Flash",    /* Device Name */
    ONCHIP,                            /* Device Type */
    0x08000000,                        /* Device Start Address */
    0x00040000,                        /* Device Size in Bytes (256KB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size  8kB (32 Sectore) */
    SECTOR_END
  };
#endif

#ifdef STM32U5xx_256K_Secure
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32U5xx 256K Secure Flash",     /* Device Name */
    ONCHIP,                            /* Device Type */
    0x0C000000,                        /* Device Start Address */
    0x00040000,                        /* Device Size in Bytes (256KB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size  8kB (32 Sectore) */
    SECTOR_END
  };
#endif

#ifdef STM32U5xx_128K_NSecure
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32U5xx 128K NSecure Flash",    /* Device Name */
    ONCHIP,                            /* Device Type */
    0x08000000,                        /* Device Start Address */
    0x00020000,                        /* Device Size in Bytes (128KB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size  8kB (16 Sectore) */
    SECTOR_END
  };
#endif

#ifdef STM32U5xx_128K_Secure
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32U5xx 128K Secure Flash",     /* Device Name */
    ONCHIP,                            /* Device Type */
    0x0C000000,                        /* Device Start Address */
    0x00020000,                        /* Device Size in Bytes (128KB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size  8kB (16 Sectore) */
    SECTOR_END
  };
#endif

#endif /* FLASH_MEM */
