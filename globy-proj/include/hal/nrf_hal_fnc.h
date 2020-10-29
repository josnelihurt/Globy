#ifndef INCLUDE_NRF_HAL_FNC_H

#include <nrf_log.h>
#include <stdint.h>
#include <string.h>
#include <nordic_common.h>
#include <nrf.h>
#include <app_error.h>
#include <ble.h>
#include <ble_hci.h>
#include <ble_srv_common.h>
#include <ble_advdata.h>
#include <ble_advertising.h>
#include <ble_dis.h>
#include <ble_conn_params.h>
#include <sensorsim.h>
#include <softdevice_handler.h>
#include <app_timer.h>
#include <bsp.h>
#include <nrf_delay.h>
#include <bsp_btn_ble.h>
#include <fds.h>
#include <fstorage.h>
#include <ble_conn_state.h>
#include <nrf_log.h>
#include <nrf_log_ctrl.h>

#include "settings.h"

/**@brief Callback function for asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num   Line number of the failing ASSERT call.
 * @param[in] file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name);

/**@brief Function for starting advertising.
 */
void advertising_start(void);

/**@brief Function for handling File Data Storage events.
 *
 * @param[in] p_evt  Peer Manager event.
 * @param[in] cmd
 */
void fds_evt_handler(fds_evt_t const * const p_evt);

/**@brief Function for initializing services that will be used by the application.
 *
 * @details Initialize the Device Information services including its characteristics.
 */
void services_init(void);

/**@brief Function for starting application timers.
 */
void application_timers_start(void);

/**@brief Function for handling the Connection Parameters Module.
 *
 * @details This function will be called for all events in the Connection Parameters Module which
 *          are passed to the application.
 *          @note All this function does is to disconnect. This could have been done by simply
 *                setting the disconnect_on_fail config parameter, but instead we use the event
 *                handler mechanism to demonstrate its use.
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module.
 */
void on_conn_params_evt(ble_conn_params_evt_t * p_evt);

/**@brief Function for handling a Connection Parameters error.
 *
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
void conn_params_error_handler(uint32_t nrf_error);

/**@brief Function for initializing the Connection Parameters module.
 */
void conn_params_init(void);

/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module. This creates and starts application timers.
 */
void timers_init(void);

/**@brief Function for initializing buttons and leds.
 *
 * @param[out] p_erase_bonds  Will be true if the clear bonding button was pressed to wake the application up.
 */
void buttons_leds_init(bool* p_erase_bonds);

void led_on(int pin);
void led_off(int ping);

#endif