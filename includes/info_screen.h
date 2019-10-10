#ifndef INFO_SCREEN_H
#define INFO_SCREEN_H

#include "types.h"

//30*64
#define INFO_SCREEN_SIZE 4096
#define INFO_SCREEN_WORD_MAX 48
#define INFO_SCREEN_LINE_MAX 192
#define INFO_SCREEN_MAX_LINES 10

#define INFO_SCREEN_MSG_TO_LONG -700
#define INFO_SCREEN_WORD_TO_LONG -701
#define INFO_SCREEN_NEGATIVE_POSITION -702
#define INFO_SCREEN_OUTOFRANGE_POSITION -703

#define INFO_SCREEN_OK	0
#define INFO_SCREEN_ENDL 704
#define INFO_SCREEN_ENDMSG 705

void 	info_screen_init(Info_screen *is);
void 	info_screen_add_msg(Info_screen *is, char *nmsg);
void 	info_screen_set_msg(Info_screen *is,char *msg);
void 	info_screen_clear(Info_screen *is);

int 	info_screen_read_word(Info_screen *is, char *word);
int	info_screen_get_line(Info_screen *is, char *line);

void 	info_screen_back_position(Info_screen *is, int bpos);
void 	info_screen_reset_position(Info_screen *is);

void 	info_screen_free(Info_screen *is);

#endif //INFO_SCREEN_H
