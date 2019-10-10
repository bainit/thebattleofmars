#include "includes/types.h"

#include "includes/player.h"
#include "includes/map.h"
#include "includes/vector.h"
#include "includes/unit.h"
#include "includes/util.h"

void player_init(Player *p, Color_type pc) {

	exit_on_error(vector_init(&p->units, VDEFSIZE, sizeof(Unit)), "Error in player_init during initialization of player.units");
	p->pcolor = pc;

}

void player_add_unit(Player *p, Unit_type type, int posx, int posy) {

	Unit u;

	exit_on_error(unit_set_values(&u,type, posx, posy,p), "Error in player_add_unit() during seting values of unit");

	exit_on_error(vector_push_back(&p->units,&u), "Error in player_add_unit() during adding unit to player.units");	

}

void player_post_units(Player *p, Map *m) {

	int i;
	int pos;
	for(i=0;i<p->units.size; i++) {
		Unit *u = vector_get_data(&p->units,i);
		pos = u->posy * (m->sizex) + u->posx;
		m->fields[pos].type = u->type;
		m->fields[pos].item = u;
	}

}

void player_next_round(Player *p) {

	exit_on_null(p,"Error in player_next_round, player is null");

	int i;
	Unit *u;
	for(i=0;i<p->units.size;i++) {
		u = (Unit*)vector_get_data(&p->units,i);
		unit_next_round(u);
	}

}

Boolean player_is_same(Player *p1, Player *p2) {

	exit_on_null(p1, "Error in player_is_same, player 1 is null");
	exit_on_null(p2, "Error in player_is_same, player 2 is null");

	if(p1->pcolor == p2->pcolor)
		return BOOL_TRUE;
	
	return BOOL_FALSE;

}

void player_remove_destroyed_units(Player *p, Map *m) {

	exit_on_null(p,"Error in player_remove_destroyed_units, player is null");
	exit_on_null(m,"Error in player_remove_destroyed_units, map is null");

	Vector *v = &(p->units);
	int sz = vector_get_size(v);

	int i;
	Unit *u;
	for(i=0;i<sz;i++) {
		u = (Unit*)vector_get_data(v,i);
		if(unit_is_destroyed(u)) {
			
			map_set_field(m,u->posx,u->posy,MAP_TYPE_DESTROYED_UNIT);
			vector_remove_data(v,i);
			sz--;
		}
	}

}

Point player_rand_position(Map *m, char t) {

	exit_on_null(m,"Error in player_rand_position, map is null");
	Point p;	

	int minx,miny,maxx,maxy;
	if(t==TEAM_PLAYER) {
		minx = 0;
		miny = 0;
		//maxx = m->sizex/4;
		//maxy = m->sizey/4;
		maxx = 7;
		maxy = 7;
	}else if (t==TEAM_FOE) {
		//minx = m->sizex/4;
		//miny = m->sizey/4;
		//maxx = m->sizex/2;
		//maxy = m->sizey/2;
		minx = 40;
		miny = 10;
		maxx = 48;
		maxy = 18;
		//minx = 10;
		//miny = 7;
		//maxx = 20;
		//maxy = 9;
	}else {
		exit_on_error(-1,"Error in player_rand_position, cant recognize team");
	}
	
	p.x = rand_in_range(minx,maxx);
	p.y = rand_in_range(miny,maxy);	

	return p;

}

void player_free(Player *p) {

	exit_on_error(vector_free(&p->units), "Error in player_free() during freeing player.units");

}

