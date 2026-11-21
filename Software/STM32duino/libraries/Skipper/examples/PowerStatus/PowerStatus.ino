// Shows where Skipper is getting power from.
// Open the Serial Monitor at 115200 baud.

void setup() {
  Serial.begin(115200);
  pinMode(USB_UFP_DETECT, INPUT);  // Debug port has power
  pinMode(BUCK_DETECT, INPUT);     // 7-36 V terminal input has power
}

void loop() {
  Serial.print("Debug port: ");
  Serial.print(digitalRead(USB_UFP_DETECT) == HIGH ? "yes" : "no");
  Serial.print("   Terminal input: ");
  Serial.println(digitalRead(BUCK_DETECT) == HIGH ? "yes" : "no");
  delay(1000);
}
