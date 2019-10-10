#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "includes/types.h"

#include "includes/util.h"
#include "includes/unit.h"
#include "includes/player.h"
#include "includes/map.h"
#include "includes/info_screen.h"
#include "includes/keys.h"
#include "includes/cursor.h"
#include "includes/game.h"
#include "includes/terminal.h"
#include "includes/vector.h"
#include "includes/weapon.h"
#include "includes/ai.h"
#include "includes/colors.h"

#define UNIT_WEAPON_FIRST 1
#define UNIT_WEAPON_SECOND 2

const int unit_health[UNITS_SIZE] = {
	45,	//light mech 
	70, 	//medium mech
	90, 	//heavy mech
	99	//super heavy mech
};

const int unit_action_points[UNITS_SIZE] = {
	8,	//ligth mech 
	6, 	//medium mech
	4,	//heavy mech 
	4	//super heavy mech
};

const char *unit_type_names[UNITS_SIZE] = {
	"Light mech",
	"Medium mech",
	"Heavy mech",
	"Super mech"
};

const Weapon_type unit_first_weapon[UNITS_SIZE] = {
	WEAPON_MACHINEGUN,
	WEAPON_MACHINEGUN,
	WEAPON_ROCKET,
	WEAPON_BIG_ROCKET

};

const Weapon_type unit_second_weapon[UNITS_SIZE] = {
	WEAPON_NONE,
	WEAPON_LASER,
	WEAPON_LASER,
	WEAPON_NONE

};

int unit_set_values(Unit *u, Unit_type t, int px, int py, Player *owner) {

	if(t <=UNIT_TYPE_START || t >= UNIT_TYPE_END)
		return UERR_TYPE_OUT_OF_RANGE;

	exit_on_null(owner, "Error in unit_set_values(), owner is null");

	u->type = t;
	u->posx = px;
	u->posy = py;

	u->health = unit_health[unit_adjust_type(t)];
	u->experience = 0;
	u->action_points = unit_action_points[unit_adjust_type(t)];
	u->improvements = 0;

	u->owner = owner;
	
	unit_set_weapon(unit_get_weapon_type_for_unit_type(t,UNIT_WEAPON_FIRST),&(u->fweapon));
	unit_set_weapon(unit_get_weapon_type_for_unit_type(t,UNIT_WEAPON_SECOND),&(u->sweapon));

	u->aweapon = UNIT_WEAPON_FIRST;

	u->isaiming = BOOL_FALSE;
	u->isfiring = BOOL_FALSE;

	return UNIT_OK;
}

void unit_set_weapon(Weapon_type wt, Weapon_slot *slot) {

	if(wt <= WEAPON_START || wt >= WEAPON_END)
		exit_on_error(UERR_TYPE_OUT_OF_RANGE, "Error in unit_set_weapon, weapon type out of range");

	slot->wtype = wt;
	slot->rounds = weapon_get_rounds(wt);
}

int unit_get_weapon_range(Unit *u) {

	exit_on_null(u,"Error in unit_get_weapon_range, unit is null");

	if(u->aweapon==UNIT_WEAPON_SECOND)
		return weapon_range(u->sweapon.wtype);

	return weapon_range(u->fweapon.wtype);	
}

Weapon_type unit_get_weapon_type_for_unit_type(Unit_type ut, int w) {

	if(w==UNIT_WEAPON_FIRST) {
		return unit_first_weapon[unit_adjust_type(ut)];
	}else if(w==UNIT_WEAPON_SECOND) {
		return unit_second_weapon[unit_adjust_type(ut)];
	}

	exit_on_error(-1,"Error in unit_get_weapon_type_for_unit_type, no such weapon description");

}

int unit_get_weapon_rounds(Unit *u) {

	exit_on_null(u,"Error in unit_get_rounds, unit is null");

	if(u->aweapon==UNIT_WEAPON_FIRST) {
		return u->fweapon.rounds;
	}else {
		return u->sweapon.rounds;
	}
}

