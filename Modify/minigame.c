#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "xwin.h"
#include "ppm.h"
#include "minigame.h"

void init_MiniGame()
{
    car1Image      = ppm6GetImage("./images/car1.ppm");
    car2Image      = ppm6GetImage("./images/car2.ppm");
    car3Image      = ppm6GetImage("./images/car3.ppm");
    backgroundImage      = ppm6GetImage("./images/MiniGameBackground.ppm");
    //create opengl texture elements
    glGenTextures(1, &minitexture);
    glGenTextures(1, &car1texture);
    glGenTextures(1, &car2texture);
    glGenTextures(1, &car3texture);
    //-------------------------------------------------------------------------
    //car1
    int w = car1Image->width;
    int h = car1Image->height;
    glBindTexture(GL_TEXTURE_2D, car1texture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data
    unsigned char *Transparent = buildAlphaData(car1Image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, Transparent);
    free(Transparent);
    //
    //---------------------------------------------------------------
    //car2

    glBindTexture(GL_TEXTURE_2D, car2texture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data
    Transparent = buildAlphaData(car2Image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, Transparent);
    free(Transparent);

    //---------------------------------------------------------------
    //car  3

    glBindTexture(GL_TEXTURE_2D, car3texture);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
            GL_RGB, GL_UNSIGNED_BYTE, car3Image->data);

    glBindTexture(GL_TEXTURE_2D, car3texture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data
    Transparent = buildAlphaData(car3Image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, Transparent);
    free(Transparent);

    //---------------------------------------------------------------
    // Background - minigame
    //
    glBindTexture(GL_TEXTURE_2D, minitexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    Transparent = buildAlphaData(backgroundImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, backgroundImage->width,
            backgroundImage->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Transparent);
    free(Transparent);
}

void mini_render(void)
{
    if(show_MiniGame) {

        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,minitexture);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
        glEnd();
        glPopMatrix();

        if(show_car) {

            glPushMatrix();
            glBindTexture(GL_TEXTURE_2D,car1texture);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
            glEnd();
            glPopMatrix();

            if(punch_count == 40) {
                glPushMatrix();
                glPushMatrix();
                glBindTexture(GL_TEXTURE_2D,car2texture);
                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
                glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
                glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
                glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
                glEnd();
                glPopMatrix();
            }

            if(punch_count == 80) {
                glPushMatrix();
                glBindTexture(GL_TEXTURE_2D,car3texture);
                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
                glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
                glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
                glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
                glEnd();
                glPopMatrix();
            }
        }
    }
}

