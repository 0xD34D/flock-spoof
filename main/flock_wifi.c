#include "flock_wifi.h"

#include "esp_check.h"
#include "esp_log.h"
#include "esp_random.h"
#include "esp_wifi.h"

static const char* TAG = "flock-wifi";

esp_err_t wifi_init(void) {
  ESP_LOGI(TAG, "Initializing WiFi");
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_RETURN_ON_ERROR(esp_wifi_init(&cfg), TAG, "Failed to initialize WiFi");
  ESP_RETURN_ON_ERROR(esp_wifi_set_mode(WIFI_MODE_AP), TAG,
                      "Failed to set WiFi mode");
  return ESP_OK;
}

esp_err_t wifi_set_mac_prefix(const uint8_t prefix[]) {
  uint8_t custom_mac[6];
  esp_fill_random(custom_mac, sizeof(custom_mac));

  memcpy(custom_mac, prefix, 3);
  ESP_LOGI(TAG, "Setting custom MAC address: %02x:%02x:%02x:%02x:%02x:%02x",
           custom_mac[0], custom_mac[1], custom_mac[2], custom_mac[3],
           custom_mac[4], custom_mac[5]);
  ESP_RETURN_ON_ERROR(esp_wifi_set_mac(WIFI_IF_AP, custom_mac), TAG,
                      "Failed to set custom MAC address");

  return ESP_OK;
}

esp_err_t wifi_start_flock_spoof(const char* ssid, const uint8_t mac_prefix[]) {
  ESP_LOGI(TAG, "Starting Flock WiFi spoofing: ssid=%s mac_prefix=%s",
           ssid ? ssid : "no", mac_prefix ? "yes" : "no");
  if (mac_prefix) {
    ESP_RETURN_ON_ERROR(wifi_set_mac_prefix(mac_prefix), TAG,
                        "Failed to set Flock MAC address");
  }

  wifi_config_t wifi_config = {
      .ap =
          {
              .ssid = "flock",
              .ssid_len = 5,
              .ssid_hidden = 0,
              .password = "FlockYouToo!",
              .authmode = WIFI_AUTH_WPA2_PSK,
              .channel = (esp_random() % 11) + 1,
              .beacon_interval = 100,
          },
  };

  if (ssid) {
    memcpy(wifi_config.ap.ssid, ssid, strlen(ssid));
    wifi_config.ap.ssid_len = strlen(ssid);
  } else {
    wifi_config.ap.ssid_hidden = 1;
  }

  ESP_RETURN_ON_ERROR(esp_wifi_set_config(WIFI_IF_AP, &wifi_config), TAG,
                      "Failed to set WiFi configuration");
  ESP_RETURN_ON_ERROR(esp_wifi_start(), TAG, "Failed to start WiFi");

  return ESP_OK;
}

esp_err_t wifi_stop_flock_spoof(void) {
  ESP_LOGI(TAG, "Stopping Flock WiFi");
  ESP_RETURN_ON_ERROR(esp_wifi_stop(), TAG, "Failed to stop WiFi");
  return ESP_OK;
}