void unit_reduce_rounds(Unit *u) {

	exit_on_null(u,"Error in unit_reduce_rounds, unit is null");

	int rr = weapon_get_consume_rounds(unit_get_actual_weapon(u));
	int wr = unit_get_weapon_rounds(u);

	if(wr<rr) {
		exit_on_error(wr-rr,"Error in unit_reduce_rounds, unit weapon rounds is less then rounds to reduce");
	}

	if(u->aweapon==UNIT_WEAPON_FIRST){
		u->fweapon.rounds -= rr;
	}else {
		u->sweapon.rounds -= rr;
	}
}

const char *unit_get_weapon_name(Unit *u) {

	exit_on_null(u,"Error in unit_get_weapon_name, unit is null");

	if(u->aweapon==UNIT_WEAPON_SECOND)
		return weapon_name(u->sweapon.wtype);
	
	return weapon_name(u->fweapon.wtype);
}

void unit_use_first_weapon(Unit *u, Game_media *media) {

	exit_on_null(u, "Error in unit_use_first_weapon, unit is null");
	exit_on_null(media, "Error in unit_use_first_weapon, media is null");

	if(u->aweapon != UNIT_WEAPON_FIRST) {
		u->aweapon = UNIT_WEAPON_FIRST;
		const char *wname = weapon_name(u->fweapon.wtype);
		char msg[MAXLINE];
		strcpy(msg,"Changed to ");
		strcat(msg,wname);
		if(!ai_is_ai_turn(&(media->mgame->ai))){
			terminal_add_msg(media->mterm,msg,TERMINAL_MSG_NORMAL);
		}
	}

	unit_msg_to_infoscreen(u,media);
	unit_calc_weapon_range(media,u);
}

void unit_use_second_weapon(Unit *u, Game_media *media) {

	exit_on_null(u, "Error in unit_use_second_weapon, unit is null");
	exit_on_null(media, "Error in unit_use_second_weapon, media is null");

	if(u->aweapon != UNIT_WEAPON_SECOND) {
		u->aweapon = UNIT_WEAPON_SECOND;
		const char *wname = weapon_name(u->sweapon.wtype);
		char msg[MAXLINE];
		strcpy(msg,"Changed to ");
		strcat(msg,wname);
		if(!ai_is_ai_turn(&(media->mgame->ai))) {
			terminal_add_msg(media->mterm,msg, TERMINAL_MSG_NORMAL);
		}
	}

	unit_msg_to_infoscreen(u,media);
	unit_calc_weapon_range(media,u);
}

Weapon_type unit_get_actual_weapon(Unit *u){

	exit_on_null(u,"Error in unit_get_actual_weapon, unit is null");

	if(u->aweapon == UNIT_WEAPON_SECOND){
		return u->sweapon.wtype;
	}

	return u->fweapon.wtype;

}

Weapon_type unit_get_not_actual_weapon(Unit *u) {

	exit_on_null(u,"Error in unit_get_not_actual_weapon, unit is null");

	if(u->aweapon == UNIT_WEAPON_SECOND) {
		return u->fweapon.wtype;
	}

	return u->sweapon.wtype;

}

void unit_reduce_action_points(Unit *u, int val) {

	exit_on_null(u,"Error in unit_reduce_action_points, unit is null");
	if(val>u->action_points)
		exit_on_error(u->action_points,"Error in unit_reduce_action_points, val is larger than unit action points");

	u->action_points -= val;

}

const char *unit_get_type_name(Unit *u) {

	exit_on_null(u,"Error in unit_get_type_name, unit is null");
	return unit_type_names[unit_adjust_type(u->type)];

}

