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
 *
 * uSolder Skipper: STM32H523ZET6 (LQFP144), 8 MHz HSE, 32.768 kHz LSE.
 *
 * Arduino pin numbers follow the eight 10-pin user headers, then the pins
 * wired to on-board functions:
 *
 *   D0  - D7   Header 1 (J2)   pins 2-9      D0-D7 are also A0-A7
 *   D8  - D15  Header 2 (J3)   pins 2-9      D8 (PB2) drives the user LED
 *   D16 - D23  Header 3 (J4)   pins 2-9
 *   D24 - D31  Header 4 (J5)   pins 2-9
 *   D32 - D39  Header 5 (J6)   pins 2-9
 *   D40 - D47  Header 6 (J9)   pins 2-9
 *   D48 - D55  Header 7 (J10)  pins 2-9
 *   D56 - D63  Header 8 (J11)  pins 2-9      D56-D63 are also A8-A15
 *   D64 - D111 Dedicated on-board signals (see below)
 *
 * Every header has pin 1 = 3.3V and pin 10 = GND.
 */
#pragma once

/*----------------------------------------------------------------------------
 *        STM32 pins number
 *----------------------------------------------------------------------------*/
// Header 1 (J2)
#define PA4                     PIN_A0  // D0,  H1P1
#define PA3                     PIN_A1  // D1,  H1P2
#define PA2                     PIN_A2  // D2,  H1P3
#define PA1                     PIN_A3  // D3,  H1P4
#define PA0                     PIN_A4  // D4,  H1P5
#define PC3                     PIN_A5  // D5,  H1P6
#define PC2                     PIN_A6  // D6,  H1P7
#define PC1                     PIN_A7  // D7,  H1P8
// Header 2 (J3)
#define PB2                     8       // H2P1, user LED (D1)
#define PF10                    9       // H2P2
#define PF9                     10      // H2P3
#define PF8                     11      // H2P4
#define PF7                     12      // H2P5
#define PF6                     13      // H2P6
#define PF5                     14      // H2P7
#define PF4                     15      // H2P8
// Header 3 (J4)
#define PE0                     16      // H3P1
#define PF3                     17      // H3P2
#define PF2                     18      // H3P3
#define PE6                     19      // H3P4
#define PE5                     20      // H3P5
#define PE4                     21      // H3P6
#define PE3                     22      // H3P7
#define PE2                     23      // H3P8
// Header 4 (J5)
#define PB7                     24      // H4P1
#define PB6                     25      // H4P2
#define PB5                     26      // H4P3
#define PB4                     27      // H4P4
#define PG15                    28      // H4P5
#define PG14                    29      // H4P6
#define PG13                    30      // H4P7
#define PG12                    31      // H4P8
// Header 5 (J6)
#define PD4                     32      // H5P1
#define PD3                     33      // H5P2
#define PA15                    34      // H5P3
#define PC7                     35      // H5P4
#define PC6                     36      // H5P5
#define PG8                     37      // H5P6
#define PG7                     38      // H5P7
#define PG6                     39      // H5P8
// Header 6 (J9)
#define PD15                    40      // H6P1
#define PD14                    41      // H6P2
#define PD13                    42      // H6P3
#define PD12                    43      // H6P4
#define PD11                    44      // H6P5
#define PD10                    45      // H6P6
#define PB10                    46      // H6P7
#define PE15                    47      // H6P8
// Header 7 (J10)
#define PE14                    48      // H7P1
#define PE13                    49      // H7P2
#define PE12                    50      // H7P3
#define PE11                    51      // H7P4
#define PE10                    52      // H7P5
#define PE9                     53      // H7P6
#define PE8                     54      // H7P7
#define PE7                     55      // H7P8
// Header 8 (J11)
#define PC0                     PIN_A8  // D56, H8P1
#define PB1                     PIN_A9  // D57, H8P2
#define PB0                     PIN_A10 // D58, H8P3
#define PC5                     PIN_A11 // D59, H8P4
#define PC4                     PIN_A12 // D60, H8P5
#define PA7                     PIN_A13 // D61, H8P6
#define PA6                     PIN_A14 // D62, H8P7
#define PA5                     PIN_A15 // D63, H8P8
// I2C1 (J12, combined header J16; also the TUSB320 USB-C controller)
#define PB8                     64      // I2C1_SCL, 1.5k pull-up
#define PB9                     65      // I2C1_SDA, 1.5k pull-up
// I2C2 (J13, combined header J17)
#define PF1                     66      // I2C2_SCL, 1.5k pull-up
#define PF0                     67      // I2C2_SDA, 1.5k pull-up
// SPI1 (J7, combined header J16)
#define PG10                    68      // SPI1_CS, 15k pull-up
#define PG11                    69      // SPI1_SCLK
#define PG9                     70      // SPI1_MISO
#define PD7                     71      // SPI1_MOSI
// SPI2 (J8, combined header J17)
#define PB12                    72      // SPI2_CS, 15k pull-up
#define PB13                    73      // SPI2_SCLK
#define PB14                    74      // SPI2_MISO
#define PB15                    75      // SPI2_MOSI
// USART1 (J14, combined header J16)
#define PA9                     76      // UART1_TX
#define PA10                    77      // UART1_RX
// USART2 (J15, combined header J17)
#define PD5                     78      // UART2_TX
#define PD6                     79      // UART2_RX
// USART3: virtual COM port of the on-board DAPLink probe
#define PD8                     80      // UART3_TX
#define PD9                     81      // UART3_RX
// 6-bit DIP switch SW1 (ON = HIGH, 200k pull-downs)
#define PG0                     82      // DIP0
#define PG1                     83      // DIP1
#define PG2                     84      // DIP2
#define PG3                     85      // DIP3
#define PG4                     86      // DIP4
#define PG5                     87      // DIP5
// microSD slot (SDMMC1, 4-bit)
#define PC8                     88      // SDMMC_D0
#define PC9                     89      // SDMMC_D1
#define PC10                    90      // SDMMC_D2
#define PC11                    91      // SDMMC_D3
#define PC12                    92      // SDMMC_CLK
#define PD2                     93      // SDMMC_CMD
#define PF11                    94      // SDMMC_DETECT
// CAN (FDCAN1 to TCAN1044V transceiver)
#define PD1                     95      // FDCAN_TX
#define PD0                     96      // FDCAN_RX
// Power and USB-C status
#define PF12                    97      // USB_UFP_DETECT (DAPLink USB-C has VBUS)
#define PF13                    98      // BUCK_DETECT (5V buck from bus power is up)
#define PF14                    99      // USB_DRP_FAULT (host power switch fault, active low)
#define PF15                    100     // USB_DRP_INT (TUSB320 interrupt, active low)
// USB FS (USB-C DRP connector)
#define PA11                    101     // USB_D_N
#define PA12                    102     // USB_D_P
// Debug (wired to the on-board DAPLink probe)
#define PA13                    103     // SWDIO
#define PA14                    104     // SWCLK
#define PB3                     105     // SWO
// Other
#define PA8                     106     // MCO1, test point TP1
#define PC13                    107     // Not connected (errata 2.2.1)
#define PC14                    108     // LSE OSC32_IN
#define PC15                    109     // LSE OSC32_OUT
#define PH0                     110     // HSE OSC_IN
#define PH1                     111     // HSE OSC_OUT

