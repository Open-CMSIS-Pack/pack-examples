/**
  ******************************************************************************
  * @file    stm32u5xx_hal_pwr_ex.c
  * @author  MCD Application Team
  * @brief   Extended PWR HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Power Controller extension peripheral :
  *           + Power Supply Control Functions
  *           + Low Power Control Functions
  *           + Voltage Monitoring Functions
  *           + Memories Retention Functions
  *           + I/O Pull-Up Pull-Down Configuration Functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"

/** @addtogroup STM32U5xx_HAL_Driver
  * @{
  */

/** @defgroup PWREx PWREx
  * @brief    PWR Extended HAL module driver
  * @{
  */

#if defined (HAL_PWR_MODULE_ENABLED)

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/** @defgroup PWR_Extended_Private_Defines PWR Extended Private Defines
  * @{
  */
/* PORTI pins mask */
#if defined(GPIOJ)
#define PWR_PORTI_AVAILABLE_PINS  (0xFFFFU)
#define PWR_PORTJ_AVAILABLE_PINS  (0x0FFFU)
#else
#define PWR_PORTI_AVAILABLE_PINS  (0xFFU)
#endif
/*!< Time out value of flags setting */
#define PWR_FLAG_SETTING_DELAY    (0x32U)

/** @defgroup PWR_PVM_Mode_Mask PWR PVM Mode Mask
  * @{
  */
#define PVM_RISING_EDGE  (0x01U)  /*!< Mask for rising edge set as PVM
                                       trigger                                */
#define PVM_FALLING_EDGE (0x02U)  /*!< Mask for falling edge set as PVM
                                       trigger                                */
#define PVM_MODE_IT      (0x04U)  /*!< Mask for interruption yielded by PVM
                                       threshold crossing                     */
#define PVM_MODE_EVT     (0x08U)  /*!< Mask for event yielded by PVM threshold
                                       crossing                               */
/**
  * @}
  */

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup PWREx_Exported_Functions PWR Extended Exported Functions
  * @{
  */

/** @defgroup PWREx_Exported_Functions_Group1 Power Supply Control Functions
  * @brief    Power supply control functions
  *
@verbatim
 ===============================================================================
                  ##### Power supply control functions #####
 ===============================================================================
    [..]
@endverbatim
  * @{
  */

/**
  * @brief  Configure the main internal regulator output voltage to achieve
  *         a tradeoff between performance and power consumption.
  * @param  VoltageScaling : Specifies the regulator output voltage scale.
  *                          This parameter can be one of the following values :
  *                          @arg @ref PWR_REGULATOR_VOLTAGE_SCALE1 : Regulator voltage output scale 1.
  *                                                                   Provides a typical output voltage at 1.2 V.
  *                                                                   Used when system clock frequency is up to 160 MHz.
  *                          @arg @ref PWR_REGULATOR_VOLTAGE_SCALE2 : Regulator voltage output scale 2.
  *                                                                   Provides a typical output voltage at 1.1 V.
  *                                                                   Used when system clock frequency is up to 100 MHz.
  *                          @arg @ref PWR_REGULATOR_VOLTAGE_SCALE3 : Regulator voltage output scale 3.
  *                                                                   Provides a typical output voltage at 1.0 V.
  *                                                                   Used when system clock frequency is up to 50 MHz.
  *                          @arg @ref PWR_REGULATOR_VOLTAGE_SCALE4 : Regulator voltage output scale 4.
  *                                                                   Provides a typical output voltage at 0.9 V.
  *                                                                   Used when system clock frequency is up to 24 MHz.
  * @note  Before moving to voltage scaling 2, it is mandatory to ensure that
  *        the system frequency is between 50 MHz and 100 MHz.
  * @note  Before moving to voltage scaling 3, it is mandatory to ensure that
  *        the system frequency is between 24 MHz and 50 MHz.
  * @note  Before moving to voltage scaling 4, it is mandatory to ensure that
  *        the system frequency is below 24 MHz.
  * @retval HAL Status.
  */
HAL_StatusTypeDef HAL_PWREx_ControlVoltageScaling(uint32_t VoltageScaling)
{
  uint32_t timeout = 0;

  /* Check the parameter */
  assert_param(IS_PWR_VOLTAGE_SCALING_RANGE(VoltageScaling));

  /* Get the current voltage scale applied */
  uint32_t vos_old = READ_BIT(PWR->SVMSR, PWR_SVMSR_ACTVOS);

  /* No change, nothing to do */
  if (vos_old == VoltageScaling)
  {
    return HAL_OK;
  }

  /* Check voltage scaling level */
  /*
     The Embedded power distribution (EPOD) must be enabled before switching to
     voltage scale 1 / 2 from voltage scale lower.
  */
  if (VoltageScaling > PWR_REGULATOR_VOLTAGE_SCALE3)
  {
    MODIFY_REG(PWR->VOSR, (PWR_VOSR_VOS | PWR_VOSR_BOOSTEN), (VoltageScaling | PWR_VOSR_BOOSTEN));
  }
  else
  {
    MODIFY_REG(PWR->VOSR, (PWR_VOSR_VOS | PWR_VOSR_BOOSTEN), VoltageScaling);
  }

  /* Wait until VOSRDY is rised */
  timeout = (PWR_FLAG_SETTING_DELAY * (SystemCoreClock / 1000000));
  while ((timeout != 0) && ((HAL_IS_BIT_CLR(PWR->SVMSR, PWR_SVMSR_ACTVOSRDY)) || \
         (HAL_IS_BIT_CLR(PWR->VOSR, PWR_VOSR_VOSRDY))))
  {
    timeout--;
  }

  /* Check time out  */
  if (timeout == 0U)
  {
    return HAL_TIMEOUT;
  }

  return HAL_OK;
}

/**
  * @brief  Return Voltage Scaling Range.
  * @retval Applied voltage scaling value.
  */
uint32_t HAL_PWREx_GetVoltageRange(void)
{
  return  (PWR->SVMSR & PWR_SVMSR_ACTVOS);
}

/**
  * @brief  Configure the system Power Supply.
  * @param  SupplySource : Specifies the Power Supply source to set after a
  *                        system startup.
  *                        This parameter can be one of the following values :
  *                        @arg PWR_LDO_SUPPLY  : The LDO regulator supplies the Vcore Power Domains.
  *                        @arg PWR_SMPS_SUPPLY : The SMPS regulator supplies the Vcore Power Domains.
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_PWREx_ConfigSupply(uint32_t SupplySource)
{
  uint32_t timeout = 0U;

  /* Check the parameter */
  assert_param(IS_PWR_SUPPLY(SupplySource));

  /* Set maximum time out */
  timeout = (PWR_FLAG_SETTING_DELAY * (SystemCoreClock / 1000000));

  /* Configure the LDO as system regulator supply  */
  if (SupplySource == PWR_LDO_SUPPLY)
  {
    /* Set the power supply configuration */
    CLEAR_BIT(PWR->CR3, PWR_CR3_REGSEL);

    /* Wait until system switch on new regulator */
    while ((timeout != 0) && (HAL_IS_BIT_SET(PWR->SVMSR, PWR_SVMSR_REGS) != 0U))
    {
      timeout--;
    }
  }
  /* Configure the SMPS as system regulator supply  */
  else
  {
    /* Set the power supply configuration */
    SET_BIT(PWR->CR3, PWR_CR3_REGSEL);

    /* Wait until system switch on new regulator */
    while ((timeout != 0) && (HAL_IS_BIT_CLR(PWR->SVMSR, PWR_SVMSR_REGS) != 0U))
    {
      timeout--;
    }
  }

  /* Check time out  */
  if (timeout == 0U)
  {
    return HAL_TIMEOUT;
  }

  return HAL_OK;
}

/**
  * @brief  Get the power supply configuration.
  * @retval The supply configured.
  */
uint32_t  HAL_PWREx_GetSupplyConfig(void)
{
  return (PWR->SVMSR & PWR_SVMSR_REGS);
}

/**
  * @brief  Enable fast soft start for the current regulator.
  * @retval None.
  */
void HAL_PWREx_EnableFastSoftStart(void)
{
  SET_BIT(PWR->CR3, PWR_CR3_FSTEN);
}

