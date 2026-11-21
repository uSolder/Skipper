# DAPLink firmware for Skipper

Skipper's on-board debugger is an STM32F103CBT6 wired with the standard
DAPLink `stm32f103xb` pinout. These are stock DAPLink images for it:

| File | What | Flash address |
| --- | --- | --- |
| `skipper_daplink_bootloader.hex` | DAPLink bootloader (`stm32f103xb_bl`) | 0x08000000 |
| `skipper_daplink_interface.hex` | DAPLink interface (`stm32f103xb_if`) | 0x0800C000 |
| `skipper_daplink_interface.bin` | Same interface, for updating through the MAINTENANCE drive | |

Built from [DAPLink](https://github.com/ARMmbed/DAPLink) v0258-39-gca1bfb28
with GNU Arm Embedded Toolchain 10.3-2021.10.

With this firmware the UFP USB-C port gives you:

* CMSIS-DAP debugging of the STM32H523 (OpenOCD, pyOCD, STM32CubeIDE)
* A virtual COM port connected to USART3 (PD8/PD9)

Not supported yet:

* Drag-and-drop programming. The DAPLink drive appears, but this interface
  has no STM32H5 flash algorithm, so copying a file to it will not program
  the H523. Use DFU or a debugger instead.
* SWO trace.

## Flashing a new board

You need an ST-LINK (or any SWD probe) and
[STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html).

J21 (DAP programming header):

| Pin | Signal |
| --- | --- |
| 1 | 3.3V (reference only, do not power the board from it) |
| 2 | SWCLK |
| 3 | GND |
| 4 | SWDIO |
| 5 | NRST |

1. Plug a USB cable into the **UFP** USB-C port. The F103 is held in reset
   while that port has no power, so it has to be connected while flashing.
2. Connect the ST-LINK to J21.
3. Flash both files:

   ```bat
   STM32_Programmer_CLI -c port=SWD -e all
   STM32_Programmer_CLI -c port=SWD -w skipper_daplink_bootloader.hex -v
   STM32_Programmer_CLI -c port=SWD -w skipper_daplink_interface.hex -v -rst
   ```

   In the STM32CubeProgrammer GUI: connect, *Full chip erase*, then open and
   download each .hex file.

4. Disconnect the ST-LINK and replug the UFP cable. A `DAPLINK` drive and a
   COM port should appear.

## Updating the interface

1. Hold the **RESET** button and plug in the UFP USB-C cable. A
   `MAINTENANCE` drive appears.
2. Copy `skipper_daplink_interface.bin` to it. The board restarts with the
   new firmware.

## Rebuilding

From a DAPLink checkout, following its
[developers guide](https://github.com/ARMmbed/DAPLink/blob/main/docs/DEVELOPERS-GUIDE.md):

```sh
progen generate -t cmake_gcc_arm -o generator=ninja -p stm32f103xb_bl -b
progen generate -t cmake_gcc_arm -o generator=ninja -p stm32f103xb_if -b
```

Use the `*_crc.hex` / `*_crc.bin` files from
`projectfiles/cmake_gcc_arm/<project>/build`.

On Windows the build breaks if the checkout path contains spaces. Map it to
a drive letter first (`subst R: "C:\path with spaces\DAPLink"`).
