#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "includes/types.h"

#include "includes/util.h"
#include "includes/thebattle.h"

#define MIN_TEAMS 1
#define MAX_TEAMS 3
#define WINDOW_WIDTH 80
#define WINDOW_HEIGHT 24

Boolean thebattle_check_window() {

	struct winsize w;
    	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	
	if(w.ws_col >= WINDOW_WIDTH && w.ws_row >= WINDOW_HEIGHT){
		return BOOL_TRUE;
	}

	return BOOL_FALSE;

}

void thebattle_close() {

	printf("Window has no proper size.\n");
	printf("Change window size and restart game\n");
	exit(0);

}

void thebattle_starting_subtitles() {

        pre_print();
        printf("       _____         _______  __   __  _____\n");
	printf("    __/.....\\__     /__  __/ / /  / / / ___/\n");
	printf("   /  \\ ### /  \\      / /   / /__/ / / /__\n");
	printf("  / /  \\_-_/  \\ \\    / /   / /  / / / /__  \n");
	printf("  \\/ \\__/ \\__/ \\/   /_/   /_/  /_/ /____/ \n");
	printf("  ||   \\___/   ||    ____    __   _______  _______  __      _____ \n");
	printf("      /  _  \\       /    /  /  | /__  __/ /__  __/ / /     / ___/\n");
	printf("     /  | |  \\     /  . /  / _ |   / /      / /   / /     / /__  \n");
	printf("    |__|   |__|   /  . \\  / // |  / /      / /   / /__   / /__  \n");
	printf("                 /_____/ /_/ /_| /_/      /_/   /_____/ /____/\n");
	printf("           ___    _____  \n");
	printf("         / __ \\  / ___/\n");
	printf("        / / / / / /_\n");
	printf("       / /_/ / / __/\n");
	printf("       \\____/ /_/\n");
	printf("      __    __     __     _____     ______\n");
	printf("     /  |  /  |   /  |   /  .  /   /  ___/\n");
	printf("    /   | /   |  / _ |  /  __ /   /__/___\n");
	printf("   / /| |/ /| | / // | / /| \\     ____/ /\n");
	printf("  /_/ |___/ |_|/_/ /_|/_/  \\_\\   /_____/\n");
	printf("\n");
	printf("  version 1.0 Savage Swarm by bainit\n");
	printf("  (press any key to continue)\n");

	system("setterm -cursor off");
	getchar();
	//system("setterm -cursor on");
}

Boolean thebattle_team_selection(char *team) {

	exit_on_null(team,"Error in thebattle_team_selection, team is null");

	pre_print();
	printf("\n");
	printf("Please select a team by pressing (1), (2) or (3)\n");
	printf("\n");
	printf("(1) Light Mech  (machine gun)\n");
	printf("    Light Mech  (machine gun)\n");
	printf("    Light Mech  (machine gun)\n");
	printf("    Medium Mech (machine gun, laser)\n");
	printf("    Heavy Mech  (rockets, laser)\n");
	
	printf("\n");

	printf("(2) Light Mech  (machine gun)\n");
        printf("    Medium Mech (machine gun, laser)\n");
	printf("    Medium Mech (machine gun, laser)\n");
	printf("    Heavy Mech  (rockets, laser)\n");

	printf("\n");	
	
	printf("(3) Ligth Mech   (machine gun)\n");
	printf("    Medium Mech  (machine gun, laser)\n");
	printf("    Heavy Mech   (rockets, laser)\n");
	printf("    Super Heavy Mech (big rockets)\n");

	system("setterm -cursor off");

	char c = getchar();
	unsigned int teamnr = c-'0';

	if(teamnr<MIN_TEAMS || teamnr>MAX_TEAMS) 
		return BOOL_FALSE; 

	thebattle_get_team(teamnr,team);
		return BOOL_TRUE;
}

void thebattle_get_team(unsigned int c, char *team) {

	exit_on_null(team,"Error in thebattle_get_team, team is null");

	FILE* teams;
	teams = fopen("data/teams.data","r");
	exit_on_null(teams,"Error in thebattle_get_team, problems reading teams data file");

	int i;
	int status;
	for(i=0;i<c;i++) {		
		status = fscanf(teams,"%s\n", team);
		if(status<1) {
			exit_on_error(status,"Error in thebattle_get_team, problems reading line of teams data file");
		}
	}
	fclose(teams);

}

void thebattle_ending_subtitles() {

        pre_print();
        printf("Credits: bainit\n");
        printf("Distributed under MIT Licence\n");
	system("setterm -cursor off");
	getchar();
	system("setterm -cursor on");

}