/**
  * @brief  Disable fast soft start for the current regulator.
  * @retval None.
  */
void HAL_PWREx_DisableFastSoftStart(void)
{
  CLEAR_BIT(PWR->CR3, PWR_CR3_FSTEN);
}
/**
  * @}
  */

#if defined(PWR_CR2_FPWM)
/**
  * @brief  Enable SMPS PWM mode (Range 1, 2, 3 only)
  * @note   This bit is used to reduce the SMPS switching harmonics in range 1, 2 or 3. Setting this bit has no effect in Range 4.
  * @retval None.
  */
void HAL_PWREx_EnableSMPSPWMMode(void)
{
  SET_BIT(PWR->CR2, PWR_CR2_FPWM);
}

/**
  * @brief  Disable SMPS PWM mode (Range 1, 2, 3 only)
  * @retval None.
  */
void HAL_PWREx_DisableSMPSPWMMode(void)
{
  CLEAR_BIT(PWR->CR2, PWR_CR2_FPWM);
}
/**
  * @}
  */
#endif

/** @defgroup PWREx_Exported_Functions_Group2 Low Power Control Functions
  * @brief    Low power control functions
  *
@verbatim
 ===============================================================================
                     ##### Low power control functions #####
 ===============================================================================
    [..]
@endverbatim
  * @{
  */

/**
  * @brief  Enter the whole system to STOP 1 mode.
  * @note   STOP 1 offers the largest number of active peripherals and wakeup
  *         sources, a smaller wakeup time but a higher consumption.
  * @note   STOP mode achieves the lowest power consumption while retaining
  *         the content of SRAM and registers. All clocks in the VCORE domain
  *         are stopped. The PLL, the MSI (MSIS and MSIK) RC, the HSI16 RC and
  *         the HSE crystal oscillators are disabled. The LSE or LSI is still
  *         running.
  * @note   The system clock when exiting from Stop mode can be either MSIS up
  *         to 24 MHz or HSI16, depending on software configuration.
  * @param  STOPEntry : Specifies if STOP mode is entered with WFI or WFE
  *                     instruction.
  *                     This parameter can be one of the following values :
  *                     @arg @ref PWR_STOPENTRY_WFI enter STOP mode with Wait
  *                               For Interrupt request.
  *                     @arg @ref PWR_STOPENTRY_WFE enter STOP mode with Wait
  *                               For Event request.
  * @retval None.
  */
void HAL_PWREx_EnterSTOP1Mode(uint8_t STOPEntry)
{
  /* Check the parameters */
  assert_param(IS_PWR_STOP_ENTRY(STOPEntry));

  /* Stop 1 mode */
  MODIFY_REG(PWR->CR1, PWR_CR1_LPMS, PWR_CR1_LPMS_0);

  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

  /* Select Stop mode entry */
  if(STOPEntry == PWR_STOPENTRY_WFI)
  {
    /* Request Wait For Interrupt */
    __WFI();
  }
  else
  {
    /* Request Wait For Event */
    __SEV();
    __WFE();
    __WFE();
  }

  /* Reset SLEEPDEEP bit of Cortex System Control Register */
  CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
}

/**
  * @brief  Enter the whole system to STOP 2 mode.
  * @note   In Stop 2 mode, all clocks in the VCORE domain are stopped. The PLL,
  *         MSIS, MSIK, HSI16 and HSE oscillators are disabled.
  * @note   STOP mode achieves the lowest power consumption while retaining
  *         the content of SRAM and registers. All clocks in the VCORE domain
  *         are stopped. The PLL, the MSI (MSIS and MSIK) RC, the HSI16 RC and
  *         the HSE crystal oscillators are disabled. The LSE or LSI is still
  *         running.
  * @note   The system clock when exiting from Stop mode can be either MSIS up
  *         to 24 MHz or HSI16, depending on software configuration.
  * @param  STOPEntry : Specifies if STOP mode is entered with WFI or WFE
  *                     instruction.
  *                     This parameter can be one of the following values :
  *                     @arg @ref PWR_STOPENTRY_WFI enter STOP mode with Wait
  *                               For Interrupt request.
  *                     @arg @ref PWR_STOPENTRY_WFE enter STOP mode with Wait
  *                               For Event request.
  * @retval None.
  */
void HAL_PWREx_EnterSTOP2Mode(uint8_t STOPEntry)
{
  /* Check the parameters */
  assert_param(IS_PWR_STOP_ENTRY(STOPEntry));

  /* Stop 2 mode */
  MODIFY_REG(PWR->CR1, PWR_CR1_LPMS, PWR_CR1_LPMS_1);

  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

  /* Select Stop mode entry */
  if(STOPEntry == PWR_STOPENTRY_WFI)
  {
    /* Request Wait For Interrupt */
    __WFI();
  }
  else
  {
    /* Request Wait For Event */
    __SEV();
    __WFE();
    __WFE();
  }

  /* Reset SLEEPDEEP bit of Cortex System Control Register */
  CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
}

/**
  * @brief  Enter the whole system to Stop 3 mode.
  * @note   Stop 3 is the lowest power mode with full retention, but the
  *         functional peripherals and sources of wakeup are reduced to the same
  *         ones than in Standby mode.
  * @note   STOP mode achieves the lowest power consumption while retaining
  *         the content of SRAM and registers. All clocks in the VCORE domain
  *         are stopped. The PLL, the MSI (MSIS and MSIK) RC, the HSI16 RC and
  *         the HSE crystal oscillators are disabled. The LSE or LSI is still
  *         running.
  * @note   The system clock when exiting from Stop mode can be either MSIS up
  *         to 24 MHz or HSI16, depending on software configuration.
  * @param  STOPEntry : Specifies if STOP mode is entered with WFI or WFE
  *                     instruction.
  *                     This parameter can be one of the following values :
  *                     @arg @ref PWR_STOPENTRY_WFI enter STOP mode with Wait
  *                               For Interrupt request.
  *                     @arg @ref PWR_STOPENTRY_WFE enter STOP mode with Wait
  *                               For Event request.
  * @retval None.
  */
void HAL_PWREx_EnterSTOP3Mode(uint8_t STOPEntry)
{
  /* Check the parameter */
  assert_param(IS_PWR_STOP_ENTRY(STOPEntry));

  /* Set Stop mode 3 */
  MODIFY_REG(PWR->CR1, PWR_CR1_LPMS, (PWR_CR1_LPMS_0 | PWR_CR1_LPMS_1));

  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

  /* Select Stop mode entry */
  if(STOPEntry == PWR_STOPENTRY_WFI)
  {
    /* Request Wait For Interrupt */
    __WFI();
  }
  else
  {
    /* Request Wait For Event */
    __SEV();
    __WFE();
    __WFE();
  }

  /* Reset SLEEPDEEP bit of Cortex System Control Register */
  CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
}

/**
  * @brief  Enter the whole system to Shutdown mode.
  * @note   Shutdown mode allows the lowest power consumption. The internal
  *         regulator is switched off so that the VCORE domain is powered off.
  *         The PLL, the HSI16, the MSI (MSIS and MSIK), the LSI and the HSE
  *         oscillators are also switched off.
  * @note   The SRAMs and register contents are lost except for registers in the
  *         Backup domain. The BOR is not available in Shutdown mode. No power
  *         voltage monitoring is possible in this mode, therefore the switch to
  *         Backup domain is not supported.
  * @retval None.
  */
void HAL_PWREx_EnterSHUTDOWNMode(void)
{
  /* Set Shutdown mode */
  MODIFY_REG(PWR->CR1, PWR_CR1_LPMS, (PWR_CR1_LPMS_1 | PWR_CR1_LPMS_2));

  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

  /* This option is used to ensure that store operations are completed */
#if defined (__CC_ARM)
  __force_stores();
#endif
  /* Request Wait For Interrupt */
  __WFI();
}

