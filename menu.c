#include <stdio.h>
#include <string.h>

#include "includes/types.h"

#include "includes/menu.h"
#include "includes/vector.h"
#include "includes/util.h"
#include "includes/game.h"
#include "includes/unit.h"
#include "includes/keys.h"
#include "includes/info_screen.h"
#include "includes/cursor.h"
#include "includes/map.h"
#include "includes/player.h"
#include "includes/terminal.h"

void menu_init(Menu *men, Menu_type mt) {

	exit_on_error(vector_init(&men->options, VDEFSIZE, sizeof(Menu_option)),"Error in menu init, during init vector of menu options");

	men->type = mt;

}

void menu_print_option(Menu_option *opt) {

	exit_on_null(opt,"Error in menu_print_option, menu_option is null");
	
	if(opt->isvisible)
		printf("(%c)->%s ",opt->key, opt->name);
}

void menu_print(Menu *men) {

	exit_on_null(men,"Error in menu_print, menu is null");

	printf("%s\n",men->description);

	int i;
	Menu_option *opt=NULL;
	for(i=0;i<men->options.size;i++) {
		opt = vector_get_data(&men->options,i);
		exit_on_null(opt,"Error in menu_print, option is null");
		//if(i>0 && opt->isvisible) {
		//	printf(", ");
		//}
		menu_print_option(opt);
		//if(i!=men->options.size-1) {
		//	printf(", ");
		//}
	}

	printf("\n");

}

void menu_option_init(Menu_option *mo, char *name, char key, Menu_option_type mot) {

	exit_on_null(mo,"Error in menu_option_init, menu_option is null");
	exit_on_null(name,"Error in menu_option_init, name is null");

	strcpy(mo->name,name);
	mo->key = key;
	mo->next_menu = MENU_NO_TYPE;
	mo->type = mot;
	menu_option_setvisible(mo);
}

Menu_option* menu_find_option_by_key(Menu *m, char key, int nohide) {

	exit_on_null(m,"Error in menu_find_option_by_key, menu is null");

	int i;
	Menu_option *mo;

	for(i=0;i<m->options.size;i++) {

		mo = (Menu_option*)vector_get_data(&m->options,i);
		if(nohide) {
			if(mo->isvisible && mo->key == key)
				return mo;
		}else if(mo->key == key){
			return mo;
		}
	}

	return NULL;

}

Menu_option* menu_find_option_by_name(Menu *m, char *name, int nohide) {

        exit_on_null(m,"Error in menu_find_option_by_name, menu is null");
	exit_on_null(name, "Error in menu_find_option_by_name, name is null");

        int i;
        Menu_option *mo;

        for(i=0;i<m->options.size;i++) {

                mo = (Menu_option*)vector_get_data(&m->options,i);
		if(nohide) {
			if(mo->isvisible && strcmp(name,mo->name)==0)
				return mo;
		}else if(strcmp(name,mo->name)==0){
			return mo;
		}
        }

        return NULL;

}

Menu_option* menu_find_option_by_type(Menu *m, Menu_option_type mot, int nohide) {
   
	exit_on_null(m,"Error in menu_find_option_by_type, menu is null");

        int i;
        Menu_option *mo;

        for(i=0;i<m->options.size;i++) {

                mo = (Menu_option*)vector_get_data(&m->options,i);
		if(nohide) {
			if(mo->isvisible && mo->type == mot)
				return mo;
		}else if(mo->type == mot){
			return mo;
		}
        }

        return NULL;
}

int menu_add_option(Menu *men, Menu_option *opt) {

	exit_on_error(vector_push_back(&men->options, opt), "Error menu_add_option during adding option to menu");

}

int menu_is_type(Menu *m, Menu_type type) {

	exit_on_null(m,"Error in menu_is_type, menu is null");

	if(m->type==type)
		return 1;
	else 
		return 0;

}

void menu_option_set_name(Menu_option *opt, const char *name) {

	exit_on_null(opt,"Error in menu_option_set_name, option is null");
	exit_on_null(name,"Error in menu_option_set_name, name is null");

	strcpy(opt->name,name);

}

void menu_manager_init(Menu_manager *mm) {

	exit_on_null(mm,"Error in menu_manager_init, menu_manager is null");
	
	mm->menu_map.options.data=NULL;
	mm->menu_game.options.data=NULL;
	mm->menu_unit.options.data=NULL;
	mm->menu_firing.options.data=NULL;
	
	menu_manager_create_menu_map(&mm->menu_map);
	menu_manager_create_menu_game(&mm->menu_game);
	menu_manager_create_menu_unit(&mm->menu_unit);	
	menu_manager_create_menu_firing(&mm->menu_firing);

	mm->actual_menu=&(mm->menu_game);
	mm->actual_unit = NULL;
}

