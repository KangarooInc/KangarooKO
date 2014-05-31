#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glx.h>
#include "ppm.h"
#include "xwin.h"
#include "Jose.h"
#include "struct.h"
#include "hop.h"
#include "punch.h"

extern double setLevel;
extern double setMountain;

void restartGame()
{
    lives = 3;
    high_score = 0;
    rhinoReset();
    animalReset();
    kangarooReset();
}

void draw_kangaroo(void)
{
    //Log("draw_kangaroo()...\n");
    glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
    glPushMatrix();
    glTranslatef(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBindTexture(GL_TEXTURE_2D, KangarooTexture);
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

void draw_rhino(void)
{
    glPushMatrix();
    glTranslatef(rhino.pos[0], rhino.pos[1], rhino.pos[2]);

    glBindTexture(GL_TEXTURE_2D, RhinoTexture);
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

void draw_animal(void)
{
    glPushMatrix();
    glTranslatef(animal.pos[0], animal.pos[1], animal.pos[2]);

    glBindTexture(GL_TEXTURE_2D, AnimalTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    //This is for the animal image going right to left
    glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
    glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
    //#################################################
    glEnd();
    glPopMatrix();
    glDisable(GL_ALPHA_TEST);
}

void draw_ufo(void)
{
    glPushMatrix();
    glTranslatef(ufo.pos[0], ufo.pos[1], ufo.pos[2]);
    glBindTexture(GL_TEXTURE_2D, UFOTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    if (ufo.vel[0] > 0.0) {
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
        glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
    } else {
        glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
        glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
    }
    glEnd();
    glPopMatrix();
    glDisable(GL_ALPHA_TEST);
}

void draw_white(void)
{
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, WhiteTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
    glEnd();
    glPopMatrix();
}

void draw_background(void)
{

    double *yOFFsetLevel = &setLevel;
    double *yOFFsetMountain = &setMountain;

    if(!start)
    {
        *yOFFsetLevel -= 1;
        /*if(*yOFFsetLevel>100.0)*/
        /**yOFFsetLevel -=100.0;*/

        *yOFFsetMountain -= .25;
        /*if(*yOFFsetMountain>100.0)*/
        /**yOFFsetMountain -=100.0;*/
    }

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, MountainTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glTranslatef(*yOFFsetMountain, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(xres*5, yres);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(xres*5, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, LevelTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glTranslatef(*yOFFsetLevel, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(xres*5, yres);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(xres*5, 0);
    glEnd();
    glPopMatrix();
}

void perspective(void)
{

    /////////////////////////////////////////////////////////////
    //For the Perspective
    if (kangaroo.pos[1] < rhino.pos[1])
    {
        if (kangaroo.pos[1] < animal.pos[1])
        {
            if (animal.pos[1] < rhino.pos[1]){
                if (show_rhino) {
                    draw_rhino();
                }
                if (show_animal) {
                    draw_animal();
                }
                if (show_kangaroo) {
                    draw_kangaroo();
                    punch_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                    hop_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                }
            }
            else if (animal.pos[1] >= rhino.pos[1]){
                if (show_animal) {
                    draw_animal();
                }
                if (show_rhino) {
                    draw_rhino();
                }
                if (show_kangaroo) {
                    draw_kangaroo();
                    punch_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                    hop_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                }
            }
        }

        else if (kangaroo.pos[1] >= animal.pos[1])
        {
            if (animal.pos[1] < rhino.pos[1]){
                if (show_rhino) {
                    draw_rhino();
                }
                if (show_kangaroo) {
                    draw_kangaroo();
                    punch_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                    hop_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                }
                if (show_animal) {
                    draw_animal();
                }
            }
            else if (animal.pos[1] >= rhino.pos[1]){
                if (show_kangaroo) {
                    draw_kangaroo();
                    punch_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                    hop_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                }
                if (show_animal) {
                    draw_animal();
                }
                if (show_rhino) {
                    draw_rhino();
                }
            }
        }
    }
    //////////////////////////////////////////

    else if (kangaroo.pos[1] >= rhino.pos[1])
    {
        if (kangaroo.pos[1] >= animal.pos[1])
        {
            if (animal.pos[1] >= rhino.pos[1]){
                if (show_kangaroo) {
                    draw_kangaroo();
                    punch_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                    hop_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                }
                if (show_animal) {
                    draw_animal();
                }
                if (show_rhino) {
                    draw_rhino();
                }
            }
            else if (animal.pos[1] < rhino.pos[1]){
                if (show_kangaroo) {
                    draw_kangaroo();
                    punch_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                    hop_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                }
                if (show_rhino) {
                    draw_rhino();
                }
                if (show_animal) {
                    draw_animal();
                }
            }
        }

        else if (kangaroo.pos[1] < animal.pos[1])
        {
            if (animal.pos[1] >= rhino.pos[1]){
                if (show_animal) {
                    draw_animal();
                }
                if (show_kangaroo) {
                    draw_kangaroo();
                    punch_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                    hop_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                }
                if (show_rhino) {
                    draw_rhino();
                }
            }
            else if (animal.pos[1] < rhino.pos[1]){
                if (show_rhino) {
                    draw_rhino();
                }
                if (show_animal) {
                    draw_animal();
                }
                if (show_kangaroo) {
                    draw_kangaroo();
                    punch_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                    hop_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                }
            }
        }
    }
}

void StartMenu(void)
{
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, StartTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
    glEnd();
    glPopMatrix();

    buttonRender();
    buttonsInit();
}

void mouse_click(int action)
{
    if(start)
    {
        if (action == 1)
        {
            int i=0;
            //center of a grid

            for (i=0; i<nbuttons; i++) {
                if (button[i].over) {
                    button[i].down = 1;
                    button[i].click = 1;
                    if (i==0) {
                        //user clicked TOP button (start game)
                        start = 0;
                        rhinoReset();
                        animalReset();
                        kangarooReset();
                    }
                    if (i==1) {
                        //user clicked MIDDLE button (highscore)
                        printf("Hello Kangaroo \n");
                    }
                    if (i==2) {
                        //user clicked QUIT
                        done = 1;
                    }
                }
            }
        }
    }
}

void check_mouse(XEvent *e)
{
    static int savex = 0;
    static int savey = 0;
    //Log("check_mouse()...\n");
    int x,y;
    int i;
    lbutton=0;
    rbutton=0;
    if (e->type == ButtonRelease) {
        mouse_click(2);
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
        button[i].over=0;
        button[i].down=0;
        //Log("xy: %i %i\n",x,y);
        if (x >= button[i].r.left &&
                x <= button[i].r.right &&
                y >= button[i].r.bot &&
                y <= button[i].r.top) {
            button[i].over=1;
            //Log("over button: %i\n", i);
            break;
        }
    }
    if (lbutton)
        mouse_click(1);
    if (rbutton)
        mouse_click(1);
}

void buttonRender (void)
{
    int i;
    Rect r;
    for (i=0; i<nbuttons; i++) {
        if (button[i].over) {
            glColor3f(1.0f, 0.0f, 0.0f);
            //draw a highlight around button
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
            glVertex2i(button[i].r.left-2,  button[i].r.bot-2);
            glVertex2i(button[i].r.left-2,  button[i].r.top+2);
            glVertex2i(button[i].r.right+2, button[i].r.top+2);
            glVertex2i(button[i].r.right+2, button[i].r.bot-2);
            glVertex2i(button[i].r.left-2,  button[i].r.bot-2);
            glEnd();
            glLineWidth(1);
        }
        if (button[i].down) {
            glColor3fv(button[i].color);
        } else {
            glColor3fv(button[i].color);
        }
        glBegin(GL_QUADS);
        glVertex2i(button[i].r.left,  button[i].r.bot);
        glVertex2i(button[i].r.left,  button[i].r.top);
        glVertex2i(button[i].r.right, button[i].r.top);
        glVertex2i(button[i].r.right, button[i].r.bot);
        glEnd();
        r.left = button[i].r.centerx;
        r.bot  = button[i].r.centery-8;
        r.center = 1;
        if (button[i].down) {
            ggprint16(&r, 0, button[i].text_color, "Pressed!");
        } else {
            ggprint16(&r, 0, button[i].text_color, button[i].text);
        }
    }
}

void buttonsInit(void)
{
    nbuttons=0;
    //
    //Start Game
    //size and position
    button[nbuttons].r.width = 200;
    button[nbuttons].r.height = 50;
    button[nbuttons].r.left = xres/2 - button[nbuttons].r.width/2;
    button[nbuttons].r.bot = 160;
    button[nbuttons].r.right = button[nbuttons].r.left + button[nbuttons].r.width;
    button[nbuttons].r.top = button[nbuttons].r.bot + button[nbuttons].r.height;
    button[nbuttons].r.centerx = (button[nbuttons].r.left + button[nbuttons].r.right) / 2;
    button[nbuttons].r.centery = (button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
    strcpy(button[nbuttons].text, "Start Game");
    button[nbuttons].down = 0;
    button[nbuttons].click = 0;
    button[nbuttons].color[0] = 0.4f;
    button[nbuttons].color[1] = 0.4f;
    button[nbuttons].color[2] = 0.7f;
    button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
    button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
    button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
    button[nbuttons].text_color = 0x00ffffff;
    nbuttons++;
    //
    //High Score
    //size and position
    button[nbuttons].r.width = 200;
    button[nbuttons].r.height = 50;
    button[nbuttons].r.left = xres/2 - button[nbuttons].r.width/2;
    button[nbuttons].r.bot = 85;
    button[nbuttons].r.right = button[nbuttons].r.left + button[nbuttons].r.width;
    button[nbuttons].r.top = button[nbuttons].r.bot + button[nbuttons].r.height;
    button[nbuttons].r.centerx = (button[nbuttons].r.left + button[nbuttons].r.right) / 2;
    button[nbuttons].r.centery = (button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
    strcpy(button[nbuttons].text, "High Score");
    button[nbuttons].down = 0;
    button[nbuttons].click = 0;
    button[nbuttons].color[0] = 0.4f;
    button[nbuttons].color[1] = 0.4f;
    button[nbuttons].color[2] = 0.7f;
    button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
    button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
    button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
    button[nbuttons].text_color = 0x00ffffff;
    nbuttons++;
    //
    //Quit button
    //size and position
    button[nbuttons].r.width = 200;
    button[nbuttons].r.height = 50;
    button[nbuttons].r.left = xres/2 - button[nbuttons].r.width/2;
    button[nbuttons].r.bot = 10;
    button[nbuttons].r.right = button[nbuttons].r.left + button[nbuttons].r.width;
    button[nbuttons].r.top = button[nbuttons].r.bot + button[nbuttons].r.height;
    button[nbuttons].r.centerx = (button[nbuttons].r.left + button[nbuttons].r.right) / 2;
    button[nbuttons].r.centery = (button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
    strcpy(button[nbuttons].text, "Quit");
    button[nbuttons].down = 0;
    button[nbuttons].click = 0;
    button[nbuttons].color[0] = 0.4f;
    button[nbuttons].color[1] = 0.4f;
    button[nbuttons].color[2] = 0.7f;
    button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
    button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
    button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
    button[nbuttons].text_color = 0x00ffffff;
    nbuttons++;
}

