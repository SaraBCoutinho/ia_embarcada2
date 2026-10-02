# HELLO_WORLD_TFLITE

Projeto desenvolvido com ESP-IDF para ESP32-S3, utilizando TensorFlow Lite Micro e ESP-NN por meio do ESP-IDF Component Manager.

O projeto tem como objetivo reproduzir e analisar o exemplo **Hello World com TensorFlow Lite Micro** em um ESP32-S3, utilizando o Wokwi para simulação do dispositivo.

## Tecnologias utilizadas

- ESP32-S3
- ESP-IDF 5.1
- TensorFlow Lite Micro
- ESP-NN
- Docker
- Wokwi
- C/C++

## Estrutura do projeto

Os principais arquivos da aplicação estão no diretório `main/`.

Entre eles:

- `main.cc`: ponto de entrada da aplicação (`app_main`);
- `main_functions.cc`: inicialização e execução da inferência com TensorFlow Lite Micro;
- `model.cc` / `model.h`: modelo TensorFlow Lite embarcado;
- `output_handler.cc`: tratamento da saída do modelo;
- `constants.cc` / `constants.h`: constantes utilizadas pela aplicação.

## Build com Docker

O projeto foi compilado utilizando a imagem ESP-IDF 5.1 em Docker.

A partir da raiz do projeto:

```powershell
docker run -it --name esp-idf -v "${PWD}:/project" -w /project espressif/idf:v5.1
