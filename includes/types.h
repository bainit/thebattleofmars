#ifndef TYPES_H
#define TYPES_H

#define MAXNAME 48
#define MAXLINE 128

#define TERMINAL_MAX_LINES 5
#define TERMINAL_LINE_LENGTH 128
#define MAX_WEAPON_RANGE 29
#define TEAM_PLAYER 'p'
#define TEAM_FOE 'f'

typedef enum Boolean {

	BOOL_TRUE = 1,
	BOOL_FALSE = 0

} Boolean;

typedef enum Color_type {

	COLOR_RESET,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_YELLOW,
	COLOR_BLUE,
	COLOR_MAGENTA,
	COLOR_CYAN,
	COLOR_LIGHT_GRAY,
	COLOR_DAR_GRAY,
	COLOR_LIGHT_RED,
	COLOR_LIGHT_GREEN,
	COLOR_LIGHT_YELLOW,
	COLOR_LIGHT_BLUE,
	COLOR_LIGHT_MAGENTA,
	COLOR_LIGHT_CYAN,
	COLOR_WHITE,
	COLOR_BLACK

} Color_type;

typedef enum Map_type {
	MAP_TYPE_START = 100,
	MAP_TYPE_TREE = 101,
	MAP_TYPE_ROCK = 102,
	MAP_TYPE_DESTROYED_UNIT = 103,
	MAP_TYPE_END = 199

} Map_type;

typedef enum Unit_type {

	UNIT_TYPE_START = 200, 
	UNIT_TYPE_LIGHT_MECH = 201, 	
	UNIT_TYPE_MEDIUM_MECH = 202,
	UNIT_TYPE_HEAVY_MECH = 203, 	
	UNIT_TYPE_MONSTER = 204,
	UNIT_TYPE_END = 299

} Unit_type;

typedef enum Weapon_type {

	WEAPON_START = 300,
	WEAPON_MACHINEGUN = 301,
	WEAPON_LASER = 302,
	WEAPON_ROCKET = 303,
	WEAPON_BIG_ROCKET = 304,
	WEAPON_NONE = 398,
	WEAPON_END = 399

} Weapon_type;

typedef enum Menu_type {
	MENU_NO_TYPE,
	MENU_MAP,
	MENU_GAME,
	MENU_UNIT,
	MENU_FIRING

}Menu_type;

typedef enum Menu_option_type {
	MENU_OPTION_NO_TYPE,
	MENU_OPTION_MAP_MENU,
	MENU_OPTION_NEXT_TURN,
	MENU_OPTION_END_GAME,
	MENU_OPTION_MAIN_MENU,
	MENU_OPTION_UNIT_MENU,
	MENU_OPTION_UP,
	MENU_OPTION_DOWN,
	MENU_OPTION_LEFT,
	MENU_OPTION_RIGHT,
	MENU_OPTION_FIRING_MENU,
	MENU_OPTION_MOVEUP,
	MENU_OPTION_MOVEDOWN,
	MENU_OPTION_MOVELEFT,
	MENU_OPTION_MOVERIGHT,
	MENU_OPTION_CHANGE_WEAPON,
	MENU_OPTION_FIRST_WEAPON,
	MENU_OPTION_SECOND_WEAPON,
	MENU_OPTION_FIRE

} Menu_option_type;

typedef enum Terminal_msg_type {

	TERMINAL_MSG_IMPORTANT,
	TERMINAL_MSG_MEDIUM,
	TERMINAL_MSG_NORMAL,
	TERMINAL_MSG_GOOD_NEWS

} Terminal_msg_type;

typedef struct Point {

	int x,y;

} Point;

typedef struct Point2D {

	double x,y;

} Point2D;

typedef struct Obstacle {

	Point2D cord;

	double atanl;
	double atanh;

} Obstacle;

typedef struct Vector {
	char *data;
	int size;
	int max_size;
	int data_size;

} Vector;

typedef struct AI {

	int actual_unit;
	Boolean ai_turn;

	int ai_sleep;

} AI;

typedef struct Cursor {

	int x;
	int y;
	int type;

} Cursor;

typedef struct Info_screen {

	char *msg;
	int length;
	int position;

} Info_screen;

typedef struct Map_field {

	int type;
	void *item;

}Map_field;

typedef struct Map {
	Map_field *fields;
	int sizex;
	int sizey;

	int offsetx;
	int offsety;
}Map;

typedef struct Menu {
	Menu_type type;
	char description[MAXLINE];
	Vector options;
	struct Menu *previous;
} Menu;

typedef struct Menu_option{

	Boolean isvisible;
	Menu_type next_menu;
	Menu_option_type type;
        char name[MAXNAME];
        char key;
        void (*action)();

} Menu_option;

typedef struct Weapon_slot {

	Weapon_type wtype;
	int rounds;	

} Weapon_slot;

typedef struct Player {

	Color_type pcolor;
	Vector units;

} Player;

typedef struct Unit {

	Unit_type type;

	int posx;
	int posy;

	int health;
	int experience;
	int action_points;
	int improvements;

	int aweapon;

	Weapon_slot fweapon;
	Weapon_slot sweapon;

	Boolean isaiming;

	Boolean isfiring;
	
	Player *owner;

} Unit;

typedef struct Menu_manager {

	Menu menu_map;
	Menu menu_game;
	Menu menu_unit;
	Menu menu_firing;

	Menu *actual_menu;
	Unit *actual_unit;

} Menu_manager;


typedef struct Terminal {

	int actual_position;
	char messages[TERMINAL_MAX_LINES][TERMINAL_LINE_LENGTH];

} Terminal;

typedef struct Missile {

	Boolean isactive;
	double posx,posy;
	
	int endx, endy;

	double angle;
	int counter;
	Weapon_type weapon;

} Missile;

typedef struct Game {

	int is_running;

	int round;
	
	Player player;
	Player opponent;

	AI ai;	

} Game;

typedef struct Unit_weapon_range {

	char weapon_range[MAX_WEAPON_RANGE][MAX_WEAPON_RANGE];

} Unit_weapon_range;

typedef struct Game_media {

	Terminal *mterm;
	Map *mmap;
	Cursor *mcursor;
	Info_screen *minfo;
	Menu_manager *mmanager;
	
	Missile *mmissile;	
	
	Game *mgame;

	//foe target unit
	Unit *mftunit;

	Unit_weapon_range mweapon_range;

} Game_media;

#define DEFAULT_VECTOR {NULL, 0, 0, 0}
#define DEFAULT_PLAYER {0,DEFAULT_VECTOR}
#define DEFAULT_MAP {NULL, 0,0,0,0}
#define DEFAULT_MAP_VIEW {NULL}
#define DEFAULT_INFO_SCREEN {NULL,0,0}
#define DEFAULT_MENU {MENU_NO_TYPE,DEFAULT_VECTOR}
#define DEFAULT_CURSOR {0,0,CURSOR}
#define DEFAULT_MISSILE {BOOL_FALSE,0,0,0,0,0};

#endif //TYPES_H
