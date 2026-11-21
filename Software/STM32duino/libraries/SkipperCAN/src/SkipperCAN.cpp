/*
 * SkipperCAN - CAN and CAN-FD for the uSolder Skipper.
 *
 * Uses FDCAN1 on PD1 (TX) / PD0 (RX), wired to the TCAN1044V transceiver.
 * The FDCAN kernel clock comes from the Skipper variant (80 MHz).
 *
 * The STM32H5's receive FIFO only holds 3 messages, so received messages
 * are moved into a larger buffer from the interrupt as they arrive.
 */
#include "SkipperCAN.h"

SkipperCAN CAN;

static FDCAN_HandleTypeDef hfdcan;

// Data length code -> number of bytes.
static const uint8_t dlcBytes[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64 };

static uint32_t lengthToDlc(uint8_t length)
{
  for (uint32_t dlc = 0; dlc < 16; dlc++) {
    if (dlcBytes[dlc] >= length) {
      return dlc;
    }
  }
  return 15;
}

struct BitTiming {
  uint32_t prescaler;
  uint32_t seg1;
  uint32_t seg2;
};

// Find a prescaler and segment split that gives exactly `rate` from
// `clock`, with as many time quanta per bit as the hardware allows and the
// sample point close to `samplePoint` (per mille).
static bool findTiming(uint32_t clock, uint32_t rate, uint32_t maxPrescaler, uint32_t maxSeg1,
                       uint32_t maxSeg2, uint32_t samplePoint, BitTiming &timing)
{
  if (rate == 0) {
    return false;
  }
  for (uint32_t prescaler = 1; prescaler <= maxPrescaler; prescaler++) {
    if (clock % (prescaler * rate) != 0) {
      continue;
    }
    uint32_t quanta = clock / (prescaler * rate);
    if (quanta < 4 || quanta > 1 + maxSeg1 + maxSeg2) {
      continue;
    }
    uint32_t seg2 = (quanta * (1000 - samplePoint) + 500) / 1000;
    seg2 = constrain(seg2, 1UL, maxSeg2);
    uint32_t seg1 = quanta - 1 - seg2;
    if (seg1 < 1 || seg1 > maxSeg1) {
      continue;
    }
    timing = { prescaler, seg1, seg2 };
    return true;
  }
  return false;
}

bool SkipperCAN::begin(uint32_t bitRate)
{
  return begin(bitRate, 0);
}

bool SkipperCAN::begin(uint32_t bitRate, uint32_t dataBitRate)
{
  end();
  nominalRate = bitRate;
  dataRate = dataBitRate;
  fdMode = dataBitRate != 0;

  __HAL_RCC_FDCAN_CLK_ENABLE();
  pinmap_pinout(digitalPinToPinName(PIN_CAN_TX), PinMap_CAN_TD);
  pinmap_pinout(digitalPinToPinName(PIN_CAN_RX), PinMap_CAN_RD);

  return start();
}

bool SkipperCAN::start()
{
  uint32_t clock = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_FDCAN);

  // Nominal phase: 87.5 % sample point, the usual value for CAN.
  BitTiming nominal;
  if (!findTiming(clock, nominalRate, 512, 256, 128, 875, nominal)) {
    return false;
  }

  // Data phase (CAN-FD only): 75 % sample point.
  BitTiming data = { 1, 1, 1 };
  bool bitRateSwitch = fdMode && dataRate > nominalRate;
  if (bitRateSwitch && !findTiming(clock, dataRate, 32, 32, 16, 750, data)) {
    return false;
  }

  hfdcan.Instance = FDCAN1;
  hfdcan.Init.ClockDivider = FDCAN_CLOCK_DIV1;
  hfdcan.Init.FrameFormat = !fdMode ? FDCAN_FRAME_CLASSIC
                            : bitRateSwitch ? FDCAN_FRAME_FD_BRS : FDCAN_FRAME_FD_NO_BRS;
  hfdcan.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan.Init.AutoRetransmission = ENABLE;
  hfdcan.Init.TransmitPause = DISABLE;
  hfdcan.Init.ProtocolException = DISABLE;
  hfdcan.Init.NominalPrescaler = nominal.prescaler;
  hfdcan.Init.NominalTimeSeg1 = nominal.seg1;
  hfdcan.Init.NominalTimeSeg2 = nominal.seg2;
  hfdcan.Init.NominalSyncJumpWidth = nominal.seg2;
  hfdcan.Init.DataPrescaler = data.prescaler;
  hfdcan.Init.DataTimeSeg1 = data.seg1;
  hfdcan.Init.DataTimeSeg2 = data.seg2;
  hfdcan.Init.DataSyncJumpWidth = data.seg2;
  hfdcan.Init.StdFiltersNbr = 1;
  hfdcan.Init.ExtFiltersNbr = 1;
  hfdcan.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan) != HAL_OK) {
    return false;
  }

  // Fast data phases need the transceiver's delay compensated.
  if (bitRateSwitch && dataRate >= 1000000) {
    uint32_t offset = min(data.prescaler * data.seg1, 127UL);
    HAL_FDCAN_ConfigTxDelayCompensation(&hfdcan, offset, 0);
    HAL_FDCAN_EnableTxDelayCompensation(&hfdcan);
  }

  if (!applyFilter()) {
    return false;
  }

  HAL_FDCAN_ActivateNotification(&hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE | FDCAN_IT_BUS_OFF, 0);
  HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);

  if (HAL_FDCAN_Start(&hfdcan) != HAL_OK) {
    return false;
  }
  running = true;
  return true;
}

