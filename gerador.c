#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SENSOR_ID 50
#define MAX_LINE_LENGTH 256
#define READINGS_PER_SENSOR 2000

typedef struct {
    time_t timestamp;
    char sensor_id[MAX_SENSOR_ID];
    char value[MAX_LINE_LENGTH];
} Reading;

void generate_random_string(char *str, int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < length - 1; i++) {
        int index = rand() % (sizeof(charset) - 1);
        str[i] = charset[index];
    }
    str[length - 1] = '\0';
}

void generate_random_value(char *value, const char *type) {
    if (strcmp(type, "CONJ_Z") == 0) {
        sprintf(value, "%d", rand() % 1000);
    } else if (strcmp(type, "CONJ_Q") == 0) {
        sprintf(value, "%.2f", (float)rand() / RAND_MAX * 1000.0);
    } else if (strcmp(type, "TEXTO") == 0) {
        generate_random_string(value, 16);
    } else if (strcmp(type, "BINARIO") == 0) {
        sprintf(value, "%s", rand() % 2 ? "true" : "false");
    }
}

void shuffle_readings(Reading *readings, int n) {
    for (int i = 0; i < n * 3; i++) {
        int j = rand() % n;
        int k = rand() % n;
        Reading temp = readings[j];
        readings[j] = readings[k];
        readings[k] = temp;
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL) ^ (int)getpid());
    
    if (argc < 7 || (argc - 5) % 2 != 0) {
        printf("Uso: %s <inicio_dia> <inicio_mes> <inicio_ano> <inicio_hora> <inicio_min> <inicio_seg> "
               "<fim_dia> <fim_mes> <fim_ano> <fim_hora> <fim_min> <fim_seg> "
               "<sensor1> <tipo1> [<sensor2> <tipo2> ...]\n", argv[0]);
        return 1;
    }

    struct tm start_time = {0};
    start_time.tm_year = atoi(argv[3]) - 1900;
    start_time.tm_mon = atoi(argv[2]) - 1;
    start_time.tm_mday = atoi(argv[1]);
    start_time.tm_hour = atoi(argv[4]);
    start_time.tm_min = atoi(argv[5]);
    start_time.tm_sec = atoi(argv[6]);

    struct tm end_time = {0};
    end_time.tm_year = atoi(argv[9]) - 1900;
    end_time.tm_mon = atoi(argv[8]) - 1;
    end_time.tm_mday = atoi(argv[7]);
    end_time.tm_hour = atoi(argv[10]);
    end_time.tm_min = atoi(argv[11]);
    end_time.tm_sec = atoi(argv[12]);

    time_t start_timestamp = mktime(&start_time);
    time_t end_timestamp = mktime(&end_time);

    if (start_timestamp == -1 || end_timestamp == -1) {
        printf("Data inválida\n");
        return 1;
    }

    int num_sensors = (argc - 13) / 2;
    int total_readings = num_sensors * READINGS_PER_SENSOR;
    
    Reading *readings = malloc(total_readings * sizeof(Reading));
    if (!readings) {
        printf("Erro ao alocar memória\n");
        return 1;
    }

    int reading_index = 0;
    for (int i = 13; i < argc; i += 2) {
        char *sensor_id = argv[i];
        char *data_type = argv[i + 1];

        for (int j = 0; j < READINGS_PER_SENSOR; j++) {
            readings[reading_index].timestamp = start_timestamp + 
                (time_t)((double)rand() / RAND_MAX * (end_timestamp - start_timestamp));
            strncpy(readings[reading_index].sensor_id, sensor_id, MAX_SENSOR_ID - 1);
            readings[reading_index].sensor_id[MAX_SENSOR_ID - 1] = '\0';
            generate_random_value(readings[reading_index].value, data_type);
            reading_index++;
        }
    }

    for (int i = 0; i < 5; i++) {
        srand(time(NULL) ^ (int)getpid() ^ i);
        shuffle_readings(readings, total_readings);
    }

    FILE *output_file = fopen("dados_teste.txt", "w");
    if (!output_file) {
        printf("Erro ao criar arquivo de saída\n");
        free(readings);
        return 1;
    }

    for (int i = 0; i < total_readings; i++) {
        fprintf(output_file, "%ld %s %s\n", 
                readings[i].timestamp, 
                readings[i].sensor_id, 
                readings[i].value);
    }

    free(readings);
    fclose(output_file);
    printf("Arquivo de teste gerado com sucesso: dados_teste.txt\n");
    return 0;
} 