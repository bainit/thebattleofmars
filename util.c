#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "includes/types.h"

#include "includes/util.h"
#include "includes/map.h"
#include "includes/menu.h"
#include "includes/colors.h"
#include "includes/unit.h"

void pre_print() {

	system( "stty -icanon" );
        system("clear");
        printf("\e[?25l");
        system("PS1=\"\"");
        printf("\e[?25h");

}

void exit_on_error(int status, char *msg) {

        if(status != 0) {
                printf("exit_on_error(), %s, status: %d\n", msg, status);
                exit(status);
        }

}


void exit_on_null(void *var, char *msg) {

	if(var == NULL) {
		printf("exit_on_null(): %s\n", msg);
		exit(NULL_VAL);
	}
}

void util_separation_line() {

        int i;
        for(i=0;i<MAP_SCREEN_X;i++) {
                printf("%c", UTIL_SEPARATION_CHAR);
        }

        printf("\n");
}

int distance(double x1, double y1, double x2, double y2) {

	double r1 = x1-x2;
	double r2 = y1-y2;
	
	double result = sqrt(r1*r1+r2*r2);
	return (int)(rint(result));

}

Point2D get_point2D(int x, int y) {

	Point2D p;
	p.x = x+0.5;
	p.y = y+0.5;
	return p;

}

void set_color_for_text(Color_type fct, Color_type bct, char *msg) {

	char *tfcolor = color_get_fcolor_w(fct);
        char *tbcolor = color_get_bcolor_w(bct);
        strcpy(msg,ANSI_COLOR_RESET);
        strcat(msg,PREF);
        strcat(msg,tfcolor);
        strcat(msg,COLON);
        strcat(msg,tbcolor);
        strcat(msg,END);
}

void set_colored_text(Color_type fct, Color_type bct, char *txt, char *msg) {

	set_color_for_text(fct,bct,txt);
	strcat(txt,msg);
	strcat(txt,END_TXT_COLOR);	

}

Boolean check_a(Point2D f, Point2D u, double a, double l, double h, int ox, int oy, int df, int du) {

	int x = u.x;
	int y = u.y;

	if(oy==y && ox<x) {

		double nox = u.x-ox-1;
		
		a = atan2(f.y-u.y,u.x-f.x);
		
		h = atan2(0.5, nox);
		l = atan2(-0.5, nox);

	}

	if(a>=l && a<=h && df>=du)
		return BOOL_TRUE;

	return BOOL_FALSE;
}

int rand_in_range(int min, int max) {

	return rand() % (max + 1 - min) + min;

}
