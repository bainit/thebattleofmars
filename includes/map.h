#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include "types.h"

#define MAP_OK 0
#define MAPERR_NO_FILE -300
#define MAPERR_NULL_PTR -301
#define MAPERR_OUT_OF_MAP_SIZE -302
#define MAPERR_COPY_WRONG_SIZES -303
#define MAPERR_SIZETOSMALL -310

#define KWERR_TO_LONG -311
#define KWERR_UNDEFINED -312
#define KWERR_FIRST_PARAM -313
#define KWERR_SECOND_PARAM -314
#define KWERR_BOTH_PARAMS -315
#define KWERR_NO_DIGIT -316
#define KWERR_NO_SUCH_KEYWORD -317
#define KWERR_POSITION_OUT_OF_RANGE -318

#define KW_EOL 10
#define KW_PARAM 1
#define KW_END_PARAMS 2
#define KW_READ_PARAMS_OK 3

#define WORDSIZE 255
#define KEYWORD 1000

#define DEF_FIELD 0

#define MAP_SCREEN_X 50
#define MAP_SCREEN_Y 20

#define KW_SIZE "size"
#define KW_TREE "tree"
#define KW_ROCK "rock"

#define MAP_TYPE_ADJUST 100

int   map_read_word(FILE *file, char word[], int wsize);
int   map_read_params(FILE *file, char word[], int wsize, int *p1, int *p2);
int   map_read_param(FILE *file, char word[], int wsize);
int   map_read_gettype(char *keyword);
Boolean   map_check_position(Map *m, int x, int y);
Boolean   map_is_free_position(Map *m, int x, int y);

Map_field* map_get_field(Map *m, int x, int y);
void  map_set_field(Map *m, int x, int y, int type);

int   map_load(char *file, Map *m);
int   map_init(Map *m, int sx, int sy);
int   map_copy(Map *source, Map *destination);
char *map_field_for_type(int type);
Boolean map_is_field_unit(int type);
Color_type map_get_fcolor_for_field(Map_field* field);
Color_type map_get_bcolor_for_field(Map_field* field);

Boolean map_is_unit_in_map_view(Unit *u, Map *m);

void  map_print_line(Game_media *g, Map *m, Cursor *c, Unit *au, int line, Vector *obs);
void  map_print(Game_media *g, Map *m, Cursor *c, Unit *au, Info_screen* is);

void  map_check_offsetx(Map *m);
void  map_check_offsety(Map *m);

void  map_msg_to_infoscreen(Map_field *mf, Info_screen *is);
void  map_field_to_string(Map_field *mf, char *str, int strl);

int   map_adjust_type(Map_type type);

void map_find_obstacles_around(Unit *u, Map *m, Vector *obs);
void map_find_obstacles(Unit *u, Map *m, Vector *obs);
void map_calc_obstacles(Unit *u, Vector *obs);

void map_post_missile(Missile *missile, Map *map);

void  map_free(Map *m);

#endif //MAP_H
