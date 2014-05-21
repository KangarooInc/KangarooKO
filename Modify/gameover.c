//cs335
//program:  gameover.c
//author:   Ignacio Medina
//date:     Spring 2014
//Create a "game over" screen.
//When lives are equal to 0, game over screen will display.
//---------------------------------------------------------
//---------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glx.h>
#include "gameover.h"
#include "ppm.h"
#include "xwin.h"

void GameOver(void)
{
    show_kangaroo = 0;
    show_rhino = 0;
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, gameoverTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2i(0, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2i(0, yres);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2i(xres, yres);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2i(xres, 0);
    glEnd();
    glPopMatrix();
}
