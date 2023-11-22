#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "logger.h"

#define SIZE 25
#define READ_END 0
#define WRITE_END 1

pid_t pid;
int fd[2];

int write_to_log_process(char *msg){
    if(write(fd[WRITE_END],msg,strlen(msg))){
        return 0;
    }
    return -1;
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

        while(read(fd[READ_END],msg,sizeof(msg))){
            int sequence = 0;
            char write_msg[100];
            char formatted_time[50];
            time_t current = time(NULL);
            struct tm *time = localtime(&current);
            strftime(formatted_time, sizeof(formatted_time), "%a %b %d %H:%M:%S %Y", time);
            snprintf(write_msg,sizeof(write_msg),"%d - %s - %s\n",sequence,formatted_time,msg);

            fprintf(fp,"%s",write_msg);
            fflush(fp);

            sequence++;
            sleep(1);
        }

        close(fd[READ_END]);
        fclose(fp);
        exit(0);
    }
    else{
        close(fd[READ_END]);
        write(fd[WRITE_END], "Data file open.", 16);
    }

    return 0;
}

int end_log_process(){
    close(fd[WRITE_END]);
    waitpid(pid,NULL,0);
    return 0;
}