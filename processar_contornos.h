#ifndef PROCESSAR_CONTORNOS_H
#define PROCESSAR_CONTORNOS_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <pthread.h>
using namespace cv;
using namespace std;

namespace processarContornos {

struct validarContornoArgs {
    vector<Point> contorno;
    Mat imagemOriginal;
};

vector<Mat> processarContornos(Mat& imagemOriginal, const Mat& imagemProcessada);
void* validarContorno(void* args);

} // namespace processarContornos

#endif // PROCESSAR_CONTORNOS_H