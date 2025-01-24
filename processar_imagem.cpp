#include "processar_imagem.h"
#include <iostream>

namespace processarImagem {

cv::Mat processarImagem(const cv::Mat& imagemPlaca) {
    // Converter a imagem para tons de cinza
    cv::Mat imagemCinza;
    cv::cvtColor(imagemPlaca, imagemCinza, cv::COLOR_BGR2GRAY);
    //std::cout << "Imagem convertida para tons de cinza. Tamanho: " << imagemCinza.size() << std::endl;

    // Aplicar filtros para remover ruídos
    cv::Mat imagemFiltrada;
    cv::bilateralFilter(imagemCinza, imagemFiltrada, 11, 17, 17);
    //std::cout << "Filtro bilateral aplicado. Tamanho: " << imagemFiltrada.size() << std::endl;
    cv::GaussianBlur(imagemCinza, imagemFiltrada, cv::Size(5, 5), 0);

    // Aplicar limiarização adaptativa para tornar os caracteres mais destacados
    cv::Mat imagemLinearizada;
    cv::Canny(imagemFiltrada, imagemLinearizada, 30, 300, 3, false);
    //std::cout << "Canny aplicado. Tamanho: " << imagemLinearizada.size() << std::endl;

    cv::Mat imagemLimiarizada;
    cv::adaptiveThreshold(imagemLinearizada, imagemLimiarizada, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 4);
    //std::cout << "Limiarização adaptativa aplicada. Tamanho: " << imagemLimiarizada.size() << std::endl;

    return imagemLimiarizada;
}

} // namespace processarImagem