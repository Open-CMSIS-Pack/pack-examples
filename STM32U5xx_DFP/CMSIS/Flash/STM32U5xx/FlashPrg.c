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

/* Note:
   Flash has 8K sector size.
   STM32U5xx devices have Dual Bank Flash configuration.  */

#include "..\FlashOS.h"        /* FlashOS Structures */

typedef volatile unsigned long    vu32;
typedef          unsigned long     u32;

#define M32(adr) (*((vu32 *) (adr)))

// Peripheral Memory Map
#define FLASH_BASE       (0x40022000)
#define DBGMCU_BASE      (0xE0044000)
#define FLASHSIZE_BASE   (0x0BFA07A0)

#define FLASH           ((FLASH_TypeDef  *) FLASH_BASE)
#define DBGMCU          ((DBGMCU_TypeDef *) DBGMCU_BASE)

// Debug MCU
typedef struct {
  vu32 IDCODE;
} DBGMCU_TypeDef;

// Flash Registers
typedef struct
{
  vu32 ACR;              /* Offset: 0x00  Flash access control register */
  vu32 RESERVED1;
  vu32 NSKEYR;           /* Offset: 0x08  Flash non-secure key register */
  vu32 SECKEYR;          /* Offset: 0x0C  Flash secure key register */
  vu32 OPTKEYR;          /* Offset: 0x10  Flash option key register */
  vu32 RESERVED2;
  vu32 PDKEY1R;          /* Offset: 0x18  Flash Bank 1 power-down key register */
  vu32 PDKEY2R;          /* Offset: 0x1C  Flash Bank 2 power-down key register */
  vu32 NSSR;             /* Offset: 0x20  Flash non-secure status register */
  vu32 SECSR;            /* Offset: 0x24  Flash secure status register */
  vu32 NSCR;             /* Offset: 0x28  Flash non-secure control register */
  vu32 SECCR;            /* Offset: 0x2C  Flash secure control register */
  vu32 ECCR;             /* Offset: 0x30  Flash ECC register */
  vu32 OPSR;             /* Offset: 0x34  Flash OPSR register */
  vu32 RESERVED3[2];
  vu32 OPTR;             /* Offset: 0x40  Flash option control register */
  vu32 NSBOOTADD0R;      /* Offset: 0x44  Flash non-secure boot address 0 register */
  vu32 NSBOOTADD1R;      /* Offset: 0x48  Flash non-secure boot address 1 register */
  vu32 SECBOOTADD0R;     /* Offset: 0x4C  Flash secure boot address 0 register */
  vu32 SECWM1R1;         /* Offset: 0x50  Flash secure watermark1 register 1 */
  vu32 SECWM1R2;         /* Offset: 0x54  Flash secure watermark1 register 2 */
  vu32 WRP1AR;           /* Offset: 0x58  Flash WRP1 area A address register */
  vu32 WRP1BR;           /* Offset: 0x5C  Flash WRP1 area B address register */
  vu32 SECWM2R1;         /* Offset: 0x60  Flash secure watermark2 register 1 */
  vu32 SECWM2R2;         /* Offset: 0x64  Flash secure watermark2 register 2 */
  vu32 WRP2AR;           /* Offset: 0x68  Flash WRP2 area A address register */
  vu32 WRP2BR;           /* Offset: 0x6C  Flash WRP2 area B address register */
  vu32 OEM1KEYR1;        /* Offset: 0x70  Flash OEM1 key register 1 */
  vu32 OEM1KEYR2;        /* Offset: 0x74  Flash OEM1 key register 2 */
  vu32 OEM2KEYR1;        /* Offset: 0x78  Flash OEM2 key register 1 */
  vu32 OEM2KEYR2;        /* Offset: 0x7C  Flash OEM2 key register 2 */
  vu32 SECBB1R1;         /* Offset: 0x80  Flash secure block-based bank 1 register 1 */
  vu32 SECBB1R2;         /* Offset: 0x84  Flash secure block-based bank 1 register 2 */
  vu32 SECBB1R3;         /* Offset: 0x88  Flash secure block-based bank 1 register 3 */
  vu32 SECBB1R4;         /* Offset: 0x8C  Flash secure block-based bank 1 register 4 */
  vu32 RESERVED4[4];
  vu32 SECBB2R1;         /* Offset: 0xA0  Flash secure block-based bank 2 register 1 */
  vu32 SECBB2R2;         /* Offset: 0xA4  Flash secure block-based bank 2 register 2 */
  vu32 SECBB2R3;         /* Offset: 0xA8  Flash secure block-based bank 2 register 3 */
  vu32 SECBB2R4;         /* Offset: 0xAC  Flash secure block-based bank 2 register 4 */
  vu32 RESERVED5[4];
  vu32 SECHDPCR;         /* Offset: 0xC0  Flash secure HDP control register */
  vu32 PRIVCFGR;         /* Offset: 0xC4  Flash privilege configuration register */
  vu32 RESERVED6[2];
  vu32 PRIVBB1R1;        /* Offset: 0xD0  Flash privilege block-based bank 1 register 1 */
  vu32 PRIVBB1R2;        /* Offset: 0xD4  Flash privilege block-based bank 1 register 2 */
  vu32 PRIVBB1R3;        /* Offset: 0xD8  Flash privilege block-based bank 1 register 3 */
  vu32 PRIVBB1R4;        /* Offset: 0xDC  Flash privilege block-based bank 1 register 4 */
  vu32 RESERVED7[4];
  vu32 PRIVBB2R1;        /* Offset: 0xF0  Flash privilege block-based bank 2 register 1 */
  vu32 PRIVBB2R2;        /* Offset: 0xF4  Flash privilege block-based bank 2 register 2 */
  vu32 PRIVBB2R3;        /* Offset: 0xF8  Flash privilege block-based bank 2 register 3 */
  vu32 PRIVBB2R4;        /* Offset: 0xFC  Flash privilege block-based bank 2 register 4 */
} FLASH_TypeDef;

