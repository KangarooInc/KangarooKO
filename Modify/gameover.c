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
    show_animal = 0;
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, GameOverTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
    glEnd();
    glPopMatrix();

    GObuttonsInit();
    bRender();
}


void GOmouse_click(int action)
{
    if(!show_rhino && !show_kangaroo)
    {
        if (action == 1)
        {
            int i=0;
            //center of a grid

            for (i=0; i<nbuttons; i++) {
                if (GObutton[i].over) {
                    GObutton[i].down = 1;
                    GObutton[i].click = 1;
                    if (i==0) {
                        //user clicked Retry
                        restartGame();
                    }
                    if (i==1) {
                        //user clicked QUIT
                        done = 1;
                    }
                }
            }
        }
    }
}
void GOcheck_mouse(XEvent *e)
{
    static int savex = 0;
    static int savey = 0;
    //Log("check_mouse()...\n");
    int x,y;
    int i;
    lbutton=0;
    rbutton=0;
    if (e->type == ButtonRelease) {
        GOmouse_click(2);
        return;
    }
    if (e->type == ButtonPress) {
        if (e->xbutton.button==1) {
            //Left button is down
            lbutton=1;
        }
        if (e->xbutton.button==3) {
            //Right button is down
            rbutton=1;
        }
    }
    //Log("e->xbutton.x: %i e->xbutton.y: %i\n",e->xbutton.x,e->xbutton.y);
    x = e->xbutton.x;
    y = e->xbutton.y;
    //reverse the y position
    y = yres - y;
    if (savex != e->xbutton.x || savey != e->xbutton.y) {
        //Mouse moved
        savex = e->xbutton.x;
        savey = e->xbutton.y;
    }
    //Log("xy: %i %i\n",x,y);
    if (x == savex && y == savey)
        return;
    savex=x;
    savey=y;


    for (i=0; i<nbuttons; i++) {
        GObutton[i].over=0;
        GObutton[i].down=0;
        //Log("xy: %i %i\n",x,y);
        if (x >= GObutton[i].r.left &&
                x <= GObutton[i].r.right &&
                y >= GObutton[i].r.bot &&
                y <= GObutton[i].r.top) {
            GObutton[i].over=1;
            //Log("over button: %i\n", i);
            break;
        }
    }
    if (lbutton)
        GOmouse_click(1);
    if (rbutton)
        GOmouse_click(1);
}

void GObuttonsInit(void)
{
    nbuttons=0;
    //
    // Retry button
    //size and position
    GObutton[nbuttons].r.width = 200;
    GObutton[nbuttons].r.height = 50;
    GObutton[nbuttons].r.left = xres/2 - GObutton[nbuttons].r.width/2;
    GObutton[nbuttons].r.bot = 160;
    GObutton[nbuttons].r.right = GObutton[nbuttons].r.left + GObutton[nbuttons].r.width;
    GObutton[nbuttons].r.top = GObutton[nbuttons].r.bot + GObutton[nbuttons].r.height;
    GObutton[nbuttons].r.centerx = (GObutton[nbuttons].r.left + GObutton[nbuttons].r.right) / 2;
    GObutton[nbuttons].r.centery = (GObutton[nbuttons].r.bot + GObutton[nbuttons].r.top) / 2;
    strcpy(GObutton[nbuttons].text, "Retry");
    GObutton[nbuttons].down = 0;
    GObutton[nbuttons].click = 0;
    GObutton[nbuttons].color[0] = 0.4f;
    GObutton[nbuttons].color[1] = 0.4f;
    GObutton[nbuttons].color[2] = 0.7f;
    GObutton[nbuttons].dcolor[0] = GObutton[nbuttons].color[0] * 0.5f;
    GObutton[nbuttons].dcolor[1] = GObutton[nbuttons].color[1] * 0.5f;
    GObutton[nbuttons].dcolor[2] = GObutton[nbuttons].color[2] * 0.5f;
    GObutton[nbuttons].text_color = 0x00ffffff;
    nbuttons++;
    //
    // Quit button
    //size and position
    GObutton[nbuttons].r.width = 200;
    GObutton[nbuttons].r.height = 50;
    GObutton[nbuttons].r.left = xres/2 - GObutton[nbuttons].r.width/2;
    GObutton[nbuttons].r.bot = 85;
    GObutton[nbuttons].r.right = GObutton[nbuttons].r.left + GObutton[nbuttons].r.width;
    GObutton[nbuttons].r.top = GObutton[nbuttons].r.bot + GObutton[nbuttons].r.height;
    GObutton[nbuttons].r.centerx = (GObutton[nbuttons].r.left + GObutton[nbuttons].r.right) / 2;
    GObutton[nbuttons].r.centery = (GObutton[nbuttons].r.bot + GObutton[nbuttons].r.top) / 2;
    strcpy(GObutton[nbuttons].text, "Quit");
    GObutton[nbuttons].down = 0;
    GObutton[nbuttons].click = 0;
    GObutton[nbuttons].color[0] = 0.4f;
    GObutton[nbuttons].color[1] = 0.4f;
    GObutton[nbuttons].color[2] = 0.7f;
    GObutton[nbuttons].dcolor[0] = GObutton[nbuttons].color[0] * 0.5f;
    GObutton[nbuttons].dcolor[1] = GObutton[nbuttons].color[1] * 0.5f;
    GObutton[nbuttons].dcolor[2] = GObutton[nbuttons].color[2] * 0.5f;
    GObutton[nbuttons].text_color = 0x00ffffff;
    nbuttons++;
}

void bRender(void)
{
    int i;
    Rect r;
    for (i=0; i<nbuttons; i++) {
        if (GObutton[i].over) {
            glColor3f(1.0f, 0.0f, 0.0f);
            //draw a highlight around button
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
            glVertex2i(GObutton[i].r.left-2, GObutton[i].r.bot-2);
            glVertex2i(GObutton[i].r.left-2, GObutton[i].r.top+2);
            glVertex2i(GObutton[i].r.right+2, GObutton[i].r.top+2);
            glVertex2i(GObutton[i].r.right+2, GObutton[i].r.bot-2);
            glVertex2i(GObutton[i].r.left-2, GObutton[i].r.bot-2);
            glEnd();
            glLineWidth(1);
        }
        if (GObutton[i].down) {
            glColor3fv(GObutton[i].dcolor);
        } else {
            glColor3fv(GObutton[i].color);
        }
        glBegin(GL_QUADS);
        glVertex2i(GObutton[i].r.left, GObutton[i].r.bot);
        glVertex2i(GObutton[i].r.left, GObutton[i].r.top);
        glVertex2i(GObutton[i].r.right, GObutton[i].r.top);
        glVertex2i(GObutton[i].r.right, GObutton[i].r.bot);
        glEnd();
        r.left = GObutton[i].r.centerx;
        r.bot = GObutton[i].r.centery-8;
        r.center = 1;
        if (GObutton[i].down) {
            ggprint16(&r, 0, GObutton[i].text_color, "Pressed!");
        } else {
            ggprint16(&r, 0, GObutton[i].text_color, GObutton[i].text);
        }
    }
}
