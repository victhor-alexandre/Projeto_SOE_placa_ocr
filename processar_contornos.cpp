#include "processar_contornos.h"
#include <iostream>


namespace processarContornos {

static pthread_mutex_t mutexLock;

vector<Mat> processarContornos(Mat& imagemOriginal, const Mat& imagemProcessada) {
    vector<vector<Point>> contornos;
    vector<Mat> possiveisPlacas;
    
    findContours(imagemProcessada, contornos, RETR_TREE, CHAIN_APPROX_SIMPLE);
    vector<pthread_t> thread_id;
    pthread_t thread_main;
    struct validarContornoArgs args;

    pthread_mutex_init(&mutexLock, NULL);
    pthread_create(&thread_main, NULL, &validarContorno, &args);

    for (const auto& contorno : contornos) {
        args.contorno = contorno;
        args.imagemOriginal = imagemOriginal;
        pthread_t tid;
        pthread_create(&tid, NULL, &validarContorno, &args);
        thread_id.push_back(tid);
    }

    // Join all threads to ensure they are completed
    for (pthread_t tid : thread_id) {
        Mat* placa = nullptr;
        pthread_join(tid, (void**)&placa);
        if (placa != nullptr) {
            possiveisPlacas.push_back(*placa);
            delete placa;
        }
    }
    pthread_join(thread_main, NULL);
    pthread_mutex_destroy(&mutexLock);
    //cout << "Placas encontradas: " << possiveisPlacas.size() << endl;
    
    return possiveisPlacas;
}   


void* validarContorno(void* args) {
    validarContornoArgs* arguments = static_cast<validarContornoArgs*>(args);
    vector<Point>& contorno = arguments->contorno;
    Mat& imagemOriginal = arguments->imagemOriginal;
    Mat* result = nullptr;
    // Aproximar o contorno por um polígono
    double perimetro = arcLength(contorno, true);
    vector<Point> aprox;
    approxPolyDP(contorno, aprox, 0.0125 * perimetro, true);
    double area = contourArea(contorno);
    Rect boundingRect = cv::boundingRect(contorno);

    // verificar se a altura é maior que a largura
    if (boundingRect.height > boundingRect.width) {
        return nullptr;
    }

    // verificar se há uma proporcao onde a altura é menor que 20% da largura
    if (boundingRect.height < (boundingRect.width * 0.1)) {
        return nullptr;
    }

    if (area < 10000 || area > 80000) {
        return nullptr;
    }
    // Debugging: Print contour information
    //cout << "Contour area: " << area << ", Bounding rect: " << boundingRect << endl;

    // Se o polígono tiver quatro lados, consideramos que é um retângulo
    if (aprox.size() >= 4 && aprox.size() < 10) {
        //drawContours(imagemOriginal, vector<vector<Point>>{aprox}, -1, Scalar(0, 255, 0), 2);
    //cout << "antes do recorte" << endl;
        // recortar a imagem da placa
        Mat imagemRecortada = imagemOriginal(boundingRect);
        
        // Ampliar a imagem caso ela seja muito pequena
        if (area < 15000) {
            cv::resize(imagemRecortada, imagemRecortada, cv::Size(imagemRecortada.cols * 1.75,imagemRecortada.rows * 1.75), 0, 0, 1);

        }
        // Debugging: Print cropped image information
        //cout << "Cropped image size: " << imagemRecortada.size() << endl;

        // converter a imagem da placa para tons de cinza
        Mat imagemRecortadaCinza;
        cvtColor(imagemRecortada, imagemRecortadaCinza, COLOR_BGR2GRAY);

        // aplicar limiarização para tornar os caracteres mais destacados
        Mat imagemRecortadaLimiarizada;
        threshold(imagemRecortadaCinza, imagemRecortadaLimiarizada, 0, 255,  THRESH_OTSU);

        // aplicar operação de fechamento para preencher regiões de contornos
        Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
        Mat imagemRecortadaProcessada;
        morphologyEx(imagemRecortadaLimiarizada, imagemRecortadaProcessada, MORPH_CLOSE, kernel);

        // aplicar operação de abertura para remover ruídos
        kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
        morphologyEx(imagemRecortadaProcessada, imagemRecortadaProcessada, MORPH_OPEN, kernel, Point(-1, -1), 1);

        // aplicar operação de dilatação para aumentar a espessura dos caracteres
        kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
        dilate(imagemRecortadaProcessada, imagemRecortadaProcessada, kernel, Point(-1, -1), 1);

        // aplicar operação de erosão para reduzir a espessura dos caracteres
        erode(imagemRecortadaProcessada, imagemRecortadaProcessada, kernel, Point(-1, -1), 1);

        result = new Mat(imagemRecortadaProcessada);
        //imshow("Placa Recortada Processada", imagemRecortadaProcessada);
        //waitKey(0);

    }
    
    return result;
}

} // namespace processarContornos