/*
 *******************************************************************************
 * Copyright (c) 2026, uSolder
 * Based on the STM32H523Z generic variant, Copyright (c) 2020-2026,
 * STMicroelectronics. All rights reserved.
 *
 * This software component is licensed under the BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */
#if defined(ARDUINO_SKIPPER)
#include "pins_arduino.h"

// Digital PinName array
const PinName digitalPin[] = {
  // Header 1 (J2)
  PA_4,   // D0/A0
  PA_3,   // D1/A1
  PA_2,   // D2/A2
  PA_1,   // D3/A3
  PA_0,   // D4/A4
  PC_3,   // D5/A5
  PC_2,   // D6/A6
  PC_1,   // D7/A7
  // Header 2 (J3)
  PB_2,   // D8, LED
  PF_10,  // D9
  PF_9,   // D10
  PF_8,   // D11
  PF_7,   // D12
  PF_6,   // D13
  PF_5,   // D14
  PF_4,   // D15
  // Header 3 (J4)
  PE_0,   // D16
  PF_3,   // D17
  PF_2,   // D18
  PE_6,   // D19
  PE_5,   // D20
  PE_4,   // D21
  PE_3,   // D22
  PE_2,   // D23
  // Header 4 (J5)
  PB_7,   // D24
  PB_6,   // D25
  PB_5,   // D26
  PB_4,   // D27
  PG_15,  // D28
  PG_14,  // D29
  PG_13,  // D30
  PG_12,  // D31
  // Header 5 (J6)
  PD_4,   // D32
  PD_3,   // D33
  PA_15,  // D34
  PC_7,   // D35
  PC_6,   // D36
  PG_8,   // D37
  PG_7,   // D38
  PG_6,   // D39
  // Header 6 (J9)
  PD_15,  // D40
  PD_14,  // D41
  PD_13,  // D42
  PD_12,  // D43
  PD_11,  // D44
  PD_10,  // D45
  PB_10,  // D46
  PE_15,  // D47
  // Header 7 (J10)
  PE_14,  // D48
  PE_13,  // D49
  PE_12,  // D50
  PE_11,  // D51
  PE_10,  // D52
  PE_9,   // D53
  PE_8,   // D54
  PE_7,   // D55
  // Header 8 (J11)
  PC_0,   // D56/A8
  PB_1,   // D57/A9
  PB_0,   // D58/A10
  PC_5,   // D59/A11
  PC_4,   // D60/A12
  PA_7,   // D61/A13
  PA_6,   // D62/A14
  PA_5,   // D63/A15
  // I2C1
  PB_8,   // D64, I2C1_SCL
  PB_9,   // D65, I2C1_SDA
  // I2C2
  PF_1,   // D66, I2C2_SCL
  PF_0,   // D67, I2C2_SDA
  // SPI1
  PG_10,  // D68, SPI1_CS
  PG_11,  // D69, SPI1_SCLK
  PG_9,   // D70, SPI1_MISO
  PD_7,   // D71, SPI1_MOSI
  // SPI2
  PB_12,  // D72, SPI2_CS
  PB_13,  // D73, SPI2_SCLK
  PB_14,  // D74, SPI2_MISO
  PB_15,  // D75, SPI2_MOSI
  // USART1, USART2, USART3 (DAPLink VCP)
  PA_9,   // D76, UART1_TX
  PA_10,  // D77, UART1_RX
  PD_5,   // D78, UART2_TX
  PD_6,   // D79, UART2_RX
  PD_8,   // D80, UART3_TX
  PD_9,   // D81, UART3_RX
  // DIP switch
  PG_0,   // D82, DIP0
  PG_1,   // D83, DIP1
  PG_2,   // D84, DIP2
  PG_3,   // D85, DIP3
  PG_4,   // D86, DIP4
  PG_5,   // D87, DIP5
  // microSD
  PC_8,   // D88, SDMMC_D0
  PC_9,   // D89, SDMMC_D1
  PC_10,  // D90, SDMMC_D2
  PC_11,  // D91, SDMMC_D3
  PC_12,  // D92, SDMMC_CLK
  PD_2,   // D93, SDMMC_CMD
  PF_11,  // D94, SDMMC_DETECT
  // CAN
  PD_1,   // D95, FDCAN_TX
  PD_0,   // D96, FDCAN_RX
  // Power and USB-C status
  PF_12,  // D97, USB_UFP_DETECT
  PF_13,  // D98, BUCK_DETECT
  PF_14,  // D99, USB_DRP_FAULT
  PF_15,  // D100, USB_DRP_INT
  // USB
  PA_11,  // D101, USB_D_N
  PA_12,  // D102, USB_D_P
  // Debug
  PA_13,  // D103, SWDIO
  PA_14,  // D104, SWCLK
  PB_3,   // D105, SWO
  // Other
  PA_8,   // D106, MCO1 (TP1)
  PC_13,  // D107, not connected
  PC_14,  // D108, OSC32_IN
  PC_15,  // D109, OSC32_OUT
  PH_0,   // D110, OSC_IN
  PH_1    // D111, OSC_OUT
};

