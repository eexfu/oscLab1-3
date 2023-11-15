#include <time.h>
#include "logger.h"

FILE *fp;
int sequence;

int write_to_log_process(char *msg){
    char write_msg[100];
    char formatted_time[50];
    time_t current = time(NULL);
    struct tm *time = localtime(&current);
    strftime(formatted_time, sizeof(formatted_time), "%a %b %d %H:%M:%S %Y", time);
    snprintf(write_msg,sizeof(write_msg),"%d - %s - %s\n",sequence,formatted_time,msg);

    fprintf(fp,"%s",write_msg);

    sequence++;
    return 0;
}

int create_log_process(){
    fp = fopen("gateway.log","w");

    write_to_log_process("Data file open.");

    sequence++;
    return 0;
}

int end_log_process(){
    write_to_log_process("Data file closed");

    fclose(fp);

    sequence++;
    return 0;
}