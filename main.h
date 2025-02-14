#ifndef MAIN_H
#define MAIN_H

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <tuple>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <wiringPi.h>
#include <array>
#include "reconhecer.h"

#define TRIG 18         // GPIO para o Trigger do sensor ultrassônico
#define ECHO 24         // GPIO para o Echo do sensor ultrassônico
#define GATE 16         // GPIO para controle da cancela (conectado ao optoacoplador através de resistor)
#define BUTTON 21       // GPIO para botão de abertura manual


using namespace reconhecer;
using namespace cv;
using namespace std;


void tirarFoto();
void controlarPortao();
double calculaDistancia();
void buttonInterrupt();
void adicionarLog(const std::string& placa, bool autorizada);
bool verificaPlaca(const std::string& placa);

#endif // MAIN_H