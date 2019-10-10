#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

#include "types.h"

#define VOK 0

#define VERR -100
#define VERR_OUT_OF_RANGE -101
#define VERR_NULL_VECTOR -102
#define VERR_NULL_DATA -103
#define VERR_MALLOC_ERROR -104
#define VERR_WRONG_INIT_PARAM -105
#define VERR_NOT_INITIALIZED -106
#define VERR_ALREADY_INITIALIZED -107

#define VDEFSIZE 10

int 	vector_init(Vector *v, unsigned int num, unsigned long int sz);
int 	vector_get_size(Vector *v);
int 	vector_push_back(Vector *v, void *d);
int 	vector_insert_data(Vector *v, int pos, void *d);
void* 	vector_get_data(Vector *v, int pos);
int 	vector_resize(Vector *v);
int 	vector_remove_data(Vector *v, int pos);
int 	vector_remove_last(Vector *v);
int 	vector_clear(Vector *v);

int 	vector_free(Vector *v);

#endif //VECTOR_H
