#include <stdio.h>
#include <string.h>
#include "sensor_db.h"
#include "pthread.h"

extern sbuffer_t *buffer_storagemgr;
int extern write_to_log_process(char *msg);

FILE * open_db(char * filename, bool append){
    FILE *fp;
    if(append){
        fp = fopen(filename, "a");
    }
    else
        fp = fopen(filename, "w");

    if(fp == NULL){
        return NULL;
    }
    write_to_log_process("Data csv file created.");
    return fp;
}

int insert_sensor(FILE *f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){
    if(fprintf(f, "%hu,%f,%ld\n", id, value, ts) < 0){
        return -1;
    }
    write_to_log_process("Data csv file inserted.");
    return 0;
}

int close_db(FILE * f){
    if(fclose(f) != 0){
        return -1;
    }
    write_to_log_process("Data csv file closed.");
    return 0;
}

void *create_storagemgr(void *ptr){
    //this is a thread to read data from sbuffer and write it in to a csv file
    FILE *fp;
    fp = open_db("data.csv",false);
    sensor_data_t *data= malloc(sizeof(sensor_data_t));
    data->id = 1;
    while(sbuffer_remove(buffer_storagemgr,data) == SBUFFER_SUCCESS){
        if(data->id != 0){
            insert_sensor(fp,data->id,data->value,data->ts);
        }
    }
    free(data);
    pthread_exit(NULL);
}