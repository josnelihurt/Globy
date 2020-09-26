#include <stdbool.h>
#include <stdint.h>
#include "ble_advdata.h"
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "bsp.h"
#include "app_timer.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "settings.h"
#include "hal/nrf_hal_fnc.h"
#include "ble/ble_events.h"
#include "ble/ble_init.h"


/**@brief Function for doing power management.
 */
static void power_manage(void)
{
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}

static void init(void)
{
	uint32_t err_code = NRF_SUCCESS;
	bool erase_bonds;
    // Initialize Log
    err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("%s:---- Starting ----\r\n", (uint32_t)__FUNCTION__);
    
	timers_init();
    buttons_leds_init(&erase_bonds);
	ble_stack_init();
	if (erase_bonds == true)
	{
		NRF_LOG_INFO("Bonds erased!\r\n");
	}

	NRF_LOG_INFO("%s:Configure BLE Driver\r\n", (uint32_t)__FUNCTION__);
	gap_params_init();
	advertising_init();
	gatt_init();
	services_init();
	conn_params_init();

	NRF_LOG_INFO("%s:Start advertisment\r\n", (uint32_t)__FUNCTION__);
	advertising_start();
}

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    init();
    // Enter main loop.
    for (;; )
    {
        if (NRF_LOG_PROCESS() == false)
        {
            power_manage();
        }
    }
}

