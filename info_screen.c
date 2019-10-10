#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "includes/types.h"

#include "includes/info_screen.h"
#include "includes/util.h"

void info_screen_init(Info_screen *is) {

	exit_on_null(is,"Error in info_screen_init(), info_screen is null");

	is->msg = (char*)malloc(INFO_SCREEN_SIZE*sizeof(char));

	exit_on_null(is->msg,"Error in info_screen_init(), info_screen->msg is null");

	is->length = 0;
	is->position = 0;

}

void info_screen_add_msg(Info_screen *is, char *msg) {

	exit_on_null(is,"Error in info_screen_add_msg, info_screen is null");
	exit_on_null(msg,"Error in info_screen_add_msg, msg is null");

	int msglen = strlen(msg);

	//if(msglen+1>(INFO_SCREEN_SIZE-is->length))
	if(msglen>(INFO_SCREEN_SIZE-is->length))
		exit_on_error(INFO_SCREEN_MSG_TO_LONG,"Error in info_screen_add_msg, msg to long");

	memcpy(is->msg+is->length,msg,msglen);

	is->length += msglen;
        //is->length++;
	char *ic = is->msg+is->length;
	//*ic = EOF;
	

}

void info_screen_set_msg(Info_screen *is, char *msg) {

	info_screen_clear(is);
	info_screen_add_msg(is,msg);

}

void info_screen_clear(Info_screen *is) {

	is->length = 0;
	is->position = 0;

}

int info_screen_read_word(Info_screen *is, char *word) {

	exit_on_null(is,"Error in info_screen_read_word(), info screen is null");
	exit_on_null(word,"Error in info_screen_read_word(), word is null");

	if(is->position >= is->length) {
		return INFO_SCREEN_ENDMSG;
	}

	int j=0;
	char c;

	int rval = INFO_SCREEN_OK;
	
	//while((c=is->msg[is->position])==' ' || c=='\n' || c=='\t') {
	//	is->position++;
	//}
	//c = is->msg[is->position];
	//if(c==EOF)
	//	return EOF;

	while(is->position < is->length) {
		if(j-1>=INFO_SCREEN_WORD_MAX)
			exit_on_error(INFO_SCREEN_WORD_TO_LONG,"Error in info_screen_read_word, word is to long");
		
		c = is->msg[is->position++];

		if(c=='\n' || c==EOF || c=='\0') {
			rval = INFO_SCREEN_ENDL;
			break;
		}

		word[j++] = c;

		if(c == ' '){
			break;
		}
	}
	word[j] = '\0';

	return rval;
}

int info_screen_get_line(Info_screen *is, char *line) {

	exit_on_null(is, "Error in info_screen_get_line(), info_screen is null");
	exit_on_null(line, "Error in info_screen_get_line(), line is null");

	int c;
	char word[INFO_SCREEN_WORD_MAX];

	int len = 0;
	int size = 0;
	int position = 0;	

	while((c=info_screen_read_word(is,word))!=INFO_SCREEN_ENDMSG) {
		len = strlen(word);
		size += len;
		if(size<INFO_SCREEN_LINE_MAX) {
			memcpy(line+position,word,len);
			position += len;
		}else {
			info_screen_back_position(is,len);
			break;
		}
		if(c==INFO_SCREEN_ENDL) 
			break;
	}

	line[position]='\0';

	if(c==INFO_SCREEN_ENDMSG || c==INFO_SCREEN_ENDL)
		return c;

	return INFO_SCREEN_OK;
}

void info_screen_back_position(Info_screen *is, int bpos) {

	exit_on_null(is,"Error in info_screen_back_position, info_screen is nulll");
	if(bpos<0)
		exit_on_error(INFO_SCREEN_NEGATIVE_POSITION, "Error in info_screen_back_position, position is negative");
	if(bpos>is->length)
		exit_on_error(INFO_SCREEN_OUTOFRANGE_POSITION, "Error in info_screen_back_position, position is larger than info screen length");

	is->position -= bpos;

}

void info_screen_reset_position(Info_screen *is) {

	exit_on_null(is,"Error in info_screen_reset_position, info_screen is null");

	is->position = 0;

}

void info_screen_free(Info_screen *is) {

	free(is->msg);

}
