#include <stdio.h>
#include <string.h>
#include "sensor_db.h"
#include "logger.h"

FILE * open_db(char * filename, bool append){
    FILE *fp;
    if(append){
        fp = fopen(filename, "a");
        printf("hi\n");
    }
    else
        fp = fopen(filename, "w");

    create_log_process();

    if(fp == NULL){
        write_to_log_process("ERROR: opening file.");
        return NULL;
    }

    printf("open db\n");
    write_to_log_process("Data file open.");
    printf("out\n");
    return fp;
}

int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){
    if(fprintf(f, "%hu,%f,%ld\n", id, value, ts) < 0){
        write_to_log_process("ERROR: inserting file.");
        return -1;
    }

    printf("insert db\n");
    write_to_log_process("Data inserted.");

    return 0;
}

int close_db(FILE * f){
    if(fclose(f) != 0){
        write_to_log_process("ERROR: closing file.");
        return -1;
    }

    printf("close db\n");
    write_to_log_process("Data file close.");

    end_log_process();
    return 0;
}