void menu_manager_create_menu_game(Menu *mgame) {

        exit_on_null(mgame,"Error in menu_manager_create_menu_game, menu game is null");

        menu_init(mgame,MENU_GAME);

        mgame->previous = NULL;

        Menu_option next;
        Menu_option end;
	Menu_option map_menu;

	menu_option_init(&map_menu,"map menu", KEY_GOTO_MENU, MENU_OPTION_MAP_MENU);
	map_menu.next_menu = MENU_MAP;
	map_menu.action=&menu_manager_actual_menu;

        menu_option_init(&next,"next turn",KEY_NEXT_ROUND, MENU_OPTION_NEXT_TURN);
        next.action=&game_next_round;

        menu_option_init(&end,"end game",KEY_END_GAME, MENU_OPTION_END_GAME);
	end.action=&game_end_game;

	menu_add_option(mgame,&map_menu);
	menu_add_option(mgame,&next);
        menu_add_option(mgame,&end);

	strcpy(mgame->description,"Main menu");

}

void menu_manager_create_menu_map(Menu *mmap) {

	exit_on_null(mmap,"Error in menu_manager_create_menu_map, menu map is null");

	menu_init(mmap,MENU_MAP);

        mmap->previous = NULL;

        Menu_option map_up;
        Menu_option map_down;
	Menu_option map_left;
	Menu_option map_right;
	Menu_option game_menu;
	Menu_option unit_menu;

	menu_option_init(&game_menu,"main menu", KEY_GOTO_MENU, MENU_OPTION_MAIN_MENU);
	game_menu.next_menu = MENU_GAME;
	game_menu.action=&menu_manager_actual_menu;   

	menu_option_init(&unit_menu,"unit menu", KEY_MENU_UNIT_ACTIONS, MENU_OPTION_UNIT_MENU);
	unit_menu.next_menu = MENU_UNIT;
	unit_menu.action=&menu_manager_actual_menu;
	menu_option_hide(&unit_menu);

        menu_option_init(&map_up,"up",KEY_MAP_UP, MENU_OPTION_UP);
        map_up.action=&menu_helper_cursor_action;

        menu_option_init(&map_down,"down",KEY_MAP_DOWN, MENU_OPTION_DOWN);
        map_down.action=&menu_helper_cursor_action;

        menu_option_init(&map_left,"left",KEY_MAP_LEFT, MENU_OPTION_LEFT);
        map_left.action=&menu_helper_cursor_action;

        menu_option_init(&map_right,"right",KEY_MAP_RIGHT, MENU_OPTION_RIGHT);
        map_right.action=&menu_helper_cursor_action;

	menu_add_option(mmap,&game_menu);
	menu_add_option(mmap,&unit_menu);

	menu_add_option(mmap,&map_up);
        menu_add_option(mmap,&map_down);
	menu_add_option(mmap,&map_left);
	menu_add_option(mmap,&map_right);
		
	strcpy(mmap->description,"Map menu, use keys to navigate");

}

void menu_manager_create_menu_unit(Menu *munit) {

        exit_on_null(munit,"Error in menu_manager_create_menu_unit, menu unit is nulll");

        menu_init(munit,MENU_UNIT);

        Menu_option moveup;
        Menu_option movedown;
        Menu_option moveleft;
        Menu_option moveright;
	Menu_option map_menu;
	Menu_option change_weapon;

	Menu_option firing;

	menu_option_init(&map_menu, "map menu", KEY_GOTO_MENU, MENU_OPTION_MAP_MENU);
	map_menu.next_menu = MENU_MAP;
	map_menu.action = &menu_manager_actual_menu;

	menu_option_init(&firing, "firing menu", KEY_UNIT_FIRING, MENU_OPTION_FIRING_MENU);
	firing.next_menu = MENU_FIRING;
	firing.action = &menu_manager_actual_menu;

        menu_option_init(&moveup, "moveup", KEY_UNIT_MOVE_UP, MENU_OPTION_MOVEUP);
        moveup.action = &unit_move_in_direction;
        menu_option_init(&movedown,"movedown", KEY_UNIT_MOVE_DOWN, MENU_OPTION_MOVEDOWN);
        movedown.action = &unit_move_in_direction;
        menu_option_init(&moveleft,"moveleft", KEY_UNIT_MOVE_LEFT, MENU_OPTION_MOVELEFT);
        moveleft.action = &unit_move_in_direction;
        menu_option_init(&moveright,"moveright",KEY_UNIT_MOVE_RIGHT, MENU_OPTION_MOVERIGHT);
        moveright.action = &unit_move_in_direction;

	menu_option_init(&change_weapon, "change weapon", KEY_UNIT_CHANGE_WEAPON, MENU_OPTION_CHANGE_WEAPON);		
	change_weapon.action = &menu_helper_change_weapon;	
	menu_option_hide(&change_weapon);

	menu_add_option(munit,&map_menu);
	menu_add_option(munit,&firing);

	menu_add_option(munit,&moveup);
        menu_add_option(munit,&movedown);
        menu_add_option(munit,&moveleft);
        menu_add_option(munit,&moveright);

	menu_add_option(munit,&change_weapon);
	
	strcpy(munit->description,"Unit menu, use keys to move unit");

}

