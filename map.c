#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "includes/types.h"

#include "includes/map.h"
#include "includes/util.h"
#include "includes/colors.h"
#include "includes/unit.h"
#include "includes/cursor.h"
#include "includes/info_screen.h"
#include "includes/vector.h"
#include "includes/weapon.h"
#include "includes/game.h"
#include "includes/ai.h"

//char *map_field_types[] = {"\u22c5", "\u1403", "\u29eb", "\u0AE5"};

char *map_field_types[] = {".", "T", "R", "X"};
char *map_field_names[] = {"", "Tree", "Rock", "Destroyed unit"};
int  map_field_move_cost[] = {0,2,1,0};
int  map_field_defense_bonus[] = {0,3,1,0};
int  map_field_hide_bonus [] = {0,0,2,0};

char *map_field_units[] = {"L", "M", "H", "S"};

char *map_field_missiles[] = {".", "*", "*","*"};

int map_read_keyword(FILE *file, char word[], int wsize) {

        int c;
        int i=0;
        while((c=fgetc(file))!=EOF && c!=':') {
                if(i>=wsize)
                        return KWERR_TO_LONG;
                if(c=='\n')
                        continue;
                word[i++]=c;
        }

        word[i]='\0';

        if(c==EOF)
                return EOF;

        if(c==':') {
                return KEYWORD;
        }

        return KWERR_UNDEFINED;

}

int map_read_params(FILE *file, char word[], int wsize, int *p1, int *p2) {

        int r;
        int counter = 0;

        r = map_read_param(file,word,wsize);

        if(r==KW_EOL)
                return KW_EOL;

        if(r==KWERR_NO_DIGIT)
                return KWERR_NO_DIGIT;

        if(r==KW_PARAM) {
                *p1 = atoi(word);
                counter++;
        }else {
                return KWERR_FIRST_PARAM;
        }

        r = map_read_param(file,word,wsize);
        if(r==KW_END_PARAMS) {
                *p2 = atoi(word);
                 counter++;
        }else {
                return KWERR_SECOND_PARAM;
        }

        if(counter!=2)
                return KWERR_BOTH_PARAMS;

        return KW_READ_PARAMS_OK;

}

int map_read_param(FILE *file, char word[], int wsize) {

        char c;
        int i=0;
        while((c=fgetc(file))!=EOF && c!=',' && c!=';' && c!='\n') {
                if(i>=wsize)
                        return KWERR_TO_LONG;
                if(c==' ')
                        continue;
                if(isdigit(c))
                        word[i++] = c;
                else
                        return KWERR_NO_DIGIT;
        }

        word[i]='\0';

        if(c==EOF)
                return EOF;
        if(c=='\n')
                return KW_EOL;

        if(c==',')
                return KW_PARAM;
        if(c==';')
                return KW_END_PARAMS;

        return 1;


}

int map_load(char *filename, Map *destmap) {

	FILE *fp = fopen(filename, "r");
        if(fp==NULL) {
                return MAPERR_NO_FILE;
        }
        char word[WORDSIZE];
        char kw[WORDSIZE];

        int x,y;
        int r;

        int mtype=0;

        while((r=map_read_keyword(fp,word, WORDSIZE))!=EOF) {
                if(strcmp(word,KW_SIZE)==0) {
                        r = map_read_params(fp,word, WORDSIZE, &x,&y);
                        if(r==KW_READ_PARAMS_OK) {
				int rr;
				rr = map_init(destmap,x,y);
				if(rr!=MAP_OK)
					return rr;					
                        }else {
				return r;
                        }
                }else if((mtype=map_read_gettype(word))!=KWERR_NO_SUCH_KEYWORD) {
			while((r=map_read_params(fp,word,WORDSIZE,&x,&y))!=EOF) {
                                if(r==KW_READ_PARAMS_OK) {
					
					if(map_check_position(destmap,x,y)==BOOL_FALSE)
						exit_on_error(MAPERR_OUT_OF_MAP_SIZE, "Error in map_load, position is out of map boundaries");
					map_set_field(destmap,x,y,mtype);

                                }else if(r==KW_EOL) {
                                        break;
                                }else {
                                        return r;
                                }
                        };
		}else if(r==KWERR_NO_SUCH_KEYWORD) {
			return KWERR_NO_SUCH_KEYWORD;
		}

        }

        return MAP_OK;

}