// Alternate pins number
#define PA0_ALT1                (PA0  | ALT1)
#define PA1_ALT1                (PA1  | ALT1)
#define PA1_ALT2                (PA1  | ALT2)
#define PA2_ALT1                (PA2  | ALT1)
#define PA2_ALT2                (PA2  | ALT2)
#define PA3_ALT1                (PA3  | ALT1)
#define PA3_ALT2                (PA3  | ALT2)
#define PA4_ALT1                (PA4  | ALT1)
#define PA5_ALT1                (PA5  | ALT1)
#define PA6_ALT1                (PA6  | ALT1)
#define PA7_ALT1                (PA7  | ALT1)
#define PA7_ALT2                (PA7  | ALT2)
#define PA9_ALT1                (PA9  | ALT1)
#define PA10_ALT1               (PA10 | ALT1)
#define PA11_ALT1               (PA11 | ALT1)
#define PA12_ALT1               (PA12 | ALT1)
#define PA15_ALT1               (PA15 | ALT1)
#define PB0_ALT1                (PB0  | ALT1)
#define PB0_ALT2                (PB0  | ALT2)
#define PB1_ALT1                (PB1  | ALT1)
#define PB1_ALT2                (PB1  | ALT2)
#define PB3_ALT1                (PB3  | ALT1)
#define PB4_ALT1                (PB4  | ALT1)
#define PB5_ALT1                (PB5  | ALT1)
#define PB6_ALT1                (PB6  | ALT1)
#define PB6_ALT2                (PB6  | ALT2)
#define PB7_ALT1                (PB7  | ALT1)
#define PB10_ALT1               (PB10 | ALT1)
#define PB13_ALT1               (PB13 | ALT1)
#define PB14_ALT1               (PB14 | ALT1)
#define PB14_ALT2               (PB14 | ALT2)
#define PB15_ALT1               (PB15 | ALT1)
#define PB15_ALT2               (PB15 | ALT2)
#define PC0_ALT1                (PC0  | ALT1)
#define PC1_ALT1                (PC1  | ALT1)
#define PC2_ALT1                (PC2  | ALT1)
#define PC3_ALT1                (PC3  | ALT1)
#define PC4_ALT1                (PC4  | ALT1)
#define PC5_ALT1                (PC5  | ALT1)
#define PC6_ALT1                (PC6  | ALT1)
#define PC7_ALT1                (PC7  | ALT1)
#define PC8_ALT1                (PC8  | ALT1)
#define PC9_ALT1                (PC9  | ALT1)
#define PC10_ALT1               (PC10 | ALT1)
#define PC11_ALT1               (PC11 | ALT1)