void unit_move_in_direction(Unit *u, Game_media *gm, char direction) {

	exit_on_null(gm, "Error in unit_move_in_direction, Game_media is null");
	exit_on_null(u,"Error in unit_move_in_direction, unit is null");

	switch (direction) {
		case KEY_UNIT_MOVE_LEFT : {
			if(unit_may_movex(u,gm->mmap, gm->mterm)) {
				unit_move_left(u,gm->mmap,gm->mcursor, gm->mterm);
			}
			unit_msg_to_infoscreen(u,gm);
			break;
		}
		case KEY_UNIT_MOVE_RIGHT : {
			if(unit_may_movex(u,gm->mmap, gm->mterm)){
				unit_move_right(u,gm->mmap,gm->mcursor, gm->mterm);
			}
			unit_msg_to_infoscreen(u,gm);
			break;
		}
		case KEY_UNIT_MOVE_DOWN : {
			if(unit_may_movey(u,gm->mmap, gm->mterm)) {
				unit_move_down(u,gm->mmap,gm->mcursor, gm->mterm);
			}
			unit_msg_to_infoscreen(u,gm);
			break;
		}
		case KEY_UNIT_MOVE_UP : {
			if(unit_may_movey(u,gm->mmap, gm->mterm)) {
				unit_move_up(u,gm->mmap,gm->mcursor, gm->mterm);
			}
			unit_msg_to_infoscreen(u,gm);
			break;
		}
	}

	unit_calc_weapon_range(gm,u);
}

void unit_to_string(Unit *u, char *line, int lsize, Game_media *gm) {

	exit_on_null(u,"Error in unit_to_string, unit is null");
	exit_on_null(line,"Error in unit_to_string, word is null");
	exit_on_null(gm, "Error in unit_to_string, game_media is null");

	if(game_is_unit_friend(gm->mgame,u)) {
		unit_friend_to_string(u,line,lsize);
	}else {
		unit_foe_to_string(u,line,lsize);
	}
}

void unit_friend_to_string(Unit *u, char *line, int lsize) {

	exit_on_null(u,"Error in unit_friend_to_string, unit is null");
	exit_on_null(line,"Error in unit_friend_to_string, word is null");

	if(lsize <1 || lsize > UNIT_MAX_LINE_SIZE) {
		exit_on_error(UERR_LINE_OUT_OF_RANGE,"Error in unit_friend_to_string, line lenght is out of range");
	}

	Player *p = u->owner;
	const char *unit_name = unit_get_type_name(u);
	const int weapon_range = unit_get_weapon_range(u);

	char team[MAXNAME];
	char health[MAXNAME];
	
	unit_get_team_string(u,team,TEAM_PLAYER);
	unit_get_health_string(u,health);

	sprintf(line,"Team: %s\nUnit type: %s\n Health: %s\n Experience: %d\n APs left: %d\n", team, unit_name, health, u->experience, u->action_points);

}

void unit_foe_to_string(Unit *u, char *line, int lsize) {

	exit_on_null(u,"Error in unit_foe_to_string, unit is null");
	exit_on_null(line,"Error in unit_foe_to_string, word is null");

	if(lsize <1 || lsize > UNIT_MAX_LINE_SIZE) {
		exit_on_error(UERR_LINE_OUT_OF_RANGE,"Error in unit_foe_to_string, line lenght is out of range");
	}

	Player *p = u->owner;
	const char *unit_name = unit_get_type_name(u);

	char team[MAXNAME];
	char health[MAXNAME];
	
	unit_get_team_string(u,team,TEAM_FOE);
	unit_get_foe_health_string(u,health);
	
	sprintf(line,"Team: %s\nUnit type: %s\n%s\n", team, unit_name, health);
}

void unit_msg_to_infoscreen(Unit *au, Game_media *gm) {

	unit_basic_msg_to_infoscreen(au,gm);
	unit_weapon_msg_to_infoscreen(au,gm);

}

void unit_aimed_unit_to_infoscreen(Unit *f, Game_media *gm) {

	exit_on_null(gm,"Error in unit_add_msg_to_infoscreen, game_media is null");
	if(f==NULL)
		return;
	
	if(game_is_unit_friend(gm->mgame, f)) {
		return;
	}

	static char aimed[] = "Aimed unit:\n";
	const static int aimed_length = 12;
	char line[INFO_SCREEN_LINE_MAX-aimed_length];
	char line2[INFO_SCREEN_LINE_MAX];
        unit_to_string(f,line,INFO_SCREEN_LINE_MAX,gm);
	strcpy(line2,aimed);
	strcat(line2,line);
        info_screen_add_msg(gm->minfo,line2);
}

