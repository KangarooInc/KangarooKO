#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "ppm.h"
#include "xwin.h"
#include "struct.h"
#include "Bjarne.h"
#include "Angel.h"

#define USE_SOUND

#ifdef USE_SOUND
#include <FMOD/fmod.h>
#include <FMOD/wincompat.h>
#include "fmod.h"
#endif //USE_SOUND

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
    if (show_kangaroo) {

        //punch_image += 1;
        if (hop_image == 1 || hop_image == 2) {
            glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
            glPushMatrix();
            glTranslatef(x, y, z);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.0f);
            glBindTexture(GL_TEXTURE_2D, lowhopTexture);
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
        else if (hop_image == 3 || hop_image == 4) {
            glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
            glPushMatrix();
            glTranslatef(x, y, z);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.0f);
            glBindTexture(GL_TEXTURE_2D, highhopTexture);
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
            hop_image = 0;
        }
    }
}

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
        else {
            punch_image = 0;
        }
    }
}
void punchKey(void)
{
    Flt punch_dist, hit_dist;
    punch ^= 1;
    fmod_playsound(2);

    punch_dist = kangaroo.pos[0] + kangaroo.height2;
    hit_dist = rhino.pos[0] - rhino.height2;
    if (rhino.pos[1] >= (kangaroo.pos[1] - kangaroo.height2)
            && rhino.pos[1] <= (kangaroo.pos[1] + kangaroo.height2)) {
        if ((hit_dist - punch_dist) >= 25 && (hit_dist - punch_dist <= 75.0))
        {
            if (show_rhino && !show_ufo) {
                rhinoReset();
                high_score += 100;
            }
            if (show_ufo && !ufochoice) {
                high_score += 50;
            }
            if (high_score%5000 ==  0) {
                ufocount++;
                printf("%d\n", ufocount);
            }
            printf("len: %f height: %f\n", kangaroo.pos[0], kangaroo.pos[1]);
        }
    }
    hit_dist = animal.pos[0] - animal.height2;
    if (animal.pos[1] >= (kangaroo.pos[1] - kangaroo.height2)
            && animal.pos[1] <= (kangaroo.pos[1] + kangaroo.height2)) {
        if ((hit_dist - punch_dist) >= 25 && (hit_dist - punch_dist <= 75.0))
        {
            if (show_animal && !show_ufo) {
                animalReset();
                high_score += 500;
            }
            if (show_ufo && !ufochoice) {
                high_score += 50;
            }
            if (high_score%5000 ==  0) {
                ufocount++;
                printf("%d\n", ufocount);
            }
        }
    }

    applecollision();

    // Punch the kangaroo on the start page for 2 extra lives
    if(kangaroo.pos[0] == 462 && kangaroo.pos[1] == 127 && start) {
        fmod_playsound(3);
        lives = 5;
    }
}

void move_rhino()
{
    //auto move rhino...
    //int addgrav = 1;
    //Update position
    if(!show_ufo)
        rhino.pos[0] += rhino.vel[0]*1.5;
    rhino.pos[1] += rhino.vel[1];
    //Check for collision with window edges
    if ((rhino.pos[0] < -140.0 && rhino.vel[0] < 0.0) ||
            (rhino.pos[0] >= (float)xres+140.0 && rhino.vel[0] > 0.0)) {
        rhino.vel[0] = -rhino.vel[0];
        rhinoReset();
    }
}

void move_animal()
{
    //auto move animal...
    //int addgrav = 1;
    //Update position
    if(!show_ufo)
        animal.pos[0] += animal.vel[0]*2;
    animal.pos[1] += animal.vel[1];
    //Check for collision with window edges
    if ((animal.pos[0] < -140.0 && animal.vel[0] < 0.0) ||
            (animal.pos[0] >= (float)xres+140.0 && animal.vel[0] > 0.0)) {
        animal.vel[0] = -animal.vel[0];
        animalReset();
    }
}

void physics(void)
{
    Flt d0,d1,dist;
    Flt hit_dist;

    if(punch)
    {
        punch_image += 1;
        if(punch_image == 5)
        {
            punch_image = 0;
            punch ^= 1;
        }
    }

    if(hop)
    {
        hop_image += 1;
        if(hop_image == 5)
        {
            hop_image = 0;
            hop ^= 1;
        }
    }

    if(yellow)
    {
        yellow_image += 1;
        if(yellow_image == 8)
        {
            yellow_image = 0;
            yellow ^= 1;
        }
    }

    if(white)
    {
        white_image += 1;
        if(white_image == 8)
        {
            white_image = 0;
            white ^= 1;
        }
    }

    if (show_rhino && !show_ufo) {
        move_rhino();
        hit_dist = rhino.pos[0] - 100.0;
        d0 = kangaroo.pos[0] - hit_dist;
        d1 = kangaroo.pos[1] - rhino.pos[1];
        dist = d0*d0+d1*d1;
        if (dist < (50.0*50.0)) {
            kangarooDeath();
        }
    }
    if (show_animal && !show_ufo) {
        move_animal();
        hit_dist = animal.pos[0] - 100.0;
        d0 = kangaroo.pos[0] - hit_dist;
        d1 = kangaroo.pos[1] - animal.pos[1];
        dist = d0*d0+d1*d1;
        if (dist < (50.0*50.0)) {
            kangarooDeath();
        }
    }

    applePhysics();

    if (show_ufo)
        move_ufo();
}


