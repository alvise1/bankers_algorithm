#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "file_reader.h"

int request_resources(int customer_num, int request[]);
void release_resources(int customer_num, int release[]);

#endif
