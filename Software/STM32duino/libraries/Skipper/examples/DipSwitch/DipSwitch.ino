// Prints the 6-bit DIP switch (SW1) whenever it changes.
// Open the Serial Monitor on the DAPLink COM port at 115200 baud.

#include <Skipper.h>

uint8_t last = 0xFF;

void setup()
{
  Serial.begin(115200);
  Skipper::begin();
}

void loop()
{
  uint8_t value = Skipper::dipSwitch();
  if (value != last) {
    last = value;
    Serial.print("DIP switch: ");
    for (int bit = DIP_COUNT - 1; bit >= 0; bit--) {
      Serial.print((value >> bit) & 1);
    }
    Serial.print(" = ");
    Serial.println(value);
  }
  delay(20);
}
