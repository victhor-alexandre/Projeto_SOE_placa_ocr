#include "main.h"

// Estado do portão
volatile int gateState = 0; 

// Configura os GPIOs
void setup() {
    wiringPiSetupGpio();                                        // Inicializa a WiringPi
    pinMode(TRIG, OUTPUT);                                  // Configura Trigger como saída
    pinMode(ECHO, INPUT);                                   // Configura Echo como entrada
    pinMode(GATE, OUTPUT);                                  // Configura GPIO do portão como saída
    pinMode(BUTTON, INPUT);                                 // Configura GPIO do botão como entrada
    pullUpDnControl(BUTTON, PUD_UP);                        // Habilita o resistor de pull-up no botão
    wiringPiISR(BUTTON, INT_EDGE_FALLING, &buttonInterrupt);// Configura a interrupção do botão
    digitalWrite(GATE, LOW);                               // Inicializa o portão fechado (HIGH para não acionar o transistor)
}

void takePhotos() {
    const char* command = "fswebcam -r 1920x1080 -v -S 5 --no-banner --set brightness=50% --png 0 images/image.png";
    system(command);
}

// Função responsável para obter a distância medida pelo HC-SR04
double getDistancia() {
    digitalWrite(TRIG, LOW);                                // Desabilita o TRIGGER (garantir que esteja pronto para gerar o pulso)
    usleep(2);                                              // Aguarda 2us
    digitalWrite(TRIG, HIGH);                               // Habilita o TRIGGER para gerar um pulso
    usleep(10);                                             // Aguarda 10us (duração do pulso)
    digitalWrite(TRIG, LOW);                                // Desabilita o TRIGGER encerrando o pulso

    while (digitalRead(ECHO) == 0);                         // Aguarda o ECHO mudar de estado para capturar o instante da mudança de estado
    unsigned long inicio = millis();                        // Captura o momento em que o pino alterou para HIGH
    while (digitalRead(ECHO) == 1);                         // Aguarda o ECHO mudar de estado para capturar o instante da mudança de estado
    unsigned long fim = millis();                           // Captura o momento em que o pino alterou para LOW

    // Calcula a duração da viagem do sinal
    double duracao = (double)(fim - inicio) / 1000.0;       // Converte microssegundos para segundos
    double distancia = (duracao * 34300) / 2;               // Calcula a distância (velocidade do som = 34300 cm/s)

    return distancia;
}

void controlGate(){
    digitalWrite(GATE, HIGH);                               // Aciona o portão (HIGH para acionar o optoacoplador)
    delay(1000);                                            // Aguarda 1 segundo
    digitalWrite(GATE, LOW);                                // Desliga o portão (LOW para desligar o optoacoplador)
}

// Função que trata a interrupção do botão
void buttonInterrupt() {
    controlGate();                                 // Envia o novo estado para a função de acionamento do portão
   cout << "Liberação manual do portão" << endl;
}

// Function to log the detected plate with date and time
void add_to_log(const std::string& placa) {
    FILE *log_file = fopen("log.txt", "a");
    if (log_file == NULL) {
        perror("Failed to open log file");
        return;
    }

    // Get the current date and time
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char time_str[100];
    strftime(time_str, sizeof(time_str) - 1, "%d-%m-%Y %H:%M:%S", t);

    // Write the log entry
    fprintf(log_file, "[%s] Entrada autorizada: Placa nº %s\n", time_str, placa.c_str());
    fclose(log_file);
}


bool verifica_placa_autorizada(const std::string& placa) {
    std::ifstream file("placas_autorizadas.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open file: placas_autorizadas.txt" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line == placa) {
            return true; // Placa found
        }
    }

    return false; // Placa not found
}

// Função principal do programa
int main() {
    setup();
    string placa;
    std::vector<std::string> placas;
    unordered_map<string, int> placa_count;
    while (1) {
        cout << "\n--- Verificando distância ---\n";
        double distancia = getDistancia();
        cout << "Distância medida = " << distancia << " cm" << endl;

        if (distancia < 50.0) {

            //     

            for (int i = 0; i < 5; i++) {
                takePhotos();
                placa = reconhecer::reconhecerPlaca();
                if (!placa.empty()) {
                    placas.push_back(placa);
                    placa_count[placa]++;

                    // if (placa_count[placa] > 3) {
                    //     break;
                    // }
                }
            }
            // if (placa_count[placa] < 3) {
            //     cout << "Placa não detectada. Continuando a monitorar..." << endl;
            //     continue;
            // }
            

            if (verifica_placa_autorizada(placa)) {                                           // Verica se esta liberado
                controlGate();                                             // Abre a cancela
                add_to_log(placa);
                while (getDistancia() < 60.0) {                             // Espera o carro sair 
                    usleep(2000000);
                }
                controlGate();                                             // Fecha a cancela
            } else {
                cout << "Acesso negado. Continuando a monitorar..." << endl;      // Caso contrario, a cancela permance fechada
            }
        } else {
            cout << "Distância maior que 50 cm. Continuando a monitorar..." << endl;
        }
        usleep(500000);                                                     // Aguarda 500ms para nova medição
    }
    return 0;
}