void unit_basic_msg_to_infoscreen(Unit *au, Game_media *gm) {

	exit_on_null(au,"Error in unit_basic_msg_to_infoscreen, unit is null");
	exit_on_null(gm,"Error in unit_basic_msg_to_infoscreen, game_media is null");

	char austring[UNIT_MAX_LINE_SIZE];
	unit_to_string(au,austring, UNIT_MAX_LINE_SIZE,gm);
	info_screen_set_msg(gm->minfo,austring);

}

void unit_weapon_msg_to_infoscreen(Unit *au, Game_media *gm) {

	exit_on_null(au,"Error in unit_weapon_msg_to_infoscreen, unit is null");
	exit_on_null(gm, "Error in unit_weapon_msg_to_infoscreen, game_medai is null");
	
	char austring[UNIT_MAX_LINE_SIZE];

	if(game_is_unit_friend(gm->mgame,au)) {
		
		unit_get_weapon_string(au,UNIT_WEAPON_FIRST,austring);
		info_screen_add_msg(gm->minfo,austring);

		if(unit_has_two_weapons(au)) {
			unit_get_weapon_string(au,UNIT_WEAPON_SECOND,austring);
			info_screen_add_msg(gm->minfo,austring);
		}
	}
}

void unit_next_round(Unit *u) {

	exit_on_null(u,"Error in unit_next_round, unit is null");
	u->action_points = unit_action_points[unit_adjust_type(u->type)];

}

int unit_may_movex(Unit *u, Map *m, Terminal *t) {

	exit_on_null(u,"Error in unit_may_movex, unit is null");
	exit_on_null(m,"Error in unit_may_movey, map is null");

	if(u->action_points>0 && u->posx>=0 && u->posx<m->sizex) 
		return 1;
	
	if(u->action_points<=0)
		terminal_add_msg(t,"No more actions points",TERMINAL_MSG_MEDIUM);
	return 0;
}

int unit_may_movey(Unit *u, Map *m, Terminal *t) {

        exit_on_null(u,"Error in unit_may_movey, unit is null");
	exit_on_null(m,"Error in unit_may_movey, map is null");	

        if(u->action_points>0 && u->posy>=0 && u->posy<m->sizey) 
                return 1;

	if(u->action_points<=0)
		terminal_add_msg(t,"No more actions points", TERMINAL_MSG_MEDIUM);
        return 0;
}

void unit_move_right(Unit *u, Map *m, Cursor *c, Terminal *t) {

	if(map_is_free_position(m,u->posx+1,u->posy)) {
		u->posx++;
		unit_check_posx(u,m);
        	if(u->posx>MAP_SCREEN_X-1) {
        		m->offsetx++;
                	map_check_offsetx(m);
        	}
        	u->action_points--;
		cursor_set_position(c,u->posx,u->posy);
	}else {
		terminal_add_msg(t,"Can not move there", TERMINAL_MSG_NORMAL);
	}
}

void unit_move_left(Unit *u, Map *m, Cursor *c, Terminal *t) {

	if(map_is_free_position(m,u->posx-1,u->posy)) {
		u->posx--;
        	unit_check_posx(u,m);

       		if(u->posx<=m->sizex-MAP_SCREEN_X){
        		m->offsetx--;
                	map_check_offsetx(m);
        	}
        	u->action_points--;
		cursor_set_position(c,u->posx,u->posy);
	}else {
		terminal_add_msg(t,"Can not move there", TERMINAL_MSG_NORMAL);
	}
}


void unit_move_up(Unit *u, Map *m, Cursor *c, Terminal *t) {

	if(map_is_free_position(m,u->posx,u->posy-1)) {
		u->posy--;
		unit_check_posy(u,m);
	        if(u->posy<=m->sizey-MAP_SCREEN_Y){
        		m->offsety--;
                	map_check_offsety(m);
       		}
		u->action_points--;
		cursor_set_position(c,u->posx,u->posy);
	}else {
		terminal_add_msg(t,"Can not move there", TERMINAL_MSG_NORMAL);
	}
}

