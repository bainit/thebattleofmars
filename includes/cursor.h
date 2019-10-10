#ifndef CURSOR_FUN_H
#define CURSOR_FUN_H

#include "types.h"

#define CURSOR 301

#define CURSOR_UP 'w'
#define CURSOR_DOWN 's'
#define CURSOR_LEFT 'a'
#define CURSOR_RIGHT 'd'

#define CURSOR_STOP 't'
#define CURSOR_SBOTTOM 'g'
#define CURSOR_SLEFT 'f'
#define CURSOR_SRIGHT 'h'
#define CURSOR_SCENTER 'r'

#define CURSOR_MTOP 'i'
#define CURSOR_MBOTTOM 'k'
#define CURSOR_MLEFT 'j'
#define CURSOR_MRIGHT 'l'
#define CURSOR_MCENTER 'u'

void 		cursor_insert_to_map(Map *m, Cursor *c);
Map_field*  	cursor_action_on_char(Cursor *cur, Map *m, char c);
Map_field*	cursor_move(Cursor *cur, Map *m, char c);

void 		cursor_set_position(Cursor *c, int x, int y);
void  		cursor_move_screen_top(Cursor *c);
void  		cursor_move_screen_bottom(Cursor *c);
void  		cursor_move_screen_left(Cursor *c);
void  		cursor_move_screen_right(Cursor *c);
void  		cursor_move_screen_center(Cursor *c);

void  		cursor_move_map_top(Cursor *c, Map *m);
void  		cursor_move_map_bottom(Cursor *c, Map *m);
void  		cursor_move_map_left(Cursor *c, Map *m);
void  		cursor_move_map_right(Cursor *c, Map *m);
void  		cursor_move_map_center(Cursor *c, Map *m);

#endif //CURSOR_FUN_H
