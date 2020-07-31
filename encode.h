#ifndef _EVODEC_ENCODE_H_
#define _EVODEC_ENCODE_H_

#include <string>

#include "message.h"

std::string encode_raw(const Message &msg);
std::string encode_json(const Message &msg);
std::string encode_hgi80(const Message &msg);

#endif // _EVODEC_ENCODE_H_