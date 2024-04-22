/* -----------------------------------------------------------------------------
 * Copyright (c) 2014 - 2018 ARM Ltd.
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
 * $Date:        10. September 2021
 * $Revision:    V1.0.0
 *
 * Project:      Flash Programming Functions for ST STM32C0x Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.0.0
 *    Initial release
 */

#include "..\FlashOS.h"        // FlashOS Structures

typedef volatile unsigned long    vu32;
typedef          unsigned long     u32;

#define M32(adr) (*((vu32 *) (adr)))


// Peripheral Memory Map
#define FLASH_BASE        0x40022000
#define IWDG_BASE         0x40003000
#define WWDG_BASE         0x40002C00

#define FLASH           ((FLASH_TypeDef  *) FLASH_BASE)
#define IWDG                ((IWDG_TypeDef *) IWDG_BASE)
#define WWDG                ((WWDG_TypeDef *) WWDG_BASE)
// Flash Registers
typedef struct
{
  vu32  ACR;              /*!< FLASH access control register,            Address offset: 0x00 */
  vu32  PDKEYR;           /*!< FLASH power down key register,            Address offset: 0x04 */
  vu32  KEYR;             /*!< FLASH key register,                       Address offset: 0x08 */
  vu32  OPTKEYR;          /*!< FLASH option key register,                Address offset: 0x0C */
  vu32  SR;               /*!< FLASH status register,                    Address offset: 0x10 */
  vu32  CR;               /*!< FLASH control register,                   Address offset: 0x14 */
  vu32  ECCR;             /*!< FLASH ECC register,                       Address offset: 0x18 */
  vu32  RESERVED1;        /*!< Reserved1,                                Address offset: 0x1C */
  vu32  OPTR;             /*!< FLASH option register,                    Address offset: 0x20 */
  vu32  PCROP1SR;         /*!< FLASH bank1 PCROP start address register, Address offset: 0x24 */
  vu32  PCROP1ER;         /*!< FLASH bank1 PCROP end address register,   Address offset: 0x28 */
  vu32  WRP1AR;           /*!< FLASH bank1 WRP area A address register,  Address offset: 0x2C */
  vu32  WRP1BR;           /*!< FLASH bank1 WRP area B address register,  Address offset: 0x30 */
  vu32  RESERVED2[4];     /*!< Reserved2,                                Address offset: 0x34 */
  vu32  PCROP2SR;         /*!< FLASH bank2 PCROP start address register, Address offset: 0x44 */
  vu32  PCROP2ER;         /*!< FLASH bank2 PCROP end address register,   Address offset: 0x48 */
  vu32  WRP2AR;           /*!< FLASH bank2 WRP area A address register,  Address offset: 0x4C */
  vu32  WRP2BR;           /*!< FLASH bank2 WRP area B address register,  Address offset: 0x50 */
} FLASH_TypeDef;

typedef struct
{
  vu32  KR;   /*!< IWDG Key register,       Address offset: 0x00 */
  vu32  PR;   /*!< IWDG Prescaler register, Address offset: 0x04 */
  vu32  RLR;  /*!< IWDG Reload register,    Address offset: 0x08 */
  vu32  SR;   /*!< IWDG Status register,    Address offset: 0x0C */
  vu32  WINR; /*!< IWDG Window register,    Address offset: 0x10 */
} IWDG_TypeDef;

typedef struct
{
   vu32 CR;   /*!< WWDG Control register,       Address offset: 0x00 */
   vu32 CFR;  /*!< WWDG Configuration register, Address offset: 0x04 */
   vu32 SR;   /*!< WWDG Status register,        Address offset: 0x08 */
} WWDG_TypeDef;

// Flash Keys
#define FLASH_KEY1               0x45670123
#define FLASH_KEY2               0xCDEF89AB
#define FLASH_OPTKEY1            0x08192A3B
#define FLASH_OPTKEY2            0x4C5D6E7F

// Flash Control Register definitions
#define FLASH_CR_PG                          ((unsigned int)0x00000001)
#define FLASH_CR_PER                         ((unsigned int)0x00000002)
#define FLASH_CR_MER1                        ((unsigned int)0x00000004)
#define FLASH_CR_PNB                         ((unsigned int)0x000007F8)
#define FLASH_CR_STRT                        ((unsigned int)0x00010000)
#define FLASH_CR_LOCK                        ((unsigned int)0x80000000)
// Flash OPTION Control Register definitions
#define FLASH_OPTCR_IWDG_SW                 ((unsigned int)0x00010000)
// Flash Status Register definitions
#define FLASH_SR_EOP                         ((unsigned int)0x00000001)
#define FLASH_SR_OPERR                       ((unsigned int)0x00000002)
#define FLASH_SR_PROGERR                     ((unsigned int)0x00000008)
#define FLASH_SR_WRPERR                      ((unsigned int)0x00000010)
#define FLASH_SR_PGAERR                      ((unsigned int)0x00000020)
#define FLASH_SR_SIZERR                      ((unsigned int)0x00000040)
#define FLASH_SR_PGSERR                      ((unsigned int)0x00000080)
#define FLASH_SR_MISSERR                     ((unsigned int)0x00000100)
#define FLASH_SR_FASTERR                     ((unsigned int)0x00000200)
#define FLASH_SR_RDERR                       ((unsigned int)0x00004000)
#define FLASH_SR_OPTVERR                     ((unsigned int)0x00008000)
#define FLASH_SR_BSY                         ((unsigned int)0x00010000)

