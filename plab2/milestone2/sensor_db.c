#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "sensor_db.h"
#include "logger.h"

pid_t pid;

FILE * open_db(char * filename, bool append){
    pid = fork();
    if(pid < 0){
        printf("Fork failed");
        return NULL;
    }
    else if(pid == 0){
        //logger
        create_log_process();
        return NULL;
    }
    else{
        //storage manager
        FILE *fp = NULL;

        if(append){
            fp = fopen(filename,"a");
        }
        else{
            fp = fopen(filename,"w");
        }

        return fp;
    }
}

int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){
    if(pid < 0){
        printf("Fork failed");
        return 1;
    }
    else if(pid == 0){
        //logger
        write_to_log_process("Data inserted.");
    }
    else{
        //storage manager
        fprintf(f,"%hu, %lf, %ld\n",id,value,ts);
    }

    return 0;
}

int close_db(FILE * f){
    if(pid < 0){
        printf("Fork failed");
        return 1;
    }
    else if(pid ==0){
        //logger
        end_log_process();
    }
    else{
        //storage manager
        fclose(f);
    }

    return 0;
}