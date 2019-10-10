#ifndef TERMINAL_H
#define TERMINAL_H

#include "types.h"

#define TERMINAL_OK 0
#define TERMINAL_ERR_NULL_TERMINAL -1001

int terminal_init(Terminal *t);
void terminal_add_msg(Terminal *t, char *msg, Terminal_msg_type type);
void terminal_print(Terminal *t);

Color_type terminal_get_bcolor_for_msg_type(Terminal_msg_type tmt);

void terminal_welcome_message(Terminal *t);

#endif
