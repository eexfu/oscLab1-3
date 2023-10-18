#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>

#define frequency 1
#define max_temp 35.0
#define min_temp 10.0

int main(void){
    srand(time(NULL));
    time_t now;

    while(1){
        time(&now);

        // 转换为本地时间
        struct tm *local_time = localtime(&now);

//        char* timeString = asctime(local_time);

        float temperature = ((float) rand() / RAND_MAX) * (max_temp - min_temp) + min_temp;
        printf("Temperature = %f @ %d-%d-%d/%d:%d:%d\n", temperature, local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        fflush(stdout);
//        printf("%s",timeString);
//        fflush(stdout);
        sleep(1/frequency);
    }
}