void SkipperCAN::end()
{
  if (running) {
    HAL_NVIC_DisableIRQ(FDCAN1_IT0_IRQn);
    HAL_FDCAN_Stop(&hfdcan);
    HAL_FDCAN_DeInit(&hfdcan);
    running = false;
  }
  hfdcan.Instance = nullptr;
  head = 0;
  tail = 0;
  dropped = 0;
}

bool SkipperCAN::send(uint32_t id, const uint8_t *data, uint8_t length)
{
  return id <= 0x7FF && sendFrame(id, false, data, length, true);
}

bool SkipperCAN::sendExtended(uint32_t id, const uint8_t *data, uint8_t length)
{
  return id <= 0x1FFFFFFF && sendFrame(id, true, data, length, true);
}

bool SkipperCAN::send(const CanMessage &message)
{
  return message.extended ? sendExtended(message.id, message.data, message.length)
                          : send(message.id, message.data, message.length);
}

bool SkipperCAN::sendFrame(uint32_t id, bool extended, const uint8_t *data, uint8_t length, bool wait)
{
  if (!running || length > (fdMode ? 64 : 8) || (length > 0 && data == nullptr)) {
    return false;
  }
  recoverFromBusOff();

  // Messages wait in the queue until another node acknowledges them. From
  // loop(), give a busy bus a moment, but don't hang if nobody is listening.
  // From an interrupt, never wait.
  if (wait) {
    uint32_t start = millis();
    while (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan) == 0) {
      if (millis() - start > 10) {
        return false;
      }
    }
  }

  // CAN-FD lengths above 8 come in fixed sizes; pad with zeros.
  uint8_t frame[64] = {};
  memcpy(frame, data, length);

  FDCAN_TxHeaderTypeDef header = {};
  header.Identifier = id;
  header.IdType = extended ? FDCAN_EXTENDED_ID : FDCAN_STANDARD_ID;
  header.TxFrameType = FDCAN_DATA_FRAME;
  header.DataLength = lengthToDlc(length);
  header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  header.BitRateSwitch = (fdMode && dataRate > nominalRate) ? FDCAN_BRS_ON : FDCAN_BRS_OFF;
  header.FDFormat = fdMode ? FDCAN_FD_CAN : FDCAN_CLASSIC_CAN;
  header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  header.MessageMarker = 0;

  // loop() and the periodic broadcasts (SysTick interrupt) both add to
  // the same queue; don't let them pick the same slot.
  uint32_t primask = __get_PRIMASK();
  __disable_irq();
  bool ok = HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan) > 0
            && HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan, &header, frame) == HAL_OK;
  __set_PRIMASK(primask);
  return ok;
}

// ---- Variables filled by listen() ----------------------------------------------

SkipperCAN::Listener *SkipperCAN::findListener(uint32_t id)
{
  for (Listener &a : listeners) {
    if (a.used && a.id == id) {
      return &a;
    }
  }
  return nullptr;
}

