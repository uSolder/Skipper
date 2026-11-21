// Connects UART header 1 (J14) to the Serial Monitor: what you type goes
// out of the header, and what comes into the header is printed.
//
// Serial  = the Debug port (Serial Monitor)
// Serial1 = UART header 1 (J14), Serial2 = UART header 2 (J15)

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
}

void loop() {
  if (Serial.available()) {
    Serial1.write(Serial.read());
  }
  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }
}
