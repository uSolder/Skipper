// Receives the status sent by CANBroadcast. CAN.listen() keeps `status` up
// to date by itself whenever a message with ID 0x200 arrives; loop() just
// reads it. Open the Serial Monitor at 115200 baud.

#include <SkipperCAN.h>

struct Status {
  byte dip;
  byte counter;
};

volatile Status status;

// Called for every 0x200 message. It runs inside an interrupt, so keep it
// short: no delay(), no Serial, no waiting.
void messageArrived() {
  digitalToggle(LED_BUILTIN);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  CAN.begin(500000);
  CAN.listen(0x200, status, messageArrived);
}

void loop() {
  if (CAN.age(0x200) > 500) {
    Serial.println("Nothing from the other board");
  } else {
    Serial.print("DIP switch: ");
    Serial.print(status.dip);
    Serial.print("   counter: ");
    Serial.println(status.counter);
  }
  delay(500);
}
