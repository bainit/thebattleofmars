#ifndef COLORS_H
#define COLORS_H

#include "types.h"

#define COLOR_TABLE_LENGTH 2

#define COLOR_TABLE_OUT_OF_RANGE -1101

#define PREF "\x1b["
#define COLON ";"
#define END "m"
#define END_TXT_COLOR "\033[0m"

#define ANSI_COLOR_RESET "\x1b[0m"

char* color_get_ftransparent();
char* color_get_btransparent();

char* color_get_fcolor(Color_type t);
char* color_get_bcolor(Color_type t);

char* color_get_bcolor_w(Color_type t);
char* color_get_fcolor_w(Color_type t);

#endif //COLORS_H 
