#include "nrf_hal_fnc.h"
#include "../app/transfer_service.h"
#include "../ble/ble_events.h"

void assert_nrf_callback(uint16_t line_num, const uint8_t *p_file_name)
{
    NRF_LOG_DEBUG("%s:\r\n", (uint32_t) __FUNCTION__);
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}

void advertising_start(void)
{
    NRF_LOG_DEBUG("%s:\r\n", (uint32_t) __FUNCTION__);
    ret_code_t err_code;

    err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
    APP_ERROR_CHECK(err_code);
}

void fds_evt_handler(fds_evt_t const *const p_evt)
{
    NRF_LOG_DEBUG("%s:\r\n", (uint32_t) __FUNCTION__);
    if (p_evt->id == FDS_EVT_GC)
    {
        NRF_LOG_INFO("GC completed\n");
    }
}

void services_init(void)
{
    NRF_LOG_DEBUG("%s:\r\n", (uint32_t) __FUNCTION__);
    uint32_t err_code;
    err_code = transfer_service_init();
    APP_ERROR_CHECK(err_code);
}

void application_timers_start(void)
{
    NRF_LOG_DEBUG("%s:\r\n", (uint32_t) __FUNCTION__);
}

void on_conn_params_evt(ble_conn_params_evt_t *p_evt)
{
    NRF_LOG_DEBUG("%s:\r\n", (uint32_t) __FUNCTION__);
    uint32_t err_code;

    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}

void conn_params_error_handler(uint32_t nrf_error)
{
    NRF_LOG_DEBUG("%s:\r\n", (uint32_t) __FUNCTION__);
    APP_ERROR_HANDLER(nrf_error);
}

void conn_params_init(void)
{
    NRF_LOG_DEBUG("%s:\r\n", (uint32_t) __FUNCTION__);
    uint32_t err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.disconnect_on_fail = false;
    cp_init.evt_handler = on_conn_params_evt;
    cp_init.error_handler = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}

void timers_init(void)
{
    NRF_LOG_DEBUG("%s:\r\n", (uint32_t) __FUNCTION__);
    // Initialize timer module.
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);
    NRF_LOG_INFO("timers_init no timers\r\n");
}

void buttons_leds_init(bool *p_erase_bonds)
{
    NRF_LOG_DEBUG("%s:\r\n", (uint32_t) __FUNCTION__);
    bsp_event_t startup_event;

    uint32_t err_code = bsp_init(BSP_INIT_LED | BSP_INIT_BUTTONS,
                                 APP_TIMER_TICKS(100, APP_TIMER_PRESCALER),
                                 bsp_event_handler);

    APP_ERROR_CHECK(err_code);

    err_code = bsp_btn_ble_init(NULL, &startup_event);
    APP_ERROR_CHECK(err_code);

    *p_erase_bonds = (startup_event == BSP_EVENT_CLEAR_BONDING_DATA);

    nrf_gpio_cfg_output(BN_LED_R);
    nrf_gpio_cfg_output(BN_LED_G);
    nrf_gpio_cfg_output(BN_LED_B);
}

void led_on(int pin)
{
    nrf_gpio_pin_write(pin, LEDS_ACTIVE_STATE ? 1 : 0);
}

void led_off(int pin)
{
    nrf_gpio_pin_write(pin, LEDS_ACTIVE_STATE ? 0 : 1);
}