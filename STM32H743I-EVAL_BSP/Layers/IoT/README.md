Board: STMicroelectronics [STM32H743I-EVAL](https://www.st.com/en/evaluation-tools/stm32h743i-eval.html)
------------------------------------------

Device: **STM32H743XIH6**
System Core Clock: **400 MHz**

This setup is configured using **STM32CubeMX**, an interactive tool provided by STMicroelectronics for device configuration.
Refer to ["Configure STM32 Devices with CubeMX"](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/blob/main/docs/CubeMX.md) for additional information.

### STM32CubeMX configuration
Start STM32CubeMX with default STM32H743I-EVAL board setup and make the following changes:
1. Under **Pinout & Configuration** select **Categories - System Core - SYS**:
    - Select **TIM17** for **Timebase Source**
2. Under **Pinout & Configuration** select **Categories - System Core - NVIC**:
    - Under **NVIC** tab change:
      - **USART1 global interrupt - Enabled** to **checked**
      - **SDMMC1 global interrupt - Enabled** to **checked**
      - **Ethernet global interrupt - Enabled** to **checked**
    - Under **Code Generation** tab change:
      - **System service call via SWI instruction - Generate IRQ handler** to **unchecked**
      - **Pendable request for system service - Generate IRQ handler** to **unchecked**
      - **System tick timer - Generate IRQ handler** to **unchecked**
3. Under **Project Manager** select **Project**:
    - Under **Linker Settings** change:
      - **Minimum Heap Size** set **0x400**
      - **Minimum Stack Size** set **0x400**
4. Under **Project Manager** select **Advanced Settings**:
    - Under **Generated Function Calls** section change:
      - **Do Not Generate Function Calls** to **checked** for **Peripheral Instance Name - SDMMC1** 
5. Click on **GENERATE**

### System Configuration

| System resource         | Setting
|:------------------------|:--------------------------------------------
| Heap                    | 1 kB (configured in the STM32CubeMX)
| Stack (MSP)             | 1 kB (configured in the STM32CubeMX)

### STDIO mapping

**STDIO** is routed to Virtual COM port on the ST-Link (using USART1 peripheral)

### CMSIS-Driver mapping

| CMSIS-Driver  | Peripheral
|:--------------|:----------
| ETH_MAC0      | ETH
| ETH_PHY0      | LAN8742A (external)
| MCI0          | SDMMC1
| USBD0         | USB_OTG_FS

### CMSIS-Driver Virtual I/O mapping

| CMSIS-Driver VIO  | Physical resource
|:------------------|:-----------------------
| vioBUTTON0        | Button USER (GPIO C.13)
| vioLED0           | LED3 RED (GPIO A.4)
| vioLED1           | LED1 GREEN (GPIO F.10)

## Board configuration
**Board setup**
  - Insure that the **JP10** is in the **PSU** position and connect 5V power supply to the **CN10** connector
  - Check that the Boot selection switch **SW1** is in the **0** / default position
  - Connect a **USB micro-B cable** between the **STLK** connector and your **Personal Computer**
