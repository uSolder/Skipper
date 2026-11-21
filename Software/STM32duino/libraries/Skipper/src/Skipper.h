/*
 * Skipper board helpers.
 *
 * The pin names used here (DIP0-DIP5, USB_UFP_DETECT, ...) are defined by
 * the Skipper variant, so this header only builds for the Skipper board.
 */
#pragma once

#include <Arduino.h>

#if !defined(ARDUINO_SKIPPER)
  #error "Skipper.h needs the 'uSolder Skipper' board selected."
#endif

namespace Skipper {

// Configure the DIP switch and status lines as inputs. All of them have
// external pull resistors, so no internal pulls are enabled.
inline void begin()
{
  const uint32_t inputs[] = {
    DIP0, DIP1, DIP2, DIP3, DIP4, DIP5,
    USB_UFP_DETECT, BUCK_DETECT, USB_DRP_FAULT, USB_DRP_INT
  };
  for (uint32_t pin : inputs) {
    pinMode(pin, INPUT);
  }
}

// DIP switch SW1 as a number 0-63: switch 1 (DIP0) is bit 0, ON = 1.
inline uint8_t dipSwitch()
{
  const uint32_t dips[DIP_COUNT] = { DIP0, DIP1, DIP2, DIP3, DIP4, DIP5 };
  uint8_t value = 0;
  for (uint8_t i = 0; i < DIP_COUNT; i++) {
    if (digitalRead(dips[i]) == HIGH) {
      value |= (uint8_t)(1U << i);
    }
  }
  return value;
}

// True when the DAPLink USB-C connector (UFP) has VBUS.
inline bool usbPowered()
{
  return digitalRead(USB_UFP_DETECT) == HIGH;
}

// True when the 5V buck converter, fed from the bus connector, is running.
inline bool busPowered()
{
  return digitalRead(BUCK_DETECT) == HIGH;
}

// True while the USB-C DRP host power switch reports a fault.
inline bool usbHostFault()
{
  return digitalRead(USB_DRP_FAULT) == LOW;
}

// True while the TUSB320 USB-C controller is asserting its interrupt.
inline bool usbCInterrupt()
{
  return digitalRead(USB_DRP_INT) == LOW;
}

} // namespace Skipper
