#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include "includes/util.h"
#include "includes/types.h"
#include "includes/map.h"
#include "includes/terminal.h"
#include "includes/weapon.h"
#include "includes/ai.h"
#include "includes/unit.h"
#include "includes/info_screen.h"

//Must be the same number as Weapon_type
const char *weapon_names[WEAPONS_SIZE] = {
        "Gun",
        "Laser",
        "Rockets",
	"Big rockets"
};

const int weapon_ranges[WEAPONS_SIZE] = {
  	8,
        6,
        12,
	14
};


const int weapon_consumed_action_points[WEAPONS_SIZE] = {
	2,
	2,
	3,
	3,
};

const int weapon_rounds[WEAPONS_SIZE] = {
	100,
	50,
	32,
	16
};

const int weapon_consumed_average_hit_points[WEAPONS_SIZE] = {
	1,
	10,
	8,
	18

};

const int weapon_consume_rounds[WEAPONS_SIZE] = {

	2,
	2,
	1,
	1
};

const int weapon_fire_sleep[WEAPONS_SIZE] = {
	50000,
	50000,
	50000,
	50000

};

const int weapon_counter[WEAPONS_SIZE] = {

	6,
	4,
	1,
	1

};

int weapon_missile_get_posx(Missile *m) {

	exit_on_null(m,"Error in weapon_get_posx(), missile is null");

	return round(m->posx);	

}

int weapon_missile_get_posy(Missile *m) {

	exit_on_null(m,"Error in weapon_get_posy(), missile is null");

	return round(m->posy);

}

void weapon_missile_count_fire(Unit *u, Unit *f, Game_media *gm, Missile *m) {

	exit_on_null(u, "Error in weapon_count_fire(), unit is null");
	exit_on_null(gm, "Error in weapon_count_fire(), game_media is null");
        exit_on_null(m, "Error in weapon_count_fire(), missile is null");

        if(m->isactive) {

                if(!weapon_missile_check_end(u,m)) {
			weapon_calc_damages(u,f, gm, m);

			if(!ai_is_ai_turn(&(gm->mgame->ai))) {
				unit_msg_to_infoscreen(u,gm);
				unit_aimed_unit_to_infoscreen(f,gm);
			}

			if(ai_is_ai_turn(&(gm->mgame->ai))) {
				unit_use_first_weapon(u,gm);
			}
                        
			return;
		}
                weapon_missile_update_position(m,u);
        }
}

Boolean weapon_missile_check_end(Unit *u, Missile *m) {

        exit_on_null(u, "Erro in weapon_check_missile_end(), unit is null");
        exit_on_null(m, "Error in weapon_check_missile_end(), missile is null");

	Weapon_type wt = unit_get_actual_weapon(u);

	if(weapon_check_end_machinegun(wt,m) || weapon_check_end_rocket(wt,m) || weapon_check_end_laser(wt,m)){
		weapon_set_end_missile(u,m);
		return BOOL_FALSE;
	}

	        

	return BOOL_TRUE;
	
}

Boolean weapon_check_end_machinegun(Weapon_type wt, Missile *m) {

	exit_on_null(m,"Error in weapon_chedk_end_rocket, missile is null");

	if(wt==WEAPON_MACHINEGUN && m->counter<0)
		return BOOL_TRUE;

	return BOOL_FALSE;

}

Boolean weapon_check_end_rocket(Weapon_type wt, Missile *m) {

	exit_on_null(m,"Error in weapon_check_end_rocket, missile is null");

	if((wt == WEAPON_ROCKET || wt == WEAPON_BIG_ROCKET) && abs(weapon_missile_get_posx(m)-m->endx)<TOLERANCE && abs(weapon_missile_get_posy(m)-m->endy)<TOLERANCE) {
        	return BOOL_TRUE;
	}

	return BOOL_FALSE; 

}

Boolean weapon_check_end_laser(Weapon_type wt, Missile *m) {

	exit_on_null(m,"Error in weapon_check_end_laser, missile is null");

	if(wt == WEAPON_LASER && m->counter<0)
		return BOOL_TRUE;

	return BOOL_FALSE;

}

