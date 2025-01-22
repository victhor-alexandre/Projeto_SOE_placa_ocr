all:
	g++ -o main.out main.cpp aplicar_ocr.cpp processar_contornos.cpp processar_imagem.cpp `pkg-config --cflags --libs opencv4 tesseract`
clean:
	rm main.out