// Flash Keys
#define FLASH_KEY1               0x45670123
#define FLASH_KEY2               0xCDEF89AB
#define FLASH_OPTKEY1            0x08192A3B
#define FLASH_OPTKEY2            0x4C5D6E7F

// Flash Control Register definitions
#define FLASH_CR_PG             ((u32)(  1U      ))
#define FLASH_CR_PER            ((u32)(  1U <<  1))
#define FLASH_CR_MER1           ((u32)(  1U <<  2))
#define FLASH_CR_PNB_MSK        ((u32)(0xFF <<  3))
#define FLASH_CR_BKER           ((u32)(  1U << 11))
#define FLASH_CR_MER2           ((u32)(  1U << 15))
#define FLASH_CR_STRT           ((u32)(  1U << 16))
#define FLASH_CR_OPTSTRT        ((u32)(  1U << 17))
#define FLASH_CR_OBL_LAUNCH     ((u32)(  1U << 27))
#define FLASH_CR_OPTLOCK        ((u32)(  1U << 30))
#define FLASH_CR_LOCK           ((u32)(  1U << 31))

// Flash Status Register definitions
#define FLASH_SR_EOP            ((u32)(  1U      ))
#define FLASH_SR_OPERR          ((u32)(  1U <<  1))
#define FLASH_SR_PROGERR        ((u32)(  1U <<  3))
#define FLASH_SR_WRPERR         ((u32)(  1U <<  4))
#define FLASH_SR_PGAERR         ((u32)(  1U <<  5))
#define FLASH_SR_SIZERR         ((u32)(  1U <<  6))
#define FLASH_SR_PGSERR         ((u32)(  1U <<  7))
#define FLASH_SR_OPTWERR        ((u32)(  1U << 13))
#define FLASH_SR_BSY            ((u32)(  1U << 16))