void weapon_post_laser_in_map(Missile *m, Map *map) {

	exit_on_null(m,"Error in weapon_post_laser_in_map, missile is null");
	exit_on_null(map, "Error in weapon_post_laser_in_map, map is null");

	int i;
	double x = m->posx;
	double y = m->posy;
	
	int dist = distance(m->posx, m->posy, m->endx, m->endy);

	for(i=0;i<dist;i++) {
		x += WEAPON_UNIT_SIZE*cos(m->angle);
        	y += WEAPON_UNIT_SIZE*sin(m->angle);
		map_set_field(map,(int)x,(int)y,WEAPON_LASER);
	}

}

Boolean weapon_is_weapon_type(int type) {

	if(type >= WEAPON_START && type <= WEAPON_END) {
		return BOOL_TRUE;
	}

	return BOOL_FALSE;

}

Boolean weapon_is_no_weapon_type(int type) {

	if(type >= WEAPON_START && type <= WEAPON_END)
		return BOOL_FALSE;

	return BOOL_TRUE;

}

void weapon_set_end_missile(Unit *u, Missile *m){

	exit_on_null(u,"Error in weapon_set_end_missile, unit is null");
	exit_on_null(m,"Error in weapon_set_end_missile, missile is null");

 	m->isactive = BOOL_FALSE;
        u->isfiring = BOOL_FALSE;

}

void weapon_set_init_position_for_missile(Missile *m, Unit *u, Unit *foe) {

	exit_on_null(m,"Error in weapon_set_init_position_for_missile, missile is null");
	exit_on_null(u,"Error in weapon_set_init_position_for_missile, unit is null");
	exit_on_null(foe,"Error in weapon_set_init_position_for_missile, foe unit is null");

	if(m->weapon==WEAPON_MACHINEGUN) {
		m->posx = foe->posx;
	       	m->posy = foe->posy;
	}else {
		m->posx = u->posx;
        	m->posy = u->posy;
	}
}

void weapon_missile_update_position(Missile *m, Unit *u){
 
        exit_on_null(m, "Error in weapon_update_missile_position(), missile is null"); 
	exit_on_null(u, "Error in weapon_update_missile_position(), unit is null");         

	Weapon_type wt = unit_get_actual_weapon(u);

	if(wt == WEAPON_MACHINEGUN) {
		weapon_update_position_machinegun(m);
	}else if(wt == WEAPON_LASER) {
		weapon_update_position_laser(m);
	}else if(wt == WEAPON_ROCKET || wt == WEAPON_BIG_ROCKET) {
		weapon_update_position_rocket(m);
        }
 
        usleep(weapon_get_fire_sleep(wt)); 
         
}

void weapon_update_position_machinegun(Missile *m) {

	exit_on_null(m,"Error in weapon_update_position_machinegun, missile is null");
	
	m->counter--;

}

void weapon_update_position_rocket(Missile *m) {

	exit_on_null(m,"Error in weapon_update_position_rocket, missile is null");

	m->posx += WEAPON_UNIT_SIZE*cos(m->angle); 
       	m->posy += WEAPON_UNIT_SIZE*sin(m->angle);  

}

void weapon_update_position_laser(Missile *m) {

	exit_on_null(m,"Error in weapon_update_position_laser, missile is null");

	m->counter--;

}

int weapon_get_damages_for_weapon(Weapon_type wt) {

	if(wt == WEAPON_MACHINEGUN) {
		return weapon_get_damages_for_machinegun();
	}else if(wt == WEAPON_LASER) {
		return weapon_get_damages_for_laser();
	}else if(wt == WEAPON_ROCKET) {
		return weapon_get_damages_for_rocket();
	}else if(wt == WEAPON_BIG_ROCKET) {
		return weapon_get_damages_for_big_rocket();
	}

	exit_on_error(-1,"Error in weapon_get_damages_for_weapon, no such weapon");
}

int weapon_get_damages_for_machinegun() {

	int d = weapon_get_consumed_average_hit_points(WEAPON_MACHINEGUN);
	int c = weapon_get_counter(WEAPON_MACHINEGUN);

	int damages=0;
	int i;

	for(i=0;i<c;i++) {
		if(rand()%2==0)
		damages +=d;
	}
	
	return damages;

}

int weapon_get_damages_for_laser() {

        int d = weapon_get_consumed_average_hit_points(WEAPON_LASER);
        int c = weapon_get_counter(WEAPON_LASER);

        int damages=0;

	if(rand()%5>0)
		damages = rand()%d+d/2;
        int i;

        for(i=0;i<c;i++) {
		int p = rand()%10;
		if(p>3) {
                	damages += rand()%d;
		}
        }

        return damages;

}

