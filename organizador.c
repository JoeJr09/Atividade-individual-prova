#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SENSOR_ID 50
#define MAX_LINE_LENGTH 256

typedef struct {
    time_t timestamp;
    char sensor_id[MAX_SENSOR_ID];
    char value[MAX_LINE_LENGTH];
} SensorReading;

int compare_timestamps(const void *a, const void *b) {
    const SensorReading *reading_a = (const SensorReading *)a;
    const SensorReading *reading_b = (const SensorReading *)b;
    return (reading_b->timestamp - reading_a->timestamp);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo_entrada>\n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        printf("Erro ao abrir arquivo de entrada\n");
        return 1;
    }

    int total_readings = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), input_file)) {
        total_readings++;
    }
    rewind(input_file);

    SensorReading *readings = malloc(total_readings * sizeof(SensorReading));
    if (!readings) {
        printf("Erro ao alocar memória\n");
        fclose(input_file);
        return 1;
    }

    int i = 0;
    while (fgets(line, sizeof(line), input_file)) {
        char sensor_id[MAX_SENSOR_ID];
        char value[MAX_LINE_LENGTH];
        time_t timestamp;

        if (sscanf(line, "%ld %s %s", &timestamp, sensor_id, value) == 3) {
            readings[i].timestamp = timestamp;
            strncpy(readings[i].sensor_id, sensor_id, MAX_SENSOR_ID - 1);
            readings[i].sensor_id[MAX_SENSOR_ID - 1] = '\0';
            strncpy(readings[i].value, value, MAX_LINE_LENGTH - 1);
            readings[i].value[MAX_LINE_LENGTH - 1] = '\0';
            i++;
        }
    }
    fclose(input_file);

    qsort(readings, total_readings, sizeof(SensorReading), compare_timestamps);

    char current_sensor[MAX_SENSOR_ID] = "";
    FILE *current_file = NULL;

    for (i = 0; i < total_readings; i++) {
        if (strcmp(readings[i].sensor_id, current_sensor) != 0) {
            if (current_file) {
                fclose(current_file);
            }
            strncpy(current_sensor, readings[i].sensor_id, MAX_SENSOR_ID - 1);
            current_sensor[MAX_SENSOR_ID - 1] = '\0';
            
            char filename[MAX_SENSOR_ID + 5];
            snprintf(filename, sizeof(filename), "%s.txt", current_sensor);
            current_file = fopen(filename, "w");
            if (!current_file) {
                printf("Erro ao criar arquivo para sensor %s\n", current_sensor);
                continue;
            }
        }

        fprintf(current_file, "%ld %s %s\n", 
                readings[i].timestamp, 
                readings[i].sensor_id, 
                readings[i].value);
    }

    if (current_file) {
        fclose(current_file);
    }

    free(readings);
    return 0;
} 