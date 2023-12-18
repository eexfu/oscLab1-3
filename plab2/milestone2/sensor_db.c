#include <stdio.h>
#include <string.h>
#include "sensor_db.h"
#include "logger.h"

FILE * open_db(char * filename, bool append){
    FILE *fp;
    if(append){
        fp = fopen(filename, "a");
    }
    else
        fp = fopen(filename, "w");

    create_log_process();

    if(fp == NULL){
        write_to_log_process("ERROR: opening file.\0");
        return NULL;
    }

    write_to_log_process("Data file open.\0");
    return fp;
}

int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){
    if(fprintf(f, "%hu,%f,%ld\n", id, value, ts) < 0){
        write_to_log_process("ERROR: inserting file.\0");
        return -1;
    }

    char msg[100];
    snprintf(msg, sizeof(msg),"%s%hu", "Data inserted ", id);
    write_to_log_process(msg);

    return 0;
}

int close_db(FILE * f){
    if(fclose(f) != 0){
        write_to_log_process("ERROR: closing file.\0");
        return -1;
    }

    write_to_log_process("Data file close.\0");
    write_to_log_process("break");
    end_log_process();
    return 0;
}