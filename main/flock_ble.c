#include "flock_ble.h"

#include "esp_check.h"
#include "esp_log.h"
#include "esp_nimble_hci.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "host/ble_hs.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "services/gap/ble_svc_gap.h"

static const char* TAG = "flock-ble";

// Infinite task to run the NimBLE stack
static void host_task(void* param) {
  ESP_LOGI(TAG, "BLE Host Task Started");
  nimble_port_run();  // This function will return only when nimble_port_stop()
                      // is called
  nimble_port_freertos_deinit();
}

esp_err_t ble_init(void) {
  ESP_RETURN_ON_ERROR(nimble_port_init(), TAG, "Failed to initialize NimBLE");
  nimble_port_freertos_init(host_task);

  return ESP_OK;
}

esp_err_t ble_set_mac_prefix(const uint8_t prefix[]) {
  ble_addr_t custom_mac;
  ble_hs_id_gen_rnd(0, &custom_mac);
  custom_mac.val[5] = prefix[0];
  custom_mac.val[4] = prefix[1];
  custom_mac.val[3] = prefix[2];

  ESP_LOGI(TAG, "Setting custom ble MAC address: %02x:%02x:%02x:%02x:%02x:%02x",
           custom_mac.val[0], custom_mac.val[1], custom_mac.val[2],
           custom_mac.val[3], custom_mac.val[4], custom_mac.val[5]);
  ESP_RETURN_ON_ERROR(ble_hs_id_set_rnd(custom_mac.val), TAG,
                      "Failed to set custom MAC address");

  return ESP_OK;
}

esp_err_t ble_start_flock_spoof(const uint8_t* prefix,
                                const ble_uuid128_t* uuid,
                                const char* device_name) {
  char uuid_str[37];
  if (uuid) {
    ble_uuid_to_str((ble_uuid_t*)uuid, uuid_str);
  }
  ESP_LOGI(TAG, "Starting Flock BLE spoofing: prefix=%s uuid=%s name=%s",
           prefix ? "yes" : "no", uuid ? uuid_str : "no",
           device_name ? device_name : "no");
  struct ble_gap_adv_params adv_params;
  struct ble_hs_adv_fields fields;

  if (prefix) {
    ble_set_mac_prefix(prefix);
  }

  memset(&fields, 0, sizeof(fields));

  fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;
  size_t len = device_name ? strlen(device_name) : 0;
  if (!uuid || (uuid && len > 0 && len <= 8)) {
    fields.name = (const uint8_t*)device_name;
    fields.name_len = len;
  }
  if (uuid) {
    fields.uuids128 = uuid;
    fields.num_uuids128 = 1;
    fields.uuids128_is_complete = 1;
  }

  ESP_RETURN_ON_ERROR(ble_gap_adv_set_fields(&fields), TAG,
                      "Failed to set adv fields");
  memset(&adv_params, 0, sizeof(adv_params));

  adv_params.conn_mode = BLE_GAP_CONN_MODE_NON;
  adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

  // Start Advertising
  uint8_t addr_type = prefix ? BLE_ADDR_RANDOM : BLE_ADDR_PUBLIC;
  ESP_RETURN_ON_ERROR(ble_gap_adv_start(addr_type, NULL, BLE_HS_FOREVER,
                                        &adv_params, NULL, NULL),
                      TAG, "Failed to start advertising");

  return ESP_OK;
}

esp_err_t ble_stop_flock_spoof(void) {
  ESP_LOGI(TAG, "Stopping Flock BLE");
  ESP_RETURN_ON_ERROR(ble_gap_adv_stop(), TAG, "Failed to stop advertising");

  return ESP_OK;
}
