#ifndef _STRUCT_H_
#define _STRUCT_H_

typedef double Vec[3];

//structures
typedef struct t_rhino {
    Vec pos;
    Vec vel;
    float height;
    float height2;
} Rhino;
Rhino rhino;

typedef struct t_animal {
    Vec pos;
    Vec vel;
    float height;
    float height2;
} Animal;
Animal animal;

typedef struct t_ufo {
    Vec pos;
    Vec vel;
} Ufo;
Ufo ufo;

typedef struct t_kangaroo {
    int shape;
    Vec pos;
    Vec lastpos;
    float width;
    float width2;
    float height;
    float height2;
} Kangaroo;
Kangaroo kangaroo;

#endif
