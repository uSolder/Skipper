// Prints the 6-position DIP switch as a number (0-63) whenever it changes.
// Open the Serial Monitor at 115200 baud.

const int dipPins[] = { DIP0, DIP1, DIP2, DIP3, DIP4, DIP5 };
int lastValue = -1;

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 6; i++) {
    pinMode(dipPins[i], INPUT);
  }
}

void loop() {
  int value = 0;
  for (int i = 0; i < 6; i++) {
    if (digitalRead(dipPins[i]) == HIGH) {
      value += 1 << i;
    }
  }

  if (value != lastValue) {
    Serial.print("DIP switch: ");
    Serial.println(value);
    lastValue = value;
  }
  delay(20);
}