// Flash option register definitions
#define FLASH_OPTR_RDP          ((u32)(0xFF      ))
#define FLASH_OPTR_RDP_NO       ((u32)(0xAA      ))
#define FLASH_OPTR_DBANK        ((u32)(  1U << 21))
#define FLASH_OPTR_TZEN         ((u32)(  1U << 31))


#define FLASH_PGERR             (FLASH_SR_OPERR  | FLASH_SR_PROGERR | FLASH_SR_WRPERR  | \
                                 FLASH_SR_PGAERR | FLASH_SR_SIZERR  | FLASH_SR_PGSERR  | FLASH_SR_OPTWERR)

#if defined FLASH_MEM
static u32 gFlashBase;                  /* Flash base address */
static u32 gFlashSize;                  /* Flash size in bytes */

static vu32 *pFlashCR;                  /* Pointer to Flash Control register */
static vu32 *pFlashSR;                  /* Pointer to Flash Status register */
#endif /* FLASH_MEM */

static void DSB(void) {
  __asm("DSB");
}

static void NOP(void) {
  __asm("NOP");
}


/*
 * Get Flash security Mode
 *    Return Value:   0 = non-secure Flash
 *                    1 = secure Flash
 */

static u32 GetFlashSecureMode (void) {
  u32 flashSecureMode;

  flashSecureMode = (FLASH->OPTR & FLASH_OPTR_TZEN) ? 1U : 0U;

  return (flashSecureMode);
}


/*
 * Get Flash Type
 *    Return Value:   0 = Single-Bank flash
 *                    1 = Dual-Bank Flash (configurable)
 */

#if defined FLASH_MEM
static u32 GetFlashType (void) {
//  u32 flashType = 0U;
//
//  switch ((DBGMCU->IDCODE & 0xFFFU))
//  {
//    case 0x482:             /* Flash Category ? devices, 8k sectors (STM32U575/585) */
//    default:                /* devices have a dual bank flash, configurable via FLASH_OPTR.DBANK */
//      flashType = 1U;       /* Dual-Bank Flash type */
//    break;
//  }
//
//  return (flashType);
  return (1U);              /* always Dual-Bank Flash */
}
#endif /* FLASH_MEM */


/*
 * Get Flash Bank Mode
 *    Return Value:   0 = Single-Bank mode
 *                    1 = Dual-Bank mode
 */

#if defined FLASH_MEM
static u32 GetFlashBankMode (void) {
  u32 flashBankMode;

  flashBankMode = (FLASH->OPTR & FLASH_OPTR_DBANK) ? 1U : 0U;

  return (flashBankMode);
}
#endif /* FLASH_MEM */


/*
 * Get Flash Bank Number
 *    Parameter:      adr:  Sector Address
 *    Return Value:   Bank Number (0..1)
 *                    Flash bank size is always the half of the Flash size
 */

#if defined FLASH_MEM
static u32 GetFlashBankNum(u32 adr) {
  u32 flashBankNum;

  if (GetFlashType() == 1U)
  {
    /* Dual-Bank Flash */
    if (GetFlashBankMode() == 1U)
    {
      /* Dual-Bank Flash configured as Dual-Bank */
      if (adr >= (gFlashBase + (gFlashSize >> 1)))
      {
        flashBankNum = 1U;
      }
      else
      {
        flashBankNum = 0U;
      }
    }
    else
    {
      /* Dual-Bank Flash configured as Single-Bank */
      flashBankNum = 0U;
    }
  }
  else
  {
    /* Single-Bank Flash */
    flashBankNum = 0U;
  }

  return (flashBankNum);
}
#endif /* FLASH_MEM */


/*
 * Get Flash Page Number
 *    Parameter:      adr:  Page Address
 *    Return Value:   Page Number (0..255)
 */

