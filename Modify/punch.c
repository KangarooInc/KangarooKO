/* Author: Bjarne Henriquez
 * None of this code is usable yet
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "ppm.h"
#include "punch.h"
#include "struct.h"

void init_punch_texture(int w2, int h2)
{
    punchleftImage      = ppm6GetImage("./images/punchleft.ppm");
    punchrightImage      = ppm6GetImage("./images/punchright.ppm");
    /*punch3Image      = ppm6GetImage("./images/3punch.ppm");*/
    //
    //create opengl texture elements
    glGenTextures(1, &punchleftTexture);
    glGenTextures(1, &punchrightTexture);
    /*glGenTextures(1, &punch3Texture);*/
    //-------------------------------------------------------------------------
    //punch left

    glBindTexture(GL_TEXTURE_2D, punchleftTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data
    unsigned char *Transparent = buildAlphaData(punchleftImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w2, h2, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, Transparent);
    free(Transparent);
    //
    //---------------------------------------------------------------
    //punch right

    glBindTexture(GL_TEXTURE_2D, punchrightTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data
    Transparent = buildAlphaData(punchrightImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w2, h2, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, Transparent);
    free(Transparent);
    /*
     *    //---------------------------------------------------------------
     *    //punch 3
     *
     *    glBindTexture(GL_TEXTURE_2D, punch3Texture);
     *
     *    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
     *    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
     *    glTexImage2D(GL_TEXTURE_2D, 0, 3, w2, h2, 0,
     *            GL_RGB, GL_UNSIGNED_BYTE, punch3Image->data);
     *
     *    glBindTexture(GL_TEXTURE_2D, punch3Texture);
     *    //
     *    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
     *    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
     *    //
     *    //must build a new set of data
     *    Transparent = buildAlphaData(punch3Image);
     *    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w2, h2, 0,
     *            GL_RGBA, GL_UNSIGNED_BYTE, Transparent);
     *    free(Transparent);
     */
    //---------------------------------------------------------------
}

void punch_render(double x, double y, double z)
{
    if (show_kangaroo) {

        //punch_image += 1;
        if (punch_image == 1 || punch_image == 2) {
            glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
            glPushMatrix();
            glTranslatef(x, y, z);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.0f);
            glBindTexture(GL_TEXTURE_2D, punchleftTexture);
            glBegin(GL_QUADS);
            float w = kangaroo.width2;
            glTexCoord2f(0.0f, 0.0f); glVertex2f(-w,  w);
            glTexCoord2f(1.0f, 0.0f); glVertex2f( w,  w);
            glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -w);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(-w, -w);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_ALPHA_TEST);
            glPopMatrix();
        }

        else if (punch_image == 3 || punch_image == 4) {
            glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
            glPushMatrix();
            glTranslatef(x, y, z);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.0f);
            glBindTexture(GL_TEXTURE_2D, punchrightTexture);
            glBegin(GL_QUADS);
            float w = kangaroo.width2;
            glTexCoord2f(0.0f, 0.0f); glVertex2f(-w,  w);
            glTexCoord2f(1.0f, 0.0f); glVertex2f( w,  w);
            glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -w);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(-w, -w);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_ALPHA_TEST);
            glPopMatrix();
        }
        /*else if (punch_image == 3) {
          glPushMatrix();

          glTranslatef(x, y, z);
          glBindTexture(GL_TEXTURE_2D, punch3Texture);
          glEnable(GL_ALPHA_TEST);
          glAlphaFunc(GL_GREATER, 0.0f);
          glColor4ub(255,255,255,255);
          glBegin(GL_QUADS);
        //if (umbrella.vel[0] < 0.0) {
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
        glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
        [>} else {
        glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
        glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
        //}<]
        glEnd();
        glPopMatrix();
        //
        glDisable(GL_ALPHA_TEST);
        }*/
        else {
            punch_image = 0;
        }
    }
}
