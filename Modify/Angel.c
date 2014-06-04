// Author: Angel G 
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
    MakeVector(150.0,180.0,0.0, apple.pos);
    MakeVector(-6.0,0.0,0.0, apple.vel);
    apple.height = 200.0;
    apple.height2 = apple.height * 0.5;
}

void applecollision(void)
{
    apple_hit_dist = apple.pos[0] - apple.height2;
    if (apple.pos[1] >= (kangaroo.pos[1] - kangaroo.height2)
            && apple.pos[1] <= (kangaroo.pos[1] + kangaroo.height2)) {
        if ((apple_hit_dist - punch_dist) >= 25 && (apple_hit_dist - punch_dist <= 90.0)) {
            if (show_apple) {
                lives++;
                show_apple = 0;
            }
        }
    }
}

void applePhysics()
{
    if (show_apple) { 
        move_apple();
        d0 = kangaroo.pos[0] - apple.pos[0];
        d1 = kangaroo.pos[1] - apple.pos[1];
        dist = d0*d0+d1*d1;
    }
}

void init_apple_texture()
{
    int w2, h2;
    appleImage = ppm6GetImage("./images/apple.ppm");
    glGenTextures(1, &appletexture);
    w2 = appleImage->width;
    h2 = appleImage->height;
    glBindTexture(GL_TEXTURE_2D, appletexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *Transparent = buildAlphaData(appleImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w2, h2, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, Transparent);
    free(Transparent);
}

void apple_render()
{
    if (show_apple) { 
        float wid = 35.0f;
        glPushMatrix();
        glTranslatef(apple.pos[0], apple.pos[1], apple.pos[2]);
        glBindTexture(GL_TEXTURE_2D, appletexture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);
        glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
        glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
        glEnd();
        glPopMatrix();
        glDisable(GL_ALPHA_TEST);
    }
}

void appleReset()
{
    if (show_apple) { 
        int i = rand() % 4;
        if(high_score >= 1000) {
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
    if (show_apple) { 
        apple.pos[0] += apple.vel[0]*2;
        apple.pos[1] += apple.vel[1];
        if ((apple.pos[0] < -140.0 && apple.vel[0] < 0.0) ||
                (apple.pos[0] >= (float)xres+140.0 && apple.vel[0] > 0.0)) {
            apple.vel[0] = -apple.vel[0];
        }
    }
}

void bad_applevector (void)
{
    MakeVector(150.0,180.0,0.0, bapple.pos);
    MakeVector(-6.0,0.0,0.0, bapple.vel);
    bapple.height = 200.0;
    bapple.height2 = bapple.height * 0.5;
}

void bapplecollision(void)
{
    bapple_hit_dist = bapple.pos[0] - bapple.height2;
    if (bapple.pos[1] >= (kangaroo.pos[1] - kangaroo.height2)
            && bapple.pos[1] <= (kangaroo.pos[1] + kangaroo.height2)) {
        if ((bapple_hit_dist - punch_dist) >= 25 && (bapple_hit_dist - punch_dist <= 90.0))
        {
            if (show_bapple) {
                lives--;
                show_bapple = 0;
            }
        }
    }
}

void bapplePhysics()
{
    if (show_bapple) { 
        move_bapple();
        d0 = kangaroo.pos[0] - bapple.pos[0];
        d1 = kangaroo.pos[1] - bapple.pos[1];
        dist = d0*d0+d1*d1;
    }
}

void init_bapple_texture()
{
    int w2, h2;
    bappleImage      = ppm6GetImage("./images/bapple.ppm");
    glGenTextures(1, &bappletexture);
    w2 = bappleImage->width;
    h2 = bappleImage->height;
    glBindTexture(GL_TEXTURE_2D, bappletexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *Transparent = buildAlphaData(bappleImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w2, h2, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, Transparent);
    free(Transparent);
}

void bapple_render()
{
    if (show_bapple) { 
        float wid = 45.0f;
        glPushMatrix();
        glTranslatef(bapple.pos[0], bapple.pos[1], bapple.pos[2]);

        glBindTexture(GL_TEXTURE_2D, bappletexture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);
        glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
        glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
        glEnd();
        glPopMatrix();
        glDisable(GL_ALPHA_TEST);
    }
}
void bappleReset()
{
    if (show_bapple) { 

        int i = rand() % 4;

        if(high_score >= 500)
        {
            bapple_render();
            if (i == 1)
                bapple.pos[1] = 260.0;
            else if (i == 2)
                bapple.pos[1] = 190.0;
            else if (i == 3)
                bapple.pos[1] = 120.0;
            else
                bapple.pos[1] = 60.0;
        }
    }
}

void move_bapple()
{
    if (show_bapple) { 
        bapple.pos[0] += bapple.vel[0]*2;
        bapple.pos[1] += bapple.vel[1];
        if ((bapple.pos[0] < -140.0 && bapple.vel[0] < 0.0) ||
                (bapple.pos[0] >= (float)xres+140.0 && bapple.vel[0] > 0.0)) {
            bapple.vel[0] = -bapple.vel[0];
        }
    }
}