bool SkipperCAN::listenVariable(uint32_t id, bool extended, volatile void *variable, uint8_t size,
                                void (*callback)())
{
  if (id > (extended ? 0x1FFFFFFFUL : 0x7FFUL)) {
    return false;
  }
  noInterrupts();
  Listener *slot = nullptr;
  for (Listener &a : listeners) {
    if (a.used && a.id == id && a.extended == extended) {
      slot = &a;  // listening again replaces the old one
      break;
    }
    if (!a.used && slot == nullptr) {
      slot = &a;
    }
  }
  if (slot != nullptr) {
    slot->extended = extended;
    slot->id = id;
    slot->variable = variable;
    slot->size = size;
    slot->callback = callback;
    slot->updated = false;
    slot->seen = false;
    slot->lastTime = 0;
    slot->used = true;
  }
  interrupts();
  return slot != nullptr;
}

bool SkipperCAN::stopListening(uint32_t id)
{
  noInterrupts();
  Listener *a = findListener(id);
  if (a != nullptr) {
    a->used = false;
  }
  interrupts();
  return a != nullptr;
}

bool SkipperCAN::updated(uint32_t id)
{
  Listener *a = findListener(id);
  if (a == nullptr || !a->updated) {
    return false;
  }
  a->updated = false;
  return true;
}

uint32_t SkipperCAN::age(uint32_t id)
{
  Listener *a = findListener(id);
  if (a == nullptr || !a->seen) {
    return 0xFFFFFFFF;
  }
  return millis() - a->lastTime;
}

bool SkipperCAN::copyVariable(uint32_t id, void *destination, uint8_t size)
{
  noInterrupts();
  Listener *a = findListener(id);
  if (a != nullptr) {
    memcpy(destination, (const void *)a->variable, min(size, a->size));
  }
  interrupts();
  return a != nullptr;
}

// ---- Periodic broadcasts -------------------------------------------------

bool SkipperCAN::broadcastVariable(uint32_t id, bool extended, const volatile void *variable, uint8_t size,
                                  uint32_t intervalMs)
{
  if (id > (extended ? 0x1FFFFFFFUL : 0x7FFUL) || intervalMs == 0) {
    return false;
  }
  noInterrupts();
  Broadcast *slot = nullptr;
  for (Broadcast &t : broadcasts) {
    if (t.used && t.id == id && t.extended == extended) {
      slot = &t;
      break;
    }
    if (!t.used && slot == nullptr) {
      slot = &t;
    }
  }
  if (slot != nullptr) {
    slot->extended = extended;
    slot->id = id;
    slot->variable = variable;
    slot->size = size;
    slot->interval = intervalMs;
    slot->lastTime = millis() - intervalMs;  // send the first one right away
    slot->used = true;
  }
  interrupts();
  return slot != nullptr;
}

bool SkipperCAN::stopBroadcast(uint32_t id)
{
  bool found = false;
  noInterrupts();
  for (Broadcast &t : broadcasts) {
    if (t.used && t.id == id) {
      t.used = false;
      found = true;
    }
  }
  interrupts();
  return found;
}

bool SkipperCAN::sendVariable(uint32_t id, bool extended, const volatile void *variable, uint8_t size)
{
  uint8_t data[64];
  noInterrupts();
  memcpy(data, (const void *)variable, size);
  interrupts();
  return extended ? sendExtended(id, data, size) : send(id, data, size);
}

// Every millisecond, from the SysTick interrupt.
void SkipperCAN::handleTick()
{
  if (!running) {
    return;
  }
  uint32_t now = HAL_GetTick();
  for (Broadcast &t : broadcasts) {
    if (!t.used || now - t.lastTime < t.interval) {
      continue;
    }
    uint8_t data[64];
    memcpy(data, (const void *)t.variable, t.size);
    // If the queue is full, try again next millisecond. Keep the rhythm
    // unless we fell more than a whole interval behind.
    if (sendFrame(t.id, t.extended, data, t.size, false)) {
      t.lastTime = (now - t.lastTime >= 2 * t.interval) ? now : t.lastTime + t.interval;
    }
  }
}

int SkipperCAN::available()
{
  recoverFromBusOff();
  return (head - tail + bufferSize) % bufferSize;
}

CanMessage SkipperCAN::read()
{
  CanMessage message;
  if (head != tail) {
    message = buffer[tail];
    tail = (tail + 1) % bufferSize;
  }
  return message;
}

bool SkipperCAN::filter(uint32_t id, uint32_t mask)
{
  filterType = 1;
  filterId = id & 0x7FF;
  filterMask = mask & 0x7FF;
  return applyFilter();
}

bool SkipperCAN::filterExtended(uint32_t id, uint32_t mask)
{
  filterType = 2;
  filterId = id & 0x1FFFFFFF;
  filterMask = mask & 0x1FFFFFFF;
  return applyFilter();
}

