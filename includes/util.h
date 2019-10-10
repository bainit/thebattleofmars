#ifndef UTIL_H
#define UTIL_H

#include "types.h"

#define TOLERANCE 1e-7

#define NULL_VAL -200

//default word size
#define DWORD_SIZE 64
#define UTIL_SEPARATION_CHAR 'o'

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void pre_print();

void exit_on_error(int status, char *msg);
void exit_on_null(void *var, char *msg);

void util_separation_line();
int  distance(double x1, double y1, double x2, double y2);
Point2D get_point2D(int x, int y);
Boolean check_a(Point2D pf, Point2D pu, double a, double l, double h, int px, int py, int dist1, int dist2);

void set_color_for_text(Color_type fct, Color_type bct, char *msg);
void set_colored_text(Color_type fct, Color_type bct,char *txt, char *msg);

int rand_in_range(int min, int max);

#endif //UTIL_H
