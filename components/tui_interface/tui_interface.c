#include <stdio.h>
#include <string.h>
#include "tui_interface.h"
#include "wifi_scanner.h" 

void tui_init(void) {
    // Por enquanto não precisa de setup especial, mas já deixamos pronto
}

void tui_show_menu(void) {
    printf(TUI_CLS); 
    printf(TUI_COLOR_CYAN "########################################\n");
    printf("#      ESP32-S3 WI-FI SCANNER v2.0     #\n");
    printf("########################################" TUI_COLOR_RESET "\n\n");
    
    printf("Escolha uma opção:\n");
    printf(TUI_COLOR_GREEN " [1] " TUI_COLOR_RESET "Listar Redes (Scanner Padrão)\n");
    printf(TUI_COLOR_GREEN " [2] " TUI_COLOR_RESET "Gráfico de Ocupação de Canais\n");
    printf(TUI_COLOR_RED   " [3] " TUI_COLOR_RESET "Detecção de Rogue APs (Em breve)\n");
    printf("\nDigite sua opção: ");
}

void tui_print_scan_list(int network_count) {
    printf(TUI_CLS);
    printf(TUI_COLOR_GREEN "=== RESULTADO DO SCAN ===" TUI_COLOR_RESET "\n");
    
    if (network_count == 0) {
        printf(TUI_COLOR_RED "Nenhuma rede encontrada.\n" TUI_COLOR_RESET);
        return;
    }

    printf(TUI_COLOR_CYAN "%-4s | %-32s | %-10s | %s" TUI_COLOR_RESET "\n", "ID", "SSID", "SINAL", "CH");
    printf("----------------------------------------------------------\n");

    for (int i = 0; i < network_count; i++) {
        wifi_network_t* net = wifi_scanner_get_network(i);
        if (net) {
            const char* display_ssid = (strlen(net->ssid) == 0) ? "<Rede Oculta>" : net->ssid;

            printf("%-4d | %-32s | %-4d dBm | %2d\n", 
                   i + 1, 
                   display_ssid, 
                   net->rssi, 
                   net->channel);
        }
    }
    printf("\n" TUI_COLOR_YELLOW "[Pressione 'M' para voltar ao menu]" TUI_COLOR_RESET "\n");
}

void tui_print_channel_graph(int network_count) {
    int channel_counts[15] = {0}; 
    
    for (int i = 0; i < network_count; i++) {
        wifi_network_t* net = wifi_scanner_get_network(i);
        if (net && net->channel >= 1 && net->channel <= 14) {
            channel_counts[net->channel]++;
        }
    }

    printf(TUI_CLS);
    printf(TUI_COLOR_CYAN "=== OCUPAÇÃO DO ESPECTRO 2.4GHz ===" TUI_COLOR_RESET "\n\n");
    
    for (int ch = 1; ch <= 13; ch++) {
        printf("CH %02d | ", ch);
        
        if (channel_counts[ch] == 0) {
            printf(TUI_COLOR_GREEN "." TUI_COLOR_RESET);
        } else {
            const char* bar_color = (channel_counts[ch] > 5) ? TUI_COLOR_RED : TUI_COLOR_YELLOW;
            printf("%s", bar_color);
            for (int k = 0; k < channel_counts[ch]; k++) printf("#");
            printf(TUI_COLOR_RESET " (%d)", channel_counts[ch]);
        }
        printf("\n");
    }
    printf("\n" TUI_COLOR_CYAN "===================================" TUI_COLOR_RESET "\n");
    printf("[Pressione 'M' para voltar ao menu]\n");
}

void tui_print_alert(const char* msg) {
    printf(TUI_COLOR_RED "\n[ALERTA] %s" TUI_COLOR_RESET "\n", msg);
}