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

void init_punch_texture(void)
{
    punch1Image      = ppm6GetImage("./images/1punch.ppm");
    punch2Image      = ppm6GetImage("./images/2punch.ppm");
    punch3Image      = ppm6GetImage("./images/3punch.ppm");
    //
    //create opengl texture elements
    glGenTextures(1, &punch1Texture);
    glGenTextures(1, &punch2Texture);
    glGenTextures(1, &punch3Texture);
    //-------------------------------------------------------------------------
    //punch 1
    int w2 = punch1Image->width;
    int h2 = punch1Image->height;

    glBindTexture(GL_TEXTURE_2D, punch1Texture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data
    unsigned char *Transparent = buildAlphaData(punch1Image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w2, h2, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, Transparent);
    free(Transparent);
    //
    //---------------------------------------------------------------
    //punch 2
    w2 = punch2Image->width;
    h2 = punch2Image->height;

    glBindTexture(GL_TEXTURE_2D, punch2Texture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data
    Transparent = buildAlphaData(punch2Image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w2, h2, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, Transparent);
    free(Transparent);
    //---------------------------------------------------------------
    //punch 3
    w2 = punch3Image->width;
    h2 = punch3Image->height;

    glBindTexture(GL_TEXTURE_2D, punch3Texture);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w2, h2, 0,
            GL_RGB, GL_UNSIGNED_BYTE, punch3Image->data);

    glBindTexture(GL_TEXTURE_2D, punch3Texture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data
    Transparent = buildAlphaData(punch3Image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w2, h2, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, Transparent);
    free(Transparent);
    //---------------------------------------------------------------
}

void punch_render(double x, double y, double z)
{
    float wid = 120.0f;
    if (show_kangaroo) {

        //punch_image += 1;
        if (punch_image == 1) {
            glPushMatrix();

            glTranslatef(x, y, z);
            glBindTexture(GL_TEXTURE_2D, punch1Texture);
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
        else if (punch_image == 2) {
            glPushMatrix();

            glTranslatef(x, y, z);
            glBindTexture(GL_TEXTURE_2D, punch2Texture);
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
        else if (punch_image == 3) {
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
            /*} else {
              glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
              glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
              glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
              glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
            //}*/
            glEnd();
            glPopMatrix();
            //
            glDisable(GL_ALPHA_TEST);
        }
        else {
            punch_image = 0;
        }
    }
}