/**
  * @brief  Configure the SRD domain when the System in STOP 0/1/2 mode.
  * @param  SRDState : Specifies the SRD state.
  *                    This parameter can be one of the following values :
  *                    @arg PWR_SRD_DOMAIN_STOP : SRD in STOP mode when system
  *                                               goes to STOP 0/1/2 mode
  *                    @arg PWR_SRD_DOMAIN_RUN  : SRD in RUN mode when system
  *                                               goes to STOP 0/1/2 mode
  * @retval None.
  */
void HAL_PWREx_ConfigSRDDomain(uint32_t SRDState)
{
  /* Check the parameter */
  assert_param (IS_PWR_SRD_STATE (SRDState));

  /* Config the SRD domain */
  MODIFY_REG (PWR->CR2, PWR_CR2_SRDRUN, SRDState);
}

/**
  * @brief  Enable BOR ultra-low power mode.
  * @note   BOR operating can be in discontinuous (ultra-low power) mode in
  *         Stop 1, Stop 2, Stop 3 and Standby modes and when the regulator is
  *         in Range 4 (Run, Sleep or Stop 0 mode).
  * @retval None.
  */
void HAL_PWREx_EnableUltraLowPowerMode(void)
{
  SET_BIT(PWR->CR1, PWR_CR1_ULPMEN);
}

/**
  * @brief  Disable BOR ultra-low power mode.
  * @retval None.
  */
void HAL_PWREx_DisableUltraLowPowerMode(void)
{
  CLEAR_BIT(PWR->CR1, PWR_CR1_ULPMEN);
}

/**
  * @brief  This function handles the PWR Wake up from STOP3 interrupt request.
  * @note   This API should be called under the PWR_S3WU_IRQHandler().
  * @retval None.
  */
void HAL_PWREx_S3WU_IRQHandler(uint32_t WakeUpPin)
{
  /* Check PWR wake up line 1 */
  if ((WakeUpPin & PWR_WAKEUP_PIN1) == PWR_WAKEUP_PIN1)
  {
    /* Clear PWR wake up flag line 1 */
    SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF1);

    /* PWR S3WU interrupt user callback */
    HAL_PWREx_S3WUCallback(WakeUpPin);
  }

  /* Check PWR wake up line 2 */
  if ((WakeUpPin & PWR_WAKEUP_PIN2) == PWR_WAKEUP_PIN2)
  {
    /* Clear PWR wake up flag line 2 */
    SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF2);

    /* PWR S3WU interrupt user callback */
    HAL_PWREx_S3WUCallback(WakeUpPin);
  }

  /* Check PWR wake up line 3 */
  if ((WakeUpPin & PWR_WAKEUP_PIN3) == PWR_WAKEUP_PIN3)
  {
    /* Clear PWR wake up flag line 3 */
    SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF3);

    /* PWR S3WU interrupt user callback */
    HAL_PWREx_S3WUCallback(WakeUpPin);
  }

  /* Check PWR wake up line 4 */
  if ((WakeUpPin & PWR_WAKEUP_PIN4) == PWR_WAKEUP_PIN4)
  {
    /* Clear PWR wake up flag line 4 */
    SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF4);

    /* PWR S3WU interrupt user callback */
    HAL_PWREx_S3WUCallback(WakeUpPin);
  }

  /* Check PWR wake up line 5 */
  if ((WakeUpPin & PWR_WAKEUP_PIN5) == PWR_WAKEUP_PIN5)
  {
    /* Clear PWR wake up flag line 5 */
    SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF5);

    /* PWR S3WU interrupt user callback */
    HAL_PWREx_S3WUCallback(WakeUpPin);
  }

  /* Check PWR wake up line 6 */
  if ((WakeUpPin & PWR_WAKEUP_PIN6) == PWR_WAKEUP_PIN6)
  {
    /* Clear PWR wake up flag line 6 */
    SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF6);

    /* PWR S3WU interrupt user callback */
    HAL_PWREx_S3WUCallback(WakeUpPin);
  }

  /* Check PWR wake up line 7 */
  if ((WakeUpPin & PWR_WAKEUP_PIN7) == PWR_WAKEUP_PIN7)
  {
    /* Clear PWR wake up flag line 7 */
    SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF7);

    /* PWR S3WU interrupt user callback */
    HAL_PWREx_S3WUCallback(WakeUpPin);
  }

  /* Check PWR wake up line 8 */
  if ((WakeUpPin & PWR_WAKEUP_PIN8) == PWR_WAKEUP_PIN8)
  {
    /* Clear PWR wake up flag line 8 */
    SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF8);

    /* PWR S3WU interrupt user callback */
    HAL_PWREx_S3WUCallback(WakeUpPin);
  }
}

/**
  * @brief  PWR S3WU interrupt callback.
  * @retval None.
  */
__weak void HAL_PWREx_S3WUCallback(uint32_t WakeUpPin)
{
  /* NOTE : This function should not be modified; when the callback is needed,
            HAL_PWREx_S3WU_IRQHandler() API can be implemented in the user file
   */
}
/**
  * @}
  */

/** @defgroup PWREx_Exported_Functions_Group3 Voltage Monitoring Functions
  * @brief    Voltage monitoring functions
  *
@verbatim
 ===============================================================================
                     ##### Voltage Monitoring Functions #####
 ===============================================================================
    [..]
@endverbatim
  * @{
  */

/**
  * @brief  Configure the voltage monitor threshold detected by the Peripheral
  *         voltage monitoring (PVM).
  * @param  sConfigPVM : Pointer to a PWR_PVMTypeDef structure that contains the
  *                      PVM configuration information (PVMType and EventMode).
  * @retval None.
  */
