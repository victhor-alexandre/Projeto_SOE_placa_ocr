#include "main.h"
#include <filesystem>
#include <tuple> // Include the tuple header

using namespace cv;
using namespace std;
namespace fs = std::filesystem;

void exibirResultado(const Mat& imagemOriginal, const Mat& imagemProcessada, const Mat& placaRecortada, const Mat& placaRecortadaProcessada, const string& placaDetectada) {
    // Debugging: Print information about the images and detected plate
    //cout << "Exibindo resultados:" << endl;
    cout << "Placa Detectada: " << placaDetectada << endl;

    //cout << "Imagem Original - Tamanho: " << imagemOriginal.size() << ", Tipo: " << imagemOriginal.type() << endl;
    //imshow("Imagem Original", imagemOriginal);
    
    //cout << "Imagem Processada - Tamanho: " << imagemProcessada.size() << ", Tipo: " << imagemProcessada.type() << endl;
    //imshow("Imagem Processada", imagemProcessada);
    
    //cout << "Placa Recortada - Tamanho: " << placaRecortada.size() << ", Tipo: " << placaRecortada.type() << endl;
    //imshow("Placa Recortada", placaRecortada);
    
    //cout << "Placa Recortada Processada - Tamanho: " << placaRecortadaProcessada.size() << ", Tipo: " << placaRecortadaProcessada.type() << endl;
    imshow("Placa Recortada Processada", placaRecortadaProcessada);
    waitKey(0);
}

void detectarPlaca(const string& imagemPath) {
    Mat imagemOriginal = imread(imagemPath);
    if (imagemOriginal.empty()) {
        cerr << "Erro ao abrir a imagem!" << endl;
        return;
    }

    //Processa a imagem, reduzindo o ruído e destacando as bordas
    //Mat imagemProcessada = processarImagem::processarImagem(imagemOriginal);
    Mat imagemProcessada = processarImagem::processarImagem(imagemOriginal);

    //Processa os contornos da imagem, identificando possíveis placas
    vector<pair<Mat, Mat>> possiveisPlacas = processarContornos::processarContornos(imagemOriginal, imagemProcessada);

    if (possiveisPlacas.empty()) {
        return;
    }

    string placaDetectada;
    Mat placaRecortada, placaRecortadaProcessada;
    tie(placaDetectada, placaRecortada, placaRecortadaProcessada) = aplicarOCR::aplicarOCR(possiveisPlacas);

    if(!placaDetectada.empty()) {
        exibirResultado(imagemOriginal, imagemProcessada, placaRecortada, placaRecortadaProcessada, placaDetectada);
    }
}

int main(int argc, char** argv) {
    string pastaImagens = "images";
    for (const auto& entry : fs::directory_iterator(pastaImagens)) {
        string imagemFile = entry.path().string();
        if (imagemFile.find(".jpg") != string::npos || imagemFile.find(".jpeg") != string::npos || imagemFile.find(".png") != string::npos) {
            detectarPlaca(imagemFile);
        }
    }

    return 0;
}