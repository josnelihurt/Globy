#include "ble_init.h"
#include "ble_events.h"

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
#include <ble_bas.h>
#include <ble_dis.h>
#include <ble_conn_params.h>
#include <boards.h>
#include <sensorsim.h>
#include <softdevice_handler.h>
#include <app_timer.h>
#include <bsp.h>
#include <nrf_delay.h>
#include <bsp_btn_ble.h>
#include <peer_manager.h>
#include <fds.h>
#include <fstorage.h>
#include <ble_conn_state.h>
#include <nrf_log.h>
#include <nrf_ble_gatt.h>

#define TRANSFER_SERVICE_BLE_UUID                 0xFE5A

static ble_uuid_t m_adv_uuids[] = {{TRANSFER_SERVICE_BLE_UUID, BLE_GATTS_SRVC_TYPE_PRIMARY}};
static nrf_ble_gatt_t m_gatt;


/*GATT generic Event handler*/
void gatt_evt_handler(nrf_ble_gatt_t * p_gatt, nrf_ble_gatt_evt_t * p_evt)
{
    NRF_LOG_DEBUG("%s:\r\n", (uint32_t)__FUNCTION__);
    //ble_hrs_on_gatt_evt(&m_hrs, p_evt);
}

/*GATT Module init*/
void gatt_init(void)
{
    NRF_LOG_DEBUG("%s:\r\n", (uint32_t)__FUNCTION__);
    ret_code_t err_code = nrf_ble_gatt_init(&m_gatt, gatt_evt_handler);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing the Advertising functionality.
 */
void advertising_init(void)
{
    NRF_LOG_DEBUG("%s:\r\n", (uint32_t)__FUNCTION__);

    uint32_t err_code;
    ble_advdata_t advdata;
    ble_adv_modes_config_t options;

    // Build advertising data struct to pass into @ref ble_advertising_init.
    memset(&advdata, 0, sizeof(advdata));

    advdata.name_type               = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance      = true;
    advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    advdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    advdata.uuids_complete.p_uuids  = m_adv_uuids;
    advdata.p_manuf_specific_data->company_identifier = COMPANY_ID;// Id

    memset(&options, 0, sizeof(options));
    options.ble_adv_fast_enabled  = true;
    options.ble_adv_fast_interval = APP_ADV_INTERVAL;
    options.ble_adv_fast_timeout  = APP_ADV_TIMEOUT_IN_SECONDS;

    err_code = ble_advertising_init(&advdata, NULL, &options, on_adv_evt, NULL);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for the GAP initialization.
 *
 * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
 *          device including the device name, appearance, and the preferred connection parameters.
 */
void gap_params_init(void)
{
	NRF_LOG_DEBUG("%s:\r\n", (uint32_t)__FUNCTION__);
    uint32_t err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    static uint8_t addr[BLE_GAP_ADDR_LEN];       /**< 48-bit address, LSB format. */
    addr[0]=0x21;
    addr[1]=0x07;
    addr[2]=0x87;
    addr[3]=0x13;
    addr[4]=0x07;
    addr[5]=0x77;

    static ble_gap_addr_t gap_addr;
    gap_addr.addr_type = BLE_GAP_ADDR_TYPE_RANDOM_STATIC;
    // "my_random_addr" must be random generated.
    memcpy(&gap_addr.addr, addr, sizeof(gap_addr.addr));
    gap_addr.addr[5] |= 0xc0; // 2 MSBit must be '11' for RANDOM_STATIC address, see v4.0, Vol 3, Part C, chapter 10.8
    err_code = sd_ble_gap_address_set(BLE_GAP_ADDR_CYCLE_MODE_NONE, &gap_addr);
    APP_ERROR_CHECK(err_code);


    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *)DEVICE_NAME,
                                          strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_UNKNOWN);
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
void ble_stack_init(void)
{
    NRF_LOG_DEBUG("%s", (uint32_t)__FUNCTION__);
    uint32_t err_code;

    nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;

    // Initialize the SoftDevice handler module.
    SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, NULL);

    NRF_LOG_DEBUG("%s soft device handler init");
    ble_enable_params_t ble_enable_params;
    err_code = softdevice_enable_get_default_config(NRF_BLE_CENTRAL_LINK_COUNT,
                                                    NRF_BLE_PERIPHERAL_LINK_COUNT,
                                                    &ble_enable_params);
    APP_ERROR_CHECK(err_code);

    // Check the ram settings against the used number of links
    CHECK_RAM_START_ADDR(NRF_BLE_CENTRAL_LINK_COUNT, NRF_BLE_PERIPHERAL_LINK_COUNT);

    // Enable BLE stack.
    NRF_LOG_DEBUG("%s enable soft device");
    err_code = softdevice_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_sys_evt_handler_set(sys_evt_dispatch);
    APP_ERROR_CHECK(err_code);
    
    NRF_LOG_DEBUG("%s Done", (uint32_t)__FUNCTION__);

}
