#include <string.h>
#include <stdio.h>

#include "includes/types.h"

#include "includes/terminal.h"
#include "includes/colors.h"
#include "includes/util.h"

int terminal_init(Terminal *t) {

	if(t==NULL)
		return TERMINAL_ERR_NULL_TERMINAL;
	int i;
	for(i=0;i<TERMINAL_MAX_LINES;i++) {
		t->messages[i][0] = '\0';
	}

	t->actual_position = 0;

	return TERMINAL_OK;
}

void terminal_add_msg(Terminal *t, char *msg, Terminal_msg_type type) {

	exit_on_null(t,"Error in terminal_add_msg, terminal is null");
	exit_on_null(msg,"Error in terminal_add_msg, msg is null");

	int msg_len = strlen(msg);
	if(msg_len>=TERMINAL_LINE_LENGTH)
		msg[msg_len] = '\0';
	
	Color_type bg = terminal_get_bcolor_for_msg_type(type);

	set_colored_text(COLOR_BLACK,bg,t->messages[t->actual_position],msg);
	
	if(t->actual_position<TERMINAL_MAX_LINES-1)
		t->actual_position++;
	else 
		t->actual_position = 0;
}

void terminal_print(Terminal *t) {

	int i,pos;
	int j=t->actual_position;
	for(i=0;i<TERMINAL_MAX_LINES;i++,j++) {
		pos=j%TERMINAL_MAX_LINES;
		printf("%s\n",t->messages[pos]);
	}

}

Color_type terminal_get_bcolor_for_msg_type(Terminal_msg_type tmt) {

	if(tmt == TERMINAL_MSG_IMPORTANT)
		return COLOR_RED;
	if(tmt == TERMINAL_MSG_MEDIUM)
		return COLOR_YELLOW;
	if(tmt == TERMINAL_MSG_NORMAL)
		return COLOR_WHITE;
	if(tmt == TERMINAL_MSG_GOOD_NEWS)
		return COLOR_GREEN;

}

void terminal_welcome_message(Terminal *t) {

	terminal_add_msg(t,"Welcome to The battle of mars game", TERMINAL_MSG_NORMAL);
	terminal_add_msg(t,"Your objective is to destroy your opponent", TERMINAL_MSG_NORMAL);
	terminal_add_msg(t,"Use keys to move your units", TERMINAL_MSG_NORMAL);
	terminal_add_msg(t,"Dont loos all your units", TERMINAL_MSG_NORMAL);
	terminal_add_msg(t,"Good luck!", TERMINAL_MSG_NORMAL);

}
