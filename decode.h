#ifndef _EVODEC_DECODE_H_
#define _EVODEC_DECODE_H_

#include <string>

#include "message.h"

Message decode(const std::string &data);

void decode_header(const Message &msg, size_t *num_device_ids, const char **verb);

#endif // _EVODEC_DECODE_H_