// Prints every CAN message it receives.
// Open the Serial Monitor at 115200 baud.

#include <SkipperCAN.h>

void setup() {
  Serial.begin(115200);
  CAN.begin(500000);  // 500 kbit/s
}

void loop() {
  if (CAN.available()) {
    CanMessage msg = CAN.read();

    Serial.print("ID 0x");
    Serial.print(msg.id, HEX);
    Serial.print(":");
    for (int i = 0; i < msg.length; i++) {
      Serial.print(" ");
      Serial.print(msg.data[i]);
    }
    Serial.println();
  }
}