int map_read_gettype(char *keyword) {

	exit_on_null(keyword,"Error in map_read_gettype, keyword is null");	

	if(strcmp(keyword,KW_TREE)==0) {
		return MAP_TYPE_TREE;
	}else if(strcmp(keyword,KW_ROCK)==0) {
       		return MAP_TYPE_ROCK;
       	}	
	return KWERR_NO_SUCH_KEYWORD;

}

Boolean map_check_position(Map *m, int x, int y) {

	if(x<0 || y<0 || x>=m->sizex || y>=m->sizey)
		return BOOL_FALSE;

	return BOOL_TRUE;

}

Boolean map_is_free_position(Map *m, int x, int y) {

	exit_on_null(m,"Error in map_is_free_position, map is null");
	
	if(!map_check_position(m,x,y))
		return BOOL_FALSE;
	
	Map_field *mf = map_get_field(m,x,y);
	if(mf->type==DEF_FIELD)
		return BOOL_TRUE;
	else return BOOL_FALSE;

}

Map_field* map_get_field(Map *m, int x, int y) {
        
	exit_on_null(m,"Error in map_get_field, map is null");
	
	//if(x<0 || y< 0|| x>=m->sizex || y>=m->sizey)
	//	return NULL;

	return &(m->fields[y*m->sizex+x]);
}

void map_set_field(Map *m, int x, int y, int type) {

	exit_on_null(m,"Error in map_set_field(), map is null");
	if(x<0 || y<0 || x>=m->sizex || y>=m->sizey)
		exit_on_error(MAPERR_OUT_OF_MAP_SIZE, "Error in map_set_field()");

	int pos = y * (m->sizex) + x;
	m->fields[pos].type = type;		

}

int map_init(Map *m, int sx, int sy) {

	if(m == NULL)
		return MAPERR_NULL_PTR;
	if(sx<=0 || sy<=0 || sx<MAP_SCREEN_X || sy<MAP_SCREEN_Y)
		return MAPERR_SIZETOSMALL;

	m->fields = malloc(sx*sy*sizeof(Map_field));

	if(m->fields==NULL) 
		return MAPERR_NULL_PTR;

	int i,j;
	for(i=0;i<sx*sy;i++){
		m->fields[i].type = 0;
		m->fields[i].item = NULL;
	}
	
	m->sizex = sx;
	m->sizey = sy;
	m->offsetx = 0;
	m->offsety = 0;

	return MAP_OK;

}

int map_copy(Map *src, Map *dest) {

        if(src == NULL || dest == NULL)
                return MAPERR_NULL_PTR;

	if(src->sizex!=dest->sizex || src->sizey!=dest->sizey)
		return MAPERR_COPY_WRONG_SIZES;

	int end = src->sizex*src->sizey;
        int i;
        for(i=0;i<end;i++) {
                dest->fields[i].type=src->fields[i].type;
		dest->fields[i].item = NULL;
        }

	//dest->offsetx = src->offsetx;
	//dest->offsety = src->offsety;

        return MAP_OK;
}

char* map_field_for_type(int type) {

	if(type == DEF_FIELD  || type == CURSOR) {
		return map_field_types[0];
	}

	if(type > MAP_TYPE_START && type < MAP_TYPE_END) {
		return map_field_types[map_adjust_type(type)];
	}
	
	if(type > UNIT_TYPE_START && type <=UNIT_TYPE_END) {
		return map_field_units[unit_adjust_type(type)];
	}

	if(type > WEAPON_START && type <= WEAPON_END) {
		return map_field_missiles[weapon_adjust_type(type)];
	}
}

Boolean map_is_field_unit(int type) {
	
	if(type > UNIT_TYPE_START && type <=UNIT_TYPE_END) 
		return BOOL_TRUE;

	return BOOL_FALSE;
}

