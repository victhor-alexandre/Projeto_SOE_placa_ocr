para rodar o código, é necessário instalar os pacotes abaixo

sudo apt-get update
sudo apt-get install libopencv-dev
sudo apt-get install tesseract-ocr libtesseract-dev

para compilar, pode-se executar o comando make, ou o comando abaixo
g++ -o main.out main.cpp aplicar_ocr.cpp processar_contornos.cpp processar_imagem.cpp `pkg-config --cflags --libs opencv4 tesseract`