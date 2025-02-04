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
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <wiringPi.h>
#include <array> // Ensure this header is included
#include "reconhecer.h"

using namespace cv;
using namespace std;

// Function declarations
double read_distance();
void take_picture(int index);
bool file_exists(const char *filename);
void log_detected_plate(const std::string& placa);
bool is_placa_autorizada(const char* placa);
void add_to_log(const std::string& placa);
void abrir_portao();
vector<string> reconhecerPlaca();

#endif // MAIN_H