Color_type map_get_fcolor_for_field(Map_field* field) {
	
	exit_on_null(field, "Error in map_get_fcolor_for_field, field is null");

	int type = field->type;

	Unit *unit = NULL;
	if(map_is_field_unit(type)) {
		unit = (Unit*)(field->item);
		return unit_get_fcolor(unit);
	}else {
		return COLOR_RESET;
	}
}

Color_type map_get_bcolor_for_field(Map_field *field) {

	exit_on_null(field, "Error in map_get_bcolor_for_field, field is null");

	int type = field->type;

	Unit *unit = NULL;
	if(map_is_field_unit(type)){
		unit = (Unit*)(field->item);
		return unit_get_bcolor(unit);
	}else if(weapon_is_weapon_type(type)) {
		return COLOR_RED;
	}else {
		return COLOR_WHITE;
	}
}

Boolean map_is_unit_in_map_view(Unit *u, Map *m) {

	exit_on_null(u, "Error in map_is_unit_in_map_view, unit is null");
	exit_on_null(m, "Error in map_is_unit_in_map_view, map is null");

	if(u->posx>=m->offsetx 
		&& 
		u->posy>=m->offsety 
		&& 
		u->posx<=(m->offsetx+MAP_SCREEN_X) 
		&& 
		u->posy<=(m->offsety+MAP_SCREEN_Y)) {

			return BOOL_TRUE;
	}

	return BOOL_FALSE;

}

void map_print_line(Game_media *gm, Map *m, Cursor *c, Unit *au, int line, Vector *obs) {

	exit_on_null(m,"Error in map_print_line(), map is null");
	exit_on_null(gm, "Error in map_print_line(), game is null");

	Game *g = gm->mgame;

	if(line < 0 || line > m->sizey) 
		exit_on_error(MAPERR_OUT_OF_MAP_SIZE, "Error in map_print_line, line is out of map size");

	Color_type fcolor;
	Color_type bcolor;
	char *ftype;
	Map_field* field;	
	
	char toprint[MAXNAME];

	int j;
	int offsetj, offsetline;
	for(j=0;j<MAP_SCREEN_X;j++) {

		fcolor = COLOR_BLACK;
		bcolor = COLOR_WHITE;
		
		offsetj = j + m->offsetx;
		offsetline = line + m->offsety;

		field = map_get_field(m,offsetj,offsetline);
		ftype = map_field_for_type(field->type);

		fcolor = map_get_fcolor_for_field(field);		

		if(au != NULL && offsetj == au->posx && offsetline == au->posy){
			fcolor = COLOR_MAGENTA;
		}

		//cursor - green      
		if(weapon_is_no_weapon_type(field->type) && c != NULL && line==c->y && j==c->x) {
			bcolor = COLOR_GREEN;
		}else if(weapon_is_no_weapon_type(field->type) 
			 && 
			 unit_is_no_unit_type(field->type)
			 &&
			 !ai_is_ai_turn(&(gm->mgame->ai)) && au!=NULL && game_is_unit_friend(g,au)
			 &&
			 unit_roughly_range(au,offsetj,offsetline,au->posx,au->posy,MAX_WEAPON_RANGE/2) 
			 &&
			 unit_get_weapon_range_char(&(gm->mweapon_range),offsetj,offsetline,au->posx,au->posy)) {

					bcolor = COLOR_LIGHT_GRAY; 
			
		}else {
			bcolor = map_get_bcolor_for_field(field);
		}

		set_colored_text(fcolor,bcolor,toprint,ftype);
 		printf("%s",toprint);
	}

}

void map_print(Game_media *g, Map *m, Cursor *c, Unit *au, Info_screen* is) {

	exit_on_null(m, "Error in map_print(), map is null");
	exit_on_null(g, "Error in map_print(), game is null");
	//exit_on_null(c, "Error in map_print(), cursor is null");

	int i;

	char is_line[INFO_SCREEN_LINE_MAX];	
	is_line[0]='\0';

	Vector obstacles = DEFAULT_VECTOR;
	
	map_find_obstacles(au,m,&obstacles);	

	for(i=0;i<MAP_SCREEN_Y;i++) {
		map_print_line(g,m,c,au,i,&obstacles);
		if(is->length>0) {
			info_screen_get_line(is,is_line);
		}

		printf("%s", is_line);		
		printf("\n");
	}

}

