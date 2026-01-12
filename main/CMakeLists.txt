idf_component_register(SRCS "main.c" "flock_ble.c" "flock_wifi.c"
                    INCLUDE_DIRS "."
                    REQUIRES bt esp_wifi esp_system nvs_flash)