#if defined FLASH_MEM
static u32 GetFlashPageNum (unsigned long adr) {
  u32 flashPageNum;

  if (GetFlashType() == 1U)
  {
    /* Dual-Bank Flash */
    if (GetFlashBankMode() == 1U)
    {
      /* Dual-Bank Flash configured as Dual-Bank */
      flashPageNum = (((adr & ((gFlashSize >> 1) - 1U)) ) >> 13); /* 8K sector size */
    }
    else
    {
      /* Dual-Bank Flash configured as Single-Bank */
      flashPageNum = (((adr & (gFlashSize        - 1U)) ) >> 13); /* 8K sector size */
    }
  }
  else
  {
      /* Single-Bank Flash */
      flashPageNum = (((adr & (gFlashSize        - 1U)) ) >> 13); /* 8K sector size */
  }

  return (flashPageNum);
}
#endif /* FLASH_MEM */


/*
 * Get Flash Page Size
 *    Return Value:   flash page size (in Bytes)
 */

#if 0 //defined FLASH_MEM
static u32 GetFlashPageSize (void)
{
  u32 flashPageSize;

  if (GetFlashType() == 1U)
  {
    /* Dual-Bank Flash */
    if (GetFlashBankMode() == 1U)
    {
      /* Dual-Bank Flash configured as Dual-Bank */
      flashPageSize = 0x2000;                            /* 8K sector size */
    }
    else
    {
      /* Dual-Bank Flash configured as Single-Bank */
      flashPageSize = 0x2000;                            /* 8K sector size */
    }
  }
  else
  {
    /* Single-Bank Flash */
      flashPageSize = 0x2000;                            /* 8K sector size */
  }

  return (flashPageSize);
}
#endif /* FLASH_MEM */


/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init (unsigned long adr, unsigned long clk, unsigned long fnc)
{
  (void)clk;
  (void)fnc;

	 __disable_irq();

#if defined FLASH_MEM
  if (GetFlashSecureMode() == 0U)
  {                                                      /* Flash non-secure */
    /* set used Control, Status register */
    pFlashCR = &FLASH->NSCR;
    pFlashSR = &FLASH->NSSR;

    /* unlock FLASH_NSCR */
    FLASH->NSKEYR = FLASH_KEY1;
    FLASH->NSKEYR = FLASH_KEY2;
    DSB();
    while (FLASH->NSSR & FLASH_SR_BSY) NOP();            /* Wait until operation is finished */
  }
  else
  {                                                      /* Flash secure */
    /* set used Control, Status register */
    pFlashCR = &FLASH->SECCR;
    pFlashSR = &FLASH->SECSR;

    /* unlock FLASH_SECCR */
    FLASH->SECKEYR = FLASH_KEY1;
    FLASH->SECKEYR = FLASH_KEY2;
    DSB();
    while (FLASH->SECSR & FLASH_SR_BSY) NOP();           /* Wait until operation is finished */

    /* Flash block-based secure bank1 */
    FLASH->SECBB1R1 = 0xFFFFFFFF;
    FLASH->SECBB1R2 = 0xFFFFFFFF;
    FLASH->SECBB1R3 = 0xFFFFFFFF;
    FLASH->SECBB1R4 = 0xFFFFFFFF;

    if (GetFlashBankMode() == 1U)                        /* Flash secure DUAL BANK */
    {
      /* Flash block-based secure bank2 */
      FLASH->SECBB2R1 = 0xFFFFFFFF;
      FLASH->SECBB2R2 = 0xFFFFFFFF;
      FLASH->SECBB2R3 = 0xFFFFFFFF;
      FLASH->SECBB2R4 = 0xFFFFFFFF;
    }
  }

  while (*pFlashSR & FLASH_SR_BSY) NOP();                /* Wait until operation is finished */

  gFlashBase = adr;
  gFlashSize = (M32(FLASHSIZE_BASE) & 0x0000FFFF) << 10;
#endif /* FLASH_MEM */

#if defined FLASH_OPT
  (void)adr;

  /* unlock FLASH_NSCR */
  FLASH->NSKEYR = FLASH_KEY1;
  FLASH->NSKEYR = FLASH_KEY2;
  DSB();
  while (FLASH->NSSR & FLASH_SR_BSY) NOP();              /* Wait until operation is finished */

  /* Unlock Option Bytes operation */
  FLASH->OPTKEYR = FLASH_OPTKEY1;
  FLASH->OPTKEYR = FLASH_OPTKEY2;
  DSB();
  while (FLASH->NSSR & FLASH_SR_BSY) NOP();              /* Wait until operation is finished */

  /* load option bytes */
//FLASH->NSCR = FLASH_CR_OBL_LAUNCH;
//DSB();
//while (FLASH->NSCR & FLASH_CR_OBL_LAUNCH);             /* Wait until option bytes are updated */
#endif /* FLASH_OPT */

  return (0);
}


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc)
{
  (void)fnc;

#if defined FLASH_MEM
  /* Lock Flash operation */
  *pFlashCR = FLASH_CR_LOCK;
  DSB();
  while (*pFlashSR & FLASH_SR_BSY) NOP();                /* Wait until operation is finished */
#endif /* FLASH_MEM */

#if defined FLASH_OPT
  /* Lock option bytes operation */
  FLASH->NSCR = FLASH_CR_OPTLOCK;
  DSB();
  while (FLASH->NSCR & FLASH_SR_BSY) NOP();              /* Wait until operation is finished */

  /* Load option bytes */
//FLASH->NSCR  = FLASH_CR_OBL_LAUNCH;
//DSB();
//while (FLASH->NSCR & FLASH_CR_OBL_LAUNCH);             /* Wait until option bytes are updated */

  /* Lock FLASH CR */
  FLASH->NSCR = FLASH_CR_LOCK;
  DSB();
  while (FLASH->NSCR & FLASH_SR_BSY) NOP();              /* Wait until operation is finished */
#endif /* FLASH_OPT */

  return (0);
}


