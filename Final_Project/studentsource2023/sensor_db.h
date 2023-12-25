/**
 * \author Bert Lagaisse
 */

#ifndef _SENSOR_DB_H_
#define _SENSOR_DB_H_

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "sbuffer.h"

#include <stdbool.h>
FILE * open_db(char * filename, bool append);
int insert_sensor(sensor_id_t id, sensor_value_t value, sensor_ts_t ts);
int close_db();
void *create_storagemgr(void *ptr);
#endif /* _SENSOR_DB_H_ */