#include "aplicar_ocr.h"
#include <regex>


namespace aplicarOCR {

string encontrarPlaca(const string& str) {
    regex padrao("[A-Z]{3}\\d{4}");
    smatch match;
    if (regex_search(str, match, padrao)) {
        return match.str(0);
    }
    return "";
}

string encontrarPlacaMercosul(const string& str) {
    regex padrao("[A-Z]{3}[0-9][0-9A-Z][0-9]{2}");
    smatch match;
    if (regex_search(str, match, padrao)) {
        return match.str(0);
    }
    return "";
}

tuple<string, cv::Mat> aplicarOCR(const vector<Mat>& possiveisPlacas) {
    tesseract::TessBaseAPI ocr;
    ocr.Init("./tessdata_best", "eng", tesseract::OEM_TESSERACT_LSTM_COMBINED);
    ocr.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK); 
    ocr.SetVariable("tessedit_char_whitelist", 
		"0123456789QWERTYUIOPASDFGHJKLZXCVBNM");
        //"0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM");

    for (const cv::Mat& placaRecortadaProcessada : possiveisPlacas) {

        ocr.SetImage(placaRecortadaProcessada.data, 
                    placaRecortadaProcessada.cols, 
                    placaRecortadaProcessada.rows, 
                    1, 
                    placaRecortadaProcessada.step);
        ocr.SetSourceResolution(300);
        string outText = string(ocr.GetUTF8Text());
        //Debug
        cout << "Texto: " << outText << endl;
        //imshow("Placa Recortada Processada", placaRecortadaProcessada);
        //waitKey(0); 
        // Se encontra alguma placa válida (normal ou padrão mercosul), para a iteração entre as possíveis placas, e retorna a placa atual
        string placa = encontrarPlaca(outText);
        if (placa != "") {
            return make_tuple(placa, placaRecortadaProcessada);
        }

        placa = encontrarPlacaMercosul(outText);
        if (placa != "") {
            return make_tuple(placa, placaRecortadaProcessada);
        }
    }

    return make_tuple("", Mat());
}

} // namespace aplicarOCR