void menu_manager_create_menu_firing(Menu *mfiring) {

	exit_on_null(mfiring, "Error in menu_manager_create_menu_firing, menu firing is null");

	menu_init(mfiring, MENU_FIRING);
	
	Menu_option unit_menu;
	Menu_option moveleft;
	Menu_option moveright;
	Menu_option moveup;
	Menu_option movedown;
	Menu_option fire;
	
	menu_option_init(&unit_menu, "unit menu", KEY_GOTO_MENU, MENU_OPTION_UNIT_MENU);
	unit_menu.next_menu = MENU_UNIT;
	unit_menu.action = &menu_manager_actual_menu;
	
	menu_option_init(&moveleft, "moveleft", KEY_UNIT_MOVE_LEFT, MENU_OPTION_MOVELEFT);
	moveleft.action = &menu_helper_firing_menu;

	menu_option_init(&moveright, "moveright", KEY_UNIT_MOVE_RIGHT, MENU_OPTION_MOVERIGHT);
	moveright.action = &menu_helper_firing_menu;

	menu_option_init(&moveup, "moveup", KEY_UNIT_MOVE_UP, MENU_OPTION_MOVEUP);
	moveup.action = &menu_helper_firing_menu;

	menu_option_init(&movedown, "movedown", KEY_UNIT_MOVE_DOWN, MENU_OPTION_MOVEDOWN);
	movedown.action = &menu_helper_firing_menu;

	menu_option_init(&fire, "fire", KEY_UNIT_FIRING, MENU_OPTION_FIRE);
	fire.action = &unit_fire;
	menu_option_hide(&fire);

	menu_add_option(mfiring, &unit_menu);

	menu_add_option(mfiring, &moveleft);
	menu_add_option(mfiring, &moveright);
	menu_add_option(mfiring, &moveup);
	menu_add_option(mfiring, &movedown);
	menu_add_option(mfiring, &fire);

	strcpy(mfiring->description,"Firing menu, use keys to aime enemy unit");

}

void menu_manager_actual_menu(Menu_manager *mm, Menu_type mt, Game_media *media) {

        if(mt==MENU_GAME) {
                mm->actual_menu = &(mm->menu_game);
        }else if(mt==MENU_UNIT) {
                mm->actual_menu = &(mm->menu_unit);
		mm->actual_unit->isaiming = BOOL_FALSE;
		menu_check_for_unit_weapon_options(mm);
        }else if(mt==MENU_MAP){
		mm->actual_menu = &(mm->menu_map);		
	}else if(mt==MENU_FIRING){
		mm->actual_menu = &(mm->menu_firing);
		mm->actual_unit->isaiming = BOOL_TRUE;
	
	}else{
                exit_on_error(MMERR_NO_SUCH_MENU,"Error in menu_manager_set_actual_menu, no such menu");
        }

	if(mm->actual_unit != NULL)
		unit_msg_to_infoscreen(mm->actual_unit,media);
}

void menu_check_for_unit_weapon_options(Menu_manager *mm) {

	exit_on_null(mm,"Error in menu_check_for_unit_weapon_options, menu manager is null");

	if(unit_has_two_weapons(mm->actual_unit)) {
		menu_show_unit_weapon_options(mm);
	}else {
		menu_hide_unit_weapon_options(mm);
	}
}

