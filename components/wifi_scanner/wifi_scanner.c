#include "wifi_scanner.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

static const char *TAG = "WIFI_SCANNER";

static wifi_network_t networks[WIFI_SCANNER_MAX_NETWORKS];
static int network_count = 0;
static void (*scan_complete_callback)(int) = NULL;

const char* wifi_auth_mode_to_string(wifi_auth_mode_t auth_mode) {
    switch(auth_mode) {
        case WIFI_AUTH_OPEN: return "OPEN";
        case WIFI_AUTH_WEP: return "WEP";
        case WIFI_AUTH_WPA_PSK: return "WPA_PSK";
        case WIFI_AUTH_WPA2_PSK: return "WPA2_PSK";
        case WIFI_AUTH_WPA_WPA2_PSK: return "WPA_WPA2_PSK";
        case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2_ENTERPRISE";
        case WIFI_AUTH_WPA3_PSK: return "WPA3_PSK";
        case WIFI_AUTH_WPA2_WPA3_PSK: return "WPA2_WPA3_PSK";
        default: return "UNKNOWN";
    }
}

const char* wifi_rssi_to_strength(int8_t rssi) {
    if (rssi >= -50) return "Excelente";
    else if (rssi >= -60) return "Bom";
    else if (rssi >= -70) return "Regular";
    else return "Fraco";
}

static void wifi_scan_handler(void* arg, esp_event_base_t event_base, 
                              int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_SCAN_DONE) {
        uint16_t ap_count = 0;
        esp_wifi_scan_get_ap_num(&ap_count);
        
        if (ap_count == 0) {
            if (scan_complete_callback) scan_complete_callback(0);
            return;
        }
        
        wifi_ap_record_t *ap_records = malloc(sizeof(wifi_ap_record_t) * ap_count);
        if (esp_wifi_scan_get_ap_records(&ap_count, ap_records) == ESP_OK) {
            network_count = (ap_count < WIFI_SCANNER_MAX_NETWORKS) ? ap_count : WIFI_SCANNER_MAX_NETWORKS;
            
            for (int i = 0; i < network_count; i++) {
                strncpy(networks[i].ssid, (char*)ap_records[i].ssid, sizeof(networks[i].ssid) - 1);
                networks[i].ssid[sizeof(networks[i].ssid) - 1] = '\0';
                networks[i].rssi = ap_records[i].rssi;
                networks[i].auth_mode = ap_records[i].authmode;
                networks[i].channel = ap_records[i].primary;
            }
        }
        free(ap_records);
        
        if (scan_complete_callback) {
            scan_complete_callback(network_count);
        }
    }
}

void wifi_scanner_start_scan(void) {
    wifi_scan_config_t scan_config = {
        .ssid = NULL,
        .bssid = NULL,
        .channel = 0,
        .show_hidden = true,
        .scan_type = WIFI_SCAN_TYPE_ACTIVE,
        .scan_time.active.min = 100,
        .scan_time.active.max = 300
    };
    esp_wifi_scan_start(&scan_config, false);
}

void wifi_scanner_init(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_SCAN_DONE, &wifi_scan_handler, NULL);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();
}

void wifi_scanner_register_scan_complete_callback(void (*callback)(int)) {
    scan_complete_callback = callback;
}

int wifi_scanner_get_network_count(void) {
    return network_count;
}

wifi_network_t* wifi_scanner_get_network(int index) {
    if (index >= 0 && index < network_count) return &networks[index];
    return NULL;
}

void wifi_scanner_led_init(void) {
    gpio_reset_pin(WIFI_SCANNER_LED_GPIO);
    gpio_set_direction(WIFI_SCANNER_LED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(WIFI_SCANNER_LED_GPIO, 0);
}

void wifi_scanner_led_blink(void) {
    static uint8_t state = 0;
    state = !state;
    gpio_set_level(WIFI_SCANNER_LED_GPIO, state);
}

void wifi_scanner_led_set(bool state) {
    gpio_set_level(WIFI_SCANNER_LED_GPIO, state);
}