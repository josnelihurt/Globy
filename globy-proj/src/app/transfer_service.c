
#include "app/transfer_service.h"
#include "settings.h"
#include "hal/nrf_hal_fnc.h"

#include <app_error.h>
#include <ble_types.h>
#include <ble.h>
#include "nordic_common.h"
#include <stdint.h>
#include <string.h>
#include <nrf_log.h>
#include <nrf_gpio.h>

//static uint32_t m_full_address;
//static ble_xx_service_t                     m_xxs;

#define LOCAL_SERVICE_UUID 0x2107 /**< Proprietary UUID for local service. */
#define LOCAL_CHAR_UUID 0x8080    /**< Proprietary UUID for local characteristic. */
#define APP_CFG_CHAR_LEN 1        /**< Size of the characteristic value being notified (in bytes). */

typedef struct
{
    uint16_t *handle;
    void (*function)(ble_gatts_evt_write_t *);
} char_handle_write_callback_t;

static void on_char_write_simple(ble_gatts_evt_write_t *p_evt_write);

static void on_char_write_set_transfer_len(ble_gatts_evt_write_t *p_evt_write);
static void on_char_write_transfer_val(ble_gatts_evt_write_t *p_evt_write);

static uint8_t m_char_value[APP_CFG_CHAR_LEN];  /**< Value of the characteristic that will be sent as a notification to the central. */

static ble_gatts_char_handles_t m_char_handles; /**< Handles of local characteristic (as provided by the BLE stack).*/
static ble_gatts_char_handles_t m_char_handles_transfer_len; 
static uint8_t m_char_transfer_len_value[1];

static ble_gatts_char_handles_t m_char_handles_transfer_val; 
static uint8_t m_char_transfer_val_value[8];

static uint8_t m_buffer[256];
static uint16_t m_index = 0;
static uint16_t m_len = 0;

static char_handle_write_callback_t m_char_handles_write_callback[3] = {
    {.handle = &m_char_handles.value_handle, .function = on_char_write_simple},
    {.handle = &m_char_handles_transfer_len.value_handle, .function = on_char_write_set_transfer_len},
    {.handle = &m_char_handles_transfer_val.value_handle, .function = on_char_write_transfer_val},
};

static uint16_t m_service_handle; /**< Handle of local service (as provided by the BLE stack).*/
/**@brief 128-bit UUID base List. */
static const ble_uuid128_t m_base_uuid128 =
    {
        {0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15,
         0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}};

/**@brief Function for handling the HVC event.
 *
 * @details Handles HVC events from the BLE stack.
 *
 * @param[in]   p_mesh      Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_hvc(ble_evt_t *p_ble_evt)
{
    //ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    // Check if write event is performed on our characteristic or the CCCD
    //if (p_evt_write->handle == p_mesh_service->msn_handle.value_handle)
    //	msn_handle_evt(p_mesh_service, p_ble_evt);
    NRF_LOG_DEBUG("[Service]\ton_hvc\r\n");
}

/**@brief Function for handling the BLE write event.
 *
 * @param[in]   ble_evt_t   BLE event
 */
static void on_write(ble_evt_t *p_ble_evt)
{
    led_on(LED1);
    ble_gatts_evt_write_t *p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
    uint16_t hnd = p_evt_write->handle;
    bool found = false;

    for (int i = 0; i < 3 && !found; ++i)
    {
        if (hnd == *m_char_handles_write_callback[i].handle)
        {
            NRF_LOG_INFO("%s:execute fnc for 0x%x\r\n", (uint32_t) __FUNCTION__, hnd);
            m_char_handles_write_callback[i].function(p_evt_write);
            found = true;
        }
    }

    if (!found)
    {
        NRF_LOG_INFO("%s:hnd not found for 0x%x\r\n", (uint32_t) __FUNCTION__, hnd);
    }
    led_off(LED1);
}

void transfer_on_ble_evt(ble_evt_t *p_ble_evt)
{

    switch (p_ble_evt->header.evt_id)
    {
    case BLE_GAP_EVT_CONNECTED:
        NRF_LOG_DEBUG("%s:connected\r\n", (uint32_t) __FUNCTION__);
        break;

    case BLE_GAP_EVT_DISCONNECTED:
        NRF_LOG_DEBUG("%s:disconnected\r\n", (uint32_t) __FUNCTION__);
        led_off(LED4);
        break;

    case BLE_GATTS_EVT_WRITE:
        NRF_LOG_DEBUG("%s:write\r\n", (uint32_t) __FUNCTION__);
        on_write(p_ble_evt);

        break;
    case BLE_GATTS_EVT_HVC:
        on_hvc(p_ble_evt);
        break;

    default:
        // No implementation needed.
        break;
    }
}

