#ifndef PROCESSAR_CONTORNOS_H
#define PROCESSAR_CONTORNOS_H

#include <opencv2/opencv.hpp>
#include <vector>

namespace processarContornos {
    std::vector<std::pair<cv::Mat, cv::Mat>> processarContornos(cv::Mat& imagemOriginal, const cv::Mat& imagemProcessada);
}

#endif // PROCESSAR_CONTORNOS_H