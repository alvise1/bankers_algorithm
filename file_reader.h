#ifndef FILE_READER_H
#define FILE_READER_H

#include <stdio.h>

extern int num_customers;
extern int num_resources;
extern int **maximum;
extern int *available;
extern int **allocation;
extern int **need;

int read_file(const char *filename);

#endif
