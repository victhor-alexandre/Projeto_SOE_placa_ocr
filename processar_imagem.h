#ifndef PROCESSAR_IMAGEM_H
#define PROCESSAR_IMAGEM_H

#include <opencv2/opencv.hpp>


namespace processarImagem {
    cv::Mat processarImagem(const cv::Mat& imagemPlaca);
}

#endif // PROCESSAR_IMAGEM_H