#ifndef TUI_INTERFACE_H
#define TUI_INTERFACE_H

#include "wifi_scanner.h" // Para conhecer a struct wifi_network_t

// Definições de Cores (Macros para facilitar o uso em todo o projeto)
#define TUI_COLOR_RESET   "\e[0m"
#define TUI_COLOR_RED     "\e[31m"
#define TUI_COLOR_GREEN   "\e[32m"
#define TUI_COLOR_YELLOW  "\e[33m"
#define TUI_COLOR_CYAN    "\e[36m"
#define TUI_CLS           "\e[2J\e[H"

// Inicializa a TUI (se precisar configurar algo no futuro)
void tui_init(void);

// Limpa a tela e mostra o Menu Principal
void tui_show_menu(void);

// Exibe a lista de redes (o scan clássico) formatada
void tui_print_scan_list(int network_count);

// Exibe o Gráfico de Barras (Issue #3)
void tui_print_channel_graph(int network_count);

// Exibe mensagem de erro ou aviso
void tui_print_alert(const char* msg);

#endif // TUI_INTERFACE_H