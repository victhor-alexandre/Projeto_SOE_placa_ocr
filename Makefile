all:
	g++ -o main.out main.cpp aplicar_ocr.cpp processar_contornos.cpp processar_imagem.cpp `pkg-config --cflags --libs opencv4 tesseract` -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lpthread -llept -ltesseract
clean:
	rm main.out