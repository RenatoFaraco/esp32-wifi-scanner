#ifndef WIFI_SCANNER_H
#define WIFI_SCANNER_H

#include "esp_wifi.h"
#include <stdbool.h>

#define WIFI_SCANNER_MAX_NETWORKS 30
#define WIFI_SCANNER_LED_GPIO 48 
typedef struct {
    char ssid[33];
    int8_t rssi;
    wifi_auth_mode_t auth_mode;
    uint8_t channel;
} wifi_network_t;

void wifi_scanner_init(void);
void wifi_scanner_start_scan(void);
void wifi_scanner_register_scan_complete_callback(void (*callback)(int network_count));
int wifi_scanner_get_network_count(void);
wifi_network_t* wifi_scanner_get_network(int index);

void wifi_scanner_led_init(void);
void wifi_scanner_led_set(bool state);
void wifi_scanner_led_blink(void);

#endif