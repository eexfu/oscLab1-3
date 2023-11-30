#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pthread.h"
#include "config.h"
#include "sbuffer.h"

FILE *fp_writer;
FILE *fp_reader;

void *writeData(void *buffer_void){
//    FILE *fp;
//    fp = fopen("sensor_data","r");
    sbuffer_t *buffer = (sbuffer_t *)buffer_void;
    sensor_id_t sensorID;
    sensor_value_t temperature;
    sensor_ts_t timestamp;
    sensor_data_t *data = malloc(sizeof(sensor_data_t));

    while(fread(&sensorID, sizeof(sensorID), 1, fp_writer) == 1 &&
          fread(&temperature, sizeof(temperature), 1, fp_writer) == 1 &&
          fread(&timestamp, sizeof(timestamp), 1, fp_writer) == 1){
        data->id = sensorID;
        data->value = temperature;
        data->ts = timestamp;
        sbuffer_insert(buffer,data);
        usleep(10000);
    }

    data->id = 0;
    sbuffer_insert(buffer,data);

    free(data);

    pthread_exit(NULL);
}

void *readData(void *buffer_void){
//    FILE *fp;
//    fp = fopen("sensor_data_out.csv","w");
    sbuffer_t *buffer = (sbuffer_t *)buffer_void;
    sensor_data_t *data = malloc(sizeof(sensor_data_t));
    data->id = 1;

    while(sbuffer_remove(buffer,data) == SBUFFER_SUCCESS){
        if(data->id != 0){
            fprintf(fp_reader,"sensorID: %hu, temperature: %f, timestamp: %ld\n",data->id,data->value,data->ts);
            usleep(25000);
        }
    }

    free(data);
    pthread_exit(NULL);
}

int main(){
    sbuffer_t *buffer;
    sbuffer_init(&buffer);

    pthread_t writer_thread;
    pthread_t reader_thread1;
    pthread_t reader_thread2;

    fp_writer = fopen("sensor_data","r");
    fp_reader = fopen("sensor_data_out.csv","w");

    pthread_create(&writer_thread,NULL,writeData,(void *)buffer);
    pthread_create(&reader_thread1,NULL,readData,(void *)buffer);
    pthread_create(&reader_thread2,NULL,readData,(void *)buffer);

    pthread_join(writer_thread,NULL);
    pthread_join(reader_thread1,NULL);
    pthread_join(reader_thread2,NULL);

    fclose(fp_writer);
    fclose(fp_reader);

    sbuffer_free(&buffer);
}