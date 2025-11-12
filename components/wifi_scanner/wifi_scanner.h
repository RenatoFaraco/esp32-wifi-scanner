#ifndef WIFI_SCANNER_H
#define WIFI_SCANNER_H

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "driver/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

// Configurações
#define WIFI_SCANNER_MAX_NETWORKS 20
#define WIFI_SCANNER_LED_GPIO     2

// Estrutura para armazenar informações das redes
typedef struct {
    char ssid[32];
    int8_t rssi;
    wifi_auth_mode_t auth_mode;
    uint8_t channel;
} wifi_network_t;

// Funções públicas
void wifi_scanner_init(void);
void wifi_scanner_start_scan(void);
void wifi_scanner_register_scan_complete_callback(void (*callback)(int network_count));
int wifi_scanner_get_network_count(void);
wifi_network_t* wifi_scanner_get_network(int index);
void wifi_scanner_led_init(void);
void wifi_scanner_led_blink(void);
void wifi_scanner_led_set(bool state);

// Funções utilitárias
const char* wifi_auth_mode_to_string(wifi_auth_mode_t auth_mode);
const char* wifi_rssi_to_strength(int8_t rssi);

#ifdef __cplusplus
}
#endif

#endif // WIFI_SCANNER_H