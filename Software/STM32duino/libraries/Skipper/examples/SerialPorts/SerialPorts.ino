// Bridges the UART headers to the computer.
//
//   Serial   USART3, the DAPLink virtual COM port (USB-C UFP connector)
//   Serial1  USART1 on UART Header 1 (J14): pin 4 = TX, pin 5 = RX
//   Serial2  USART2 on UART Header 2 (J15): pin 4 = TX, pin 5 = RX
//
// Text typed in the Serial Monitor goes out of both headers; anything
// received on a header is printed with its port name.

Uart Serial1(PIN_SERIAL1_RX, PIN_SERIAL1_TX);
Uart Serial2(PIN_SERIAL2_RX, PIN_SERIAL2_TX);

void forward(Stream &from, const char *name)
{
  while (from.available()) {
    char c = from.read();
    Serial.print(name);
    Serial.print(": ");
    Serial.println(c);
  }
}

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  Serial.println("Skipper serial bridge");
}

void loop()
{
  while (Serial.available()) {
    char c = Serial.read();
    Serial1.write(c);
    Serial2.write(c);
  }
  forward(Serial1, "UART1");
  forward(Serial2, "UART2");
}
