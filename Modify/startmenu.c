#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glx.h>
#include "startmenu.h"
#include "ppm.h"
#include "xwin.h"

int done = 0;
int lbutton = 0;
int rbutton = 0;
int nbuttons = 0;

void StartMenu(void)
{
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, startTexture);
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
    if (action == 1)
    {
        int i=0;
        //center of a grid

        for (i=0; i<nbuttons; i++) {
            if (button[i].over) {
                button[i].down = 1;
                button[i].click = 1;
                if (i==0) {
                    //user clicked button 0
                    printf("Hello Rhino \n");
                    show_rhino ^= 1;
                }
                if (i==1) {
                    //user clicked button 0
                    printf("Hello Kangaroo \n");
                    show_kangaroo ^= 1;
                }
                if (i==2) {
                    //user clicked QUIT
                    done = 1;
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
            glColor3fv(button[i].dcolor);
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



