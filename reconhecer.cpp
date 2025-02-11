#include "reconhecer.h"
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include <filesystem>
#include <tuple> // Include the tuple header

using namespace std;
using namespace cv;
namespace fs = std::filesystem;

namespace reconhecer {
    void exibirResultado(const Mat& imagemOriginal, const Mat& placaRecortadaProcessada, const string& placaDetectada) {
        // Debugging: Print information about the images and detected plate
        //cout << "Exibindo resultados:" << endl;
        cout << "Placa Detectada: " << placaDetectada << endl;

        //cout << "Imagem Original - Tamanho: " << imagemOriginal.size() << ", Tipo: " << imagemOriginal.type() << endl;
        //imshow("Imagem Original", imagemOriginal);
        
        //cout << "Placa Recortada Processada - Tamanho: " << placaRecortadaProcessada.size() << ", Tipo: " << placaRecortadaProcessada.type() << endl;
        //imshow("Placa Recortada Processada", placaRecortadaProcessada);
        //waitKey(0);
    }

    std::string detectarPlaca(const std::string& imagemPath) {
        Mat imagemOriginal = imread(imagemPath);
        if (imagemOriginal.empty()) {
            cerr << "Erro ao abrir a imagem!" << endl;
            return "";
        }

        //Processa a imagem, reduzindo o ruído e destacando as bordas
        //Mat imagemProcessada = processarImagem::processarImagem(imagemOriginal);
        Mat imagemProcessada = processarImagem::processarImagem(imagemOriginal);

        //Processa os contornos da imagem, identificando possíveis placas
        vector<Mat> possiveisPlacas = processarContornos::processarContornos(imagemOriginal, imagemProcessada);
        //cout << "Placas após processar: " << possiveisPlacas.size() << endl;
        if (possiveisPlacas.empty()) {
            return "";
        }

        string placaDetectada;
        Mat placaRecortadaProcessada;
        tie(placaDetectada, placaRecortadaProcessada) = aplicarOCR::aplicarOCR(possiveisPlacas);

        if (!placaDetectada.empty()) {
            exibirResultado(imagemOriginal, placaRecortadaProcessada, placaDetectada);
        }

        return placaDetectada;
    }


        std::string reconhecerPlaca() {
            string imagemPath = "images/image.png";
            string placa = detectarPlaca(imagemPath);
            return placa;
        }
}