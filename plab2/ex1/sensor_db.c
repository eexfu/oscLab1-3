#include <stdio.h>
#include "sensor_db.h"

FILE * open_db(char * filename, bool append){
    FILE *fp = NULL;

    if(append){
        fp = fopen(filename,"a");
    }
    else{
        fp = fopen(filename,"w");
    }

    return fp;
}

int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){
    fprintf(f,"%hu, %lf, %ld\n",id,value,ts);
    return 0;
}

int close_db(FILE * f){
    fclose(f);
    return 0;
}