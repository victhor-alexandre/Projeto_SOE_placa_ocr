#include "main.h"

volatile bool autorizacaoManual = false;
volatile bool autorizada = false;

// Configura os GPIOs
void setup() {
wiringPiSetupGpio();                                        // Inicializa a WiringPi com a numeração do GPIO, no lugar da unmeração wiringpi
    pinMode(TRIG, OUTPUT);                                 
    pinMode(ECHO, INPUT);                                   
    pinMode(GATE, OUTPUT);                                  
    pinMode(BUTTON, INPUT);                                
    pullUpDnControl(BUTTON, PUD_UP);                        // Pull-up interno do botão
    wiringPiISR(BUTTON, INT_EDGE_FALLING, &buttonInterrupt);
    digitalWrite(GATE, LOW);                               
}

void tirarFoto() {
    const char* command = "fswebcam -r 1920x1080 -v --no-banner --set brightness=30% --png 0 images/image1.png";
    system(command);
}


double calculaDistancia() {
    digitalWrite(TRIG, LOW);                                
    usleep(2);                                              
    digitalWrite(TRIG, HIGH);                               
    usleep(10);                                             
    digitalWrite(TRIG, LOW);                                

    while (digitalRead(ECHO) == 0);                         
    unsigned long inicio = millis();                        
    while (digitalRead(ECHO) == 1);                         
    unsigned long fim = millis();                           


    double duracao = (double)(fim - inicio) / 1000.0;       // [s]
    double distancia = (duracao * 34300) / 2;               // [cm]

    return distancia;
}

void controlarPortao(){
    digitalWrite(GATE, HIGH);                               
    delay(1000);                                            
    digitalWrite(GATE, LOW);                                
}


void buttonInterrupt() {
    controlarPortao();  
    adicionarLog("", false);                        // Adiciona a entrada manual ao log
    autorizacaoManual = true;                     
    autorizada = true;
    cout << "Liberação manual do portão" << endl;
}

void adicionarLog(const std::string& placa, bool autorizada) {
    FILE *log_file = fopen("log.txt", "a");
    if (log_file == NULL) {
        perror("Failed to open log file");
        return;
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char time_str[100];
    strftime(time_str, sizeof(time_str) - 1, "%d-%m-%Y %H:%M:%S", t);

    if (!autorizada) {
        fprintf(log_file, "[%s] Entrada manual autorizada\n" , time_str);
    } else{
        fprintf(log_file, "[%s] Entrada autorizada: Placa nº %s\n", time_str, placa.c_str());
    }
    fclose(log_file);
}


bool verificaPlaca(const std::string& placa) {
    FILE* file = fopen("placas_autorizadas.txt", "r");
    if (file == NULL) {
        std::cerr << "Failed to open file: placas_autorizadas.txt" << std::endl;
        return false;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        if (placa == line) {
            fclose(file);
            return true; 
        }
    }

    fclose(file);
    return false;
}


int main() {
    setup();
    string placa;
    std::vector<std::string> placas;
    unordered_map<string, int> placa_count;
    autorizada = false;
    autorizacaoManual = false;
    while (1) {
        cout << "\n--- Verificando distância ---\n";
        double distancia = calculaDistancia();
        cout << "Distância medida = " << distancia << " cm" << endl;

        if (distancia < 100.0) {

            //     

            for (int i = 0; i < 5; i++) {
                if (autorizacaoManual) {        // Se foi autorizado manualmente, não faz a detecção da placa até o portão fechar novamente
                    // autorizacaoManual = false;
                    break;
                }
                tirarFoto();
                Mat imagem = imread("images/image.png");
                if (imagem.empty()) {
                    cerr << "Erro ao carregar a imagem!" << endl;
                continue;
            }
                try {
                    placa = reconhecer::reconhecerPlaca();
                } catch (const std::exception& e) {
                    std::cerr << "Erro ao reconhecer a placa: " << e.what() << std::endl;
                    placa = ""; 
                }
                if (!placa.empty()) {
                    placas.push_back(placa);
                    placa_count[placa]++;
                    autorizada = verificaPlaca(placa);
                    if (autorizada) {
                        break;
                    }
                }
            }
            if (!autorizacaoManual){        //pula essa verificação se foi autorizado manualmente
                if (placa_count[placa] < 3) {
                    cout << "Placa não detectada. Continuando a monitorar..." << endl;
                    continue;
                }
            }
            if (!autorizacaoManual) {
                if (autorizada) {                                           
                    cout << "Placa: " << placa << " autorizada. Abrindo a cancela..." << endl;
                    controlarPortao();                                             
                    adicionarLog(placa, autorizada);
                    autorizada = false;
                    while (calculaDistancia() < 100.0) {   // Espera o carro sair 
                        usleep(5000000);
                    }

                    controlarPortao();                                         
                } else {
                    cout << "Acesso negado. Continuando a monitorar..." << endl;   
                }
            } else {
                while (calculaDistancia() < 100.0) {          // Espera o carro sair 
                    usleep(5000000);
                }
                controlarPortao();                        
            }
        } else {
            cout << "Distância maior que 100 cm. Continuando a monitorar..." << endl;
        }
        usleep(1000000);              // Aguarda 1s para nova medição
    }
    return 0;
}
