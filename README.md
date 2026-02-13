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

O projeto utiliza códigos de escape ANSI para uma experiência interativa diretamente no terminal:

### Lista de Redes (Opção 1)
```text
=== RESULTADO DO SCAN ===
ID   | SSID                             | SINAL    | CH
----------------------------------------------------------
1    | Rede 1                           | -34 dBm  | 10
2    | Rede 2                           | -44 dBm  |  9
3    | <Rede Oculta>                    | -52 dBm  |  3
4    | Rede 3                           | -62 dBm  | 11
----------------------------------------------------------
[Pressione 'M' para voltar ao menu]
```

### Gráfico de Canais (Opção 2)
```text
=== OCUPAÇÃO DO ESPECTRO 2.4GHz ===

CH 01 | ## (2)
CH 06 | . 
CH 08 | ###### (6) [Saturado!]
CH 11 | ### (3)

===================================
[Pressione 'M' para voltar ao menu]
```
## Aplicações

 - Auditoria de Segurança: Identificação de redes não autorizadas.

 - Análise de Espectro: Escolha do melhor canal para evitar interferências.

 - Site Survey: Mapeamento básico de cobertura de sinal.