void unit_move_down(Unit *u, Map *m, Cursor *c, Terminal *t) {

	if(map_is_free_position(m,u->posx,u->posy+1)) {
		u->posy++;
		unit_check_posy(u,m);
        	if(u->posy>MAP_SCREEN_Y-1) {
        		m->offsety++;
	                map_check_offsety(m);
       		}
	        u->action_points--;
		cursor_set_position(c,u->posx,u->posy);
	}else {
		terminal_add_msg(t,"Can not move there", TERMINAL_MSG_NORMAL);
	}
}

void unit_check_posx(Unit *u, Map *m) {
	if(u->posx<0) {
		u->posx=0;
		return;
	}
	if(u->posx>=m->sizex) {
		u->posx=m->sizex-1;
		return;
	}
}

void unit_check_posy(Unit *u, Map *m) {

	if(u->posy<0) {
                u->posy=0;
                return;
        }
        if(u->posy>=m->sizey) {
                u->posy=m->sizey-1;
                return;
        }

}

int unit_adjust_type(Unit_type type) {

	return type-UNIT_TYPE_ADJUST;

}

Color_type unit_get_fcolor(Unit *unit) {

	exit_on_null(unit, "Error in unit_get_fcolor, unit is null");
	//Player *player;
	//player = (Player*)(unit->owner);

	//return player->pcolor;

	return COLOR_BLACK;

}

Color_type unit_get_bcolor(Unit *unit) {

	exit_on_null(unit, "Error in unit_get_bcolor, unit is null");
	Player *player;
	player = unit->owner;
	return player->pcolor;
}

Boolean unit_roughly_range(Unit *u, int x1, int y1, int x2, int y2, int range) {

	exit_on_null(u,"Error in unit_roughly_range, unit is null");

        if((abs(x1-x2)<range) && (abs(y1-y2)<range))
                return BOOL_TRUE;

        return BOOL_FALSE;

}


Boolean unit_range(Unit *u, int dist, int range) {

	exit_on_null(u,"Error in unit_range, unit is null");

	if(dist<range)
		return BOOL_TRUE;

	return BOOL_FALSE;

}

Boolean unit_is_firing(Unit *u) {

	exit_on_null(u,"Error in unit_is_firing(), unit is null");

	return u->isfiring; 

}

Boolean unit_has_enough_action_points(Unit *u, int ac) {

	exit_on_null(u, "Error in unit_enough_action_points, unit is null");
		
	if(u->action_points>0 && u->action_points>=ac)
		return BOOL_TRUE;

	return BOOL_FALSE;

}

Boolean unit_has_enough_rounds(Unit *u, int r) {

	exit_on_null(u,"Error in unit_has_enough_rounds, unit is null");

	int ur = unit_get_weapon_rounds(u);

	if(ur>=r) {
		return BOOL_TRUE;
	}
	
	return BOOL_FALSE;

}

Boolean unit_is_no_unit_type(int type) {

	if(type > UNIT_TYPE_START && type < UNIT_TYPE_END) {
                return BOOL_FALSE;
        }

        return BOOL_TRUE;

}

Boolean unit_is_using_first_weapon(Unit *u) {

	exit_on_null(u,"Error in unit_is_using_first_weapon, unit is null");

	if(u->aweapon == UNIT_WEAPON_FIRST)
		return BOOL_TRUE;

	return BOOL_FALSE;

}

Boolean unit_is_using_second_weapon(Unit *u) {

	exit_on_null(u, "Error in unit_is_using_second_weapon, unit is null");

	if(u->aweapon == UNIT_WEAPON_SECOND)
		return BOOL_TRUE;

	return BOOL_FALSE;

}

void unit_do_random_move(Unit *u, Game_media *gm) {

	exit_on_null(u, "Error in unit_do_random_move, unit is null");
	exit_on_null(gm, "Error in unit_do_random_move, game media is null");

	gm->mgame->ai.ai_sleep = AI_MOVE_SLEEP;

	Map *m = gm->mmap;

	int r = rand() % 10;
        if(r==0 && map_is_free_position(m,u->posx+1, u->posy)){
		u->posx++;	
	}
        if(r>=1 && r<=6 && map_is_free_position(m,u->posx-1,u->posy)) {
            	u->posx--;
	}
        if(r==7 && map_is_free_position(m,u->posx,u->posy+1)) {
            	u->posy++;
	}
        if(r>=8 && r<=9 && map_is_free_position(m,u->posx,u->posy-1)) {
              	u->posy--;
	}
        u->action_points--;

}