void menu_show_unit_weapon_options(Menu_manager *mm) {

	exit_on_null(mm,"Error in menu_show_unit_weapon_options, menu manager is null");
	
	Menu_option *opt = menu_find_option_by_type(mm->actual_menu, MENU_OPTION_CHANGE_WEAPON, BOOL_FALSE);
	if(opt==NULL){
		exit_on_error(-1, "Error in menu_show_unit_weapon_options, no first weapon option menu");	
	}

	Unit *au = mm->actual_unit;
	Weapon_type wt = unit_is_using_first_weapon(au) ? au->sweapon.wtype : au->fweapon.wtype;
	const char *wname = weapon_name(wt);
	char msg[MAXLINE];
	strcpy(msg,"change to ");
	strcat(msg,wname);
	menu_option_set_name(opt,msg);
	
	menu_option_setvisible(opt);
}

void menu_hide_unit_weapon_options(Menu_manager *mm) {
	
	exit_on_null(mm,"Error in menu_hide_unit_weapon_options, menu manager is null");

	Menu_option *opt = menu_find_option_by_type(mm->actual_menu, MENU_OPTION_CHANGE_WEAPON, BOOL_FALSE);
	if(opt==NULL){
		exit_on_error(-1, "Error in menu_show_unit_weapon_options, no change weapon option menu");	
	}
	menu_option_hide(opt);
	
}

void menu_helper_cursor_action(Game_media *gm, Menu_manager *mm, char c) {

	Map_field *afield = cursor_action_on_char(gm->mcursor, gm->mmap, c);
	if(afield!=NULL) {
        	int t = afield->type;
                if(t>UNIT_TYPE_START && t<UNIT_TYPE_END) {
			
			Unit *u = (Unit*)(afield->item);
                	unit_msg_to_infoscreen(u,gm);
			if(game_is_unit_friend(gm->mgame,u)) {
				mm->actual_unit = u;
				unit_calc_weapon_range(gm,u);
				Menu_option *opt = menu_find_option_by_name(mm->actual_menu,"unit menu",BOOL_FALSE);
				menu_option_setvisible(opt);
			}

              	}else if(t>MAP_TYPE_START && t<MAP_TYPE_END) {
                        map_msg_to_infoscreen(afield,gm->minfo);
			mm->actual_unit = NULL;
                }else {
                        Menu_option *opt = menu_find_option_by_name(mm->actual_menu,"unit menu",BOOL_FALSE);
                        menu_option_hide(opt);
			mm->actual_unit = NULL;
                }
        }
}

void menu_helper_change_weapon(Unit *u, Game_media *m, char c, Menu_manager *mm) {

	if(c==KEY_UNIT_CHANGE_WEAPON && unit_has_two_weapons(u)) {
		if(unit_is_using_first_weapon(u)) {
			unit_use_second_weapon(u,m);
		}else {
			unit_use_first_weapon(u,m);
		}
		menu_show_unit_weapon_options(mm);
	}
}

void menu_helper_firing_menu(Unit *u, Game_media *gm, char c, Menu_manager *mm) {

	exit_on_null(u, "Error in menu_helper_firing_menu, unit is null");
	exit_on_null(gm, "Error in menu_helper_firing_menu, game media is null");
	exit_on_null(mm, "Error in menu_helper_firing_menu, menu manager is null");

	Unit *eu = NULL;

	Map_field *afield = cursor_action_on_char(gm->mcursor, gm->mmap,c);
	if(afield!=NULL) {
		int t = afield->type;
		if(t>UNIT_TYPE_START && t<UNIT_TYPE_END) {
			
			eu = (Unit*)(afield->item);
			Player *pu = u->owner;
			Player *peu = eu->owner;
			if(!player_is_same(pu,peu)) {
				if(unit_is_in_weapon_range(gm,u,eu) && !unit_is_destroyed(eu)){ 
					Menu_option *opt = menu_find_option_by_name(mm->actual_menu,"fire",BOOL_FALSE);
					menu_option_setvisible(opt);
					gm->mftunit = eu;
				}else {
					char msg[]="Unit out of range";
					terminal_add_msg(gm->mterm,msg,TERMINAL_MSG_NORMAL);
				}
			}
              	}else {
                        Menu_option *opt = menu_find_option_by_name(mm->actual_menu,"fire",BOOL_FALSE);
                        menu_option_hide(opt);
			gm->mftunit = NULL;
                }
	
		unit_msg_to_infoscreen(u,gm);
		unit_aimed_unit_to_infoscreen(eu,gm);
					
	}
}

void menu_option_setvisible(Menu_option *mo) {

	exit_on_null(mo,"Error in menu_option_setvisible, menu_option is null");
	
	mo->isvisible = BOOL_TRUE;

}

void menu_option_hide(Menu_option *mo) {

	exit_on_null(mo,"Error in menu_option_hide, menu_option is null");

	mo->isvisible = BOOL_FALSE;

}
