#include "processar_imagem.h"
#include <iostream>
using namespace std;
using namespace cv;

namespace processarImagem {

Mat processarImagem(const Mat& imagemPlaca) {
    // Converter a imagem para tons de cinza
    cvtColor(imagemPlaca, imagemPlaca, COLOR_BGR2GRAY);

    // Aplicar filtros para remover ruídos
    bilateralFilter(imagemPlaca, imagemPlaca, 11, 17, 17);

    GaussianBlur(imagemPlaca, imagemPlaca, Size(5, 5), 0);

    // Aplicar limiarização adaptativa para tornar os caracteres mais destacados
    Canny(imagemPlaca, imagemPlaca, 30, 300, 3, false);

    adaptiveThreshold(imagemPlaca, imagemPlaca, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 4);

    return imagemPlaca;
}

} // namespace processarImagem