HAL_StatusTypeDef HAL_PWREx_ConfigPVM(PWR_PVMTypeDef *sConfigPVM)
{
  /* Check the parameters */
  assert_param(IS_PWR_PVM_TYPE(sConfigPVM->PVMType));
  assert_param(IS_PWR_PVM_MODE(sConfigPVM->Mode));

  /* Check the peripheral voltage monitor type */
  switch (sConfigPVM->PVMType)
  {
    case PWR_UVM: /* Independent USB voltage monitor */

      /* Disable EXTI UVM event and interrupt */
      __HAL_PWR_UVM_EXTI_DISABLE_EVENT();
      __HAL_PWR_UVM_EXTI_DISABLE_IT();
      __HAL_PWR_UVM_EXTI_DISABLE_RISING_EDGE();
      __HAL_PWR_UVM_EXTI_DISABLE_FALLING_EDGE();

      /* Configure the UVM in interrupt mode */
      if((sConfigPVM->Mode & PVM_MODE_IT) == PVM_MODE_IT)
      {
        __HAL_PWR_UVM_EXTI_ENABLE_IT();
      }

      /* Configure the UVM in event mode */
      if((sConfigPVM->Mode & PVM_MODE_EVT) == PVM_MODE_EVT)
      {
        __HAL_PWR_UVM_EXTI_ENABLE_EVENT();
      }

      /* Configure the UVM in rising edge */
      if((sConfigPVM->Mode & PVM_RISING_EDGE) == PVM_RISING_EDGE)
      {
        __HAL_PWR_UVM_EXTI_ENABLE_RISING_EDGE();
      }

      /* Configure the UVM in falling edge */
      if((sConfigPVM->Mode & PVM_FALLING_EDGE) == PVM_FALLING_EDGE)
      {
        __HAL_PWR_UVM_EXTI_ENABLE_FALLING_EDGE();
      }

      break;

    case PWR_IO2VM: /* Independent I/Os voltage monitor */

      /* Disable EXTI IO2VM event and interrupt */
      __HAL_PWR_IO2VM_EXTI_DISABLE_EVENT();
      __HAL_PWR_IO2VM_EXTI_DISABLE_IT();
      __HAL_PWR_IO2VM_EXTI_DISABLE_RISING_EDGE();
      __HAL_PWR_IO2VM_EXTI_DISABLE_FALLING_EDGE();

      /* Configure the IO2VM in interrupt mode */
      if((sConfigPVM->Mode & PVM_MODE_IT) == PVM_MODE_IT)
      {
        __HAL_PWR_IO2VM_EXTI_ENABLE_IT();
      }

      /* Configure the IO2VM in event mode */
      if((sConfigPVM->Mode & PVM_MODE_EVT) == PVM_MODE_EVT)
      {
        __HAL_PWR_IO2VM_EXTI_ENABLE_EVENT();
      }

      /* Configure the IO2VM in rising edge */
      if((sConfigPVM->Mode & PVM_RISING_EDGE) == PVM_RISING_EDGE)
      {
        __HAL_PWR_IO2VM_EXTI_ENABLE_RISING_EDGE();
      }

      /* Configure the IO2VM in falling edge */
      if((sConfigPVM->Mode & PVM_FALLING_EDGE) == PVM_FALLING_EDGE)
      {
        __HAL_PWR_IO2VM_EXTI_ENABLE_FALLING_EDGE();
      }

      break;

    case PWR_AVM1: /* VDDA Independent analog supply voltage monitor 1 (1.6V threshold) */

      /* Disable EXTI AVM1 event and interrupt */
      __HAL_PWR_AVM1_EXTI_DISABLE_EVENT();
      __HAL_PWR_AVM1_EXTI_DISABLE_IT();
      __HAL_PWR_AVM1_EXTI_DISABLE_RISING_EDGE();
      __HAL_PWR_AVM1_EXTI_DISABLE_FALLING_EDGE();

      /* Configure the AVM1 in interrupt mode */
      if((sConfigPVM->Mode & PVM_MODE_IT) == PVM_MODE_IT)
      {
        __HAL_PWR_AVM1_EXTI_ENABLE_IT();
      }

      /* Configure the AVM1 in event mode */
      if((sConfigPVM->Mode & PVM_MODE_EVT) == PVM_MODE_EVT)
      {
        __HAL_PWR_AVM1_EXTI_ENABLE_EVENT();
      }

      /* Configure the AVM1 in rising edge */
      if((sConfigPVM->Mode & PVM_RISING_EDGE) == PVM_RISING_EDGE)
      {
        __HAL_PWR_AVM1_EXTI_ENABLE_RISING_EDGE();
      }

      /* Configure the AVM1 in falling edge */
      if((sConfigPVM->Mode & PVM_FALLING_EDGE) == PVM_FALLING_EDGE)
      {
        __HAL_PWR_AVM1_EXTI_ENABLE_FALLING_EDGE();
      }

      break;

    case PWR_AVM2: /* VDDA Independent analog supply voltage monitor 1 (1.8V threshold) */

      /* Disable EXTI AVM2 event and interrupt */
      __HAL_PWR_AVM2_EXTI_DISABLE_EVENT();
      __HAL_PWR_AVM2_EXTI_DISABLE_IT();
      __HAL_PWR_AVM2_EXTI_DISABLE_RISING_EDGE();
      __HAL_PWR_AVM2_EXTI_DISABLE_FALLING_EDGE();

      /* Configure the AVM2 in interrupt mode */
      if((sConfigPVM->Mode & PVM_MODE_IT) == PVM_MODE_IT)
      {
        __HAL_PWR_AVM2_EXTI_ENABLE_IT();
      }

      /* Configure the AVM2 in event mode */
      if((sConfigPVM->Mode & PVM_MODE_EVT) == PVM_MODE_EVT)
      {
        __HAL_PWR_AVM2_EXTI_ENABLE_EVENT();
      }

      /* Configure the AVM2 in rising edge */
      if((sConfigPVM->Mode & PVM_RISING_EDGE) == PVM_RISING_EDGE)
      {
        __HAL_PWR_AVM2_EXTI_ENABLE_RISING_EDGE();
      }

      /* Configure the AVM2 in falling edge */
      if((sConfigPVM->Mode & PVM_FALLING_EDGE) == PVM_FALLING_EDGE)
      {
        __HAL_PWR_AVM2_EXTI_ENABLE_FALLING_EDGE();
      }

      break;

    default: /* No valid voltage monitor selected */

      return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  Enable VDDUSB supply.
  * @note   Remove VDDUSB electrical and logical isolation, once VDDUSB supply
  *         is present for consumption saving.
  * @retval None.
  */
void HAL_PWREx_EnableVddUSB(void)
{
  SET_BIT(PWR->SVMCR, PWR_SVMCR_USV);
}

/**
  * @brief  Disable VDDUSB supply.
  * @retval None.
  */
void HAL_PWREx_DisableVddUSB(void)
{
  CLEAR_BIT(PWR->SVMCR, PWR_SVMCR_USV);
}

/**
  * @brief  Enable VDDIO2 supply.
  * @note   Remove VDDIO2 electrical and logical isolation, once VDDIO2 supply
  *         is present for consumption saving.
  * @retval None.
  */
void HAL_PWREx_EnableVddIO2(void)
{
  SET_BIT(PWR->SVMCR, PWR_SVMCR_IO2SV);
}

/**
  * @brief  Disable VDDIO2 supply.
  * @retval None.
  */
void HAL_PWREx_DisableVddIO2(void)
{
  CLEAR_BIT(PWR->SVMCR, PWR_SVMCR_IO2SV);
}

/**
  * @brief  Enable VDDA supply.
  * @note   Remove VDDA electrical and logical isolation, once VDDA supply is
  *         present for consumption saving.
  * @retval None.
  */
void HAL_PWREx_EnableVddA(void)
{
  SET_BIT(PWR->SVMCR, PWR_SVMCR_ASV);
}

/**
  * @brief  Disable VDDA supply.
  * @retval None.
  */
void HAL_PWREx_DisableVddA(void)
{
  CLEAR_BIT(PWR->SVMCR, PWR_SVMCR_ASV);
}

/**
  * @brief  Enable the UVM Voltage Monitoring : VDDUSB versus 1.2 V.
  * @retval None.
  */
void HAL_PWREx_EnableUVM(void)
{
  SET_BIT(PWR->SVMCR, PWR_SVMCR_UVMEN);
}

/**
  * @brief  Disable the UVM Voltage Monitoring : VDDUSB versus 1.2 V.
  * @retval None.
  */
void HAL_PWREx_DisableUVM(void)
{
  CLEAR_BIT(PWR->SVMCR, PWR_SVMCR_UVMEN);
}

/**
  * @brief  Enable the IO2VM Voltage Monitoring : VDDIO2 versus 0.9 V.
  * @retval None.
  */
void HAL_PWREx_EnableIO2VM(void)
{
  SET_BIT(PWR->SVMCR, PWR_SVMCR_IO2VMEN);
}

/**
  * @brief  Disable the IO2VM Voltage Monitoring : VDDIO2 versus 0.9 V.
  * @retval None.
  */
void HAL_PWREx_DisableIO2VM(void)
{
  CLEAR_BIT(PWR->SVMCR, PWR_SVMCR_IO2VMEN);
}

/**
  * @brief  Enable the AVM1 Voltage Monitoring : VDDA versus 1.6 V.
  * @retval None.
  */
void HAL_PWREx_EnableAVM1(void)
{
  SET_BIT(PWR->SVMCR, PWR_SVMCR_AVM1EN);
}

/**
  * @brief  Disable the AVM1 Voltage Monitoring : VDDA versus 1.6 V.
  * @retval None.
  */
void HAL_PWREx_DisableAVM1(void)
{
  CLEAR_BIT(PWR->SVMCR, PWR_SVMCR_AVM1EN);
}

/**
  * @brief  Enable the AVM2 Voltage Monitoring : VDDA versus 1.8 V.
  * @retval None.
  */
void HAL_PWREx_EnableAVM2(void)
{
  SET_BIT(PWR->SVMCR, PWR_SVMCR_AVM2EN);
}

/**
  * @brief  Disable the AVM2 Voltage Monitoring : VDDA versus 1.8 V.
  * @retval None.
  */
void HAL_PWREx_DisableAVM2(void)
{
  CLEAR_BIT(PWR->SVMCR, PWR_SVMCR_AVM2EN);
}

/**
  * @brief  Enable the VBAT and temperature monitoring.
  * @retval None.
  */
void HAL_PWREx_EnableMonitoring(void)
{
  SET_BIT(PWR->BDCR1, PWR_BDCR1_MONEN);
}

/**
  * @brief  Disable the VBAT and temperature monitoring.
  * @retval None.
  */
void HAL_PWREx_DisableMonitoring(void)
{
  CLEAR_BIT(PWR->BDCR1, PWR_BDCR1_MONEN);
}

/**
  * @brief  Enable UCPD configuration memorization in Standby mode.
  * @retval None.
  */
void HAL_PWREx_EnableUCPDStandbyMode(void)
{
  SET_BIT(PWR->UCPDR, PWR_UCPDR_UCPD_STDBY);
}

/**
  * @brief  Disable UCPD configuration memorization in Standby mode.
  * @note   This function must be called on exiting the Standby mode and before
  *         any UCPD configuration update.
  * @retval None.
  */
void HAL_PWREx_DisableUCPDStandbyMode(void)
{
  CLEAR_BIT(PWR->UCPDR, PWR_UCPDR_UCPD_STDBY);
}

/**
  * @brief  Enable dead battery behavior.
  * @note   After exiting reset, the USB Type-C “dead battery” behavior is
  *         enabled, which may have a pull-down effect on CC1 and CC2 pins.
  *         It is recommended to disable it in all cases, either to stop this
  *         pull-down or to handover control to the UCPD (the UCPD must be
  *         initialized before doing the disable).
  * @retval None.
  */
void HAL_PWREx_EnableUCPDDeadBattery(void)
{
  CLEAR_BIT(PWR->UCPDR, PWR_UCPDR_UCPD_DBDIS);
}

/**
  * @brief  Disable dead battery behavior.
  * @note   After exiting reset, the USB Type-C “dead battery” behavior is
  *         enabled, which may have a pull-down effect on CC1 and CC2 pins.
  *         It is recommended to disable it in all cases, either to stop this
  *         pull-down or to handover control to the UCPD (the UCPD must be
  *         initialized before doing the disable).
  * @retval None.
  */
void HAL_PWREx_DisableUCPDDeadBattery(void)
{
  SET_BIT(PWR->UCPDR, PWR_UCPDR_UCPD_DBDIS);
}

/**
  * @brief  Enable the Battery charging.
  * @note   When VDD is present, charge the external battery through an internal
  *         resistor.
  * @param  ResistorValue : Specifies the charging resistor.
  *                         This parameter can be one of the following values :
  *                         @arg PWR_BATTERY_CHARGING_RESISTOR_5   : 5 KOhm resistor.
  *                         @arg PWR_BATTERY_CHARGING_RESISTOR_1_5 : 1.5 KOhm resistor.
  * @retval None.
  */
void HAL_PWREx_EnableBatteryCharging(uint32_t ResistorValue)
{
  /* Check the parameter */
  assert_param(IS_PWR_BATTERY_RESISTOR_SELECT (ResistorValue));

  /* Specify the charging resistor */
  MODIFY_REG(PWR->BDCR2, PWR_BDCR2_VBRS, ResistorValue);

  /* Enable the Battery charging */
  SET_BIT(PWR->BDCR2, PWR_BDCR2_VBE);
}

/**
  * @brief  Disable the Battery charging.
  * @retval None.
  */
void HAL_PWREx_DisableBatteryCharging(void)
{
  CLEAR_BIT (PWR->BDCR2, PWR_BDCR2_VBE);
}

/**
  * @brief  This function handles the PWR PVD/PVM interrupt request.
  * @note   This API should be called under the PVD_PVM_IRQHandler().
  * @retval None.
  */
void HAL_PWREx_PVD_PVM_IRQHandler(void)
{
  uint32_t  rising_flag;
  uint32_t  falling_flag;

  /* Get pending flags */
  rising_flag = READ_REG(EXTI->RPR1);
  falling_flag = READ_REG(EXTI->FPR1);

  /* Check PWR exti flags for PVD */
  if(((rising_flag | falling_flag) & PWR_EXTI_LINE_PVD) != RESET)
  {
    /* PWR PVD interrupt user callback */
    HAL_PWR_PVDCallback();

    /* Clear PVD exti pending bit */
    WRITE_REG(EXTI->RPR1, PWR_EXTI_LINE_PVD);
    WRITE_REG(EXTI->FPR1, PWR_EXTI_LINE_PVD);
  }

  /* Check PWR exti flags for UVM */
  if(((rising_flag | falling_flag) & PWR_EXTI_LINE_UVM) != RESET)
  {
    /* PWR UVM interrupt user callback */
    HAL_PWREx_UVMCallback();

    /* Clear UVM exti pending bit */
    WRITE_REG(EXTI->RPR1, PWR_EXTI_LINE_UVM);
    WRITE_REG(EXTI->FPR1, PWR_EXTI_LINE_UVM);
  }

  /* Check PWR exti flags for IO2VM */
  if(((rising_flag | falling_flag) & PWR_EXTI_LINE_IO2VM) != RESET)
  {
    /* PWR IO2VM interrupt user callback */
    HAL_PWREx_IO2VMCallback();

    /* Clear IO2VM exti pending bit */
    WRITE_REG(EXTI->RPR1, PWR_EXTI_LINE_IO2VM);
    WRITE_REG(EXTI->FPR1, PWR_EXTI_LINE_IO2VM);
  }

  /* Check PWR exti flags for AVM1 */
  if(((rising_flag | falling_flag) & PWR_EXTI_LINE_AVM1) != RESET)
  {
    /* PWR AVM1 interrupt user callback */
    HAL_PWREx_AVM1Callback();

    /* Clear AVM1 exti pending bit */
    WRITE_REG(EXTI->RPR1, PWR_EXTI_LINE_AVM1);
    WRITE_REG(EXTI->FPR1, PWR_EXTI_LINE_AVM1);
  }

  /* Check PWR exti flags for AVM2 */
  if(((rising_flag | falling_flag) & PWR_EXTI_LINE_AVM2) != RESET)
  {
    /* PWR AVM2 interrupt user callback */
    HAL_PWREx_AVM2Callback();

    /* Clear AVM2 exti pending bit */
    WRITE_REG(EXTI->RPR1, PWR_EXTI_LINE_AVM2);
    WRITE_REG(EXTI->FPR1, PWR_EXTI_LINE_AVM2);
  }
}

/**
  * @brief  PWR UVM interrupt callback.
  * @retval None.
  */
__weak void HAL_PWREx_UVMCallback(void)
{
  /* NOTE : This function should not be modified; when the callback is needed,
            HAL_PWREx_UVMCallback() API can be implemented in the user file
   */
}

/**
  * @brief  PWR IO2VM interrupt callback.
  * @retval None.
  */
__weak void HAL_PWREx_IO2VMCallback(void)
{
  /* NOTE : This function should not be modified; when the callback is needed,
            HAL_PWREx_IO2VMCallback() API can be implemented in the user file
   */
}

/**
  * @brief  PWR AVM1 interrupt callback.
  * @retval None.
  */
__weak void HAL_PWREx_AVM1Callback(void)
{
  /* NOTE : This function should not be modified; when the callback is needed,
            HAL_PWREx_AVM1Callback() API can be implemented in the user file
   */
}

/**
  * @brief  PWR AVM2 interrupt callback.
  * @retval None.
  */
__weak void HAL_PWREx_AVM2Callback(void)
{
  /* NOTE : This function should not be modified; when the callback is needed,
            HAL_PWREx_AVM2Callback() API can be implemented in the user file
   */
}
/**
  * @}
  */

/** @defgroup PWREx_Exported_Functions_Group4 Memories Retention Functions
  * @brief    Memories retention functions
  *
@verbatim
 ===============================================================================
                     ##### Memories Retention Functions #####
 ===============================================================================
    [..]
@endverbatim
  * @{
  */

/**
  * @brief  Enable SRAM2 page(s) content retention in STOP 3 and Standby mode.
  * @note   When RRSx bit is set, SRAM2 is powered by the low-power regulator in
  *         STOP 3 and Standby mode and its content is kept.
  * @param  SRAM2Pages : Specifies the SRAM2 pages.
  *                      This parameter can be one of the following values :
  *                      @arg PWR_SRAM2_PAGE1_STNADBY_RETENTION : SRAM2 page 1 retention.
  *                      @arg PWR_SRAM2_PAGE2_STNADBY_RETENTION : SRAM2 page 2 retention.
  *                      @arg PWR_SRAM2_FULL_STNADBY_RETENTION  : SRAM2 page 1 and page 2 retention.
  * @retval None.
  */
void HAL_PWREx_EnableSRAM2ContentRetention(uint32_t SRAM2Pages)
{
  /* Check the parameters */
  assert_param(IS_PWR_SRAM2_STANDBY_RETENTION(SRAM2Pages));

  /* Set RRSx bit(s) */
  MODIFY_REG(PWR->CR1, PWR_SRAM2_FULL_STNADBY_RETENTION, SRAM2Pages);
}

/**
  * @brief  Disable SRAM2 page(s) content retention in Standby mode.
  * @note   When RRSx bit is reset, SRAM2 is powered off in Standby mode and its
  *         content is lost.
  * @retval None.
  */
void HAL_PWREx_DisableSRAM2ContentRetention(void)
{
  CLEAR_BIT(PWR->CR1, PWR_SRAM2_FULL_STNADBY_RETENTION);
}

/**
  * @brief  Enable RAM page(s) content retention in Stop mode (Stop 0, 1, 2, 3).
  * @note   When enabling content retention for a given ram, memory kept powered
  *         in Stop mode. (Consommation not optimized)
  * @param RAMSelection: Specifies RAM page(s) to be retained in STOP mode.
  *                      This parameter can be a value of
  *                      @ref PWREx_RAM_Contents_Retention.
  * @retval None.
  */
void HAL_PWREx_EnableRAMsContentRetention(uint32_t RAMSelection)
{
  /* Check the parameters */
  assert_param(IS_PWR_RAM_STOP_RETENTION(RAMSelection));

#if defined(PWR_CR4_SRAM1PDS4)
  if (RAMSelection == PWR_SRAM1_FULL_STOP_RETENTION)
  {
    CLEAR_BIT(PWR->CR2, PWR_SRAM1_PAGE1_3_STOP_RETENTION);
    CLEAR_BIT(PWR->CR4, (PWR_SRAM1_PAGE4_12_STOP_RETENTION & (~PWR_CR4_FLAG)));
  }
  else if (RAMSelection == PWR_SRAM3_FULL_STOP_RETENTION)
  {
    CLEAR_BIT(PWR->CR2, PWR_SRAM3_PAGE1_8_STOP_RETENTION);
    CLEAR_BIT(PWR->CR4, (PWR_SRAM3_PAGE9_13_STOP_RETENTION & (~PWR_CR4_FLAG)));
  }
  else if ((RAMSelection & PWR_CR4_FLAG) == PWR_CR4_FLAG)
  {
    CLEAR_BIT(PWR->CR4, (RAMSelection & (~PWR_CR4_FLAG)));
  }
  else
  {
    CLEAR_BIT(PWR->CR2, RAMSelection);
  }
#else
  /* Enable RAM retention in stop mode */
  CLEAR_BIT(PWR->CR2, RAMSelection);
#endif
}

/**
  * @brief Disable RAM page(s) content lost in Stop mode (Stop 0, 1, 2, 3).
  * @note  When disabling content lost for a given ram, memory powered down
  *        in Stop mode. (consommation optimized)
  * @param RAMSelection: Specifies RAM page(s) to be lost in STOP mode.
  *                      This parameter can be a value of
  *                      @ref PWREx_RAM_Contents_Retention.
  * @retval None.
  */
void HAL_PWREx_DisableRAMsContentRetention(uint32_t RAMSelection)
{
  /* Check the parameters */
  assert_param(IS_PWR_RAM_STOP_RETENTION(RAMSelection));

#if defined(PWR_CR4_SRAM1PDS4)
  if (RAMSelection == PWR_SRAM1_FULL_STOP_RETENTION)
  {
    SET_BIT(PWR->CR2, PWR_SRAM1_PAGE1_3_STOP_RETENTION);
    SET_BIT(PWR->CR4, (PWR_SRAM1_PAGE4_12_STOP_RETENTION & (~PWR_CR4_FLAG)));
  }
  else if (RAMSelection == PWR_SRAM3_FULL_STOP_RETENTION)
  {
    SET_BIT(PWR->CR2, PWR_SRAM3_PAGE1_8_STOP_RETENTION);
    SET_BIT(PWR->CR4, (PWR_SRAM3_PAGE9_13_STOP_RETENTION & (~PWR_CR4_FLAG)));
  }
  else if ((RAMSelection & PWR_CR4_FLAG) == PWR_CR4_FLAG)
  {
    SET_BIT(PWR->CR4, (RAMSelection & (~PWR_CR4_FLAG)));
  }
  else
  {
    SET_BIT(PWR->CR2, RAMSelection);
  }
#else
  /* Set loss content bit */
  SET_BIT(PWR->CR2, RAMSelection);
#endif
}

/**
  * @brief  Enable the Backup RAM retention in Standby, Shutdown and VBAT modes.
  * @note   If BREN is reset, the backup RAM can still be used in Run, Sleep and
  *         Stop modes. However, its content is lost in Standby, Shutdown and
  *         VBAT modes. This bit can be writte
  * @retval None.
  */
HAL_StatusTypeDef HAL_PWREx_EnableBkupRAMRetention(void)
{
  /*
    Backup ram retention in Standby, Shutdown and VBAT should be enabled
    when the Vcore is alimented by the LDO regulator
  */
  if (HAL_PWREx_GetSupplyConfig() == PWR_LDO_SUPPLY)
  {
    SET_BIT(PWR->BDCR1, PWR_BDCR1_BREN);
  }
  /* SMPS regulator selected */
  else
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  Disable the Backup RAM retention in Standby, Shutdown and VBAT modes.
  * @note   If BREN is reset, the backup RAM can still be used in Run, Sleep and
  *         Stop modes. However, its content is lost in Standby, Shutdown and
  *         VBAT modes. This bit can be writte
  * @retval None.
  */
void HAL_PWREx_DisableBkupRAMRetention(void)
{
  CLEAR_BIT(PWR->BDCR1, PWR_BDCR1_BREN);
}

/**
  * @brief  Enable the flash memory fast wakeup from Stop 0 and Stop 1 modes.
  * @note   This feature is used to obtain the best trade-off between low-power
  *         consumption and wakeup time when exiting the Stop 0 or Stop 1 modes.
  *         When this feature is enabled, the Flash memory remains in normal
  *         mode in Stop 0 and Stop 1 modes, which offers a faster startup time
  *         with higher consumption.
  * @retval None.
  */
void HAL_PWREx_EnableFlashFastWakeUp(void)
{
  SET_BIT(PWR->CR2, PWR_CR2_FLASHFWU);
}

/**
  * @brief  Disable the Flash Power Down in Stop mode.
  * @note   This feature is used to obtain the best trade-off between low-power
  *         consumption and wakeup time when exiting the Stop 0 or Stop 1 modes.
  *         When this feature is disabled, the Flash memory enters low-power
  *         mode in Stop 0 and Stop 1 modes, which causes a slower startup time
  *         with lower consumption.
  * @retval None.
  */
void HAL_PWREx_DisableFlashFastWakeUp(void)
{
  CLEAR_BIT(PWR->CR2, PWR_CR2_FLASHFWU);
}

#if defined(PWR_CR2_RLPSN)
/**
  * @brief  Enable RAMs low power mode in Stop modes.
  * @note   When PWR_CR2_RLPNS bit is set, the RAMs do not enter the low-power mode when the CPU domain or system enters Stop.
  * @retval None.
  */
void HAL_PWREx_EnableRAMsLowPowerMode(void)
{
  CLEAR_BIT(PWR->CR2, PWR_CR2_RLPSN);
}

/**
  * @brief  Disable RAMs low power mode in Stop modes.
  * @note   When PWR_CR2_RLPNS bit is set, the RAMs do not enter the low-power mode when the CPU domain or system enters Stop.
  * @retval None.
  */
void HAL_PWREx_DisableRAMsLowPowerMode(void)
{
  SET_BIT(PWR->CR2, PWR_CR2_RLPSN);
}
#endif
/**
  * @}
  */

/** @defgroup PWREx_Exported_Functions_Group5 I/O Pull-Up Pull-Down Configuration Functions
  * @brief    I/O pull-up / pull-down configuration functions
  *
@verbatim
 ===============================================================================
                     ##### Voltage monitoring Functions #####
 ===============================================================================
    [..]
@endverbatim
  * @{
  */

/**
  * @brief  Enable pull-up and pull-down configuration.
  * @note   When APC bit is set, the I/O pull-up and pull-down configurations
  *         defined in PWR_PUCRx and PWR_PDCRx registers are applied in Standby
  *         and Shutdown modes.
  * @note   Pull-up set by PUy bit of PWR_PUCRx register is not activated if the
  *         corresponding PDy bit of PWR_PDCRx register is also set (pull-down
  *         configuration priority is higher). HAL_PWREx_EnableGPIOPullUp() and
  *         HAL_PWREx_EnableGPIOPullDown() API's ensure there is no conflict
  *         when setting PUy or PDy bit.
  * @retval None.
  */
void HAL_PWREx_EnablePullUpPullDownConfig(void)
{
  SET_BIT(PWR->APCR, PWR_APCR_APC);
}

/**
  * @brief  Disable pull-up and pull-down configuration.
  * @note   When APC bit is cleared, the I/O pull-up and pull-down configurations
  *         defined in PWR_PUCRx and PWR_PDCRx registers are not applied in
  *         Standby and Shutdown modes.
  * @retval None.
  */
void HAL_PWREx_DisablePullUpPullDownConfig(void)
{
  CLEAR_BIT(PWR->APCR, PWR_APCR_APC);
}

/**
  * @brief  Enable GPIO pull-up state in Standby and Shutdown modes.
  * @note   Set the relevant PUy bits of PWR_PUCRx register to configure the I/O
  *         in pull-up state in Standby and Shutdown modes.
  * @note   This state is effective in Standby and Shutdown modes only if APC
  *         bit is set through HAL_PWREx_EnablePullUpPullDownConfig() API.
  * @note   The configuration is lost when exiting the Shutdown mode due to the
  *         power-on reset, maintained when exiting the Standby mode.
  * @note   To avoid any conflict at Standby and Shutdown modes exits, the
  *         corresponding PDy bit of PWR_PDCRx register is cleared unless it is
  *         reserved.
  * @note   Even if a PUy bit to set is reserved, the other PUy bits entered as
  *         input parameter at the same time are set.
  * @param  GPIO_Port : Specify the IO port.
  *                     This parameter can be a value of
  *                     @ref PWREx_GPIO_Port.
  * @param  GPIO_Pin  : Specify the I/O pins numbers.
  *                     This parameter can be a value of
  *                     @ref PWREx_GPIO_Pin_Mask.
  * @retval HAL Status.
  */
HAL_StatusTypeDef HAL_PWREx_EnableGPIOPullUp(uint32_t GPIO_Port, uint32_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_PWR_GPIO_PORT(GPIO_Port));
  assert_param(IS_PWR_GPIO_PIN_MASK(GPIO_Pin));

  /* Check GPIO port */
  switch (GPIO_Port)
  {
    case PWR_GPIO_A: /* Apply Pull Up to GPIO port A */
       SET_BIT(PWR->PUCRA, (GPIO_Pin & (~(PWR_GPIO_BIT_14))));
       CLEAR_BIT(PWR->PDCRA, (GPIO_Pin & (~(PWR_GPIO_BIT_13|PWR_GPIO_BIT_15))));
       break;

    case PWR_GPIO_B: /* Apply Pull Up to GPIO port B */
       SET_BIT(PWR->PUCRB, GPIO_Pin);
       CLEAR_BIT(PWR->PDCRB, (GPIO_Pin & (~(PWR_GPIO_BIT_4))));
       break;

    case PWR_GPIO_C: /* Apply Pull Up to GPIO port C */
       SET_BIT(PWR->PUCRC, GPIO_Pin);
       CLEAR_BIT(PWR->PDCRC, GPIO_Pin);
       break;

    case PWR_GPIO_D: /* Apply Pull Up to GPIO port D */
       SET_BIT(PWR->PUCRD, GPIO_Pin);
       CLEAR_BIT(PWR->PDCRD, GPIO_Pin);
       break;

    case PWR_GPIO_E: /* Apply Pull Up to GPIO port E */
       SET_BIT(PWR->PUCRE, GPIO_Pin);
       CLEAR_BIT(PWR->PDCRE, GPIO_Pin);
       break;

    case PWR_GPIO_F: /* Apply Pull Up to GPIO port F */
       SET_BIT(PWR->PUCRF, GPIO_Pin);
       CLEAR_BIT(PWR->PDCRF, GPIO_Pin);
       break;

    case PWR_GPIO_G: /* Apply Pull Up to GPIO port G */
       SET_BIT(PWR->PUCRG, GPIO_Pin);
       CLEAR_BIT(PWR->PDCRG, GPIO_Pin);
       break;

    case PWR_GPIO_H: /* Apply Pull Up to GPIO port H */
       SET_BIT(PWR->PUCRH, GPIO_Pin);
       CLEAR_BIT(PWR->PDCRH, GPIO_Pin);
       break;

    case PWR_GPIO_I: /* Apply Pull Up to GPIO port I */
       SET_BIT(PWR->PUCRI, (GPIO_Pin & PWR_PORTI_AVAILABLE_PINS));
       CLEAR_BIT(PWR->PDCRI, (GPIO_Pin & PWR_PORTI_AVAILABLE_PINS));
       break;

#if defined(GPIOJ)
    case PWR_GPIO_J: /* Apply Pull Up to GPIO port J */
       SET_BIT(PWR->PUCRJ, (GPIO_Pin & PWR_PORTJ_AVAILABLE_PINS));
       CLEAR_BIT(PWR->PDCRJ, (GPIO_Pin & PWR_PORTJ_AVAILABLE_PINS));
       break;
#endif

    default:
       return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief Disable GPIO pull-up state in Standby mode and Shutdown modes.
  * @note  Reset the relevant PUy bits of PWR_PUCRx register used to configure
  *        the I/O in pull-up state in Standby and Shutdown modes.
  * @note  Even if a PUy bit to reset is reserved, the other PUy bits entered as
  *        input parameter at the same time are reset.
  * @param  GPIO_Port : Specify the IO port.
  *                     This parameter can be a value of
  *                     @ref PWREx_GPIO_Port.
  * @param  GPIO_Pin  : Specify the I/O pins numbers.
  *                     This parameter can be a value of
  *                     @ref PWREx_GPIO_Pin_Mask.
  * @retval HAL Status.
  */
HAL_StatusTypeDef HAL_PWREx_DisableGPIOPullUp(uint32_t GPIO_Port, uint32_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_PWR_GPIO_PORT(GPIO_Port));
  assert_param(IS_PWR_GPIO_PIN_MASK(GPIO_Pin));

  /* Check GPIO port */
  switch (GPIO_Port)
  {
    case PWR_GPIO_A: /* Disable Pull Up for GPIO port A */
       CLEAR_BIT(PWR->PUCRA, (GPIO_Pin & (~(PWR_GPIO_BIT_14))));
       break;

    case PWR_GPIO_B: /* Disable Pull Up for GPIO port B */
       CLEAR_BIT(PWR->PUCRB, GPIO_Pin);
       break;

    case PWR_GPIO_C: /* Disable Pull Up for GPIO port C */
       CLEAR_BIT(PWR->PUCRC, GPIO_Pin);
       break;

    case PWR_GPIO_D: /* Disable Pull Up for GPIO port D */
       CLEAR_BIT(PWR->PUCRD, GPIO_Pin);
       break;

    case PWR_GPIO_E: /* Disable Pull Up for GPIO port E */
       CLEAR_BIT(PWR->PUCRE, GPIO_Pin);
       break;

    case PWR_GPIO_F: /* Disable Pull Up for GPIO port F */
       CLEAR_BIT(PWR->PUCRF, GPIO_Pin);
       break;

    case PWR_GPIO_G: /* Disable Pull Up for GPIO port G */
       CLEAR_BIT(PWR->PUCRG, GPIO_Pin);
       break;

    case PWR_GPIO_H: /* Disable Pull Up for GPIO port H */
       CLEAR_BIT(PWR->PUCRH, GPIO_Pin);
       break;

    case PWR_GPIO_I: /* Disable Pull Up for GPIO port I */
       CLEAR_BIT(PWR->PUCRI, (GPIO_Pin & PWR_PORTI_AVAILABLE_PINS));
       break;

#if defined(GPIOJ)
    case PWR_GPIO_J: /* Disable Pull Up for GPIO port J */
       CLEAR_BIT(PWR->PUCRJ, (GPIO_Pin & PWR_PORTJ_AVAILABLE_PINS));
       break;
#endif

    default:
       return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief Enable GPIO pull-down state in Standby and Shutdown modes.
  * @note  Set the relevant PDy bits of PWR_PDCRx register to configure the I/O
  *        in pull-down state in Standby and Shutdown modes.
  * @note  This state is effective in Standby and Shutdown modes only if APC bit
  *        is set through HAL_PWREx_EnablePullUpPullDownConfig() API.
  * @note  The configuration is lost when exiting the Shutdown mode due to the
  *        power-on reset, maintained when exiting the Standby mode.
  * @note  To avoid any conflict at Standby and Shutdown modes exits, the
  *        corresponding PUy bit of PWR_PUCRx register is cleared unless it is
  *        reserved.
  * @note  Even if a PDy bit to set is reserved, the other PDy bits entered as
  *        input parameter at the same time are set.
  * @param  GPIO_Port : Specify the IO port.
  *                     This parameter can be a value of
  *                     @ref PWREx_GPIO_Port.
  * @param  GPIO_Pin  : Specify the I/O pins numbers.
  *                     This parameter can be a value of
  *                     @ref PWREx_GPIO_Pin_Mask.
  * @retval HAL Status.
  */
HAL_StatusTypeDef HAL_PWREx_EnableGPIOPullDown(uint32_t GPIO_Port, uint32_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_PWR_GPIO_PORT(GPIO_Port));
  assert_param(IS_PWR_GPIO_PIN_MASK(GPIO_Pin));

  /* Check GPIO port */
  switch (GPIO_Port)
  {
    case PWR_GPIO_A: /* Apply Pull Down to GPIO port A */
       SET_BIT(PWR->PDCRA, (GPIO_Pin & (~(PWR_GPIO_BIT_13|PWR_GPIO_BIT_15))));
       CLEAR_BIT(PWR->PUCRA, (GPIO_Pin & (~(PWR_GPIO_BIT_14))));
       break;

    case PWR_GPIO_B: /* Apply Pull Down to GPIO port B */
       SET_BIT(PWR->PDCRB, (GPIO_Pin & (~(PWR_GPIO_BIT_4))));
       CLEAR_BIT(PWR->PUCRB, GPIO_Pin);
       break;

    case PWR_GPIO_C: /* Apply Pull Down to GPIO port C */
       SET_BIT(PWR->PDCRC, GPIO_Pin);
       CLEAR_BIT(PWR->PUCRC, GPIO_Pin);
       break;

    case PWR_GPIO_D: /* Apply Pull Down to GPIO port D */
       SET_BIT(PWR->PDCRD, GPIO_Pin);
       CLEAR_BIT(PWR->PUCRD, GPIO_Pin);
       break;

    case PWR_GPIO_E: /* Apply Pull Down to GPIO port E */
       SET_BIT(PWR->PDCRE, GPIO_Pin);
       CLEAR_BIT(PWR->PUCRE, GPIO_Pin);
       break;

    case PWR_GPIO_F: /* Apply Pull Down to GPIO port F */
       SET_BIT(PWR->PDCRF, GPIO_Pin);
       CLEAR_BIT(PWR->PUCRF, GPIO_Pin);
       break;

    case PWR_GPIO_G: /* Apply Pull Down to GPIO port G */
       SET_BIT(PWR->PDCRG, GPIO_Pin);
       CLEAR_BIT(PWR->PUCRG, GPIO_Pin);
       break;

    case PWR_GPIO_H: /* Apply Pull Down to GPIO port H */
       SET_BIT(PWR->PDCRH, GPIO_Pin);
       CLEAR_BIT(PWR->PUCRH, GPIO_Pin);
       break;

    case PWR_GPIO_I: /* Apply Pull Down to GPIO port I */
       SET_BIT(PWR->PDCRI, (GPIO_Pin & PWR_PORTI_AVAILABLE_PINS));
       CLEAR_BIT(PWR->PUCRI, (GPIO_Pin & PWR_PORTI_AVAILABLE_PINS));
       break;

#if defined(GPIOJ)
    case PWR_GPIO_J: /* Apply Pull Down to GPIO port J */
       SET_BIT(PWR->PDCRJ, (GPIO_Pin & PWR_PORTJ_AVAILABLE_PINS));
       CLEAR_BIT(PWR->PUCRJ, (GPIO_Pin & PWR_PORTJ_AVAILABLE_PINS));
       break;
#endif

    default:
       return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief Disable GPIO pull-down state in Standby and Shutdown modes.
  * @note  Reset the relevant PDy bits of PWR_PDCRx register used to configure the I/O
  *        in pull-down state in Standby and Shutdown modes.
  * @note  Even if a PDy bit to reset is reserved, the other PDy bits entered as input
  *        parameter at the same time are reset.
  * @param  GPIO_Port : Specify the IO port.
  *                     This parameter can be a value of
  *                     @ref PWREx_GPIO_Port.
  * @param  GPIO_Pin  : Specify the I/O pins numbers.
  *                     This parameter can be a value of
  *                     @ref PWREx_GPIO_Pin_Mask.
  * @retval HAL Status.
  */
HAL_StatusTypeDef HAL_PWREx_DisableGPIOPullDown(uint32_t GPIO_Port, uint32_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_PWR_GPIO_PORT(GPIO_Port));
  assert_param(IS_PWR_GPIO_PIN_MASK(GPIO_Pin));

  /* Check GPIO port */
  switch (GPIO_Port)
  {
    case PWR_GPIO_A: /* Disable Pull Down for GPIO port A */
       CLEAR_BIT(PWR->PDCRA, (GPIO_Pin & (~(PWR_GPIO_BIT_13|PWR_GPIO_BIT_15))));
       break;

    case PWR_GPIO_B: /* Disable Pull Down for GPIO port B */
       CLEAR_BIT(PWR->PDCRB, (GPIO_Pin & (~(PWR_GPIO_BIT_4))));
       break;

    case PWR_GPIO_C: /* Disable Pull Down for GPIO port C */
       CLEAR_BIT(PWR->PDCRC, GPIO_Pin);
       break;

    case PWR_GPIO_D: /* Disable Pull Down for GPIO port D */
       CLEAR_BIT(PWR->PDCRD, GPIO_Pin);
       break;

    case PWR_GPIO_E: /* Disable Pull Down for GPIO port E */
       CLEAR_BIT(PWR->PDCRE, GPIO_Pin);
       break;

    case PWR_GPIO_F: /* Disable Pull Down for GPIO port F */
       CLEAR_BIT(PWR->PDCRF, GPIO_Pin);
       break;

    case PWR_GPIO_G: /* Disable Pull Down for GPIO port G */
       CLEAR_BIT(PWR->PDCRG, GPIO_Pin);
       break;

    case PWR_GPIO_H: /* Disable Pull Down for GPIO port H */
        CLEAR_BIT(PWR->PDCRH, GPIO_Pin);
       break;

    case PWR_GPIO_I: /* Disable Pull Down for GPIO port I */
       CLEAR_BIT(PWR->PDCRI, (GPIO_Pin & PWR_PORTI_AVAILABLE_PINS));
       break;

#if defined(GPIOJ)
    case PWR_GPIO_J: /* Disable Pull Down for GPIO port J */
       CLEAR_BIT(PWR->PDCRJ, (GPIO_Pin & PWR_PORTJ_AVAILABLE_PINS));
       break;
#endif

    default:
       return HAL_ERROR;
  }

  return HAL_OK;
}
/**
  * @}
  */

#endif /* defined (HAL_PWR_MODULE_ENABLED) */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
