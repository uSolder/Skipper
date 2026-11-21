// Classic CAN at 500 kbit/s on Skipper's bus connectors (J22/J23).
//
// Sends a frame with ID 0x100 once a second carrying a counter and the DIP
// switch value, and prints every frame received. Use the DIP switch to give
// each board a different value when testing two Skippers together.
//
// Wiring: the CAN bus needs a 120 ohm terminator at each end; Skipper has
// none on board. The transceiver runs from 5V, so power the board from USB
// or the bus connector.
//
// Uses the STM32 HAL FDCAN driver directly. The FDCAN kernel clock is 80 MHz
// (set in the Skipper variant): 80 MHz / 10 = 8 MHz, 16 time quanta per bit
// = 500 kbit/s, sample point at 87.5%.

#include <Skipper.h>

FDCAN_HandleTypeDef hfdcan;

static void failed(const char *what)
{
  Serial.print("CAN error: ");
  Serial.println(what);
  while (true) {
    digitalToggle(LED_BUILTIN);
    delay(100);
  }
}

static void canBegin()
{
  __HAL_RCC_FDCAN_CLK_ENABLE();
  pinmap_pinout(digitalPinToPinName(PIN_CAN_TX), PinMap_CAN_TD);
  pinmap_pinout(digitalPinToPinName(PIN_CAN_RX), PinMap_CAN_RD);

  hfdcan.Instance = FDCAN1;
  hfdcan.Init.ClockDivider = FDCAN_CLOCK_DIV1;
  hfdcan.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan.Init.AutoRetransmission = ENABLE;
  hfdcan.Init.TransmitPause = DISABLE;
  hfdcan.Init.ProtocolException = DISABLE;
  hfdcan.Init.NominalPrescaler = 10;
  hfdcan.Init.NominalSyncJumpWidth = 2;
  hfdcan.Init.NominalTimeSeg1 = 13;
  hfdcan.Init.NominalTimeSeg2 = 2;
  hfdcan.Init.DataPrescaler = 10;
  hfdcan.Init.DataSyncJumpWidth = 2;
  hfdcan.Init.DataTimeSeg1 = 13;
  hfdcan.Init.DataTimeSeg2 = 2;
  hfdcan.Init.StdFiltersNbr = 0;
  hfdcan.Init.ExtFiltersNbr = 0;
  hfdcan.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan) != HAL_OK) {
    failed("init");
  }

  // No filters: accept every data frame into RX FIFO 0, reject remote frames.
  if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan, FDCAN_ACCEPT_IN_RX_FIFO0, FDCAN_ACCEPT_IN_RX_FIFO0,
                                   FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK) {
    failed("filter");
  }
  if (HAL_FDCAN_Start(&hfdcan) != HAL_OK) {
    failed("start");
  }
}

static void canSend(uint32_t id, const uint8_t *data, uint8_t length)
{
  FDCAN_TxHeaderTypeDef header = {};
  header.Identifier = id;
  header.IdType = FDCAN_STANDARD_ID;
  header.TxFrameType = FDCAN_DATA_FRAME;
  header.DataLength = length;  // classic CAN: DLC 0-8 equals the byte count
  header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  header.BitRateSwitch = FDCAN_BRS_OFF;
  header.FDFormat = FDCAN_CLASSIC_CAN;
  header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  header.MessageMarker = 0;

  if (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan) == 0) {
    Serial.println("TX FIFO full (is anything else on the bus?)");
    return;
  }
  if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan, &header, data) != HAL_OK) {
    Serial.println("TX failed");
  }
}

static void canPoll()
{
  FDCAN_RxHeaderTypeDef header;
  uint8_t data[8];

  while (HAL_FDCAN_GetRxFifoFillLevel(&hfdcan, FDCAN_RX_FIFO0) > 0) {
    if (HAL_FDCAN_GetRxMessage(&hfdcan, FDCAN_RX_FIFO0, &header, data) != HAL_OK) {
      return;
    }
    uint8_t length = header.DataLength > 8 ? 8 : (uint8_t)header.DataLength;
    Serial.print("RX 0x");
    Serial.print(header.Identifier, HEX);
    Serial.print(" [");
    Serial.print(length);
    Serial.print("]");
    for (uint8_t i = 0; i < length; i++) {
      Serial.print(' ');
      if (data[i] < 0x10) {
        Serial.print('0');
      }
      Serial.print(data[i], HEX);
    }
    Serial.println();
    digitalToggle(LED_BUILTIN);
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  Skipper::begin();
  canBegin();
  Serial.println("Skipper CAN, 500 kbit/s");
}

void loop()
{
  static uint32_t lastSend = 0;
  static uint8_t counter = 0;

  canPoll();

  if (millis() - lastSend >= 1000) {
    lastSend = millis();
    uint8_t data[2] = { counter++, Skipper::dipSwitch() };
    canSend(0x100, data, sizeof(data));
  }
}
