#ifndef _EVOHOME_DEVICES_H_
#define _EVOHOME_DEVICES_H_

#include <cstdint>
#include <vector>
#include <string>

typedef uint32_t DeviceID;

std::string deviceid2type(const DeviceID &id);

std::string deviceid2addr(const DeviceID &id);

#endif // _EVOHOME_DEVICES_H_