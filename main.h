#ifndef MAIN_H
#define MAIN_H

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

void detectarPlaca(const std::string& imagemPath);
void exibirResultado(const cv::Mat& imagemOriginal, const cv::Mat& placaRecortadaProcessada, const std::string& placaDetectada);

#endif // MAIN_H