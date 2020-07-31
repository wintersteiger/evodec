#include <algorithm>
#include <stdexcept>

#include "decode.h"
#include "encode.h"
#include "devices.h"

#define P(F,X) { p += snprintf(p, tmp_sz-(p-tmp), (F), (X)); }
#define U(X) P("%s", X)

std::string encode_raw(const Message &msg)
{
  if (!msg.invariant())
    throw std::runtime_error("message invariant violated");

  // Maximum message size: 1 + 9 + 2 + 1 + 48 + 1 = 62 bytes? 64?
  size_t tmp_sz = (64 + msg.unparsed.size())*2 + 1;
  char *tmp = new char[tmp_sz];
  char *p = tmp;

  P("%02x", msg.header);

  for (auto device_id : msg.device_ids)
    P("%06x", device_id);

  P("%04x", msg.command);

  P("%02x", msg.payload.size());
  for (uint8_t b: msg.payload)
    P("%02x", b);

  if (msg.unparsed.size() > 0)
    for (uint8_t b: msg.unparsed)
      P("%02x", b);

  P("%02x", msg.crc);

  std::string r = tmp;
  delete[] tmp;
  return r;
}

std::string encode_json(const Message &msg)
{
  if (!msg.invariant())
    throw std::runtime_error("message invariant violated");

  size_t tmp_sz = 1024;
  char *tmp = new char[tmp_sz];
  char *p = tmp;

  U("{");

  P("\"header\": \"%02x\", ", msg.header);

  U("\"device_ids\": [");
  bool first = true;
  for (auto device_id : msg.device_ids) {
    if (first) first = !first; else U(", ");
    P("\"%06x\"", device_id);
  }
  U("], ");

  P("\"command\": \"%04x\", ", msg.command);

  U("\"payload\": \"");
  for (uint8_t b: msg.payload)
    P("%02x", b);
  U("\", ");

  if (msg.unparsed.size() > 0) {
    U("\"unparsed\": \"");
    for (uint8_t b: msg.unparsed)
      P("%02x", b);
    U("\", ");
  }

  U("\"crc\": \"");
  P("%02x", msg.crc);
  U("\"");

  U("}");

  std::string r = tmp;
  delete[] tmp;
  return r;
}

std::string encode_hgi80(const Message &msg)
{
  if (!msg.invariant())
    throw std::runtime_error("message invariant violated");

  size_t tmp_sz = 1024;
  char *tmp = new char[tmp_sz];
  char *p = &tmp[0];

  P("%03d ", 45); // RSSI

  // Paket verb I, RQ, RP, W
  size_t num_device_ids;
  const char *verb;
  decode_header(msg, &num_device_ids, &verb);
  P("%s ", verb);

  // P("%02x ", msg.header);

  // for (size_t i=0; i < 8; i++)
  //   U((((msg.header >> (7-i)) & 0x01) ? "1" : "0"));
  // U(" ");

  U("--- "); // Sequence number

  for (size_t i=0; i < 3; i++) {
    DeviceID device_id = i < msg.device_ids.size() ? msg.device_ids[i] : 0;
    P("%s ", deviceid2addr(device_id).c_str());
  }

  P("%04x ", msg.command);

  P("%03d ", msg.payload.size());
  for (uint8_t b: msg.payload)
    P("%02x", b);

  std::string r = tmp;
  delete[] tmp;
  return r;
}