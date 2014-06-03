// Author: Angel g
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "ppm.h"
#include "Angel.h"
#include "Bjarne.h"
#include "xwin.h"
#include "struct.h"

void applevector (void)
{
    MakeVector(150.0,180.0,0.0, apple.pos);//#########################4lin
    MakeVector(-6.0,0.0,0.0, apple.vel);
    apple.height = 200.0;
    apple.height2 = apple.height * 0.5;
}

void applecollision(void)
{
    apple_hit_dist = apple.pos[0] - apple.height2;//apple line!!##################
    if (apple.pos[1] >= (kangaroo.pos[1] - kangaroo.height2)
            && apple.pos[1] <= (kangaroo.pos[1] + kangaroo.height2)) {
        if ((apple_hit_dist - punch_dist) >= 25 && (apple_hit_dist - punch_dist <= 90.0))
        {
            if (show_apple) {
                lives++;
                show_apple = 0;
            }

        }
    }
}

void applePhysics()
{
    if (show_apple) { //###################################
        move_apple();
        // hit_dist = apple.pos[0] - 100.0;
        d0 = kangaroo.pos[0] - apple.pos[0];
        d1 = kangaroo.pos[1] - apple.pos[1];
        dist = d0*d0+d1*d1;
    }
}

/*void appleReset(void) //##########################################
  {
  show_apple = 1;

  int i = random(4);

  apple.pos[0] = (float)xres + 150000000;
  if (i == 1)
  apple.pos[1] = 260.0;
  else if (i == 2)
  apple.pos[1] = 190.0;
  else if (i == 3)
  apple.pos[1] = 120.0;
  else
  apple.pos[1] = 60.0;
  } //#############################################################ANGEL
  */
void init_apple_texture()
{
    int w2, h2;
    appleImage      = ppm6GetImage("./images/apple.ppm");
    //
    //create opengl texture elements
    glGenTextures(1, &appletexture);
    //-------------------------------------------------------------------------

    w2 = appleImage->width;
    h2 = appleImage->height;

    glBindTexture(GL_TEXTURE_2D, appletexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data
    unsigned char *Transparent = buildAlphaData(appleImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w2, h2, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, Transparent);
    free(Transparent);
    //
}

void apple_render()
{
    if (show_apple) { //###################################
        float wid = 120.0f;
        glPushMatrix();
        glTranslatef(apple.pos[0], apple.pos[1], apple.pos[2]);

        glBindTexture(GL_TEXTURE_2D, appletexture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);
        glBegin(GL_QUADS);
        //This is for the rhino image going right to left
        glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
        glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
        //#################################################
        glEnd();
        glPopMatrix();
        glDisable(GL_ALPHA_TEST);
    }
}
void appleReset()
{
    if (show_apple) { //###################################

        int i = rand() % 4;

        if(high_score >= 1000)
        {
            apple_render();
            if (i == 1)
                apple.pos[1] = 260.0;
            else if (i == 2)
                apple.pos[1] = 190.0;
            else if (i == 3)
                apple.pos[1] = 120.0;
            else
                apple.pos[1] = 60.0;
        }
    }
}

void move_apple()
{
    if (show_apple) { //###################################
        //auto move animal...
        //int addgrav = 1;
        //Update position
        apple.pos[0] += apple.vel[0]*2;
        apple.pos[1] += apple.vel[1];
        //Check for collision with window edges
        if ((apple.pos[0] < -140.0 && apple.vel[0] < 0.0) ||
                (apple.pos[0] >= (float)xres+140.0 && apple.vel[0] > 0.0)) {
            apple.vel[0] = -apple.vel[0];
            // appleReset();
        }
    }
}
