#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "wifi_scanner.h"

static const char *TAG = "MAIN";

// Callback quando o scan é completado
void on_scan_complete(int network_count)
{
    ESP_LOGI(TAG, "Callback: %d redes encontradas!", network_count);
    
    // Pisca o LED baseado no número de redes
    if (network_count > 0) {
        for (int i = 0; i < (network_count < 5 ? network_count : 5); i++) {
            wifi_scanner_led_set(1);
            vTaskDelay(200 / portTICK_PERIOD_MS);
            wifi_scanner_led_set(0);
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Aplicação Wi-Fi Scanner Iniciada!");
    
    // Inicializa o Wi-Fi Scanner
    wifi_scanner_init();
    wifi_scanner_led_init();
    
    // Registra callback para quando o scan completar
    wifi_scanner_register_scan_complete_callback(on_scan_complete);
    
    ESP_LOGI(TAG, "Scanner de Redes Wi-Fi Iniciado!");
    ESP_LOGI(TAG, "Scans automáticos a cada 30 segundos");
    
    // Acende LED para indicar que iniciou
    wifi_scanner_led_set(1);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    wifi_scanner_led_set(0);
    
    // Primeiro scan
    wifi_scanner_start_scan();
    
    // Loop principal - scans periódicos
    while (1) {
        // Pisca LED durante a espera
        for (int i = 0; i < 30; i++) {
            wifi_scanner_led_blink();
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        
        ESP_LOGI(TAG, "=== NOVO SCAN AUTOMÁTICO ===");
        wifi_scanner_start_scan();
    }
}