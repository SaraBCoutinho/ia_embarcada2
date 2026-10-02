# HELL_WORLD_TFLITE

Projeto ESP-IDF para ESP32-S3 com dependências TensorFlow Lite Micro e ESP-NN gerenciadas pelo Component Manager. O exemplo de regressão senoidal será implementado nos arquivos C++ de `main/`.

## Build com Docker

Execute no PowerShell, a partir da raiz `HELLO_WORLD_TFLITE`:

```powershell
docker run --rm -v "${PWD}:/project" -w /project espressif/idf:v5.5 idf.py set-target esp32s3 build
```

O diretório `build/` e o arquivo `sdkconfig` são gerados pelo ESP-IDF durante a configuração e a compilação; não crie esses arquivos manualmente. Os artefatos `HELLO_WORLD_TFLITE.bin` e `HELLO_WORLD_TFLITE.elf` ficam em `HELLO_WORLD_TFLITE/build/`.

## Build no VS Code

Abra `ia_embarcada2.code-workspace` para carregar a pasta do projeto ESP-IDF como raiz do workspace. O botão de build da extensão só aparece depois que o ESP-IDF e o ambiente Python estiverem configurados na extensão.

## Wokwi

O `wokwi.toml` referencia os artefatos gerados pelo comando Docker. Abra o projeto no Wokwi depois que o firmware e o arquivo ELF forem gerados em `build/`.

## PSRAM

Os defaults habilitam PSRAM Octal a 80 MHz, adequado a placas ESP32-S3 com PSRAM Octal. Para uma placa sem PSRAM ou com PSRAM QSPI, ajuste as opções `CONFIG_SPIRAM_*` para o hardware antes do build.
