#ifndef RECONHECER_H
#define RECONHECER_H

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <tuple>
#include "aplicar_ocr.h"
#include "processar_contornos.h"
#include "processar_imagem.h"

using namespace cv;
using namespace std;

namespace reconhecer {
    string detectarPlaca(const string& imagemPath);
    vector<string> reconhecerPlaca();
    void exibirResultado(const cv::Mat& imagemOriginal, const cv::Mat& placaRecortadaProcessada, const std::string& placaDetectada);
}

#endif // RECONHECER_H