void map_check_offsetx(Map *m) {

	exit_on_null(m,"Error in map_calc_offsetx, map is null");

	if(m->offsetx<0) {
		m->offsetx=0;
		return;
	}
	if(m->offsetx>m->sizex-MAP_SCREEN_X) {
		m->offsetx=m->sizex-MAP_SCREEN_X;
		return;
	}

}

void map_check_offsety(Map *m) {

        exit_on_null(m,"Error in map_calc_offsety, map is null");

        if(m->offsety<0) {
                m->offsety=0;
                return;
        }
        if(m->offsety>m->sizey-MAP_SCREEN_Y) {
                m->offsety=m->sizey-MAP_SCREEN_Y;
                return;
        }

}

void map_msg_to_infoscreen(Map_field *mf, Info_screen *is) {

	exit_on_null(mf,"Error in map_msg_infoscreen, map_field is null");
	exit_on_null(is,"Error in map_msg_infoscreen, info_screen is null");

	char str[DWORD_SIZE];

	map_field_to_string(mf,str,DWORD_SIZE);
	info_screen_set_msg(is,str);

}

void map_field_to_string(Map_field *mf, char *str, int strl) {

	exit_on_null(mf,"Error in map_field_to_string, map_field is null");
	exit_on_null(str,"Error in map_field_to_string, str is null");

	int p = map_adjust_type(mf->type);

	sprintf(str,"%s\nMove cost: %d\nDefense bonus: %d\nHide bonus: %d", map_field_names[p], map_field_move_cost[p], map_field_defense_bonus[p], map_field_hide_bonus[p]);
		
}

int map_adjust_type(Map_type type) {
	return type-MAP_TYPE_ADJUST;
}

void map_find_obstacles_around(Unit *u, Map *m, Vector *obs) {

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
				ob.cord.x = x;
				ob.cord.y = y;
				vector_push_back(obs, &ob);
			}
		}
	}

}

void map_find_obstacles(Unit *u, Map *m, Vector *obs) {

	exit_on_null(m,"Error in map_find_obstacles, map is null");
	exit_on_null(obs,"Error in map_find_obstacles, obstacles vector is null");

	if(u!=NULL) {
		vector_init(obs,VDEFSIZE,sizeof(Obstacle));

		map_find_obstacles_around(u,m,obs);
		
		int sz = vector_get_size(obs);
		if(sz == VERR_NULL_VECTOR)
			exit_on_error(sz,"Error map_find_obstacles, with obstacles vector");
		map_calc_obstacles(u,obs);
	}

}

void map_calc_obstacles(Unit *u, Vector *obs) {

	exit_on_null(u,"Error in map_calc_obstacles, unit is null");
	exit_on_null(obs,"Error in map_calc_obstacles, obstacles vector is null");

	int sz;	
	sz = vector_get_size(obs);

	Obstacle *o;
	double a1,a2,a3,a4;
	Point2D middle = get_point2D(u->posx, u->posy);

	int i;
	for(i=0;i<sz;i++) {
		o = (Obstacle*)(vector_get_data(obs,i));
		a1 = atan2(o->cord.y-middle.y,o->cord.x- middle.x);
		a2 = atan2(o->cord.y+1.0-middle.y, o->cord.x-middle.x);
		a3 = atan2(o->cord.y-middle.y, o->cord.x+1.0-middle.x);
		a4 = atan2(o->cord.y+1.0-middle.y,o->cord.x+1.0-middle.x);
		o->atanl = MIN(a4,MIN(a3,MIN(a2,a1)));
		o->atanh = MAX(a4,MAX(a3,MAX(a2,a1)));
	}	
}

void map_free(Map *m) {

	exit_on_null(m, "Error in map_free(), map is null");

	free(m->fields);

}
