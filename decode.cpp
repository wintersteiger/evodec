#include <vector>
#include <stdexcept>
#include <numeric>

#include "decode.h"

bool string2hex(const std::string &data, std::vector<uint8_t> &out)
{
  if (data.size() % 2 != 0)
    return false;

  out.resize(data.size()/2, 0);

  for (size_t i = 0; i < data.size(); i+=2)
    if (sscanf(data.c_str() + i, "%02hhx", &out[i/2]) != 1)
      return false;

  return true;
}

static uint8_t next(const std::vector<uint8_t> &bytes, size_t *bpos) {
    uint8_t r = bytes[*bpos];
    *bpos += 1;
    if (*bpos >= bytes.size())
       throw std::runtime_error("not enough bytes");
    return r;
}

void decode_header(const Message &msg, size_t *num_device_ids, const char **verb)
{
  static const char *I = " I";
  // static const char *RQ = "RQ";
  // static const char *RP = "RP";
  // static const char *W = " W";
  static const char *UN = "??";
  switch (msg.header) {
    case 0x14: { *num_device_ids = 1; *verb = I; break; }
    case 0x18: { *num_device_ids = 2; *verb = UN; break; }
    case 0x1c: { *num_device_ids = 2; *verb = UN; break; }
    case 0x10: { *num_device_ids = 2; *verb = UN; break; }
    case 0x3c: { *num_device_ids = 2; *verb = UN; break; }
    case 0x0c: { *num_device_ids = 2; *verb = UN; break; }
    case 0x2c: { *num_device_ids = 2; *verb = UN; break; }
    default: {
      *num_device_ids = (msg.header >> 2) & 0x03; // total speculation.
      *verb = UN;
    }
  }
}

Message decode(const std::string &data)
{
  Message msg;

  std::vector<uint8_t> bytes;
  if (!string2hex(data, bytes))
    throw std::runtime_error("data is not hex-encoded");

  if (bytes.size() <= 4)
    throw std::runtime_error("not enough data");

  size_t num_bytes = bytes.size();
  size_t bpos = 0;

  // Checksum: All bytes add up to 0.
  uint8_t checksum = std::accumulate(bytes.begin(), bytes.end(), 0);
  msg.crc = bytes.back();

  if (checksum != 0)
    throw std::runtime_error("checksum failure");

  msg.header = next(bytes, &bpos);

  size_t num_device_ids = 0;
  const char *verb = NULL;
  decode_header(msg, &num_device_ids, &verb);

  msg.device_ids.resize(num_device_ids);

  for (size_t i = 0; i < num_device_ids; i++) {
    msg.device_ids[i] = 0;
    for (size_t j = 0; j < 3; j++)
      msg.device_ids[i] = (msg.device_ids[i] << 8) | next(bytes, &bpos);
  }

  msg.command = (next(bytes, &bpos) << 8) | next(bytes, &bpos);
  size_t payload_length = next(bytes, &bpos);

  msg.payload.resize(payload_length);
  for (size_t i = 0; i < payload_length; i++)
    msg.payload[i] = next(bytes, &bpos);

  if (bpos < num_bytes - 1)
    if ((num_bytes - 1) - bpos != 0)
      msg.unparsed = std::vector<uint8_t>(bytes.begin() + bpos, bytes.end());

  // interpret(msg);

  return msg;
}