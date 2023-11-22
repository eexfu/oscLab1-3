/**
 * \author {AUTHOR}
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>
#include "config.h"
#include "lib/tcpsock.h"

/**
 * Implements a sequential test server (only one connection at the same time)
 */

void *listen_to_a_sensor_node(void *ptr){
    if(ptr == NULL) return NULL;
    tcpsock_t *client = (tcpsock_t *)ptr;
    int bytes, result;
    sensor_data_t data;
    do {
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
            printf("sensor id = %" PRIu16 " - temperature = %g - timestamp = %ld\n", data.id, data.value,
                   (long int) data.ts);
        }
    } while (result == TCP_NO_ERROR);
    if (result == TCP_CONNECTION_CLOSED)
        printf("Peer has closed connection\n");
    else
        printf("Error occured on connection to peer\n");
    tcp_close(&client);
}

int main(int argc, char *argv[]) {
    tcpsock_t *server, *client;
    int conn_counter = 0;

    if(argc < 3) {
    	printf("Please provide the right arguments: first the port, then the max nb of clients");
    	return -1;
    }
    
    int MAX_CONN = atoi(argv[2]);
    int PORT = atoi(argv[1]);

    pthread_t thread_poll[MAX_CONN - 1];

    printf("Test server is started\n");
    if (tcp_passive_open(&server, PORT) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    do {
        if (tcp_wait_for_connection(server, &client) != TCP_NO_ERROR) exit(EXIT_FAILURE);
        conn_counter++;
        if(conn_counter < MAX_CONN){
            pthread_t thread;
            if(pthread_create(&thread, NULL, listen_to_a_sensor_node, (void *)client) != 0) exit(EXIT_FAILURE);
            thread_poll[conn_counter - 1] = thread;
            printf("Incoming client connection\n");
        }
    } while (conn_counter < MAX_CONN);
    for(int i=0; i<conn_counter - 1; i++){
        pthread_join(thread_poll[i],NULL);
    }
    if (tcp_close(&server) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    printf("Test server is shutting down\n");
    return 0;
}