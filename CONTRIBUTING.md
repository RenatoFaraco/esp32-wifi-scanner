# 🤝 Contributing Guide

Obrigado por considerar contribuir com o **ESP32 Wi-Fi Scanner**! 🎉  
Este projeto é aberto a melhorias, correções e novas funcionalidades.

---

## 📌 Como contribuir

### 1. Fork do repositório
Crie um fork do projeto para sua conta:

```bash
git fork https://github.com/RenatoFaraco/esp32-wifi-scanner.git
git clone https://github.com/SEU-USUARIO/esp32-wifi-scanner.git
cd esp32-wifi-scanner
```

### 3. Crie uma branch

Use nomes descritivos:
```bash
git checkout -b feature/nova-funcionalidade
```

Exemplos:

`feature/channel-graph-improvement`
`fix/scan-crash-bug`
`docs/readme-update`

### 4. Faça suas alterações
Mantenha o código limpo e organizado
Siga o padrão já existente no projeto
Comente partes importantes do código
Evite código desnecessário

### 5. Teste antes de enviar

Certifique-se de que:

O código compila corretamente
O scan Wi-Fi funciona
Não quebrou funcionalidades existentes
A saída no Serial Monitor está legível
6. Commit das mudanças

Use mensagens claras:
```bash
git commit -m "feat: adiciona melhoria no gráfico de canais"
```

Padrão recomendado:

`feat`: nova funcionalidade
`fix`: correção de bug
`docs`: documentação
`refactor`: melhoria de código sem alterar comportamento

Push e Pull Request

```bash
git push origin feature/nova-funcionalidade
```
