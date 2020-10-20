
#ifndef INCLUDE_BLE_INIT_H_
#define INCLUDE_BLE_INIT_H_


#define DEVICE_NAME                     "Globy"					/**< Name of device. Will be included in the advertising data. */
#define MANUFACTURER_NAME               "MakerSpace"						/**< Manufacturer. Will be passed to Device Information Service. */
#define APP_ADV_INTERVAL                300									/**< The advertising interval (in units of 0.625 ms. */
#define APP_ADV_TIMEOUT_IN_SECONDS      0									/**< The advertising timeout in units of seconds. */

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(40, UNIT_1_25_MS)           /**< Minimum acceptable connection interval (0.4 seconds). */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(70, UNIT_1_25_MS)           /**< Maximum acceptable connection interval (0.65 second). */
#define SLAVE_LATENCY                   0                                          /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)            /**< Connection supervisory timeout (4 seconds). */
#define COMPANY_ID                      0x0059

extern void gatt_init(void);
extern void ble_stack_init(void);
extern void gap_params_init(void);
extern void advertising_init(void);

#endif /* INCLUDE_BLE_INIT_H_ */