/*
 *  Blank Check Checks if Memory is Blank
 *    Parameter:      adr:  Block Start Address
 *                    sz:   Block Size (in bytes)
 *                    pat:  Block Pattern
 *    Return Value:   0 - OK,  1 - Failed
 */

int BlankCheck (unsigned long adr, unsigned long sz, unsigned char pat) {
  /* For OPT algorithm Flash is always erased */

  (void)adr;
  (void)sz;
  (void)pat;

#if defined FLASH_MEM
  /* force erase even if the content is 'Initial Content of Erased Memory'.
     Only a erased sector can be programmed. I think this is because of ECC */
  return (1);
#endif /* FLASH_MEM */

#if defined FLASH_OPT
  /* For OPT algorithm Flash is always erased */
  return (0);
#endif /* FLASH_OPT */
}


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

#if defined FLASH_MEM
int EraseChip (void)
{
  *pFlashSR = FLASH_PGERR;                               /* Reset Error Flags */

  *pFlashCR  = (FLASH_CR_MER1 | FLASH_CR_MER2);          /* Bank A/B mass erase enabled */
  *pFlashCR |=  FLASH_CR_STRT;                           /* Start erase */
  DSB();

  while (*pFlashSR & FLASH_SR_BSY) NOP();                /* Wait until operation is finished */

  return (0);                                            /* Done */
}
#endif /* FLASH_MEM */

#ifdef FLASH_OPT
int EraseChip (void) {

  /* not yet coded */
  return (0);                                            /* Done */
}
#endif /* FLASH_OPT */


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

