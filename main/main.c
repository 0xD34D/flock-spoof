#include <stdint.h>
#include <stdio.h>

#include "esp_check.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_random.h"
#include "esp_wifi.h"
#include "flock_ble.h"
#include "flock_wifi.h"
#include "memory.h"
#include "nvs_flash.h"

static const char* TAG = "flock-spoof";

#define TEST_DURATION_MS 5000

void test_flock_wifi_spoof() {
  ESP_LOGI(TAG, "Testing Flock WiFi Spoofing");

  uint8_t* prefix;
  char* ssid;

  size_t num_prefixes = sizeof(wifi_prefixes) / sizeof(wifi_prefixes[0]);
  for (uint8_t i = 0; i < num_prefixes; i++) {
    prefix = wifi_prefixes[i];
    ESP_LOGI(TAG, "Spoofing MAC Prefix %02x:%02x:%02x", prefix[0], prefix[1],
             prefix[2]);
    ESP_ERROR_CHECK(wifi_start_flock_spoof(NULL, prefix));
    vTaskDelay(pdMS_TO_TICKS(TEST_DURATION_MS));
    ESP_ERROR_CHECK(wifi_stop_flock_spoof());
  }

  size_t num_ssids = sizeof(wifi_ssid_patterns) / sizeof(wifi_ssid_patterns[0]);
  for (uint8_t i = 0; i < num_ssids; i++) {
    ssid = (char*)wifi_ssid_patterns[i];
    ESP_LOGI(TAG, "Spoofing SSID %s", ssid);
    ESP_ERROR_CHECK(wifi_start_flock_spoof(ssid, NULL));
    vTaskDelay(pdMS_TO_TICKS(TEST_DURATION_MS));
    ESP_ERROR_CHECK(wifi_stop_flock_spoof());
  }

  for (uint8_t i = 0; i < num_prefixes; i++) {
    for (uint8_t j = 0; j < num_ssids; j++) {
      prefix = wifi_prefixes[i];
      ssid = (char*)wifi_ssid_patterns[j];
      ESP_LOGI(TAG, "Spoofing MAC Prefix %02x:%02x:%02x and SSID %s", prefix[0],
               prefix[1], prefix[2], ssid);
      ESP_ERROR_CHECK(wifi_start_flock_spoof(ssid, prefix));
      vTaskDelay(pdMS_TO_TICKS(TEST_DURATION_MS));
      ESP_ERROR_CHECK(wifi_stop_flock_spoof());
    }
  }
}

void test_flock_ble_spoof() {
  ESP_LOGI(TAG, "Testing Flock BLE Spoofing");

  uint8_t* prefix;
  char* name;
  const ble_uuid128_t* uuid;

  size_t num_prefixes = sizeof(ble_prefixes) / sizeof(ble_prefixes[0]);
  for (uint8_t i = 0; i < num_prefixes; i++) {
    prefix = ble_prefixes[i];
    ESP_LOGI(TAG, "Spoofing BLE MAC Prefix %02x:%02x:%02x", prefix[0],
             prefix[1], prefix[2]);
    ESP_ERROR_CHECK(ble_start_flock_spoof(prefix, NULL, NULL));
    vTaskDelay(pdMS_TO_TICKS(TEST_DURATION_MS));
    ESP_ERROR_CHECK(ble_stop_flock_spoof());
  }

  size_t num_names =
      sizeof(device_name_patterns) / sizeof(device_name_patterns[0]);
  for (uint8_t i = 0; i < num_names; i++) {
    name = (char*)device_name_patterns[i];
    ESP_LOGI(TAG, "Spoofing BLE device name %s", name);
    ESP_ERROR_CHECK(ble_start_flock_spoof(NULL, NULL, name));
    vTaskDelay(pdMS_TO_TICKS(TEST_DURATION_MS));
    ESP_ERROR_CHECK(ble_stop_flock_spoof());
  }

  size_t num_uuids =
      sizeof(raven_service_uuids) / sizeof(raven_service_uuids[0]);
  char uuid_str[37];
  for (uint8_t i = 0; i < num_uuids; i++) {
    uuid = &raven_service_uuids[i];
    ble_uuid_to_str((ble_uuid_t*)uuid, uuid_str);
    ESP_LOGI(TAG, "Spoofing BLE UUID %s", uuid_str);
    ESP_ERROR_CHECK(ble_start_flock_spoof(NULL, uuid, NULL));
    vTaskDelay(pdMS_TO_TICKS(TEST_DURATION_MS));
    ESP_ERROR_CHECK(ble_stop_flock_spoof());
  }
}

void app_main(void) {
  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  // Initialize WiFi and BLE
  ESP_ERROR_CHECK(wifi_init());
  ESP_ERROR_CHECK(ble_init());

  while (1) {
    test_flock_wifi_spoof();
    vTaskDelay(pdMS_TO_TICKS(500));  // delay to allow cleanup
    test_flock_ble_spoof();
    vTaskDelay(pdMS_TO_TICKS(500));  // delay to allow cleanup
  }
}
