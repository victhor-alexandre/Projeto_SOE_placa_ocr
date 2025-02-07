#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>

#define TRIG 5                                              // GPIO para o Trigger do sensor ultrassônico
#define ECHO 6                                              // GPIO para o Echo do sensor ultrassônico
#define GATE 7                                              // GPIO para controle da cancela (conectado ao coletor do transistor)
#define BUTTON 8                                            // GPIO para botão de abertura manual

// Declarando funções
void takePhotos();
int verifyAccess();
void controlGate(int state);
double getDistancia();
void buttonInterrupt();

// Estado do portão
volatile int gateState = 0; 

// Configura os GPIOs
void setup() {
    wiringPiSetup();                                        // Inicializa a WiringPi
    pinMode(TRIG, OUTPUT);                                  // Configura Trigger como saída
    pinMode(ECHO, INPUT);                                   // Configura Echo como entrada
    pinMode(GATE, OUTPUT);                                  // Configura GPIO do portão como saída
    pinMode(BUTTON, INPUT);                                 // Configura GPIO do botão como entrada
    pullUpDnControl(BUTTON, PUD_UP);                        // Habilita o resistor de pull-up no botão
    wiringPiISR(BUTTON, INT_EDGE_FALLING, &buttonInterrupt);// Configura a interrupção do botão
    digitalWrite(GATE, HIGH);                               // Inicializa o portão fechado (HIGH para não acionar o transistor)
}

void takePhotos() {
    const char* command = "fswebcam -r 1920x1080 -v -S 5 --no-banner --set brightness=50% --png-1 image.png";
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

// Função para acionamento do portão (agora com coletor aberto)
void controlGate(int state) {
    if (state == 1) {
        digitalWrite(GATE, LOW);// Para abrir a cancela o pino GATE precisa ir para LOW pois o circuito é coletor aberto
        gateState = state;// Atualiza o estado do portão
        printf("Abrindo...\n");
    } else {
        digitalWrite(GATE, HIGH);// Para fechar a cancela o pino GATE precisa ir para HIGH pois o circuito é coletor aberto
        gateState = state;// Atualiza o estado do portão
        printf("Fechando...\n");
    }
}

// Função que trata a interrupção do botão
void buttonInterrupt() {
    gateState = !gateState;                                 // Atualiza o estado do portão 
    controlGate(gateState);                                 // Envia o novo estado para a função de acionamento do portão
    printf("Botão pressionado. Estado da cancela alterado.\n");
}

// Função principal do programa
int main() {
    setup();

    while (1) {
        printf("\n--- Verificando distância ---\n");
        double distancia = getDistancia();
        printf("Distância medida = %.1f cm\n", distancia);

        if (distancia < 50.0) {
            printf("Distância menor que 50 cm. Capturando fotos...\n");
            takePhotos();                                                   // Chama a função que tira foto, processa, avalia o credenciamento

            if (verifyAccess()) {                                           // Verica se esta liberado
                controlGate(1);                                             // Abre a cancela

                while (getDistancia() < 60.0) {                             // Espera o carro sair 
                    printf("Acesso liberado...\n");
                    usleep(2000000);                                         // Aguarda 2s para mandar o sinal de fechamento da cancela
                }
                controlGate(0);                                             // Fecha a cancela
            } else {
                printf("Acesso negado. Continuando a monitorar...\n");      // Caso contrario, a cancela permance fechada
            }
        } else {
            printf("Distância maior que 50 cm. Continuando a monitorar...\n");
        }
        usleep(500000);                                                     // Aguarda 500ms para nova medição
    }
    return 0;
}
