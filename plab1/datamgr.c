#include <stdio.h>
#include "datamgr.h"
#include "config.h"
#include "lib/dplist.h"

typedef struct {
    uint16_t sensorID;
    uint16_t roomID;
    double running_avg;
    time_t last_modified;
}element_t;

void *element_copy(void *element){
    element_t *data = malloc(sizeof (element_t));

    data->sensorID = ((element_t *)element)->sensorID;
    data->roomID = ((element_t *)element)->roomID;
    data->running_avg = ((element_t *)element)->running_avg;
    data->last_modified = ((element_t *)element)->last_modified;

    return (void *)data;
}

void element_free(void **element){
    free(*element);
    *element = NULL;
}

int element_compare(void *x, void *y){
    if(((element_t *)x)->running_avg > ((element_t *)y)->running_avg){
        return 1;
    }
    else if(((element_t *)x)->running_avg < ((element_t *)y)->running_avg){
        return -1;
    }
    else{
        return 0;
    }
}

dplist_t *list = NULL;
dplist_t *list_map = NULL;

void datamgr_parse_sensor_files(FILE *fp_sensor_map, FILE *fp_sensor_data){
    datamgr_free();     //if user call this function multiple times
    list = dpl_create(element_copy, element_free, element_compare);
    list_map = dpl_create(element_copy, element_free, element_compare);
    FILE *fp_stderr = fopen("stderr.txt","w+");

    element_t *data = malloc(sizeof (element_t));
    element_t *data_map = malloc(sizeof (element_t));
    uint16_t sensorID;
    uint16_t roomID;
    double temperature;
    time_t timestamp;
    int index = 0;
    int index_map = 0;

    while(fscanf(fp_sensor_map,"%hu %hu", &roomID, &sensorID) == 2){
        data_map->sensorID = sensorID;
        data_map->roomID = roomID;
        printf("sensorID: %hu ",sensorID);
        printf("roomID: %hu ",roomID);

        dpl_insert_at_index(list_map, data_map, index_map, true);
        index_map++;
        printf("index_map: %d\n",index_map);
    }

    while(fread(&sensorID, sizeof(sensorID), 1, fp_sensor_data) == 1 &&
          fread(&temperature, sizeof(temperature), 1, fp_sensor_data) == 1 &&
          fread(&timestamp, sizeof(timestamp), 1, fp_sensor_data) == 1){

        data->sensorID = sensorID;
        data->running_avg = temperature;
        data->last_modified = timestamp;
        data->roomID = datamgr_get_room_id(sensorID);

        dpl_insert_at_index(list, data, index, true);
        index++;
    }

    dplist_t *list_temp = NULL;
    dplist_t *list_sorted = NULL;

    for(int i=0;i< dpl_size(list_map);i++){
        list_temp = dpl_create(element_copy, element_free, element_compare);
        list_sorted = dpl_create(element_copy, element_free, element_compare);
        sensorID = ((element_t *)dpl_get_element_at_index(list, i))->sensorID;
        element_t *element = NULL;
        bool flag = false;

        int size = dpl_size(list);
        for(int j=0;j< size;j++){
            element = dpl_get_element_at_index(list, j);
            if(element->sensorID == sensorID){
                dpl_insert_at_index(list_temp, element, size, true);
            }
        }

        size = dpl_size(list_temp);
        element_t *element_temp;
        time_t time;
        if(size<5){
            flag = true;
        }
        else{
            for(int j=0;j<5;j++){
                timestamp = 0;
                for(int k=0;k<size;k++){
                    element = dpl_get_element_at_index(list_temp,k);
                    element_temp = dpl_get_element_at_index(list_sorted,0);
                    if(element_temp == NULL)
                        time = 0;
                    else
                        time = element_temp->last_modified;
                    if(timestamp < element->last_modified && (time > timestamp || j == 0)){
                        timestamp = element->last_modified;
                        index = k;
                    }
                }
                dpl_insert_at_index(list_sorted, dpl_get_element_at_index(list_temp,index),j,true);
            }
        }

        double sum = 0;
        for(int j=0;j<RUN_AVG_LENGTH;j++){
            sum = sum + ((element_t *)dpl_get_element_at_index(list_sorted, j))->running_avg;
        }

        element = dpl_get_element_at_index(list_map, i);
        element->running_avg = flag? 0 : sum / RUN_AVG_LENGTH;
        element->last_modified = ((element_t *)dpl_get_element_at_index(list_sorted,0))->last_modified;
        dpl_free(&list_temp,true);
        dpl_free(&list_sorted,true);
    }

    for(int i=0;i< dpl_size(list_map);i++){
        element_t *element = dpl_get_element_at_index(list_map,i);
        if(element->running_avg<SET_MIN_TEMP){
            fprintf(fp_stderr,"The temperature is lower than SET_MIN_TEMP where sensorID = %hu, roomID = %hu\n",element->sensorID,element->roomID);
        }
        else if(element->running_avg>SET_MAX_TEMP){
            fprintf(fp_stderr,"The temperature is higher than SET_MAX_TEMP where sensorID = %hu, roomID = %hu\n",element->sensorID, element->roomID);
        }
        else{

        }
    }

    element_t *element = NULL;
    for(int i=0;i< dpl_size(list_map);i++){
        element = dpl_get_element_at_index(list_map, i);
        printf("sensorID: %hu\n",element->sensorID);
        printf("roomID: %hu\n",element->roomID);
        printf("running_avg: %lf\n",element->running_avg);
        printf("last_modified: %ld\n",element->last_modified);
    }
    fclose(fp_stderr);

    free(data);
    free(data_map);
}

void datamgr_free(){
    if(list == NULL || list_map == NULL){

    }
    else {
        dpl_free(&list, true);
        dpl_free(&list_map, true);
        list = NULL;
        list_map = NULL;
    }
}

uint16_t datamgr_get_room_id(sensor_id_t sensor_id){
    if(list == NULL || list_map == NULL){
        return -1;
    }
    else {
        element_t *data = NULL;
        for (int i = 0; i < dpl_size(list_map); i++) {
            data = dpl_get_element_at_index(list_map, i);
            if (data->sensorID == sensor_id) {
                return data->roomID;
            }
        }
        ERROR_HANDLER(true, "This is not a valid result");
    }
}

sensor_value_t datamgr_get_avg(sensor_id_t sensor_id){
    if(list == NULL || list_map == NULL){
        return -1;
    }
    else {
        element_t *data = NULL;
        for (int i = 0; i < dpl_size(list_map); i++) {
            data = dpl_get_element_at_index(list, i);
            if (data->sensorID == sensor_id) {
                return data->running_avg;
            }
        }
        ERROR_HANDLER(true, "This is not a valid result");
    }
}

time_t datamgr_get_last_modified(sensor_id_t sensor_id){
    if(list == NULL || list_map == NULL){
        return -1;
    }
    else {
        element_t *data = NULL;
        for (int i = 0; i < dpl_size(list_map); i++) {
            data = dpl_get_element_at_index(list, i);
            if (data->sensorID == sensor_id) {
                return data->last_modified;
            }
        }
        ERROR_HANDLER(true, "This is not a valid result");
    }
}

int datamgr_get_total_sensors(){
    if(list == NULL || list_map == NULL){
        return -1;
    }
    else{
        return dpl_size(list_map);
    }
}