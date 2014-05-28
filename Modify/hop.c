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
#include "hop.h"

void init_hop_texture(int w2, int h2)
{
    lowhopImage      = ppm6GetImage("./images/prephop.ppm");
    highhopImage      = ppm6GetImage("./images/hop.ppm");

    /*punch3Image      = ppm6GetImage("./images/3punch.ppm");*/
    //
    //create opengl texture elements
    glGenTextures(1, &lowhopTexture);
    glGenTextures(1, &highhopTexture);
    /*glGenTextures(1, &punch3Texture);*/
    //-------------------------------------------------------------------------
    // low hop (prep hop)

    glBindTexture(GL_TEXTURE_2D, lowhopTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data
    unsigned char *Transparent = buildAlphaData(lowhopImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w2, h2, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, Transparent);
    free(Transparent);
    //
    //---------------------------------------------------------------
    //high hop

    glBindTexture(GL_TEXTURE_2D, highhopTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data
    Transparent = buildAlphaData(highhopImage);
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

void hop_render(double x, double y, double z)
{
    float wid = 120.0f;
    if (show_kangaroo) {

        //punch_image += 1;
        if (hop_image == 1) {
            glPushMatrix();

            glTranslatef(x, y, z);
            glBindTexture(GL_TEXTURE_2D, lowhopTexture);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.0f);
            glColor4ub(255,255,255,255);
            glBegin(GL_QUADS);
            //if (umbrella.vel[0] < 0.0) {
            glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
            glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
            glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
            /*} else {
              glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
              glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
              glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
              glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
              }*/
            glEnd();
            glPopMatrix();
            //
            glDisable(GL_ALPHA_TEST);
        }
        else if (hop_image == 2) {
            glPushMatrix();

            glTranslatef(x, y, z);
            glBindTexture(GL_TEXTURE_2D, highhopTexture);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.0f);
            glColor4ub(255,255,255,255);
            glBegin(GL_QUADS);
            //if (umbrella.vel[0] < 0.0) {
            glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
            glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
            glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
            /*} else {
              glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
              glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
              glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
              glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
              }*/
            glEnd();
            glPopMatrix();
            //
            glDisable(GL_ALPHA_TEST);
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
            hop_image = 0;
        }
    }
}
