#ifndef APLICAR_OCR_H
#define APLICAR_OCR_H

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <string>
#include <vector>

namespace aplicarOCR {
    std::string encontrarPlaca(const std::string& str);
    std::string encontrarPlacaMercosul(const std::string& str);
    std::tuple<std::string, cv::Mat, cv::Mat> aplicarOCR(const std::vector<std::pair<cv::Mat, cv::Mat>>& possiveisPlacas);
}

#endif // APLICAR_OCR_H