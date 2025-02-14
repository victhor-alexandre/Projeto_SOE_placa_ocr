# Projeto de Reconhecimento de Placas de Veículos

Este é um projeto da disciplina Sistemas Operacionais Embarcados da UnB, desenvolvido por Victhor Alexandre Vilarins, e Yuri Virginio.

O projeto foi desenvolvido em C++, e foi feito para rodar um programa para reconhecimento de placas de veículos do Brasil ou Mercosul. Ele foi criado para rodar em uma Raspberry Pi 3 Model B. Ele provavelmente roda em outros modelos, mas talvez será necessário ajustar os pinos no arquivo main.h

A idéia inicial do projeto surgiu de outro projeto feito em Python, que pode ser acessado em:
https://github.com/Gabriellimmaa/reconhecimento-e-leitura-placa-carro-ptBR

## Pré-requisitos

Para rodar o código, é necessário instalar os pacotes abaixo:

```sh
sudo apt-get update
sudo apt-get install libopencv-dev
sudo apt-get install tesseract-ocr libtesseract-dev
sudo apt-get install wiringpi
```

## Compilar e executar

Na primeira vez que instalar esse programa, é necessário abrir o terminal na raspberry pi e clonar esse repositório:

```sh
git clone https://github.com/victhor-alexandre/Projeto_SOE_placa_ocr

```

Para compilar, abra o diretório onde o repositório foi baixado e execute os comandos:

```sh
cd Projeto_SOE_placa_ocr
make clean
make
```

Depois de compilado, execute:

```sh
./main.out
```
