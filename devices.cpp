#include "devices.h"

// All data from https://github.com/Evsdd/The-Evohome-Protocol/wiki/Decoding-Data-Fields#device-ids

typedef struct dev_map_entry
{
  size_t id;
  const char* abbreviation;
  const char* description;
} dev_map_entry_t;

static const dev_map_entry_t device_map[] = {
    { 01, "CTL", "Controller" },
    { 02, "UFH", "Underfloor heating (HCC80, HCE80)" },
    { 03, " 30", "Unknown (HCW82?)" },
    { 04, "TRV", "Thermostatic radiator valve (HR80, HR91, HR92)" },
    { 07, "DHW", "DHW sensor (CS92)" },
    { 10, "OTB", "OpenTherm bridge (R8810)" },
    { 12, "THm", "Thermostat with setpoint schedule control (DTS92E, CME921)" },
    { 13, "BDR", "Wireless relay box (BDR91) (HC60NG too?)" },
    { 17, " 17", "Unknown (Outside weather sensor?)" },
    { 18, "HGI", "Honeywell Gateway Interface (HGI80, HGS80)" },
    { 22, "THM", "Thermostat with setpoint schedule control (DTS92E)" },
    { 30, "GWY", "Gateway (e.g. RFG100?)" },
    { 32, "VNT", "(HCE80) Ventilation (Nuaire VMS-23HB33, VMN-23LMH23)" },
    { 34, "STA", "Thermostat (T87RF)" },
    { 63, "NUL", "No device" },
};

std::string deviceid2type(const DeviceID &id)
{
  uint8_t type = id >> 18;
  for (auto i : device_map)
    if (i.id == type)
      return i.abbreviation;
  return "???";
}

std::string deviceid2addr(const DeviceID &id)
{
  static const char *error_str = "???:??????";
  char r[] = "---:------";

  if (id != 0) {
    // if (snprintf(r, sizeof(r), "%02d:%06d", id >> 18, id & 0x03FFFF) != 9)
    //   return error_str;
    if (snprintf(r, sizeof(r), "%3s:%06d", deviceid2type(id).c_str(), id & 0x03FFFF) != 10)
      return error_str;
  }

  return r;
}