int weapon_get_damages_for_rocket() {

	int d= weapon_get_consumed_average_hit_points(WEAPON_ROCKET);
	int p = rand()%10;

	if(p>2)
		return rand()%d+d;
	else 
		return 0;

}

int weapon_get_damages_for_big_rocket() {

	int d = weapon_get_consumed_average_hit_points(WEAPON_BIG_ROCKET);
	int p = rand()%10;

	if(p%2==0)
		return rand()%d+d;
	else 
		return 0;
	

}

void weapon_post_missile(Missile *missile, Map *map) {

        exit_on_null(missile,"Error in map_post_missile, missile is null");
        exit_on_null(map,"Error in map_post_missile, map is null");

	Weapon_type weapon = missile->weapon;
	int counter = missile->counter;

        if(missile->isactive) {
		if(weapon==WEAPON_MACHINEGUN && counter%2==0) {
			map_set_field(map,missile->posx,missile->posy,WEAPON_MACHINEGUN);
			return;
		}
		if(weapon==WEAPON_LASER && counter%2==0) {
			weapon_post_laser_in_map(missile,map);
			return;
		}
		if(missile->weapon==WEAPON_ROCKET) {
               		map_set_field(map,missile->posx,missile->posy,WEAPON_ROCKET);
        		return;
		}
		if(missile->weapon==WEAPON_BIG_ROCKET) {
               		map_set_field(map,missile->posx,missile->posy,WEAPON_BIG_ROCKET);
        		return;
		}
	}

}

void weapon_calc_damages(Unit *u, Unit *f, Game_media *gm, Missile *m) {

	exit_on_null(u, "Error in weapon_calc_damages, unit is null");
	exit_on_null(f, "Error in weapon_calc_damages, foe unit is null");
	exit_on_null(gm, "Error in weapon_calc_damages, game_media is null");
	exit_on_null(m, "Error in weapon_calc_damages, missile is null");

	int d = weapon_get_damages_for_weapon(m->weapon);

	char damages[TERMINAL_LINE_LENGTH];

	Terminal_msg_type tmt = TERMINAL_MSG_NORMAL;

	if(ai_is_ai_turn(&(gm->mgame->ai))) {
		if(d==0) {
			sprintf(damages,"Enemy missed");
		}else {
			sprintf(damages, "Your unit was hit by %d hit points", d);
			tmt = TERMINAL_MSG_IMPORTANT;
		}
	}else {
		if(d==0) {
			sprintf(damages, "You missed");
			tmt = TERMINAL_MSG_MEDIUM;
		}else {
			sprintf(damages, "You hit enemy unit by %d hit points", d);
			tmt = TERMINAL_MSG_GOOD_NEWS;
		}
	}	
	
	if(d!=0){
		unit_reduce_health(f,d);
	}	

	terminal_add_msg(gm->mterm, damages, tmt);
	if(unit_is_destroyed(f)) {
		gm->mftunit = NULL;
	}
}

int weapon_range(Weapon_type wt) {

        return weapon_ranges[weapon_adjust_type(wt)];

}

const char *weapon_name(Weapon_type wt) {

        return weapon_names[weapon_adjust_type(wt)];

}

int weapon_get_consumed_action_points(Weapon_type wt) {

	return weapon_consumed_action_points[weapon_adjust_type(wt)];

}

int weapon_get_rounds(Weapon_type wt) {

	return weapon_rounds[weapon_adjust_type(wt)];
}

int weapon_get_consumed_average_hit_points(Weapon_type wt) {

	return weapon_consumed_average_hit_points[weapon_adjust_type(wt)];

}

int weapon_get_consume_rounds(Weapon_type wt) {

	return weapon_consume_rounds[weapon_adjust_type(wt)];

}

int weapon_get_fire_sleep(Weapon_type wt) {

	return weapon_fire_sleep[weapon_adjust_type(wt)];

}

int weapon_get_counter(Weapon_type wt) {

	return weapon_counter[weapon_adjust_type(wt)];	

}

int weapon_adjust_type(Weapon_type type) {

	return type - WEAPON_TYPE_ADJUST;

}