void unit_find_obstacles_around(Unit *u, Map *m, Vector *obs) {

	exit_on_null(u, "Error in unit_find_obstacles_around, unit is null");
	exit_on_null(m, "Error in unit_find_obstacles_around, map is null");
	exit_on_null(obs, "Error in unit_find_obstacles_around, vector of obstacles is null");

	int range = unit_get_weapon_range(u);

	int startx = u->posx - range;
	if(startx<0) startx = 0;

	int starty = u->posy - range;
	if(starty<0) starty = 0;

	int endx = u->posx + range;
	if(endx >= m->sizex) endx = m->sizex - 1;

	int endy = u->posy + range;
	if(endy >= m->sizey) endy = m->sizey - 1;

	int x,y;
	Map_field *mf;
	for(y=starty;y<endy;y++) {
		for(x=startx;x<endx;x++) {
			mf = map_get_field(m,x,y);
			if(mf->type>MAP_TYPE_START && mf->type<MAP_TYPE_END) {
				Obstacle ob;
				vector_push_back(obs, &ob);
			}
		}
	}

}

Boolean unit_cast_ray(Unit *u, int px, int py, Vector *obs) {

	exit_on_null(u,"Error in unit_cast_ray2, unit is null");
	exit_on_null(obs,"Error in unit_cast_ray2, obstacles vector is null");

	Point2D fpos = get_point2D(px,py);
	Point2D upos = get_point2D(u->posx, u->posy);

	double a = atan2(fpos.y-upos.y,fpos.x-upos.x);

	int sz = vector_get_size(obs);
	Obstacle *ob;
	int i;
	for(i=0;i<sz;i++) {
		ob = (Obstacle*)(vector_get_data(obs,i));
		int distf = distance(px,py,u->posx,u->posy);
		int disto = distance(ob->cord.x, ob->cord.y, u->posx, u->posy); 
			
		if(check_a(fpos, upos, a, ob->atanl, ob->atanh, ob->cord.x, ob->cord.y, distf, disto))
			return BOOL_FALSE;
	}	

	return BOOL_TRUE;
}

void unit_fire(Unit *u, Game_media *gm) {

	exit_on_null(u, "Error in unit_fire(), unit is null");
	exit_on_null(gm, "Error in unit_fire(), game media is null");
		
	if(gm->mftunit==NULL) {
		return;
	}
		
	int cap = weapon_get_consumed_action_points(unit_get_actual_weapon(u));
	int rounds = weapon_get_consume_rounds(unit_get_actual_weapon(u));
	if(!unit_has_enough_action_points(u,cap)) {
		terminal_add_msg(gm->mterm,"No enough action points", TERMINAL_MSG_MEDIUM);
		return;
	}
	if(!unit_has_enough_rounds(u,rounds)) {
		terminal_add_msg(gm->mterm,"No more rounds", TERMINAL_MSG_IMPORTANT);
		return;
	}

	u->isfiring = BOOL_TRUE;
	gm->mmissile->isactive = BOOL_TRUE;
		
	gm->mmissile->weapon = unit_get_actual_weapon(u);
	
	weapon_set_init_position_for_missile(gm->mmissile,u,gm->mftunit);

	gm->mmissile->endx = gm->mftunit->posx;
	gm->mmissile->endy = gm->mftunit->posy;

	gm->mmissile->angle = atan2(gm->mmissile->endy-gm->mmissile->posy, gm->mmissile->endx-gm->mmissile->posx);

	gm->mmissile->counter = weapon_get_counter(unit_get_actual_weapon(u));

	unit_reduce_action_points(u,cap);
	unit_reduce_rounds(u);
}

