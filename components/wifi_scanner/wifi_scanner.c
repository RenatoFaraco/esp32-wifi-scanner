#include "wifi_scanner.h"
#include "string.h"
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

// Variáveis privadas
static wifi_network_t networks[WIFI_SCANNER_MAX_NETWORKS];
static int network_count = 0;
static void (*scan_complete_callback)(int) = NULL;

// Função para converter auth mode para string
const char* wifi_auth_mode_to_string(wifi_auth_mode_t auth_mode)
{
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

// Função para classificar a força do sinal
const char* wifi_rssi_to_strength(int8_t rssi)
{
    if (rssi >= -50) return "Excelente";
    else if (rssi >= -60) return "Bom";
    else if (rssi >= -70) return "Regular";
    else return "Fraco";
}

// Handler para resultados do scan
static void wifi_scan_handler(void* arg, esp_event_base_t event_base, 
                              int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_SCAN_DONE) {
        ESP_LOGI(TAG, "Scan Wi-Fi completo!");
        
        uint16_t ap_count = 0;
        esp_wifi_scan_get_ap_num(&ap_count);
        
        if (ap_count == 0) {
            ESP_LOGI(TAG, "Nenhuma rede encontrada");
            if (scan_complete_callback) {
                scan_complete_callback(0);
            }
            return;
        }
        
        wifi_ap_record_t *ap_records = malloc(sizeof(wifi_ap_record_t) * ap_count);
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_count, ap_records));
        
        network_count = (ap_count < WIFI_SCANNER_MAX_NETWORKS) ? ap_count : WIFI_SCANNER_MAX_NETWORKS;
        
        ESP_LOGI(TAG, "================================================");
        ESP_LOGI(TAG, "          REDES WI-FI ENCONTRADAS");
        ESP_LOGI(TAG, "================================================");
        
        for (int i = 0; i < network_count; i++) {
            strncpy(networks[i].ssid, (char*)ap_records[i].ssid, sizeof(networks[i].ssid) - 1);
            networks[i].ssid[sizeof(networks[i].ssid) - 1] = '\0';
            networks[i].rssi = ap_records[i].rssi;
            networks[i].auth_mode = ap_records[i].authmode;
            networks[i].channel = ap_records[i].primary;
            
            // Mostra informações da rede
            ESP_LOGI(TAG, "%2d. %-32s", i + 1, networks[i].ssid);
            ESP_LOGI(TAG, "     Sinal: %s (%d dBm)", wifi_rssi_to_strength(networks[i].rssi), networks[i].rssi);
            ESP_LOGI(TAG, "     Segurança: %s", wifi_auth_mode_to_string(networks[i].auth_mode));
            ESP_LOGI(TAG, "     Canal: %d", networks[i].channel);
            ESP_LOGI(TAG, "     ---");
        }
        
        ESP_LOGI(TAG, "Total: %d redes encontradas", network_count);
        ESP_LOGI(TAG, "================================================");
        
        free(ap_records);
        
        // Chama callback se registrado
        if (scan_complete_callback) {
            scan_complete_callback(network_count);
        }
    }
}

// Função para executar o scan
void wifi_scanner_start_scan(void)
{
    ESP_LOGI(TAG, "Iniciando scan de redes Wi-Fi...");
    
    // Configuração do scan
    wifi_scan_config_t scan_config = {
        .ssid = NULL,           // Scan todas as redes
        .bssid = NULL,          // Qualquer BSSID
        .channel = 0,           // Todos os canais
        .show_hidden = true,    // Mostra redes ocultas
        .scan_type = WIFI_SCAN_TYPE_ACTIVE,
        .scan_time = {
            .active = {
                .min = 100,     // Tempo mínimo por canal (ms)
                .max = 300      // Tempo máximo por canal (ms)
            }
        }
    };
    
    // Executa o scan
    ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, false));
}

void wifi_scanner_init(void)
{
    // Inicializa NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Inicializa Wi-Fi em modo Station
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    
    // Registra o handler para evento de scan completo
    esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_SCAN_DONE, &wifi_scan_handler, NULL);
    
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Wi-Fi Scanner inicializado!");
}

void wifi_scanner_register_scan_complete_callback(void (*callback)(int network_count))
{
    scan_complete_callback = callback;
}

int wifi_scanner_get_network_count(void)
{
    return network_count;
}

wifi_network_t* wifi_scanner_get_network(int index)
{
    if (index >= 0 && index < network_count) {
        return &networks[index];
    }
    return NULL;
}

void wifi_scanner_led_init(void)
{
    gpio_reset_pin(WIFI_SCANNER_LED_GPIO);
    gpio_set_direction(WIFI_SCANNER_LED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(WIFI_SCANNER_LED_GPIO, 0);
}

void wifi_scanner_led_blink(void)
{
    static uint8_t led_state = 0;
    led_state = !led_state;
    gpio_set_level(WIFI_SCANNER_LED_GPIO, led_state);
}

void wifi_scanner_led_set(bool state)
{
    gpio_set_level(WIFI_SCANNER_LED_GPIO, state);
}