#ifndef THEBATTLE_H
#define THEBATTLE_H

#include "types.h"

Boolean thebattle_check_window();
void thebattle_close();
void thebattle_starting_subtitles();
Boolean thebattle_team_selection(char *team);
void thebattle_get_team(unsigned int c, char *team);
void thebattle_ending_subtitles();

#endif
