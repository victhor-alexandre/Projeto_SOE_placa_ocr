#include "reconhecer.h"
#include <filesystem>
#include <tuple> // Include the tuple header

namespace fs = std::filesystem;

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

string detectarPlaca(const string& imagemPath) {
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

vector<string> reconhecerPlaca() {
    string pastaImagens = "images";
    vector<string> placas;
    for (const auto& imageEntry : fs::directory_iterator(pastaImagens)) {
        string imagemFile = imageEntry.path().string();
        if (imagemFile.find(".jpg") != string::npos || imagemFile.find(".jpeg") != string::npos || imagemFile.find(".png") != string::npos) {
            string placa = detectarPlaca(imagemFile);
            if (!placa.empty()) {
                placas.push_back(placa);
            }
        }
    }

    return placas;
}