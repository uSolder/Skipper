#!/usr/bin/env bash
# Build ST's OpenOCD fork for one platform, with libusb and hidapi linked in.
#
#   ./build.sh <target> [output-dir]
#
# Targets: linux-x64, linux-arm64 (native), win32-x64 (cross-compiled on
# Linux with MinGW-w64), darwin-arm64 (native), darwin-x64 (cross-compiled
# on an Apple Silicon Mac).
#
# Writes openocd-st-<version>-<target>.tar.gz (.zip for Windows).

set -euo pipefail

TARGET=${1:?usage: build.sh <target> [output-dir]}
OUT=$(mkdir -p "${2:-dist}" && cd "${2:-dist}" && pwd)
HERE=$(cd "$(dirname "$0")" && pwd)

# shellcheck source=versions.env
source "$HERE/versions.env"

WORK=${WORK:-$PWD/work-$TARGET}
DEPS=$WORK/deps
STAGE=$WORK/openocd-st
JOBS=$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4)

HOST=""
CMAKE_EXTRA=()
LDFLAGS_EXTRA=""
EXE=""
case "$TARGET" in
  linux-x64|linux-arm64)
    # hidraw would need libudev; the libusb backend works without it.
    CMAKE_EXTRA=(-DHIDAPI_WITH_HIDRAW=OFF -DHIDAPI_WITH_LIBUSB=ON)
    ;;
  win32-x64)
    HOST=x86_64-w64-mingw32
    CMAKE_EXTRA=(-DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER=$HOST-gcc -DCMAKE_RC_COMPILER=$HOST-windres)
    LDFLAGS_EXTRA="-static -static-libgcc"
    EXE=.exe
    ;;
  darwin-arm64)
    export CFLAGS="-arch arm64 -mmacosx-version-min=11.0"
    export LDFLAGS="-arch arm64 -mmacosx-version-min=11.0"
    CMAKE_EXTRA=(-DCMAKE_OSX_ARCHITECTURES=arm64 -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0)
    ;;
  darwin-x64)
    HOST=x86_64-apple-darwin
    export CFLAGS="-arch x86_64 -mmacosx-version-min=10.15"
    export LDFLAGS="-arch x86_64 -mmacosx-version-min=10.15"
    CMAKE_EXTRA=(-DCMAKE_OSX_ARCHITECTURES=x86_64 -DCMAKE_OSX_DEPLOYMENT_TARGET=10.15)
    ;;
  *)
    echo "Unknown target: $TARGET" >&2
    exit 1
    ;;
esac
HOST_ARG=${HOST:+--host=$HOST}

rm -rf "$WORK"
mkdir -p "$WORK" "$DEPS"
cd "$WORK"

# Only use the libraries built below, linked statically.
export PKG_CONFIG="pkg-config --static"
export PKG_CONFIG_PATH="$DEPS/lib/pkgconfig:$DEPS/lib64/pkgconfig"
export PKG_CONFIG_LIBDIR="$PKG_CONFIG_PATH"

echo "== libusb $LIBUSB_VERSION"
curl -fsSL "https://github.com/libusb/libusb/releases/download/v$LIBUSB_VERSION/libusb-$LIBUSB_VERSION.tar.bz2" | tar -xj
(
  cd "libusb-$LIBUSB_VERSION"
  ./configure $HOST_ARG --prefix="$DEPS" --enable-static --disable-shared --disable-udev
  make -j"$JOBS"
  make install
)

echo "== hidapi $HIDAPI_VERSION"
curl -fsSL "https://github.com/libusb/hidapi/archive/refs/tags/hidapi-$HIDAPI_VERSION.tar.gz" | tar -xz
PKG_CONFIG=pkg-config cmake -S "hidapi-hidapi-$HIDAPI_VERSION" -B hidapi-build \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="$DEPS" \
  -DCMAKE_PREFIX_PATH="$DEPS" \
  -DBUILD_SHARED_LIBS=OFF \
  "${CMAKE_EXTRA[@]}"
cmake --build hidapi-build -j"$JOBS"
cmake --install hidapi-build

echo "== OpenOCD ($OPENOCD_REF)"
git clone --quiet "$OPENOCD_REPO" openocd
(
  cd openocd
  git checkout --quiet "$OPENOCD_COMMIT"
  git submodule update --init --quiet jimtcl
  ./bootstrap nosubmodule

  # bin/ and openocd/scripts/ side by side, the same layout as the xPack
  # builds, so OpenOCD finds its scripts relative to the executable.
  ./configure $HOST_ARG \
    --prefix="$STAGE" \
    --datarootdir="$STAGE" \
    --disable-werror \
    --disable-doxygen-html \
    --disable-doxygen-pdf \
    --enable-cmsis-dap \
    --enable-cmsis-dap-v2 \
    --enable-stlink \
    --disable-jlink \
    --disable-internal-libjaylink \
    --disable-linuxgpiod \
    --without-capstone \
    LDFLAGS="${LDFLAGS:-} $LDFLAGS_EXTRA"
  make -j"$JOBS"
  make install-strip
)

rm -rf "$STAGE/info" "$STAGE/man" "$STAGE/doc"
cp openocd/COPYING "$STAGE/COPYING"
cat > "$STAGE/SOURCE.txt" <<EOF
OpenOCD built from STMicroelectronics' fork, unmodified:

  $OPENOCD_REPO
  branch $OPENOCD_REF, commit $OPENOCD_COMMIT

OpenOCD is licensed under the GNU GPL v2 (see COPYING). The source for
this build is the commit above. Built with libusb $LIBUSB_VERSION and
hidapi $HIDAPI_VERSION, linked statically.
EOF

test -x "$STAGE/bin/openocd$EXE"
test -f "$STAGE/openocd/scripts/target/stm32h5x.cfg"

NAME="openocd-st-$OPENOCD_VERSION-$TARGET"
cd "$WORK"
if [ "$TARGET" = win32-x64 ]; then
  rm -f "$OUT/$NAME.zip"
  zip -qr "$OUT/$NAME.zip" openocd-st
  echo "$OUT/$NAME.zip"
else
  tar -czf "$OUT/$NAME.tar.gz" openocd-st
  echo "$OUT/$NAME.tar.gz"
fi