#define NUM_DIGITAL_PINS        112
#define NUM_ANALOG_INPUTS       16

/*----------------------------------------------------------------------------
 *        Board functions
 *----------------------------------------------------------------------------*/
// Headers: SKIPPER_HEADER_PIN(h, p) is the Arduino pin of signal HhPp
// (header h = 1-8, signal p = 1-8), which is physical connector pin p + 1.
// Example: SKIPPER_HEADER_PIN(2, 1) == PB2.
#define SKIPPER_HEADER_PIN(h, p) (((h) - 1) * 8 + ((p) - 1))

// On-board LED pin number (red LED D1, active high)
#ifndef LED_BUILTIN
  #define LED_BUILTIN           PB2
#endif
#define LED_RED                 LED_BUILTIN

// Skipper has no user push button (SW2 is reset, SW3 is BOOT0).
// The DIP switch can be used instead.
#ifndef USER_BTN
  #define USER_BTN              PNUM_NOT_DEFINED
#endif

// DIP switch SW1, reads HIGH when a switch is ON
#define DIP0                    PG0
#define DIP1                    PG1
#define DIP2                    PG2
#define DIP3                    PG3
#define DIP4                    PG4
#define DIP5                    PG5
#define DIP_COUNT               6

// Power and USB-C status
#define USB_UFP_DETECT          PF12    // HIGH when the DAPLink USB-C port has VBUS
#define BUCK_DETECT             PF13    // HIGH when the 5V buck (bus power) is up
#define USB_DRP_FAULT           PF14    // LOW on USB host power switch fault
#define USB_DRP_INT             PF15    // LOW when the TUSB320 signals an event
#define TUSB320_I2C_ADDRESS     0x47    // on I2C1 ('Wire')

// CAN (FDCAN1, TCAN1044V transceiver, standby tied low)
#define PIN_CAN_TX              PD1
#define PIN_CAN_RX              PD0

// SPI definitions: 'SPI' is SPI1 (J7). SPI2 (J8) uses the PIN_SPI2_* pins.
#ifndef PIN_SPI_SS
  #define PIN_SPI_SS            PG10
#endif
#ifndef PIN_SPI_SS1
  #define PIN_SPI_SS1           PNUM_NOT_DEFINED
#endif
#ifndef PIN_SPI_SS2
  #define PIN_SPI_SS2           PNUM_NOT_DEFINED
#endif
#ifndef PIN_SPI_SS3
  #define PIN_SPI_SS3           PNUM_NOT_DEFINED
#endif
#ifndef PIN_SPI_MOSI
  #define PIN_SPI_MOSI          PD7
#endif
#ifndef PIN_SPI_MISO
  #define PIN_SPI_MISO          PG9
#endif
#ifndef PIN_SPI_SCK
  #define PIN_SPI_SCK           PG11
#endif
#define PIN_SPI2_SS             PB12
#define PIN_SPI2_MOSI           PB15
#define PIN_SPI2_MISO           PB14
#define PIN_SPI2_SCK            PB13

// I2C definitions: 'Wire' is I2C1 (J12). I2C2 (J13) uses the PIN_WIRE1_* pins.
#ifndef PIN_WIRE_SDA
  #define PIN_WIRE_SDA          PB9
