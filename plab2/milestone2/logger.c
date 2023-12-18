#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdint-gcc.h>
#include <stdbool.h>
#include "logger.h"

#define SIZE 25
#define READ_END 0
#define WRITE_END 1
#define MAX_MESSAGE_SIZE 50

pid_t pid;
int fd[2];

int write_to_log_process(char *msg){
    uint16_t len = strlen(msg);
    write(fd[WRITE_END], &len, sizeof(len));
    write(fd[WRITE_END], msg, len);
    return 0;
}

int create_log_process(){
    if(pipe(fd) == -1){
        printf("Pipe failed\n");
        return -1;
    }

    pid = fork();

    if(pid < 0){
        printf("Fork failed");
        return -1;
    }
    else if(pid == 0){
        FILE *fp = fopen("gateway.log","w");
        char msg[SIZE];

        int sequence = 0;
        uint16_t len;
        while(1){
            read(fd[READ_END], &len, sizeof(len));
            char *buffer = malloc(len + 1);
            read(fd[READ_END], buffer, len);
            buffer[len] = '\0';
            printf("%s\n",buffer);
            fflush(fp);
            if(strncmp(buffer, "break\0", len+1) == 0){
                break;
            }
            char write_msg[100];
            char formatted_time[50];
            time_t current = time(NULL);
            struct tm *time = localtime(&current);
            strftime(formatted_time, sizeof(formatted_time), "%a %b %d %H:%M:%S %Y", time);
            snprintf(write_msg,sizeof(write_msg),"%d - %s - %s\n",sequence,formatted_time,buffer);

            fprintf(fp,"%s",write_msg);
            fflush(fp);

            sequence++;
            memset(msg, 0, sizeof(msg));
        }

        close(fd[READ_END]);
        fclose(fp);
        exit(0);
    }
    else{
        close(fd[READ_END]);
    }

    return 0;
}

int end_log_process(){
    close(fd[WRITE_END]);
    waitpid(pid,NULL,0);
    return 0;
}