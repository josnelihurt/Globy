#ifndef TRANSFER_SERVICE_H


#include "nordic_common.h"
#include <stdint.h>
#include <app_error.h>
#include <ble_types.h>
#include <ble.h>
#include <ble_gatts.h>
#include <string.h>
#include <nrf_log.h>


//#define NO_INDICATE 1
// This is a 16-bit UUID.

// These UUIDs are used with the Nordic base address to create a 128-bit UUID (0x8EC9XXXXF3154F609FB8838830DAEA50).

#define TRANSFER_SERVICE_BLE_CHAR_STATUS_UUID   				0X0001

extern uint32_t transfer_service_init();

void transfer_on_ble_evt(ble_evt_t * p_ble_evt);

void transfer_service_send_last_error(uint8_t error_code);

#endif //TRANSFER_SERVICE_H