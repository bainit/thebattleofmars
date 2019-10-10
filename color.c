#include "includes/types.h"

#include "includes/colors.h"
#include "includes/map.h"
#include "includes/cursor.h"
#include "includes/util.h"

char *fcolors[]={
	"0", //reset 
	"31", //red 
	"32", //green 
	"33", //yellow 
	"34", //blue 
	"35", //magenta 
	"36", //cyan 
	"37", //light gray 
	"90", //dark gray
	"91", //light red
	"92", //light green
	"93", //light yellow
	"94", //light blue
	"95", //light magenta
	"96", //light cyan 
	"97", //white
	"30" //black
};

char *bcolors[]={
	"0", //reset
	"41", //red
	"42", //green
	"43", //yellow
	"44", //blue
	"45", //magenta
	"46", //cyan
	"47", //light gray
	"100", //dark gray
	"101", //light red
	"102", //light green
	"103", //light yellow
	"104", //light blue
	"105", //light magenta
	"106", //light cyan
	"107", //white
	"40"   //black
};

char* color_get_ftransparent() {

	return fcolors[0];

} 

char* color_get_btransparent() {

	return bcolors[0];

}

char* color_get_fcolor(Color_type t) {

	if(t==DEF_FIELD || t==CURSOR || (t>MAP_TYPE_START && t<MAP_TYPE_END))
		return fcolors[0];
	else
		return fcolors[1];

}

char* color_get_bcolor(Color_type t) {

	if(t==DEF_FIELD || (t>100 && t<200))
		return bcolors[0];
	else if(t==CURSOR)
		return bcolors[2];
	else 
		return bcolors[1];

}

char* color_get_bcolor_w(Color_type t) {

	return bcolors[t];

}

char* color_get_fcolor_w(Color_type t) {

	//if(t<0 || t>COLOR_TABLE_LENGTH)
	//	exit_on_error(COLOR_TABLE_OUT_OF_RANGE,"Error in color_get_fcolor_w, t is out of range of color table");

	return fcolors[t];
}