// Analog (Ax) pin number array
const pin_size_t analogInputPin[] = {
  0,  // A0,  PA4
  1,  // A1,  PA3
  2,  // A2,  PA2
  3,  // A3,  PA1
  4,  // A4,  PA0
  5,  // A5,  PC3
  6,  // A6,  PC2
  7,  // A7,  PC1
  56, // A8,  PC0
  57, // A9,  PB1
  58, // A10, PB0
  59, // A11, PC5
  60, // A12, PC4
  61, // A13, PA7
  62, // A14, PA6
  63  // A15, PA5
};

// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  System Clock Configuration
  *
  * HSE 8 MHz crystal:
  *   PLL1: /4 x250 = 500 MHz VCO -> P/2 = 250 MHz SYSCLK, Q/10 = 50 MHz
  *         (SDMMC1, SPI1-3)
  *   PLL2: /4 x240 = 480 MHz VCO -> Q/6 = 80 MHz FDCAN, R/8 = 60 MHz ADC/DAC
  *   PLL3: /4 x192 = 384 MHz VCO -> Q/8 = 48 MHz USB
  * HSI48 stays on for the RNG.
  */
WEAK void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLL1_SOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 250;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 10;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1_VCIRANGE_1;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1_VCORANGE_WIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2
                                | RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
    Error_Handler();
  }

  /** Configure the programming delay
  */
  __HAL_FLASH_SET_PROGRAM_DELAY(FLASH_PROGRAMMING_DELAY_2);

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC1 | RCC_PERIPHCLK_ADCDAC
                                             | RCC_PERIPHCLK_FDCAN | RCC_PERIPHCLK_USB
                                             | RCC_PERIPHCLK_SPI1 | RCC_PERIPHCLK_SPI2
                                             | RCC_PERIPHCLK_SPI3;
  PeriphClkInitStruct.PLL2.PLL2Source = RCC_PLL2_SOURCE_HSE;
  PeriphClkInitStruct.PLL2.PLL2M = 4;
  PeriphClkInitStruct.PLL2.PLL2N = 240;
  PeriphClkInitStruct.PLL2.PLL2P = 2;
  PeriphClkInitStruct.PLL2.PLL2Q = 6;
  PeriphClkInitStruct.PLL2.PLL2R = 8;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2_VCIRANGE_1;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2_VCORANGE_WIDE;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.PLL2.PLL2ClockOut = RCC_PLL2_DIVQ | RCC_PLL2_DIVR;
  PeriphClkInitStruct.PLL3.PLL3Source = RCC_PLL3_SOURCE_HSE;
  PeriphClkInitStruct.PLL3.PLL3M = 4;
  PeriphClkInitStruct.PLL3.PLL3N = 192;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 8;
  PeriphClkInitStruct.PLL3.PLL3R = 2;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3_VCIRANGE_1;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3_VCORANGE_WIDE;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.PLL3.PLL3ClockOut = RCC_PLL3_DIVQ;
  PeriphClkInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_PLL1Q;
  PeriphClkInitStruct.AdcDacClockSelection = RCC_ADCDACCLKSOURCE_PLL2R;
  PeriphClkInitStruct.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL2Q;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL3Q;
  PeriphClkInitStruct.Spi1ClockSelection = RCC_SPI1CLKSOURCE_PLL1Q;
  PeriphClkInitStruct.Spi2ClockSelection = RCC_SPI2CLKSOURCE_PLL1Q;
  PeriphClkInitStruct.Spi3ClockSelection = RCC_SPI3CLKSOURCE_PLL1Q;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
    Error_Handler();
  }
}

#ifdef __cplusplus
}
#endif
#endif /* ARDUINO_SKIPPER */
