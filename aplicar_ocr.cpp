#include "aplicar_ocr.h"
#include <regex>
using namespace std;
using namespace cv;

namespace aplicarOCR {

std::string encontrarPlaca(const std::string& str) {
    std::regex padrao("[A-Z]{3}\\d{4}");
    std::smatch match;
    if (std::regex_search(str, match, padrao)) {
        return match.str(0);
    }
    return "";
}

std::string encontrarPlacaMercosul(const std::string& str) {
    std::regex padrao("[A-Z]{3}[0-9][0-9A-Z][0-9]{2}");
    std::smatch match;
    if (std::regex_search(str, match, padrao)) {
        return match.str(0);
    }
    return "";
}

std::tuple<std::string, cv::Mat, cv::Mat> aplicarOCR(const std::vector<std::pair<cv::Mat, cv::Mat>>& possiveisPlacas) {
    tesseract::TessBaseAPI ocr;
    ocr.Init("./tessdata_best", "eng", tesseract::OEM_TESSERACT_LSTM_COMBINED);
    ocr.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK); 
    ocr.SetVariable("tessedit_char_whitelist", 
		"0123456789QWERTYUIOPASDFGHJKLZXCVBNM");
        //"0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM");

    for (const auto& tupla : possiveisPlacas) {
        const cv::Mat& placaRecortada = tupla.first;
        const cv::Mat& placaRecortadaProcessada = tupla.second;

        ocr.SetImage(placaRecortadaProcessada.data, 
                    placaRecortadaProcessada.cols, 
                    placaRecortadaProcessada.rows, 
                    1, 
                    placaRecortadaProcessada.step);
        ocr.SetSourceResolution(300);
        std::string outText = std::string(ocr.GetUTF8Text());
        //Debug
        cout << "Texto: " << outText << endl;
        imshow("Placa Recortada Processada", placaRecortadaProcessada);
        waitKey(0); 
        // Se encontra alguma placa válida (normal ou padrão mercosul), para a iteração entre as possíveis placas, e retorna a placa atual
        std::string placa = encontrarPlaca(outText);
        if (placa != "") {
            return std::make_tuple(placa, placaRecortada, placaRecortadaProcessada);
        }

        placa = encontrarPlacaMercosul(outText);
        if (placa != "") {
            return std::make_tuple(placa, placaRecortada, placaRecortadaProcessada);
        }
    }

    return std::make_tuple("", cv::Mat(), cv::Mat());
}

} // namespace aplicarOCR