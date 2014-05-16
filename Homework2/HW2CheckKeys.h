#ifndef _HW2CHECKKEYS_H_
#define _HW2CHECKKEYS_H_

//#include "HW2CheckKeys.c"

typedef double Vec[3];
void check_keys();
int show_bigfoot;
//struct Bigfoot;
//struct Umbrella;
int deflection=0;
int ndrops=1;
int show_rain=0;
int silhouette=1;
int play_sounds = 0;
int done=0;

typedef struct t_bigfoot {
    Vec pos;
    Vec vel;
} Bigfoot;
Bigfoot bigfoot;

typedef struct t_umbrella {
    int shape;
    Vec pos;
    Vec lastpos;
    float width;
    float width2;
    float radius;
} Umbrella;
Umbrella umbrella;
int show_umbrella=0;

#endif