#if defined FLASH_MEM
int EraseSector (unsigned long adr)
{
  u32 b, p;

  b = GetFlashBankNum(adr);                              /* Get Bank Number 0..1  */
  p = GetFlashPageNum(adr);                              /* Get Page Number 0..127 */

  while (*pFlashSR & FLASH_SR_BSY) NOP();                /* Wait until operation is finished */

  *pFlashSR  = FLASH_PGERR;                              /* Reset Error Flags */

  *pFlashCR  = (FLASH_CR_PER |                           /* Page Erase Enabled */
                 (p <<  3)   |                           /* page Number. 0 to 127 for each bank */
                 (b << 11)    );
  *pFlashCR |=  FLASH_CR_STRT;                           /* Start Erase */
  DSB();

  while (*pFlashSR & FLASH_SR_BSY) NOP();                /* Wait until operation is finished */

  if (*pFlashSR & FLASH_PGERR) {                         /* Check for Error */
    *pFlashSR  = FLASH_PGERR;                            /* Reset Error Flags */
    return (1);                                          /* Failed */
  }

  return (0);                                            /* Done */
}
#endif /* FLASH_MEM */

#if defined FLASH_OPT
int EraseSector (unsigned long adr) {
  /* erase sector is not needed for Flash Option bytes */

  (void)adr;

  return (0);                                              /* Done */
}
#endif /* FLASH_OPT */


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

#if defined FLASH_MEM
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf)
{

  sz = (sz + 15) & ~15U;                                 /* Adjust size for four words */

  while (*pFlashSR & FLASH_SR_BSY) NOP();                /* Wait until operation is finished */

  *pFlashSR = FLASH_PGERR;                               /* Reset Error Flags */

  *pFlashCR = FLASH_CR_PG ;	                             /* Programming Enabled */

  while (sz)
  {
//    M32(adr    ) = *((u32 *)(buf + 0));                  /* Program the 1st word of the quad-word */
//    M32(adr + 4) = *((u32 *)(buf + 4));                  /* Program the 2nd word of the quad-word */
//    M32(adr + 8) = *((u32 *)(buf + 8));                  /* Program the 3rd word of the quad-word */
//    M32(adr +12) = *((u32 *)(buf +12));                  /* Program the 4th word of the quad-word */

    M32(adr    ) = (u32)((*(buf+ 0)      ) |
                         (*(buf+ 1) <<  8) |
                         (*(buf+ 2) << 16) |
                         (*(buf+ 3) << 24) );            /* Program the 1st word of the quad-word */
    M32(adr + 4) = (u32)((*(buf+ 4)      ) |
                         (*(buf+ 5) <<  8) |
                         (*(buf+ 6) << 16) |
                         (*(buf+ 7) << 24) );            /* Program the 2nd word of the quad-word */
    M32(adr + 8) = (u32)((*(buf+ 8)      ) |
                         (*(buf+ 9) <<  8) |
                         (*(buf+10) << 16) |
                         (*(buf+11) << 24) );            /* Program the 3rd word of the quad-word */
    M32(adr +12) = (u32)((*(buf+12)      ) |
                         (*(buf+13) <<  8) |
                         (*(buf+14) << 16) |
                         (*(buf+15) << 24) );            /* Program the 4th word of the quad-word */
    DSB();

    while (*pFlashSR & FLASH_SR_BSY) NOP();              /* Wait until operation is finished */

    if (*pFlashSR & FLASH_PGERR) {                       /* Check for Error */
      *pFlashSR  = FLASH_PGERR;                          /* Reset Error Flags */
      return (1);                                        /* Failed */
    }

    adr += 16;                                           /* Next quad-word */
    buf += 16;
    sz  -= 16;
  }

  *pFlashCR = 0U;                                       /* Reset CR */

  return (0);
}
#endif /* FLASH_MEM */


#ifdef FLASH_OPT
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf)
{
  (void)adr;
  (void)sz;

  /* not yet coded */
  return (0);                                            /* Done */
}
#endif /* FLASH_OPT */


/*
 *  Verify Flash Contents
 *    Parameter:      adr:  Start Address
 *                    sz:   Size (in bytes)
 *                    buf:  Data
 *    Return Value:   (adr+sz) - OK, Failed Address
 */

#ifdef FLASH_OPT
unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf)
{
  (void)adr;
  (void)sz;

  /* not yet coded */
  return (adr + sz);
}
#endif /* FLASH_OPT */
