


// Define the GPIO pins for the ultrasonic sensor
#define TRIG_PIN 0  // GPIO pin for TRIG
#define ECHO_PIN 1  // GPIO pin for ECHO

// Function to read distance from the ultrasonic sensor
double read_distance() {
    // Send a 10us pulse to trigger the sensor
    digitalWrite(TRIG_PIN, LOW);
    usleep(20000); // 20ms delay
    digitalWrite(TRIG_PIN, HIGH);
    usleep(10); // 10us pulse
    digitalWrite(TRIG_PIN, LOW);

    // Wait for the echo to start
    while (digitalRead(ECHO_PIN) == LOW);
    long start_time = micros();

    // Wait for the echo to end
    while (digitalRead(ECHO_PIN) == HIGH);
    long end_time = micros();

    // Calculate the distance
    double duration = end_time - start_time;
    double distance = (duration * 34300) / 2.0; // Speed of sound is 34300 cm/s

    return distance;
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

bool placa_autorizada(const char* placa) {
    FILE *file = fopen("placas_autorizadas.txt", "r");
    if (file == NULL) {
        perror("Failed to open placas_autorizadas.txt");
        return false;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Remove newline character from the line
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, placa) == 0) {
            fclose(file);
            return true;
        }
    }

    fclose(file);
    return false;
}


// Function to take a picture using fswebcam
void take_picture(int index) {
    char command[100];
    snprintf(command, sizeof(command), "fswebcam -r 1280x720 --jpeg 85 -D 1 images/captured_image_%d.jpg", index);
    system(command);
}

// Function to check if a file exists
int file_exists(const char *filename) {
    return access(filename, F_OK) != -1;
}

int main() {
    vector<string> placas;

    // Initialize wiringPi and set up the GPIO pins
    wiringPiSetup();
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // Create the images directory if it doesn't exist
    system("mkdir -p images");

    // Wait for the ultrasonic sensor signal
    while (1) {
        double distance = read_distance();
        printf("Distance: %.2f cm\n", distance);
        if (distance <= 50.0) {
            printf("Distance is less than or equal to 50 cm. Taking pictures...\n");
            break;
        }
        sleep(1);
    }

    // Take four pictures with fswebcam and save them under the images folder
    for (int i = 1; i <= 4; i++) {
        take_picture(i);
    }

    // Check if the pictures were taken successfully
    for (int i = 1; i <= 4; i++) {
        char filename[100];
        snprintf(filename, sizeof(filename), "images/captured_image_%d.jpg", i);
        if (file_exists(filename)) {
            printf("Picture %d taken successfully and saved to %s\n", i, filename);
        } else {
            printf("Failed to take picture %d\n", i);
            return 1;
        }
    }

    placas = reconhecerPlaca();
    for(string placa : placas) {
        if(placa!="") {
            if(placa_autorizada(placa.c_str())) {

                //Função para abrir o portão                

                add_to_log(placa);
            }

        }
    }
    // Remove the captured images
    system("rm images/captured_image_*.jpg");

    return 0;
}