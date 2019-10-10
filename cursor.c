#include <stdio.h>

#include "includes/cursor.h"
#include "includes/map.h"
#include "includes/util.h"
#include "includes/unit.h"

void cursor_insert_to_map(Map *m, Cursor *c) {

	exit_on_null(m,"Error in cursor insert to map, map is null");
	exit_on_null(c,"Error in cursor insert to map, cursor is null");

	map_set_field(m,c->x,c->y,c->type);

}

Map_field* cursor_action_on_char(Cursor *cur, Map *m, char c) {

	exit_on_null(cur, "Error in cursor_action_on_char, cursor is null");
        exit_on_null(m, "Error in cursor_action_on_char, map is null");

	if(c==CURSOR_UP || c==CURSOR_DOWN || c==CURSOR_LEFT || c==CURSOR_RIGHT) {
		return cursor_move(cur,m,c);

	}
	else if(c==CURSOR_STOP) {
		cursor_move_screen_top(cur);
	}
	else if(c==CURSOR_SBOTTOM) {
		cursor_move_screen_bottom(cur);
	}
	else if(c==CURSOR_SLEFT) {
		cursor_move_screen_left(cur);
	}
	else if(c==CURSOR_SRIGHT) {
		cursor_move_screen_right(cur);
	}
	else if(c==CURSOR_SCENTER) {
		cursor_move_screen_center(cur);
	}
	else if(c==CURSOR_MTOP) {
		cursor_move_map_top(cur,m);
	}
	else if(c==CURSOR_MBOTTOM) {
		cursor_move_map_bottom(cur,m);
	}
	else if(c==CURSOR_MLEFT) {
		cursor_move_map_left(cur,m);
	}
	else if(c==CURSOR_MRIGHT) {
		cursor_move_map_right(cur,m);
	}
	else if(c==CURSOR_MCENTER) {
		cursor_move_map_center(cur,m);
	}

	return NULL;
}

Map_field* cursor_move(Cursor *cur, Map *m, char c) {

	if(c==CURSOR_RIGHT){
		if(cur->x < MAP_SCREEN_X-1)
			cur->x++;
		else
			m->offsetx++;
	}
	else if(c==CURSOR_LEFT) {
		if(cur->x > 0)
			cur->x--;
		else 
			m->offsetx--;
	}
	else if(c==CURSOR_UP) {
		if(cur->y > 0)
			cur->y--;
		else 
			m->offsety--;
	}
	else if(c==CURSOR_DOWN) {
		if(cur->y < MAP_SCREEN_Y-1)
			cur->y++;
		else m->offsety++;
	}

	map_check_offsetx(m);
	map_check_offsety(m);

	int posx = cur->x+m->offsetx;
	int posy = cur->y+m->offsety;

        Map_field *field = map_get_field(m,posx,posy);
	return field;
}

void cursor_set_position(Cursor *c, int x, int y) {
	c->x = x;
	c->y = y;
}

void cursor_move_screen_top(Cursor *c) {

	c->y = 0;

}

void cursor_move_screen_bottom(Cursor *c) {

	c->y = MAP_SCREEN_Y-1;
}

void cursor_move_screen_left(Cursor *c) {

	c->x = 0;

}

void cursor_move_screen_right(Cursor *c) {

	c->x = MAP_SCREEN_X-1;

}

void cursor_move_screen_center(Cursor *c) {

	c->x = MAP_SCREEN_X/2;
	c->y = MAP_SCREEN_Y/2;

}

void cursor_move_map_top(Cursor *c, Map *m) {

	cursor_move_screen_top(c);
	m->offsetx = 0;
}

void cursor_move_map_bottom(Cursor *c, Map *m) {
	
	cursor_move_screen_bottom(c);
	m->offsetx = m->sizex-MAP_SCREEN_X;
}

void cursor_move_map_left(Cursor *c, Map *m) {
	
	cursor_move_screen_left(c);
	m->offsety = 0;
}

void cursor_move_map_right(Cursor *c, Map *m) {

	cursor_move_screen_right(c);
	m->offsety = m->sizey-MAP_SCREEN_Y;

}

void cursor_move_map_center(Cursor *c, Map *m) {

	cursor_move_screen_center(c);
	
	m->offsetx = (m->sizex-MAP_SCREEN_X)/2;
	m->offsety = (m->sizey-MAP_SCREEN_Y)/2;

}
