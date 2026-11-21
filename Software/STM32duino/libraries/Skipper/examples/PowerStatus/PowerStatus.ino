// Reports where Skipper is getting power from, and the USB-C status lines.
// Open the Serial Monitor on the DAPLink COM port at 115200 baud.

#include <Skipper.h>

void setup()
{
  Serial.begin(115200);
  Skipper::begin();
}

void loop()
{
  Serial.print("DAPLink USB: ");
  Serial.print(Skipper::usbPowered() ? "yes" : "no");
  Serial.print("  Bus (7-36V): ");
  Serial.print(Skipper::busPowered() ? "yes" : "no");
  Serial.print("  USB host fault: ");
  Serial.print(Skipper::usbHostFault() ? "YES" : "no");
  Serial.print("  TUSB320 INT: ");
  Serial.println(Skipper::usbCInterrupt() ? "active" : "idle");
  delay(1000);
}
