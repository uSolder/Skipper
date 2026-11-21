// Sends a CAN message every second.
// Connect two Skippers with a 120 ohm terminator at each end of the bus,
// and run CANReceive on the other one.

#include <SkipperCAN.h>

byte counter = 0;

void setup() {
  CAN.begin(500000);  // 500 kbit/s
}

void loop() {
  byte data[] = { counter, 1, 2, 3 };
  CAN.send(0x100, data, sizeof(data));

  counter++;
  delay(1000);
}
