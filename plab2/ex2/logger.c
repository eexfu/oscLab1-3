#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(void){
    char write_msg[SIZE] = "Hello!";
    char read_msg[SIZE];
    char read_msg_show[SIZE];
    pid_t pid;
    int fd[2];

    //create the pipe
    if(pipe(fd) == -1){
        printf("Pipe failed\n");
        return 1;
    }

    //fork the child
    pid = fork();

    if(pid < 0){
        printf("Fork failed\n");
        return 1;
    }
    else if(pid == 0){
        //child
        close(fd[WRITE_END]);
        read(fd[READ_END],read_msg,SIZE);
        int i;
        for(i=0;read_msg[i]!='\0';i++){
            if(islower(read_msg[i])){
                read_msg_show[i] = toupper(read_msg[i]);
            }
            else{
                read_msg_show[i] = tolower(read_msg[i]);
            }
        }
        read_msg_show[i] = '\0';
        printf("%s\n",read_msg_show);
        close(fd[READ_END]);
    }
    else{
        //parent
        close(fd[READ_END]);
        printf("Please enter a sentence (less than 25 chars) :");
        fgets(write_msg,SIZE,stdin);
        write(fd[WRITE_END],write_msg,strlen(write_msg)+1);
        close(fd[WRITE_END]);
    }
    return 0;
};