#endif
#ifndef PIN_WIRE_SCL
  #define PIN_WIRE_SCL          PB8
#endif
#define PIN_WIRE1_SDA           PF0
#define PIN_WIRE1_SCL           PF1

// I3C definitions (I3C1 shares the I2C1 pins)
#ifndef PIN_I3C_SDA
  #define PIN_I3C_SDA           PB9
#endif
#ifndef PIN_I3C_SCL
  #define PIN_I3C_SCL           PB8
#endif

// Timer Definitions
// Use TIM6/TIM7 when possible as servo and tone don't need GPIO output pin
#ifndef TIMER_TONE
  #define TIMER_TONE            TIM6
#endif
#ifndef TIMER_SERVO
  #define TIMER_SERVO           TIM7
#endif

// UART Definitions
// 'Serial' is USART3, the DAPLink virtual COM port on the USB-C UFP connector.
#ifndef SERIAL_UART_INSTANCE
  #define SERIAL_UART_INSTANCE  3
#endif

// Default pin used for generic 'Serial' instance
// Mandatory for Firmata
#ifndef PIN_SERIAL_RX
  #define PIN_SERIAL_RX         PD9
#endif
#ifndef PIN_SERIAL_TX
  #define PIN_SERIAL_TX         PD8
#endif

// Serial1 and Serial2 are the UART headers J14 and J15, always available.
#define ENABLE_HWSERIAL1
#define ENABLE_HWSERIAL2

#ifndef PIN_SERIAL1_RX
  #define PIN_SERIAL1_RX        PA10
#endif
#ifndef PIN_SERIAL1_TX
  #define PIN_SERIAL1_TX        PA9
#endif
#ifndef PIN_SERIAL2_RX
  #define PIN_SERIAL2_RX        PD6
#endif
#ifndef PIN_SERIAL2_TX
  #define PIN_SERIAL2_TX        PD5
#endif
#ifndef PIN_SERIAL3_RX
  #define PIN_SERIAL3_RX        PD9
#endif
#ifndef PIN_SERIAL3_TX
  #define PIN_SERIAL3_TX        PD8
#endif

// Extra HAL modules
#if !defined(HAL_DAC_MODULE_DISABLED)
  #define HAL_DAC_MODULE_ENABLED
#endif
#if !defined(HAL_FDCAN_MODULE_DISABLED)
  #define HAL_FDCAN_MODULE_ENABLED
#endif
#if !defined(HAL_I3C_MODULE_DISABLED)
  #define HAL_I3C_MODULE_ENABLED
#endif
#if !defined(HAL_SD_MODULE_DISABLED)
  #define HAL_SD_MODULE_ENABLED
#endif

// SD card slot Definitions (STM32SD library)
// SDMMC signals not available
#define SDMMC_CDIR_NA
#define SDMMC_CKIN_NA
#define SDMMC_D0DIR_NA

#ifndef SD_DETECT_PIN
  #define SD_DETECT_PIN         PF11
#endif
// Card detect switch closes to GND when a card is inserted (200k pull-up)
#ifndef SD_DETECT_LEVEL
  #define SD_DETECT_LEVEL       LOW
#endif
#ifndef SDX_D0
  #define SDX_D0                PC8
#endif
#ifndef SDX_D1
  #define SDX_D1                PC9
#endif
#ifndef SDX_D2
  #define SDX_D2                PC10
#endif
#ifndef SDX_D3
  #define SDX_D3                PC11
#endif
#ifndef SDX_CMD
  #define SDX_CMD               PD2
#endif
#ifndef SDX_CK
  #define SDX_CK                PC12
#endif

// Oscillators
#define HSE_VALUE               8000000UL   // X1, 8 MHz crystal
// X2 is a 32.768 kHz crystal: the HAL default LSE_VALUE applies

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus
  // These serial port names are intended to allow libraries and architecture-neutral
  // sketches to automatically default to the correct port name for a particular type
  // of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
  // the first hardware serial port whose RX/TX pins are not dedicated to another use.
  //
  // SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
  //
  // SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
  //
  // SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
  //
  // SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
  //
  // SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
  //                            pins are NOT connected to anything by default.
  #ifndef SERIAL_PORT_MONITOR
    #define SERIAL_PORT_MONITOR   Serial
  #endif
  #ifndef SERIAL_PORT_HARDWARE
    #define SERIAL_PORT_HARDWARE  Serial
  #endif
#endif
