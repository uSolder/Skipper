# Skipper STM32duino support

Arduino board support for **Skipper** by [uSolder](https://usolder.com): the
STM32H523 project board with CAN.

> Beta. Not yet tested on production boards.

This package adds the **uSolder Skipper** board to the
[STM32duino](https://github.com/stm32duino/Arduino_Core_STM32) core. The
compiler, core, libraries and upload tools all come from STM32duino; this
folder only holds the board definition, its pin map and clock setup, and a
small `Skipper` library with examples.

## Installation

1. In the Arduino IDE open *File > Preferences* and add both of these to
   *Additional boards manager URLs*:

   ```text
   https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json
   https://usolder.github.io/Skipper/package_usolder_index.json
   ```

2. Open *Tools > Board > Boards Manager* and install
   **STM32 MCU based boards** (3.1.0 or newer), then **uSolder STM32 Boards**.
   The second one needs the first; the Boards Manager will not install it
   for you.
3. Select *Tools > Board > uSolder STM32 Boards > uSolder Skipper*.

### From git

To work on the package itself, install the STM32 core as above, clone the
repository and link this folder into your sketchbook as
`hardware/usolder/stm32`.

Windows:

```bat
git clone https://github.com/uSolder/Skipper.git
mkdir "%USERPROFILE%\Documents\Arduino\hardware\usolder"
mklink /J "%USERPROFILE%\Documents\Arduino\hardware\usolder\stm32" "%CD%\Skipper\Software\STM32duino"
```

Linux / macOS:

```sh
git clone https://github.com/uSolder/Skipper.git
mkdir -p ~/Arduino/hardware/usolder
ln -s "$PWD/Skipper/Software/STM32duino" ~/Arduino/hardware/usolder/stm32
```

Use either this or the Boards Manager version, not both. A git install
doesn't get the `openocd-st` tool that the DAPLink upload method uses; the
DFU method works without it.

With arduino-cli the board is `usolder:stm32:skipper`:

```sh
arduino-cli compile -b usolder:stm32:skipper MySketch
```

## Uploading

Choose the method under *Tools > Upload method*.

| Method | Connector | How |
| --- | --- | --- |
| DAPLink (Debug port), default | **Debug port** | Programs the STM32H523 through the on-board DAPLink with OpenOCD. Just click Upload. |
| STM32CubeProgrammer (Serial, BOOT button) | **Debug port** | STM32 ROM bootloader on USART3 through the DAPLink COM port. Hold **BOOT**, press **RESET**, release BOOT, then upload. |
| STM32CubeProgrammer (DFU, BOOT button) | **USB DRP port** | ROM bootloader over USB. Hold **BOOT**, press **RESET**, release BOOT, then upload. Works without the DAPLink. |

The default method uses [ST's OpenOCD](../OpenOCD), which the Boards
Manager installs together with this package. The serial and DFU methods need
[STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html)
installed; press RESET after uploading with them to run the sketch.

## Debugging

*Sketch > Debug* in Arduino IDE 2 works through the same DAPLink and OpenOCD.
Select *Tools > Debug symbols and core logs > Symbols Enabled (-g)* and
*Tools > Optimize > Debug (-Og)* first for useful breakpoints and variables.

## Serial ports

| Object | Peripheral | Pins | Where |
| --- | --- | --- | --- |
| `Serial` | USART3 | TX PD8, RX PD9 | Debug port (DAPLink virtual COM port) |
| `Serial1` | USART1 | TX PA9, RX PA10 | UART Header 1 (J14) |
| `Serial2` | USART2 | TX PD5, RX PD6 | UART Header 2 (J15) |
| `SerialUSB` | USB FS | PA11/PA12 | USB DRP port, with *Tools > USB support > CDC* |

Choosing *USB support > CDC (generic 'Serial' supersede U(S)ART)* makes
`Serial` the USB-C DRP port instead of the DAPLink COM port.

## Pin map

Each user header is 1x10: pin 1 is 3.3V, pins 2-9 are the signals HxP1-HxP8,
pin 10 is GND. Arduino pin numbers count along the headers, so
`SKIPPER_HEADER_PIN(h, p)` is the pin for signal HhPp.

| Signal | H1 (J2) | H2 (J3) | H3 (J4) | H4 (J5) | H5 (J6) | H6 (J9) | H7 (J10) | H8 (J11) |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| P1 (pin 2) | D0 PA4 A0 | D8 PB2 LED | D16 PE0 | D24 PB7 | D32 PD4 | D40 PD15 | D48 PE14 | D56 PC0 A8 |
| P2 (pin 3) | D1 PA3 A1 | D9 PF10 | D17 PF3 | D25 PB6 | D33 PD3 | D41 PD14 | D49 PE13 | D57 PB1 A9 |
| P3 (pin 4) | D2 PA2 A2 | D10 PF9 | D18 PF2 | D26 PB5 | D34 PA15 | D42 PD13 | D50 PE12 | D58 PB0 A10 |
| P4 (pin 5) | D3 PA1 A3 | D11 PF8 | D19 PE6 | D27 PB4 | D35 PC7 | D43 PD12 | D51 PE11 | D59 PC5 A11 |
| P5 (pin 6) | D4 PA0 A4 | D12 PF7 | D20 PE5 | D28 PG15 | D36 PC6 | D44 PD11 | D52 PE10 | D60 PC4 A12 |
| P6 (pin 7) | D5 PC3 A5 | D13 PF6 | D21 PE4 | D29 PG14 | D37 PG8 | D45 PD10 | D53 PE9 | D61 PA7 A13 |
| P7 (pin 8) | D6 PC2 A6 | D14 PF5 | D22 PE3 | D30 PG13 | D38 PG7 | D46 PB10 | D54 PE8 | D62 PA6 A14 |
| P8 (pin 9) | D7 PC1 A7 | D15 PF4 | D23 PE2 | D31 PG12 | D39 PG6 | D47 PE15 | D55 PE7 | D63 PA5 A15 |

PB2 (H2P1) also drives the red user LED, `LED_BUILTIN`.

### On-board functions

| Function | Pins | Arduino names |
| --- | --- | --- |
| I2C1, `Wire` (J12, J16) | SCL PB8, SDA PB9, 1.5k pull-ups | `PIN_WIRE_SCL`, `PIN_WIRE_SDA` |
| I2C2 (J13, J17) | SCL PF1, SDA PF0, 1.5k pull-ups | `PIN_WIRE1_SCL`, `PIN_WIRE1_SDA` |
| SPI1, `SPI` (J7, J16) | CS PG10, SCK PG11, MISO PG9, MOSI PD7 | `PIN_SPI_SS`, `PIN_SPI_SCK`, ... |
| SPI2 (J8, J17) | CS PB12, SCK PB13, MISO PB14, MOSI PB15 | `PIN_SPI2_SS`, `PIN_SPI2_SCK`, ... |
| CAN (FDCAN1, TCAN1044V) | TX PD1, RX PD0 | `PIN_CAN_TX`, `PIN_CAN_RX` |
| microSD (SDMMC1, 4-bit) | D0-D3 PC8-PC11, CLK PC12, CMD PD2, detect PF11 | `SDX_*`, `SD_DETECT_PIN` |
| DIP switch SW1 (ON = HIGH) | PG0-PG5 | `DIP0`-`DIP5` |
| DAPLink USB-C has VBUS | PF12 | `USB_UFP_DETECT` |
| 5V buck (bus power) running | PF13 | `BUCK_DETECT` |
| USB-C host power fault (low) | PF14 | `USB_DRP_FAULT` |
| TUSB320 interrupt (low) | PF15 | `USB_DRP_INT` |
| TUSB320 USB-C controller | I2C1, address 0x47 | `TUSB320_I2C_ADDRESS` |
| USB FS (USB-C DRP) | DM PA11, DP PA12 | |
| SWD to DAPLink | SWDIO PA13, SWCLK PA14, SWO PB3 | |
| MCO1 (TP1) | PA8 | |

Second I2C and SPI buses:

```cpp
TwoWire Wire1(PIN_WIRE1_SDA, PIN_WIRE1_SCL);
SPIClass SPI_2(PIN_SPI2_MOSI, PIN_SPI2_MISO, PIN_SPI2_SCK, PIN_SPI2_SS);
```

The microSD slot works with the
[STM32duino STM32SD](https://github.com/stm32duino/STM32SD) library and
needs no pin setup.

Skipper has no user push button: SW2 is RESET and SW3 is BOOT0. Use the DIP
switch for user input.

## Clocks

`SystemClock_Config()` runs the MCU at 250 MHz from the 8 MHz crystal:

| Clock | Source | Frequency |
| --- | --- | --- |
| SYSCLK, HCLK, PCLK1-3 | PLL1P | 250 MHz |
| SDMMC1, SPI1-3 | PLL1Q | 50 MHz |
| FDCAN | PLL2Q | 80 MHz |
| ADC/DAC | PLL2R | 60 MHz |
| USB | PLL3Q | 48 MHz (crystal accurate) |
| RNG | HSI48 | 48 MHz |

A sketch can replace the function by defining its own `SystemClock_Config()`
with `extern "C"` linkage.

## Examples

Under *File > Examples > Skipper*, using plain Arduino functions:

* **Blink**: the LED.
* **DipSwitch**: prints the DIP switch as a number.
* **PowerStatus**: shows which supplies are present.
* **SerialPorts**: connects UART header 1 to the Serial Monitor.

## CAN: the SkipperCAN library

`#include <SkipperCAN.h>` gives a `CAN` object for classic CAN and CAN-FD.
The bus needs a 120 ohm terminator at each end; Skipper has none on board.

```cpp
CAN.begin(500000);                    // classic CAN, 500 kbit/s
CAN.begin(500000, 2000000);           // CAN-FD with a 2 Mbit/s data phase

CAN.send(0x100, data, length);        // up to 8 bytes (64 with CAN-FD)
CAN.send(0x100, myVariable);          // a whole variable or struct

if (CAN.available()) {
  CanMessage msg = CAN.read();        // msg.id, msg.length, msg.data[]
}
```

Variables can also follow the bus with no CAN code in `loop()`:

```cpp
volatile MotorStatus motor;
volatile Settings settings;

CAN.listen(0x200, motor);               // motor is updated whenever 0x200 arrives
CAN.listen(0x201, settings, onChange);  // ...and onChange() is called
CAN.broadcast(0x300, status, 100);      // status is sent every 100 ms
```

* A function given to `listen()` runs inside an interrupt: keep it short,
  with no `delay()`, `Serial` or waiting.
* `CAN.updated(id)` is true once after new data arrived; `CAN.age(id)` is
  the milliseconds since the last message, to notice a silent sender.
* Reading a struct while a message arrives can mix old and new values. Use
  `CAN.copy(id, localCopy)` for a consistent snapshot. When changing several
  fields of a broadcast variable that belong together, do it between
  `noInterrupts()` and `interrupts()`.
* Messages for `listen()` IDs don't appear in `available()` / `read()`.
* Structs are copied byte for byte, so both ends need the same layout. Two
  Skippers always match; for other devices, mind byte order and padding.
* Also: `listenExtended`, `broadcastExtended`, `sendExtended` for 29-bit
  IDs, `stopListening(id)`, `stopBroadcast(id)`, `filter(id, mask)`,
  `onReceive(function)`, `end()`. Up to 16 `listen()` and 16 `broadcast()`
  IDs. Periodic broadcasts run from the 1 ms SysTick interrupt.

Examples under *File > Examples > SkipperCAN*: **CANSend** / **CANReceive**,
and **CANBroadcast** / **CANListen**. Load one of each pair on two boards.

## Layout

```text
boards.txt                  board definition (references STMicroelectronics:arduino)
platform.txt                platform name and version only
variants/SKIPPER/           pin map, clock setup and linker script
libraries/Skipper/          getting-started examples
libraries/SkipperCAN/       CAN and CAN-FD library
tools/package.py            builds the Boards Manager archive and index
```

`variants/SKIPPER/PeripheralPins.c`, `PinNamesVar.h` and `ldscript.ld` are
copied unchanged from the STM32duino 3.1.0 STM32H523 variants
(`H523Z(C-E)(J-T)_H533ZE(J-T)` and `H523R(C-E)T_H533RET`).

## Releasing

1. Set `version=` in `platform.txt`, e.g. `0.1.0-beta.2`.
2. Commit, then tag and push from the repository root:

   ```sh
   git tag stm32duino-v0.1.0-beta.2
   git push origin stm32duino-v0.1.0-beta.2
   ```

The release workflow builds the archive, attaches it to a GitHub release and
adds the version to `package_usolder_index.json` on the `gh-pages` branch.
Versions with a suffix such as `-beta.2`, and anything below 1.0.0, are
marked as pre-releases.

GitHub Pages has to be enabled once, serving the `gh-pages` branch, after
the first release has created it.

## License

BSD 3-Clause, the same as the STM32duino core. See [LICENSE](LICENSE).
