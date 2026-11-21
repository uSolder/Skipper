// Sends the DIP switch and a counter every 100 ms. There's no CAN code in
// loop(): CAN.broadcast() sends the current value of `status` by itself.
// Run CANListen on the other Skipper.

#include <SkipperCAN.h>

struct Status {
  byte dip;
  byte counter;
};

volatile Status status;

const int dipPins[] = { DIP0, DIP1, DIP2, DIP3, DIP4, DIP5 };

void setup() {
  for (int i = 0; i < 6; i++) {
    pinMode(dipPins[i], INPUT);
  }

  CAN.begin(500000);
  CAN.broadcast(0x200, status, 100);  // send status every 100 ms
}

void loop() {
  byte dip = 0;
  for (int i = 0; i < 6; i++) {
    if (digitalRead(dipPins[i]) == HIGH) {
      dip += 1 << i;
    }
  }

  status.dip = dip;
  status.counter++;
  delay(100);
}