#define FLASH_PGERR             (FLASH_SR_OPERR  | FLASH_SR_PROGERR | FLASH_SR_WRPERR | FLASH_SR_PGAERR  | FLASH_SR_SIZERR | \
                                 FLASH_SR_PGSERR | FLASH_SR_MISSERR  | FLASH_SR_FASTERR | FLASH_SR_RDERR | FLASH_SR_OPTVERR   )



/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {
	
	__disable_irq();
  /*Clear the error status*/
  FLASH->SR =FLASH_PGERR;
	 // Test if IWDG is running (IWDG in HW mode)
  if ((FLASH->OPTR & FLASH_OPTCR_IWDG_SW) == 0x00) 
  {
    // Set IWDG time out to ~32.768 second
    IWDG->KR  = 0xAAAA; 
    IWDG->KR  = 0x5555;                         // Enable write access to IWDG_PR and IWDG_RLR     
    IWDG->PR  = 0x06;                           // Set prescaler to 256  
    IWDG->RLR = 4095;                           // Set reload value to 4095
    WWDG->CFR = 0x1FF;
    WWDG->CR = 0x7F;
  }   
 if((FLASH->CR & FLASH_CR_LOCK) != 0)
  {
    /* Authorize the FLASH Registers access */
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
  }  
	
  return (0);                                           // Done
}


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) {

  FLASH->CR |= FLASH_CR_LOCK;                              // Lock Flash

  return (0);                                           // Done
}


/*
 *  Blank Check Checks if Memory is Blank
 *    Parameter:      adr:  Block Start Address
 *                    sz:   Block Size (in bytes)
 *                    pat:  Block Pattern
 *    Return Value:   0 - OK,  1 - Failed
 */

int BlankCheck (unsigned long adr, unsigned long sz, unsigned char pat) {

  /* force erase even if the content is 'Initial Content of Erased Memory'.
     Only a erased sector can be programmed. I think this is because of ECC */
  return (1);
}


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseChip (void) {
__disable_irq();
  while (FLASH->SR & FLASH_SR_BSY) {
  }


  
  FLASH->CR |=  FLASH_CR_MER1 ; 
  FLASH->CR |=  FLASH_CR_STRT;                               // Start Erase
  
  while (FLASH->SR & FLASH_SR_BSY) {
  }



  if (FLASH->SR & FLASH_PGERR) {                          // Check for Error
    FLASH->SR  = FLASH_PGERR;                             // Reset Error Flags
    return (1);                                           // Failed
  }
	
		  FLASH->CR &= (~FLASH_CR_MER1);


  return (0);                                           // Done
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */


int EraseSector (unsigned long adr) {
  unsigned long page;

__disable_irq();
  /*Clear the error status*/
    FLASH->SR =FLASH_PGERR;
	 
  /*Clear page and block bits*/
  FLASH->CR &= ~(FLASH_CR_PNB);

	
  /*Calculate the page and set
    the block bit if needed*/

    page = ((adr-0x8000000) >>8) ;
 
  /*Set the page and the Page erase bit*/
  FLASH->CR |= (FLASH_CR_PER | page);
  /*Start the erase operation*/  
  FLASH->CR |= FLASH_CR_STRT;	

  /*wait until the operation ends*/
  while (FLASH->SR & FLASH_SR_BSY);

	if (FLASH->SR & FLASH_PGERR) {                          // Check for Error
    FLASH->SR  = FLASH_PGERR;                             // Reset Error Flags
    return (1);                                           // Failed
  }
	
	 FLASH->CR &= (~FLASH_CR_PER);
	  if ((* (unsigned long *)0x08000000) == 0xFFFFFFFF)          //empty check
  {
    FLASH->ACR |= 0x10000;
  }
	
  return (0);                                           // Done
}





/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {

	__disable_irq();
  sz = (sz + 7) & ~7;  
unsigned long tab[8];
	int i;
	
    while (FLASH->SR & FLASH_SR_BSY) {};
    FLASH->SR = 0x000001FF;                               // Reset Error Flags
		FLASH->CR |= FLASH_CR_PG ;	                              // Programming Enabled

  while (sz) {
    
		if(sz>=8){
      M32(adr) = *((u32 *)(buf+0 ));                   // Program the first word of the Double Word
			M32(adr+4) = *((u32 *)(buf+4 )); 
			adr += 8;                                           // Go to next DoubleWord
    buf += 8;
    sz  -= 8;
      while (FLASH->SR & FLASH_SR_BSY);
		
	 }
else{
	     for(i=0;i<sz;i++)
        {
          tab[i]= *((unsigned char *)buf);
          buf=buf+1;
        }
        
        for(i=0;i<8-sz;i++)
        {
          tab[i+sz]=0xFF;
        }
 
     
       
       /*wait until the operation ends*/
       while (FLASH->SR & FLASH_SR_BSY){};
     
                  
          M32(adr) = *((u32 *)(&tab+0 ));   
		    M32(adr+4) = *((u32 *)(&tab+4 )); 
         sz =0;
	
}
    if ((FLASH->SR & FLASH_SR_EOP)) {                        // Check for Error
      FLASH->SR  = FLASH_PGERR;                           // Reset Error Flags
      return (1);                                         // Failed
    }
		
 
  }
		FLASH->CR &= (~FLASH_CR_PG) ;	  
if ((* (unsigned long *)0x08000000) != 0xFFFFFFFF)  //empty check
  {
    FLASH->ACR &= 0xFFFEFFFF;
  }  
	

	
	
  return (0);                                           // Done

}
