/*
 * SkipperCAN - CAN and CAN-FD for the uSolder Skipper.
 *
 *   CAN.begin(500000);                   // classic CAN at 500 kbit/s
 *   CAN.begin(500000, 2000000);          // CAN-FD, 2 Mbit/s data phase
 *
 *   byte data[] = { 1, 2, 3 };
 *   CAN.send(0x100, data, sizeof(data));
 *
 *   if (CAN.available()) {
 *     CanMessage msg = CAN.read();
 *   }
 *
 * Variables can also be kept in sync with the bus without any code in
 * loop(): CAN.listen() fills a variable from incoming messages, and
 * CAN.broadcast() sends a variable every few milliseconds.
 *
 * The bus needs a 120 ohm terminator at each end. Skipper has none on board.
 */
#pragma once

#include <Arduino.h>

#if !defined(ARDUINO_SKIPPER)
  #error "SkipperCAN needs the 'uSolder Skipper' board selected."
#endif

struct CanMessage {
  uint32_t id = 0;         // 11-bit (standard) or 29-bit (extended) ID
  bool extended = false;   // true for a 29-bit ID
  bool fd = false;         // true for a CAN-FD frame
  uint8_t length = 0;      // number of data bytes: 0-8, or up to 64 for CAN-FD
  uint8_t data[64] = {};
};

class SkipperCAN {
public:
  // Start the bus. Returns false if the bit rate can't be reached.
  // Give a second bit rate for CAN-FD with a faster data phase.
  bool begin(uint32_t bitRate);
  bool begin(uint32_t bitRate, uint32_t dataBitRate);
  void end();

  // Send a message. Returns false if the transmit queue is full or the
  // length is too long (8 bytes, 64 with CAN-FD).
  bool send(uint32_t id, const uint8_t *data, uint8_t length);
  bool sendExtended(uint32_t id, const uint8_t *data, uint8_t length);
  bool send(const CanMessage &message);

  // Number of received messages waiting, and the next one.
  int available();
  CanMessage read();

  // Only receive messages whose ID matches. A mask bit of 1 means that
  // bit must match; filter(id) alone accepts exactly one ID.
  // filter() with no arguments receives everything again.
  bool filter(uint32_t id, uint32_t mask = 0x7FF);
  bool filterExtended(uint32_t id, uint32_t mask = 0x1FFFFFFF);
  bool filter();

  // Call a function whenever a message arrives. It runs inside an
  // interrupt: keep it short and don't print from it.
  void onReceive(void (*callback)(const CanMessage &message));

  // Messages lost because the receive buffer was full.
  uint32_t droppedMessages() const;

  // ---- Variables kept in sync with the bus --------------------------------
  //
  // listen(): every message with this ID is copied into `variable` (up to
  // its size) as soon as it arrives. Optionally a function is called too;
  // it runs inside an interrupt, so keep it short: no delay(), no Serial,
  // no waiting. IDs you listen() to don't show up in available() / read().
  //
  //   volatile MotorStatus motor;
  //   CAN.listen(0x200, motor);
  //
  template <typename T>
  bool listen(uint32_t id, volatile T &variable, void (*callback)() = nullptr)
  {
    static_assert(sizeof(T) <= 64, "CAN messages carry at most 64 bytes");
    return listenVariable(id, false, &variable, sizeof(T), callback);
  }
  template <typename T>
  bool listenExtended(uint32_t id, volatile T &variable, void (*callback)() = nullptr)
  {
    static_assert(sizeof(T) <= 64, "CAN messages carry at most 64 bytes");
    return listenVariable(id, true, &variable, sizeof(T), callback);
  }
  bool stopListening(uint32_t id);

  // True once after a new message for this listen() ID has arrived.
  bool updated(uint32_t id);

  // Milliseconds since the last message for this listen() ID, or
  // 0xFFFFFFFF if none has arrived yet. Useful to notice a silent sender.
  uint32_t age(uint32_t id);

  // Copy a listen() variable without a message arriving halfway through
  // (a plain read of a struct can mix old and new data).
  template <typename T>
  bool copy(uint32_t id, T &destination)
  {
    return copyVariable(id, &destination, sizeof(T));
  }

  // broadcast(): send `variable` on the bus every `intervalMs` milliseconds,
  // always with its current value. Change the variable in loop() and the
  // new value goes out with the next message.
  //
  //   volatile MotorStatus motor;
  //   CAN.broadcast(0x200, motor, 100);
  //
  template <typename T>
  bool broadcast(uint32_t id, volatile T &variable, uint32_t intervalMs)
  {
    static_assert(sizeof(T) <= 64, "CAN messages carry at most 64 bytes");
    return broadcastVariable(id, false, &variable, sizeof(T), intervalMs);
  }
  template <typename T>
  bool broadcastExtended(uint32_t id, volatile T &variable, uint32_t intervalMs)
  {
    static_assert(sizeof(T) <= 64, "CAN messages carry at most 64 bytes");
    return broadcastVariable(id, true, &variable, sizeof(T), intervalMs);
  }
  bool stopBroadcast(uint32_t id);

  // Send a variable or struct once.
  template <typename T>
  bool send(uint32_t id, const volatile T &variable)
  {
    static_assert(sizeof(T) <= 64, "CAN messages carry at most 64 bytes");
    return sendVariable(id, false, &variable, sizeof(T));
  }
  template <typename T>
  bool sendExtended(uint32_t id, const volatile T &variable)
  {
    static_assert(sizeof(T) <= 64, "CAN messages carry at most 64 bytes");
    return sendVariable(id, true, &variable, sizeof(T));
  }

  // Internal: called from the interrupt handlers.
  void handleInterrupt();
  void handleTick();

private:
  bool start();
  bool applyFilter();
  bool sendFrame(uint32_t id, bool extended, const uint8_t *data, uint8_t length, bool wait);
  void recoverFromBusOff();

  bool listenVariable(uint32_t id, bool extended, volatile void *variable, uint8_t size, void (*callback)());
  bool copyVariable(uint32_t id, void *destination, uint8_t size);
  bool broadcastVariable(uint32_t id, bool extended, const volatile void *variable, uint8_t size, uint32_t intervalMs);
  bool sendVariable(uint32_t id, bool extended, const volatile void *variable, uint8_t size);

  struct Listener {
    bool used;
    bool extended;
    uint32_t id;
    volatile void *variable;
    uint8_t size;
    void (*callback)();
    volatile bool updated;
    volatile bool seen;
    volatile uint32_t lastTime;
  };
  struct Broadcast {
    bool used;
    bool extended;
    uint32_t id;
    const volatile void *variable;
    uint8_t size;
    uint32_t interval;
    uint32_t lastTime;
  };
  static const int maxListeners = 16;
  static const int maxBroadcasts = 16;
  Listener listeners[maxListeners] = {};
  Broadcast broadcasts[maxBroadcasts] = {};
  Listener *findListener(uint32_t id);

  static const int bufferSize = 32;
  CanMessage buffer[bufferSize];
  volatile int head = 0;
  volatile int tail = 0;
  volatile uint32_t dropped = 0;

  void (*receiveCallback)(const CanMessage &) = nullptr;
  bool running = false;
  bool fdMode = false;
  uint32_t nominalRate = 0;
  uint32_t dataRate = 0;

  // Current filter; type 0 = accept all, 1 = standard, 2 = extended.
  uint8_t filterType = 0;
  uint32_t filterId = 0;
  uint32_t filterMask = 0;
};

extern SkipperCAN CAN;
