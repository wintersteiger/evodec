#ifndef _EVODEC_MESSAGE_H_
#define _EVODEC_MESSAGE_H_

#include <cstdint>
#include <vector>

#include "devices.h"

class Message {
public:
  Message() {}
  virtual ~Message() {}

  uint8_t header;
  std::vector<DeviceID> device_ids;
  uint16_t command;
  std::vector<uint8_t> payload;
  std::vector<uint8_t> unparsed;
  uint8_t crc;

  bool invariant() const {
    // Note: The upper limit of the payload size is supposedly 48, but
    // that's according to one source without references.
    if (device_ids.size() == 0 || device_ids.size() > 3) return false;
    if (payload.size() < 1 || payload.size() > 48) return false;
    return true;
  }
};

#endif // _EVODEC_MESSAGE_H_