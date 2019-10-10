#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "includes/types.h"

#include "includes/thebattle.h"
#include "includes/game.h"

int main(int argc, char **argv) {

	if(thebattle_check_window()==BOOL_FALSE) {
		thebattle_close();
	}

	srand(time(NULL));

	thebattle_starting_subtitles();
	
	char team[MAXLINE];
	
	while(!thebattle_team_selection(team));

	game_play_game(team);

	thebattle_ending_subtitles();

	return 0;

}