void unit_clear_weapon_range(Unit_weapon_range *uwr) {

	exit_on_null(uwr, "Error in unit_init_weapon_range, unit weapon range is null");

	int i,j;
	for(i=0;i<MAX_WEAPON_RANGE;i++) {
		for(j=0;j<MAX_WEAPON_RANGE;j++) {
			uwr->weapon_range[i][j]=0;
		}	
	}
}

void unit_calc_weapon_range(Game_media *gm, Unit *u) {

	exit_on_null(gm, "Error in unit_calc_weappon_range, game media is null");
	exit_on_null(u, "Error in unit_calc_weapon_range, unit is null");

	int x,y;
	int half = MAX_WEAPON_RANGE/2;
	int range = unit_get_weapon_range(u);
	
	int ux = u->posx-half;
	int uy = u->posy-half;

	Vector obstacles = DEFAULT_VECTOR;

        map_find_obstacles(u,gm->mmap,&obstacles);

	for(y=0;y<MAX_WEAPON_RANGE;y++) {
		for(x=0;x<MAX_WEAPON_RANGE;x++) {
			if(unit_range(u,distance(x,y,half,half),range) && unit_cast_ray(u,x+ux,y+uy,&obstacles)) {
				gm->mweapon_range.weapon_range[y][x] = 1;
			}
			else {
				gm->mweapon_range.weapon_range[y][x] = 0;
			}
		}

	}

}

Boolean unit_is_in_weapon_range(Game_media *gm, Unit *u, Unit *eu) {

	exit_on_null(gm,"Error in unit_is_in_weapon_range, game media is null");
	exit_on_null(u, "Error in unit_is_in_weapon_range, unit is null");
	exit_on_null(eu, "Error in unit_is_in_weapon_range, enemy unit is null");

	int dist = distance(u->posx, u->posy, eu->posx, eu->posy);
        if(dist>unit_get_weapon_range(u))	
		return BOOL_FALSE;

	if(unit_get_weapon_range_char(&(gm->mweapon_range),eu->posx,eu->posy,u->posx,u->posy)) {
		return BOOL_TRUE;
	}
	return BOOL_FALSE;
	
}

char unit_get_weapon_range_char(Unit_weapon_range *wr, int x, int y, int cx, int cy) {

	exit_on_null(wr,"Error in unit_get_weapon_range_char, weapon range is null");

	int wrx = x-cx+MAX_WEAPON_RANGE/2;
        int wry = y-cy+MAX_WEAPON_RANGE/2;

       	if(wrx>=0 && wry>=0 && wrx < MAX_WEAPON_RANGE && wry < MAX_WEAPON_RANGE) {
		return wr->weapon_range[wry][wrx];
	}
	return 0;
}
			
Unit* unit_find_closest_foe_unit(Unit *u, Game_media *gm, Player *foe) {

	exit_on_null(u,"Error in unit_find_closest_foe_unit, unit is null");
	exit_on_null(gm,"Error in unit_find_closest_foe_unit, game media is null");
	exit_on_null(foe,"Error in unit_find_closest_foe_unit, foe player is null");

	int i;
	int dist=10000;
	int d=0;
	Unit *au=NULL;
	Unit *tu;

	for(i=0;i<foe->units.size;i++) {
		tu=(Unit*)(vector_get_data(&(foe->units),i));
		d = distance(u->posx,u->posy,tu->posx,tu->posy);
		if(d<dist){
			dist = d;
			au=tu;
		}
	}

	return au;

}

void unit_reduce_health(Unit *u, int rh) {

	exit_on_null(u,"Error in unit_reduce_health, unit is null");
	
	if(rh <0 || rh > UNIT_MAX_HEALTH_REDUCE) {
		exit_on_error(UERR_HEALTH_REDUCE_OUT_OF_RANGE,"Error in unit_reduce_health, health reduce is out of range");
	}

	u->health -= rh;
}

Color_type unit_get_color_for_health(double val) {

	if(val>0.66)
		return COLOR_GREEN;

	if(val<=0.66 && val >0.33)
		return COLOR_YELLOW;

	return COLOR_RED;

}

