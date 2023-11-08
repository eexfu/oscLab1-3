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

typedef struct {
    uint16_t sensorID;
    uint16_t roomID;
}element_map_t;

void *element_map_copy(void *element){
    element_map_t *data = malloc(sizeof (element_map_t));

    data->sensorID = ((element_map_t *)element)->sensorID;
    data->roomID = ((element_map_t *)element)->roomID;

    return (void *)data;
}

void element_map_free(void **element){
    free(*element);
    *element = NULL;
}

int element_map_compare(void *x, void *y){
    if(((element_map_t *)x)->sensorID > ((element_map_t *)y)->sensorID){
        return 1;
    }
    else if(((element_map_t *)x)->sensorID < ((element_map_t *)y)->sensorID){
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

        dpl_insert_at_index(list_map, data_map, index_map, true);
        index_map++;
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

    for(int i=0;i< dpl_size(list_map);i++){
        list_temp = dpl_create(NULL, NULL, NULL);
        sensorID = ((element_t *)dpl_get_element_at_index(list, i))->sensorID;
        element_t *element = NULL;
        int size = dpl_size(list);

        for(int j=0;j< size;j++){
            element = dpl_get_element_at_index(list, j);
            if(element->sensorID == sensorID){
                dpl_insert_at_index(list_temp, &(element->running_avg), size, false);
            }
        }

        size = dpl_size(list_temp);
        double sum = 0;
        for(int j=0;j<RUN_AVG_LENGTH;j++){
            sum = sum + *((double *)dpl_get_element_at_index(list_temp, size-j-1));
        }

        element = dpl_get_element_at_index(list_map, i);
        element->running_avg = sum / RUN_AVG_LENGTH;
        //element->last_modified
        dpl_free(&list_temp,false);
    }

    element_t *element = dpl_get_element_at_index(list_map, 9);
    printf("sensorID: %hu\n",element->sensorID);
    printf("roomID: %hu\n",element->roomID);
    printf("running_avg: %lf\n",element->running_avg);
    printf("last_modified: %ld\n",element->last_modified);

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
        element_map_t *data_map = NULL;
        for (int i = 0; i < dpl_size(list_map); i++) {
            data_map = dpl_get_element_at_index(list_map, i);
            if (data_map->sensorID == sensor_id) {
                return data_map->roomID;
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