bool SkipperCAN::filter()
{
  filterType = 0;
  return applyFilter();
}

bool SkipperCAN::applyFilter()
{
  if (hfdcan.Instance == nullptr) {
    return true;  // not started yet: begin() applies it
  }
  if (running) {
    HAL_FDCAN_Stop(&hfdcan);
  }

  bool ok;
  if (filterType == 0) {
    ok = HAL_FDCAN_ConfigGlobalFilter(&hfdcan, FDCAN_ACCEPT_IN_RX_FIFO0, FDCAN_ACCEPT_IN_RX_FIFO0,
                                      FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) == HAL_OK;
  } else {
    FDCAN_FilterTypeDef config = {};
    config.IdType = filterType == 2 ? FDCAN_EXTENDED_ID : FDCAN_STANDARD_ID;
    config.FilterIndex = 0;
    config.FilterType = FDCAN_FILTER_MASK;
    config.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    config.FilterID1 = filterId;
    config.FilterID2 = filterMask;
    ok = HAL_FDCAN_ConfigFilter(&hfdcan, &config) == HAL_OK
         && HAL_FDCAN_ConfigGlobalFilter(&hfdcan, FDCAN_REJECT, FDCAN_REJECT,
                                         FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) == HAL_OK;
  }

  if (running) {
    HAL_FDCAN_Start(&hfdcan);
  }
  return ok;
}

void SkipperCAN::onReceive(void (*callback)(const CanMessage &message))
{
  receiveCallback = callback;
}

uint32_t SkipperCAN::droppedMessages() const
{
  return dropped;
}

void SkipperCAN::recoverFromBusOff()
{
  // After too many errors the controller goes bus-off and stops. Restart
  // it; the hardware waits the required 128 x 11 recessive bits itself.
  if (running && (FDCAN1->PSR & FDCAN_PSR_BO) && (FDCAN1->CCCR & FDCAN_CCCR_INIT)) {
    FDCAN1->CCCR &= ~FDCAN_CCCR_INIT;
  }
}

void SkipperCAN::handleInterrupt()
{
  FDCAN_RxHeaderTypeDef header;
  uint8_t data[64];

  while (HAL_FDCAN_GetRxFifoFillLevel(&hfdcan, FDCAN_RX_FIFO0) > 0) {
    if (HAL_FDCAN_GetRxMessage(&hfdcan, FDCAN_RX_FIFO0, &header, data) != HAL_OK) {
      return;
    }
    uint32_t id = header.Identifier;
    bool extended = header.IdType == FDCAN_EXTENDED_ID;
    uint8_t length = dlcBytes[header.DataLength & 0x0F];

    // A listen() ID: update the variable, don't queue the message.
    bool listened = false;
    for (Listener &a : listeners) {
      if (a.used && a.id == id && a.extended == extended) {
        memcpy((void *)a.variable, data, min(length, a.size));
        a.lastTime = HAL_GetTick();
        a.seen = true;
        a.updated = true;
        if (a.callback != nullptr) {
          a.callback();
        }
        listened = true;
        break;
      }
    }
    if (listened) {
      continue;
    }

    int next = (head + 1) % bufferSize;
    if (next == tail) {
      dropped++;
      continue;
    }

    CanMessage &message = buffer[head];
    message.id = id;
    message.extended = extended;
    message.fd = header.FDFormat == FDCAN_FD_CAN;
    message.length = length;
    memcpy(message.data, data, length);
    head = next;

    if (receiveCallback != nullptr) {
      receiveCallback(message);
    }
  }
}

extern "C" {

void FDCAN1_IT0_IRQHandler(void)
{
  HAL_FDCAN_IRQHandler(&hfdcan);
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *handle, uint32_t interrupts)
{
  (void)interrupts;
  if (handle->Instance == FDCAN1) {
    CAN.handleInterrupt();
  }
}

void HAL_FDCAN_ErrorStatusCallback(FDCAN_HandleTypeDef *handle, uint32_t interrupts)
{
  if (handle->Instance == FDCAN1 && (interrupts & FDCAN_IT_BUS_OFF)) {
    FDCAN1->CCCR &= ~FDCAN_CCCR_INIT;
  }
}

// STM32duino calls this from the SysTick interrupt every millisecond.
void HAL_SYSTICK_Callback(void)
{
  CAN.handleTick();
}

}
