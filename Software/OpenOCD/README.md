# OpenOCD for Skipper

Builds of [ST's OpenOCD fork](https://github.com/STMicroelectronics/OpenOCD)
for Windows, macOS and Linux. Skipper uses them to upload and debug through
the on-board DAPLink (CMSIS-DAP) on the Debug port.

Why a separate build: the OpenOCD that comes with the Arduino STM32 core
(xPack) and upstream OpenOCD don't support the STM32H5 yet. ST's fork does;
it's the OpenOCD inside STM32CubeIDE.

The source is used as ST publishes it. Only the build is ours:

| File | |
| --- | --- |
| `versions.env` | which ST commit, libusb and hidapi to build |
| `build.sh` | builds one platform, libusb and hidapi linked in |
| `tool_entry.py` | writes the Boards Manager tool entry for a release |

## Platforms

| Archive | Built on |
| --- | --- |
| `win32-x64` | Linux, MinGW-w64 cross compiler |
| `linux-x64`, `linux-arm64` | Ubuntu 22.04 |
| `darwin-arm64` | macOS (Apple Silicon) |
| `darwin-x64` | macOS (Apple Silicon), cross-compiled |

Each archive has `bin/openocd` and `openocd/scripts/`, the same layout as
the xPack builds. CI tests every binary on its own platform.

## Releasing

1. Update `versions.env`.
2. Push a tag `openocd-v<OPENOCD_VERSION>`, e.g. `openocd-v0.12.0-cubeide-r7`.

The workflow builds, tests, and publishes a pre-release with the archives
and `openocd-st.json`, the entry the Arduino package index needs.

## License

OpenOCD is GPL-2.0. Every archive includes `COPYING` and `SOURCE.txt`,
which names the exact commit it was built from.