void unit_get_health_bar(double val, char *msg) {

	//strcpy(msg," ");
	int health = (int)(10*val);

	int i;
	for(i=0;i<health;i++) {
		strcat(msg," ");
	}
}

void unit_get_team_string(Unit *u, char *team, char t) {

	exit_on_null(u,"Error in unit_get_owner_to_string, unit is null");

	Player *p = u->owner;	
	char *label1 = "(You)";
	char *label2 = "(Foe)";

	char *label;
	if(t==TEAM_PLAYER)
		label = label1;
	else if (t==TEAM_FOE)
		label = label2;
	else 
		exit_on_error(-1,"Error in unit_get_team_string, cant recognize team");

	set_colored_text(COLOR_BLACK,p->pcolor,team,label);
}

void unit_get_position_string(Unit *u, char *pos) {

	exit_on_null(u,"Error in unit_get_position_string, unit is null");

	snprintf(pos,MAXNAME,"x: %d, y: %d",u->posx,u->posy);

}

void unit_get_health_string(Unit *u, char *health) {

	exit_on_null(u,"Error in unit_get_health_string, unit is null");

	double uh = (double)(u->health);
	double nh = (double)(unit_health[unit_adjust_type(u->type)]);
	double ah = uh/nh;
	
	Color_type health_color = unit_get_color_for_health(ah);

	char h[MAXNAME];
	
	snprintf(h,MAXNAME,"%d",u->health);
	unit_get_health_bar(ah,h);
	set_colored_text(COLOR_BLACK, health_color,health,h);
}

void unit_get_foe_health_string(Unit *u, char *health) {

	exit_on_null(u,"Error in unit_get_foe_health_string, unit is null");

	double uh = (double)(u->health);
	double nh = (double)(unit_health[unit_adjust_type(u->type)]);
	double ah = uh/nh;
	
	if(ah<0.5){
		set_colored_text(COLOR_BLACK, COLOR_WHITE,health,"damaged");
	}else {
		set_colored_text(COLOR_BLACK, COLOR_WHITE,health,"looks ok");
	}

}

void unit_get_weapon_string(Unit *u, int w, char *weapon) {

	exit_on_null(u,"Error in unit_get_weapon_string, unit is null");

	Weapon_type wt;

	char ws[INFO_SCREEN_LINE_MAX];
	int rounds;

	if(w == UNIT_WEAPON_FIRST) {
		wt = u->fweapon.wtype;
		rounds = u->fweapon.rounds;
	}else if(w == UNIT_WEAPON_SECOND) {
		wt = u->sweapon.wtype;
		rounds = u->sweapon.rounds;
	}else {
		exit_on_error(-1,"Error in unit_get_weapon_string, no suhc unit weapon");
	}
	
	const char *wname = weapon_name(wt);
	
	char inuse[MAXNAME];
	int wrange = weapon_range(wt);
	int wdamages = weapon_get_consumed_average_hit_points(wt);
	
	if(wt==unit_get_actual_weapon(u)) {
		strcpy(inuse,"(in use)");
	}else {
		strcpy(inuse,"");
	}
	
	snprintf(ws,INFO_SCREEN_LINE_MAX,"%s %s\n Rounds:%d(%d)\n Range: %d\n Damages: %d\n APs: %d\n",wname, inuse, rounds, weapon_get_rounds(wt), wrange, wdamages, weapon_get_consumed_action_points(wt));

	set_colored_text(COLOR_BLACK,COLOR_WHITE,weapon,ws);

}

Boolean unit_has_two_weapons(Unit *u) {

	exit_on_null(u, "Error in unit_has_two_weapons, unit is null");

	if(u->fweapon.wtype != WEAPON_NONE && u->sweapon.wtype != WEAPON_NONE) {
		return BOOL_TRUE;
	}

	return BOOL_FALSE;

}

Boolean unit_is_destroyed(Unit *u) {

	exit_on_null(u, "Error in unit_is_destroyed, unit is null");

	if(u->health <= 0)
		return BOOL_TRUE;

	return BOOL_FALSE;

}
