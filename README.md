# ESP32 Wi-Fi Scanner

Um scanner de redes Wi-Fi para ESP32 que detecta e lista todas as redes disponíveis na área.

## Funcionalidades

- Scan de redes Wi-Fi disponíveis
- Mostra força do sinal (RSSI) 
- Identifica tipo de segurança
- Mostra canal de operação
- LED indicador de status
- Scan automático a cada 30 segundos

## Hardware Necessário

- Placa ESP32 (DevKit, NodeMCU, etc.)
- LED opcional no GPIO2 para feedback visual
- Cabo USB-C para upload.

## Como configurar

1. Certifique-se de ter o [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html#installation).
2. No Gerenciador de Placas, instale o suporte para **ESP32 by Espressif Systems**.
3. Selecione a placa **ESP32S3 Dev Module**.
4. Clone este repositório:
 ```bash
   git clone [https://github.com/RenatoFaraco/esp32-wifi-scanner.git](https://github.com/RenatoFaraco/esp32-wifi-scanner.git)
 ```
5. Carregue o código e abra o Serial Monitor

## Exemplo de Saída no Serial
```bash
Iniciando Scan...
10 redes encontradas:
1: Nome_Da_Rede (-65 dBm) Ch: 6 [WPA2_PSK]
2: Wi-Fi_Visitante (-82 dBm) Ch: 11 [WPA3_SAE]
...
```
## Aplicações

 - Auditoria de Segurança: Identificação de redes não autorizadas.

 - Análise de Espectro: Escolha do melhor canal para evitar interferências.

 - Site Survey: Mapeamento básico de cobertura de sinal.