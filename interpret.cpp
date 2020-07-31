#include "message.h"

void interpret(Message &msg)
{
  // Sources of inspiration:
  // https://github.com/Evsdd/The-Evohome-Protocol/wiki
  // https://www.domoticaforum.eu/viewtopic.php?f=7&t=5806&start=30
  // (specifically https://www.domoticaforum.eu/download/file.php?id=1396)
  // https://github.com/merbanan/rtl_433/pull/1336

  // data = decode_device_ids(msg, data, 1);

  // switch(msg.command) {
  //     case 0x1030: {
  //         UNKNOWN_IF(msg.payload_length != 16);
  //         data = data_append(data, "zone_idx", "", DATA_FORMAT, "%02x", DATA_INT, msg.payload[0], NULL);
  //         for (unsigned i = 0; i < 5; i++) { // order fixed?
  //             const uint8_t *p = &msg.payload[1 + 3*i];
  //             // *(p+1) == 0x01 always?
  //             int value = *(p+2);
  //             switch (*p) {
  //                 case 0xC8: data = data_append(data, "max_flow_temp", "", DATA_INT, value, NULL); break;
  //                 case 0xC9: data = data_append(data, "pump_run_time", "", DATA_INT, value, NULL); break;
  //                 case 0xCA: data = data_append(data, "actuator_run_time", "", DATA_INT, value, NULL); break;
  //                 case 0xCB: data = data_append(data, "min_flow_temp", "", DATA_INT, value, NULL); break;
  //                 case 0xCC: /* Unknown, always 0x01? */ break;
  //                 default:
  //                     if (verbose)
  //                         printf("Unknown parameter to 0x1030: %x02d=%04d\n", *p, value);
  //             }
  //         }
  //         break;
  //     }
  //     case 0x313F: {
  //         UNKNOWN_IF(msg.payload_length != 1 && msg.payload_length != 9);
  //         switch (msg.payload_length) {
  //             case 1:
  //                 data = data_append(data, "time_request", "", DATA_INT, msg.payload[0], NULL); break;
  //             case 9: {
  //                 uint8_t unknown_0 = msg.payload[0]; /* always == 0? */
  //                 uint8_t unknown_1 = msg.payload[1]; /* direction? */
  //                 uint8_t second = msg.payload[2];
  //                 uint8_t minute = msg.payload[3];
  //                 uint8_t day_of_week = msg.payload[4] >> 5;
  //                 uint8_t hour = msg.payload[4] & 0x1F;
  //                 uint8_t day = msg.payload[5];
  //                 uint8_t month = msg.payload[6];
  //                 uint8_t year[2] = { msg.payload[7],  msg.payload[8] };
  //                 char time_str[256];
  //                 sprintf(time_str, "%02d:%02d:%02d %02d-%02d-%04d", hour, minute, second, day, month, (year[0] << 8) | year[1]);
  //                 data = data_append(data, "datetime", "", DATA_STRING, time_str, NULL);
  //                 break;
  //             }
  //         }
  //         break;
  //     }
  //     case 0x0008: {
  //         UNKNOWN_IF(msg.payload_length != 2);
  //         data = data_append(data, "domain_id", "", DATA_INT, msg.payload[0], NULL);
  //         data = data_append(data, "demand", "", DATA_DOUBLE, msg.payload[1] / 200.0 /* 0xC8 */, NULL);
  //         break;
  //     }
  //     case 0x3ef0: {
  //         UNKNOWN_IF(msg.payload_length != 3 && msg.payload_length != 6);
  //         switch (msg.payload_length) {
  //             case 3:
  //                 data = data_append(data, "status", "", DATA_DOUBLE, msg.payload[1] / 200.0 /* 0xC8 */, NULL);
  //                 break;
  //             case 6:
  //                 data = data_append(data, "boiler_modulation_level", "", DATA_DOUBLE, msg.payload[1] / 200.0 /* 0xC8 */, NULL);
  //                 data = data_append(data, "flame_status", "", DATA_INT, msg.payload[3], NULL);
  //                 break;
  //         }
  //         break;
  //     }
  //     case 0x2309: {
  //         UNKNOWN_IF(msg.payload_length != 3);
  //         data = data_append(data, "zone", "", DATA_INT, msg.payload[0], NULL);
  //         // Observation: CM921 reports a very high setpoint during binding (0x7eff); packet: 143255c1230903017efff7
  //         data = data_append(data, "setpoint", "", DATA_DOUBLE, ((msg.payload[1] << 8) | msg.payload[2]) / 100.0, NULL);
  //         break;
  //     }
  //     case 0x1100: {
  //         UNKNOWN_IF(msg.payload_length != 5 && msg.payload_length != 8);
  //         data = data_append(data, "domain_id", "", DATA_INT, msg.payload[0], NULL);
  //         data = data_append(data, "cycle_rate", "", DATA_DOUBLE, msg.payload[1] / 4.0, NULL);
  //         data = data_append(data, "minimum_on_time", "", DATA_DOUBLE, msg.payload[2] / 4.0, NULL);
  //         data = data_append(data, "minimum_off_time", "", DATA_DOUBLE, msg.payload[3] / 4.0, NULL);
  //         if (msg.payload_length == 8)
  //             data = data_append(data, "proportional_band_width", "", DATA_DOUBLE, (msg.payload[5] << 8 | msg.payload[6]) / 100.0, NULL);
  //         break;
  //     }
  //     case 0x0009: {
  //         UNKNOWN_IF(msg.payload_length != 3);
  //         data = data_append(data, "device_number", "", DATA_INT, msg.payload[0], NULL);
  //         switch (msg.payload[1]) {
  //             case 0: data = data_append(data, "failsafe_mode", "", DATA_STRING, "off", NULL); break;
  //             case 1: data = data_append(data, "failsafe_mode", "", DATA_STRING, "20-80", NULL); break;
  //             default: data = data_append(data, "failsafe_mode", "", DATA_STRING, "unknown", NULL);
  //         }
  //         break;
  //     }
  //     case 0x3B00: {
  //         UNKNOWN_IF(msg.payload_length != 2);
  //         data = data_append(data, "domain_id", "", DATA_INT, msg.payload[0], NULL);
  //         data = data_append(data, "state", "", DATA_DOUBLE, msg.payload[1] / 200.0 /* 0xC8 */, NULL);
  //         break;
  //     }
  //     case 0x30C9: {
  //       size_t num_zones = msg.payload_length/3;
  //       for (size_t i=0; i < num_zones; i++) {
  //         char name[256];
  //         snprintf(name, sizeof(name), "temperature (zone %u)", msg.payload[3*i]);
  //         int16_t temp = msg.payload[3*i+1] << 8 | msg.payload[3*i+1];
  //         data = data_append(data, name, "", DATA_DOUBLE, temp/100.0, NULL);
  //       }
  //       break;
  //     }
  //     default: /* Unknown command */
  //         UNKNOWN_IF(1);
  // }
}