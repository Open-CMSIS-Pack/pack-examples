#ifndef REGIONS_STM32H743I_EVAL_H
#define REGIONS_STM32H743I_EVAL_H


//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

// <n>Device pack:   Keil::STM32H7xx_DFP@4.0.0
// <i>Device pack used to generate this file

// <h>ROM Configuration
// =======================
// <h> FLASH_Bank1=<__ROM0>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x08000000
#define __ROM0_BASE 0x08000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00100000
#define __ROM0_SIZE 0x00100000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __ROM0_DEFAULT 1
//   <q>Startup
//   <i> Selects region to be used for startup code.
#define __ROM0_STARTUP 1
// </h>

// <h> FLASH_Bank2=<__ROM1>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x08100000
#define __ROM1_BASE 0x08100000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00100000
#define __ROM1_SIZE 0x00100000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __ROM1_DEFAULT 1
//   <q>Startup
//   <i> Selects region to be used for startup code.
#define __ROM1_STARTUP 1
// </h>

// </h>

// <h>RAM Configuration
// =======================
// <h> DTCMRAM=<__RAM0>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x20000000
#define __RAM0_BASE 0x20000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00020000
#define __RAM0_SIZE 0x00020000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM0_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM0_NOINIT 0
// </h>

// <h> RAM_D1=<__RAM1>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x24000000
#define __RAM1_BASE 0x24000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00080000
#define __RAM1_SIZE 0x00080000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM1_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM1_NOINIT 0
// </h>

// <h> RAM_D2=<__RAM2>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x30000000
#define __RAM2_BASE 0x30000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00048000
#define __RAM2_SIZE 0x00048000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM2_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM2_NOINIT 0
// </h>

// <h> RAM_D3=<__RAM3>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x38000000
#define __RAM3_BASE 0x38000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00010000
#define __RAM3_SIZE 0x00010000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM3_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM3_NOINIT 0
// </h>

// </h>


#endif /* REGIONS_STM32H743I_EVAL_H */
