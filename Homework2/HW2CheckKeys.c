#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "ppm.h"
#include "fonts.h"
#include "HW2CheckKeys.h"
#define USE_SOUND

#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
//typedef double Vec[3];
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
(c)[2]=(a)[2]-(b)[2]

void check_keys(XEvent *e)
{
    //keyboard input?
    static int shift=0;
    int key = XLookupKeysym(&e->xkey, 0);
    if (e->type == KeyRelease) {
        if (key == XK_Shift_L || key == XK_Shift_R)
            shift=0;
        return;
    }
    if (e->type == KeyPress) {
        if (key == XK_Shift_L || key == XK_Shift_R) {
            shift=1;
            return;
        }
    } else {
        return;
    }

    int i = 0;
    //
    switch(key) {
        case XK_b:
            show_bigfoot ^= 1;
            if (show_bigfoot) {
                bigfoot.pos[0] = 750.0;
                i = random(3);
                if (i == 1)
                    bigfoot.pos[1] = 250.0;
                else if (i == 2)
                    bigfoot.pos[1] = 180.0;
                else
                    bigfoot.pos[1] = 120.0;
            }
            break;
        case XK_d:
            deflection ^= 1;
            break;
        case XK_s:
            silhouette ^= 1;
            printf("silhouette: %i\n",silhouette);
            break;
        case XK_u:
            show_umbrella ^= 1;
            umbrella.shape ^= 1;
            break;
        case XK_r:
            show_rain ^= 1;
            break;
        case XK_Left:
            VecCopy(umbrella.pos, umbrella.lastpos);
            umbrella.pos[0] -= 10.0;
            break;
        case XK_Right:
            VecCopy(umbrella.pos, umbrella.lastpos);
            umbrella.pos[0] += 10.0;
            break;
        case XK_Up:
            VecCopy(umbrella.pos, umbrella.lastpos);
            umbrella.pos[1] += 65.0;
            break;
        case XK_Down:
            VecCopy(umbrella.pos, umbrella.lastpos);
            umbrella.pos[1] -= 65.0;
            break;
        case XK_equal:
            if (++ndrops > 60)
                ndrops=60;
            break;
        case XK_minus:
            if (--ndrops < 0)
                ndrops = 0;
            break;
        case XK_n:
            play_sounds ^= 1;
            break;
        case XK_w:
            if (shift) {
                //shrink the umbrella
                umbrella.width *= (1.0 / 1.05);
            } else {
                //enlarge the umbrella
                umbrella.width *= 1.05;
            }
            //half the width
            umbrella.width2 = umbrella.width * 0.5;
            umbrella.radius = (float)umbrella.width2;
            break;
        case XK_Escape:
            done=1;
            break;
    }
}