static void on_char_write_simple(ble_gatts_evt_write_t *p_evt_write)
{
    uint8_t value = p_evt_write->data[0];
    NRF_LOG_INFO("%s:data %d\r\n", (uint32_t) __FUNCTION__, value);
    if (value == 0)
    {
        NRF_LOG_DEBUG("%s:led off\r\n", (uint32_t) __FUNCTION__);
        led_off(LED4);
    }
    else
    {
        NRF_LOG_DEBUG("%s:led on\r\n", (uint32_t) __FUNCTION__);
        led_on(LED4);
    }
    
}

static void on_char_write_set_transfer_len(ble_gatts_evt_write_t *p_evt_write)
{
    m_index = 0;
    m_len = p_evt_write->data[0];
    NRF_LOG_INFO("leng set to %d\r\n",m_len);
}

static void print_buffer(){
    NRF_LOG_INFO("buffer ready\r\n");
    NRF_LOG_HEXDUMP_INFO(m_buffer,m_len);
}

static void on_char_write_transfer_val(ble_gatts_evt_write_t *p_evt_write)
{
    NRF_LOG_INFO("a new val has been received index = %d \r\n", m_index);
    if (m_index >= m_len){
        print_buffer();
    }
    NRF_LOG_HEXDUMP_INFO(p_evt_write->data,8);
    memcpy(m_buffer + m_index, p_evt_write->data, 8);
    m_index += 8;
    
    if (m_index >= m_len){
        print_buffer();
    }
}

/**@brief Function for adding the Characteristic.
 *
 * @details This function adds the characteristic to the local db.
 *
 * @param[in] uuid_type Type of service UUID assigned by the SoftDevice.
 *
 */
static void char_add(const uint8_t uuid_type)
{
    uint32_t err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t attr_char_value;
    ble_uuid_t char_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&cccd_md, 0, sizeof(cccd_md));

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read = 1;
    char_md.char_props.write = 1;
    char_md.char_props.notify = 0;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf = NULL;
    char_md.p_user_desc_md = NULL;
    char_md.p_cccd_md = &cccd_md;
    char_md.p_sccd_md = NULL;

    char_uuid.type = uuid_type;
    char_uuid.uuid = LOCAL_CHAR_UUID;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len = APP_CFG_CHAR_LEN;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len = APP_CFG_CHAR_LEN;
    attr_char_value.p_value = m_char_value;

    err_code = sd_ble_gatts_characteristic_add(m_service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &m_char_handles);
    APP_ERROR_CHECK(err_code);
}

static void char_transfer_len_add(const uint8_t uuid_type)
{
    uint32_t err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t attr_char_value;
    ble_uuid_t char_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&cccd_md, 0, sizeof(cccd_md));

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read = 1;
    char_md.char_props.write = 1;
    char_md.char_props.notify = 0;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf = NULL;
    char_md.p_user_desc_md = NULL;
    char_md.p_cccd_md = &cccd_md;
    char_md.p_sccd_md = NULL;

    char_uuid.type = uuid_type;
    char_uuid.uuid = LOCAL_CHAR_UUID + 1;//!Important this should be unique

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len = 1;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len = 1;
    attr_char_value.p_value = m_char_transfer_len_value;

    err_code = sd_ble_gatts_characteristic_add(m_service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &m_char_handles_transfer_len);
    APP_ERROR_CHECK(err_code);
}

static void char_transfer_val_add(const uint8_t uuid_type)
{
    uint32_t err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t attr_char_value;
    ble_uuid_t char_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&cccd_md, 0, sizeof(cccd_md));

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read = 1;
    char_md.char_props.write = 1;
    char_md.char_props.notify = 0;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf = NULL;
    char_md.p_user_desc_md = NULL;
    char_md.p_cccd_md = &cccd_md;
    char_md.p_sccd_md = NULL;

    char_uuid.type = uuid_type;
    char_uuid.uuid = LOCAL_CHAR_UUID + 2;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len = 8;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len = 8;
    attr_char_value.p_value = m_char_transfer_val_value;

    err_code = sd_ble_gatts_characteristic_add(m_service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &m_char_handles_transfer_val);
    APP_ERROR_CHECK(err_code);
}

static void service_add(void)
{
    ble_uuid_t service_uuid;
    uint32_t err_code;

    service_uuid.uuid = LOCAL_SERVICE_UUID;

    err_code = sd_ble_uuid_vs_add(&m_base_uuid128, &service_uuid.type);
    APP_ERROR_CHECK(err_code);

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &service_uuid, &m_service_handle);
    APP_ERROR_CHECK(err_code);

    // Add characteristics
    char_add(service_uuid.type);
    char_transfer_len_add(service_uuid.type);
    char_transfer_val_add(service_uuid.type);
}

uint32_t transfer_service_init()
{
    NRF_LOG_DEBUG("%s:\r\n", (uint32_t) __FUNCTION__);
    service_add();

    NRF_LOG_INFO("%s:Started\r\n", (uint32_t) __FUNCTION__);
    return NRF_SUCCESS;
}
