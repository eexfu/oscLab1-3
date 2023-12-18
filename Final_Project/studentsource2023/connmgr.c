#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>
#include <sys/socket.h>
#include "config.h"
#include "lib/tcpsock.h"
#include "sbuffer.h"

extern int MAX_CONN;
extern int PORT;
extern sbuffer_t *buffer_datamgr;
extern sbuffer_t *buffer_storagemgr;
int extern write_to_log_process(char *msg);

void send_end_of_stream() {
    printf("send end of dtream\n");
    sensor_data_t *data = malloc(sizeof(sensor_data_t));
    data->id=0;
    sbuffer_insert(buffer_datamgr,data);
    sbuffer_insert(buffer_storagemgr,data);
    free(data);
}

void *listen_to_a_sensor_node(void *ptr) {
    write_to_log_process("Incoming client connection.");
    if (ptr == NULL) return NULL;
    tcpsock_t *client = (tcpsock_t *) ptr;

    int bytes, result;
    sensor_data_t data;
    do {
        // todo the tcp connection should be closed if time out
        // read sensor ID
        bytes = sizeof(data.id);
        result = tcp_receive(client, (void *) &data.id, &bytes);
        // read temperature
        bytes = sizeof(data.value);
        result = tcp_receive(client, (void *) &data.value, &bytes);
        // read timestamp
        bytes = sizeof(data.ts);
        result = tcp_receive(client, (void *) &data.ts, &bytes);
        if ((result == TCP_NO_ERROR) && bytes) {
            sbuffer_insert(buffer_datamgr, &data);
            sbuffer_insert(buffer_storagemgr, &data);
        }
    } while (result == TCP_NO_ERROR);
    if (result == TCP_CONNECTION_CLOSED) {
        printf("Peer has closed connection\n");
        write_to_log_process("Peer has closed connection.");
    }
    else{
        printf("Error occured on connection to peer\n");
        write_to_log_process("Error occured on connection to peer.");
    }
    tcp_close(&client);
    return NULL;
}

void *create_connmgr(void * ptr){
    int number_of_client = 0;
    // start a server, and listen to the port
    tcpsock_t *server, *client;
    pthread_t thread_poll[MAX_CONN - 1];

    printf("Test server is started\n");
    write_to_log_process("Test server is started.");
    if (tcp_passive_open(&server, PORT) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    do {
        if (tcp_wait_for_connection(server, &client) != TCP_NO_ERROR) exit(EXIT_FAILURE);
        if(number_of_client < MAX_CONN){
            pthread_t thread;
            if(pthread_create(&thread, NULL, listen_to_a_sensor_node, (void *)client) != 0) exit(EXIT_FAILURE);
            thread_poll[number_of_client - 1] = thread;
            printf("Incoming client connection\n");
            printf("number of client: %d", number_of_client);
            printf("MAX_CONN: %d",MAX_CONN);
        }
        number_of_client++;
    } while (number_of_client < MAX_CONN);
    //wait all the client to join
    for (int i = 0; i < MAX_CONN ; i++) {
        pthread_join(thread_poll[i], NULL);
        printf("thread%d has finished.\n",i);
    }
    send_end_of_stream();
    if (tcp_close(&server) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    printf("Test server is shutting down\n");
    write_to_log_process("Test server is shutting down.");

    return NULL;
}