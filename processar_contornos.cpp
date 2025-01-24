#include "processar_contornos.h"
#include <iostream>
using namespace std;
using namespace cv;

namespace processarContornos {

std::vector<std::pair<cv::Mat, cv::Mat>> processarContornos(cv::Mat& imagemOriginal, const cv::Mat& imagemProcessada) {
    vector<vector<Point>> contornos;
    findContours(imagemProcessada, contornos, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    vector<pair<Mat, Mat>> possiveisPlacas;

    for (const auto& contorno : contornos) {
        // Aproximar o contorno por um polígono
        double perimetro = arcLength(contorno, true);
        vector<Point> aprox;
        approxPolyDP(contorno, aprox, 0.0125 * perimetro, true);
        double area = contourArea(contorno);
        Rect boundingRect = cv::boundingRect(contorno);
        
        // verificar se a altura é maior que a largura
        if (boundingRect.height > boundingRect.width) {
            continue;
        }
        
        // verificar se há uma proporcao onde a altura é menor que 20% da largura
        if (boundingRect.height < (boundingRect.width * 0.1)) {
            continue;
        }

        if (area < 10000 || area > 80000) {
            continue;
        }
        // Debugging: Print contour information
        //std::cout << "Contour area: " << area << ", Bounding rect: " << boundingRect << std::endl;
        
        // Se o polígono tiver quatro lados, consideramos que é um retângulo
        if (aprox.size() >= 4 && aprox.size() < 10) {
            cv::drawContours(imagemOriginal, std::vector<std::vector<cv::Point>>{aprox}, -1, cv::Scalar(0, 255, 0), 2);

            // recortar a imagem da placa
            cv::Mat imagemRecortada = imagemOriginal(boundingRect);

            // Debugging: Print cropped image information
            //std::cout << "Cropped image size: " << imagemRecortada.size() << std::endl;

            // converter a imagem da placa para tons de cinza
            cv::Mat imagemRecortadaCinza;
            cv::cvtColor(imagemRecortada, imagemRecortadaCinza, cv::COLOR_BGR2GRAY);

            // aplicar limiarização para tornar os caracteres mais destacados
            cv::Mat imagemRecortadaLimiarizada;
            cv::threshold(imagemRecortadaCinza, imagemRecortadaLimiarizada, 0, 255,  cv::THRESH_OTSU);

            // aplicar operação de fechamento para preencher regiões de contornos
            cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
            cv::Mat imagemRecortadaProcessada;
            cv::morphologyEx(imagemRecortadaLimiarizada, imagemRecortadaProcessada, cv::MORPH_CLOSE, kernel);

            // aplicar operação de abertura para remover ruídos
            kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
            cv::morphologyEx(imagemRecortadaProcessada, imagemRecortadaProcessada, cv::MORPH_OPEN, kernel, cv::Point(-1, -1), 1);

            // aplicar operação de dilatação para aumentar a espessura dos caracteres
            kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
            cv::dilate(imagemRecortadaProcessada, imagemRecortadaProcessada, kernel, cv::Point(-1, -1), 1);

            // aplicar operação de erosão para reduzir a espessura dos caracteres
            cv::erode(imagemRecortadaProcessada, imagemRecortadaProcessada, kernel, cv::Point(-1, -1), 1);

            possiveisPlacas.push_back(std::make_pair(imagemRecortada, imagemRecortadaProcessada));
            //imshow("Placa Recortada Processada", imagemRecortadaProcessada);
            //waitKey(0);
        }
    }
    //cout << "Placas encontradas: " << possiveisPlacas.size() << endl;
    return possiveisPlacas;
}

} // namespace processarContornos