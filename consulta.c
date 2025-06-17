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

SensorReading* binary_search(SensorReading* readings, int left, int right, time_t target) {
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (readings[mid].timestamp == target)
            return &readings[mid];
            
        if (readings[mid].timestamp > target)
            left = mid + 1;
        else
            right = mid - 1;
    }
    
    if (right < 0) return &readings[0];
    if (left >= right) return &readings[right];
    
    time_t diff_left = abs(readings[left].timestamp - target);
    time_t diff_right = abs(readings[right].timestamp - target);
    
    return (diff_left < diff_right) ? &readings[left] : &readings[right];
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <sensor_id> <timestamp>\n", argv[0]);
        return 1;
    }

    char *sensor_id = argv[1];
    time_t target_timestamp = atol(argv[2]);

    char filename[MAX_SENSOR_ID + 5];
    snprintf(filename, sizeof(filename), "%s.txt", sensor_id);
    
    FILE *sensor_file = fopen(filename, "r");
    if (!sensor_file) {
        printf("Erro ao abrir arquivo do sensor %s\n", sensor_id);
        return 1;
    }

    int total_readings = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), sensor_file)) {
        total_readings++;
    }
    rewind(sensor_file);

    SensorReading *readings = malloc(total_readings * sizeof(SensorReading));
    if (!readings) {
        printf("Erro ao alocar memória\n");
        fclose(sensor_file);
        return 1;
    }

    int i = 0;
    while (fgets(line, sizeof(line), sensor_file)) {
        char value[MAX_LINE_LENGTH];
        if (sscanf(line, "%ld %s %s", &readings[i].timestamp, readings[i].sensor_id, value) == 3) {
            strncpy(readings[i].value, value, MAX_LINE_LENGTH - 1);
            readings[i].value[MAX_LINE_LENGTH - 1] = '\0';
            i++;
        }
    }
    fclose(sensor_file);

    SensorReading *result = binary_search(readings, 0, total_readings - 1, target_timestamp);
    
    if (result) {
        printf("Leitura mais próxima encontrada:\n");
        printf("Timestamp: %ld\n", result->timestamp);
        printf("Sensor: %s\n", result->sensor_id);
        printf("Valor: %s\n", result->value);
    } else {
        printf("Nenhuma leitura encontrada\n");
    }

    free(readings);
    return 0;
} 