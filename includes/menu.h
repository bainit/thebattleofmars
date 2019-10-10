#ifndef MENU_H
#define MENU_H

#include "types.h"

#define MENU_NO_ACTION 0
#define MENU_DONE_ACTION 1

#define MMERR_NO_SUCH_MENU -901

void 		menu_init(Menu *m, Menu_type mt);
void		menu_print_option(Menu_option *opt);
void 		menu_print(Menu *men);
void 		menu_option_init(Menu_option *mo, char *name, char key, Menu_option_type mot);
Menu_option* 	menu_find_option_by_key(Menu *m, char key, int nohide);
Menu_option*    menu_find_option_by_name(Menu *m, char *name, int nohide);
Menu_option* 	menu_find_option_by_type(Menu *m, Menu_option_type mt, int nohide);
int		menu_add_option(Menu *men, Menu_option *opt);
int		menu_is_type(Menu *m, Menu_type t);
void 		menu_option_set_name(Menu_option *opt, const char *name);

void 		menu_manager_init(Menu_manager *mm);
void		menu_manager_create_menu_map(Menu *ma);
void 		menu_manager_create_menu_game(Menu *mg);
void 		menu_manager_create_menu_unit(Menu *mu);
void		menu_manager_create_menu_firing(Menu *ma);

int 		menu_manager_action_on_key(Menu_manager *mm, Info_screen *is,char key);
void		menu_manager_set_menu(Menu_manager *mm, Menu *m);

void            menu_manager_actual_menu(Menu_manager *mm, Menu_type mt, Game_media *m);
void 		menu_check_for_unit_weapon_options(Menu_manager *mm);
void 		menu_show_unit_weapon_options(Menu_manager *mm);
void 		menu_hide_unit_weapon_options(Menu_manager *mm);

void		menu_helper_cursor_action(Game_media *gm, Menu_manager *mm, char c);
void 		menu_helper_change_weapon(Unit *u, Game_media *m, char key, Menu_manager *mm);
void		menu_helper_firing_menu(Unit *u, Game_media *m, char key, Menu_manager *mm);
void		menu_option_setvisible(Menu_option *mo);
void		menu_option_hide(Menu_option *mo);

#endif //MENU_H
