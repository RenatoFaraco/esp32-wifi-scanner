#include <stdio.h>
#include <ctype.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "wifi_scanner.h"
#include "tui_interface.h" 

static const char *TAG = "MAIN";

// Estados da máquina de estados
typedef enum {
    STATE_MENU,
    STATE_WAITING_SCAN_LIST,
    STATE_WAITING_SCAN_GRAPH,
    STATE_SHOWING_RESULT
} app_state_t;

static app_state_t current_state = STATE_MENU;

// Callback: Quando o Wi-Fi terminar, o main decide quem desenha na tela
void on_scan_complete(int network_count)
{
    if (current_state == STATE_WAITING_SCAN_LIST) {
        tui_print_scan_list(network_count);
        current_state = STATE_SHOWING_RESULT;
    } 
    else if (current_state == STATE_WAITING_SCAN_GRAPH) {
        tui_print_channel_graph(network_count);
        current_state = STATE_SHOWING_RESULT;
    }
    
    wifi_scanner_led_set(1);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    wifi_scanner_led_set(0);
}

void app_main(void)
{
    // Inicializações
    ESP_LOGI(TAG, "Sistema iniciado. Abrindo interface...");
    wifi_scanner_led_init();
    wifi_scanner_init();
    tui_init();
    
    wifi_scanner_register_scan_complete_callback(on_scan_complete);

    tui_show_menu();

    while (1) {
        int c = fgetc(stdin);
        
        if (c != EOF && c != 255) {
            c = toupper(c);

            // Se estiver vendo um resultado e apertar M, volta pro menu
            if (current_state == STATE_SHOWING_RESULT && c == 'M') {
                current_state = STATE_MENU;
                tui_show_menu();
            }
            // Se estiver no Menu, aceita comandos
            else if (current_state == STATE_MENU) {
                switch (c) {
                    case '1':
                        printf("\nIniciando Scan...\n");
                        current_state = STATE_WAITING_SCAN_LIST;
                        wifi_scanner_start_scan();
                        break;
                    case '2':
                        printf("\nGerando Gráfico...\n");
                        current_state = STATE_WAITING_SCAN_GRAPH;
                        wifi_scanner_start_scan();
                        break;
                    case '3':
                        tui_print_alert("Funcionalidade em desenvolvimento!");
                        vTaskDelay(1000 / portTICK_PERIOD_MS);
                        tui_show_menu();
                        break;
                    default:
                        break;
                }
            }
        }
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}