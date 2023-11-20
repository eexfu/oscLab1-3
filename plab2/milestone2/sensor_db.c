#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "sensor_db.h"
#include "logger.h"

#define SIZE 25
#define READ_END 0
#define WRITE_END 1

pid_t pid;
int fd[2];

FILE * open_db(char * filename, bool append){
    if(pipe(fd) == -1){
        printf("Pipe failed\n");
        return NULL;
    }

    pid = fork();

    if(pid < 0){
        printf("Fork failed");
        return NULL;
    }
    else if(pid == 0){
        //logger
        char msg[SIZE];

        create_log_process();

        close(fd[WRITE_END]);
        read(fd[READ_END],msg,16);

        printf("open read_msg: %s\n",msg);

        write_to_log_process(msg);

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

        close(fd[READ_END]);
        write(fd[WRITE_END],"Data file open.",16);
        printf("open file.\n");

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
        char msg[SIZE];

        read(fd[READ_END],msg,15);

        printf("insert read_msg: %s\n",msg);

        write_to_log_process(msg);
    }
    else{
        //storage manager
        fprintf(f,"%hu, %lf, %ld\n",id,value,ts);

        write(fd[WRITE_END],"Data inserted.",15);
        printf("insert data.\n");
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
        char msg[SIZE];

        read(fd[READ_END],msg,18);
        close(fd[READ_END]);

        printf("close read_msg: %s\n",msg);

        write_to_log_process(msg);

        end_log_process();

        exit(0);
    }
    else{
        //storage manager
        fclose(f);

        write(fd[WRITE_END],"Data file closed.",18);
        close(fd[WRITE_END]);
        printf("close file.\n");
    }

    return 0;
}