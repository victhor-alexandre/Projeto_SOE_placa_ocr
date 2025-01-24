#include "processar_imagem.h"
#include <iostream>
using namespace std;
using namespace cv;

namespace processarImagem {

Mat processarImagem(const Mat& imagemPlaca) {
    // Converter a imagem para tons de cinza
    Mat imagemCinza;
    cvtColor(imagemPlaca, imagemCinza, COLOR_BGR2GRAY);
    //cout << "Imagem convertida para tons de cinza. Tamanho: " << imagemCinza.size() << endl;

    // Aplicar filtros para remover ruídos
    Mat imagemFiltrada;
    bilateralFilter(imagemCinza, imagemFiltrada, 11, 17, 17);
    //cout << "Filtro bilateral aplicado. Tamanho: " << imagemFiltrada.size() << endl;
    GaussianBlur(imagemCinza, imagemFiltrada, Size(5, 5), 0);

    // Aplicar limiarização adaptativa para tornar os caracteres mais destacados
    Mat imagemLinearizada;
    Canny(imagemFiltrada, imagemLinearizada, 30, 300, 3, false);
    //std::cout << "Canny aplicado. Tamanho: " << imagemLinearizada.size() << std::endl;

    Mat imagemLimiarizada;
    adaptiveThreshold(imagemLinearizada, imagemLimiarizada, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 4);
    //cout << "Limiarização adaptativa aplicada. Tamanho: " << imagemLimiarizada.size() << endl;

    return imagemLimiarizada;
}

} // namespace processarImagem