#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

#include "includes/types.h"
#include "includes/vector.h"

int vector_init(Vector *v, unsigned int num, unsigned long int  sz){

	if(v==NULL)
		return VERR_NULL_VECTOR;

	if(v->data != NULL)
		return VERR_ALREADY_INITIALIZED;

        if(num == 0 || sz == 0)
                return  VERR_WRONG_INIT_PARAM;

        char *d = malloc(num*sz);
	if(d==NULL)
		return VERR_MALLOC_ERROR;

	v->data = d;
	v->max_size = num;
	v->size = 0;
	v->data_size = sz;	

	return VOK;

}

int vector_get_size(Vector *v) {
	
	if(v==NULL)
		return VERR_NULL_VECTOR;
	return v->size;

}

int vector_push_back(Vector *v, void *d) {

	if(v==NULL)
		return VERR_NULL_VECTOR;
	if(v->data==NULL)
		return VERR_NOT_INITIALIZED ;
	if(d==NULL)
		return VERR_NULL_DATA;
	if(v->size >= v->max_size)
		vector_resize(v);

	memcpy(v->data+v->size*v->data_size,d,v->data_size);
	v->size++;
	return VOK;
}

int vector_insert_data(Vector *v, int pos, void *d) {
	
	if(v==NULL)
		return VERR_NULL_VECTOR;
	if(v->data==NULL)
		return VERR_NOT_INITIALIZED ;
	if(pos<0 || pos>v->size)
		return VERR_OUT_OF_RANGE;
	if(d==NULL)
		return VERR_NULL_DATA;

	if(v->size+1 > v->max_size)
		vector_resize(v);
	
	memmove(v->data+(pos+1)*v->data_size, v->data+pos*v->data_size,v->data_size*(v->size-pos));
	
	memcpy(v->data+pos*v->data_size,d,v->data_size);
        v->size++;
	
	return VOK;

}

void* vector_get_data(Vector* v, int pos) {

	if(v==NULL)
		return NULL;
	if(v->data == NULL) 
		return NULL;

	if(pos<0 || pos>=v->size)
		return NULL;
	return v->data+pos*v->data_size;

}

int vector_resize(Vector *v) {

	if(v == NULL)
		return VERR_NULL_VECTOR;
	if(v->data == NULL)
		return VERR_NOT_INITIALIZED ;
	
	int nsize = v->max_size + v->max_size/2;

	char *dd = malloc(nsize*v->data_size);
	if(dd == NULL)	
		return VERR_NULL_DATA;
	
	memcpy(dd,v->data, v->size*v->data_size);
	v->max_size = nsize;

	free(v->data);
	v->data = dd;

	return VOK;
}

int vector_remove_data(Vector *v, int pos) {

	if(v==NULL)
		return VERR_NULL_VECTOR;
	if(v->data==NULL)
		return VERR_NOT_INITIALIZED ;
	if(pos<0 || pos>=v->size)
		return VERR_OUT_OF_RANGE;

 	memmove(v->data+pos*v->data_size, v->data+(pos+1)*v->data_size,v->data_size*(v->size-pos-1));

	v->size--;
	return VOK;

}

int vector_remove_last(Vector *v) {

	if(v==NULL)
		return VERR;
	
	if(v->data==NULL)
		return VERR_NOT_INITIALIZED ;

	if(v->size==0)
		return VERR_OUT_OF_RANGE;

	v->size--;
	
	return VOK;

}

int vector_clear(Vector *v) {
	
	if(v==NULL)
		return VERR_NULL_VECTOR;
	if(v->data==NULL)
		return VERR_NOT_INITIALIZED ;

	v->size = 0;

	return VOK;

}

int vector_free(Vector *v) {
	
	if(v==NULL)
		return VERR_NULL_VECTOR;

	if(v->data==NULL)
		return VERR_NOT_INITIALIZED ;	

	free(v->data);	
	v->data = NULL;

	v->size = 0;
	v->max_size = 0;
	v->data_size = 0;

	return VOK;

}
