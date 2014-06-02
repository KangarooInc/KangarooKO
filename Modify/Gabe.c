#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <GL/glx.h>
#include"xwin.h"
#include"fonts.h"
#include"struct.h"
#include"Gabe.h"
#include "ppm.h"

void ufoFreeze(void)
{
    if(ufo.pos[1] <= 400)
    {
        ufocount--;
        show_ufo = 0;
        ufoReset();
        animalReset();
        rhinoReset();
    }
}

void ufoFlash(void)
{
    yellow ^= 1;
    if(ufo.pos[1] <= 400)
    {
        ufocount--;
        lives++;
        show_ufo = 0;
        ufoReset();
        animalReset();
        rhinoReset();
    }
}

void ufoReset(void)
{
    ufo.pos[1] = yres + 50;
}

void init_ufo(void)
{
    MakeVector(300.0,600.0,0.0, ufo.pos);
    MakeVector(0.0,-6.0,0.0, ufo.vel);
}

void move_ufo(void)
{
    if(show_ufo)
    {
        ufo.pos[1] += ufo.vel[1] + 5;


        if (ufochoice == 0)
        {
            ufoFreeze();

        }

        else if (ufochoice== 1)
        {
            ufoFlash();
        }
    }
}

void draw_yellow(void)
{
    if(show_ufo)
    {
        if (yellow_image == 1) {
            glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, YellowTexture);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
            glEnd();
            glPopMatrix();
        }
        else if (yellow_image == 8)
        {
            yellow_image = 0;
        }
    }
}

void draw_ufo(void)
{
    if(show_ufo)
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
}

void ufoOpenGL(void) {

    ufoImage        = ppm6GetImage("./images/aliens.ppm");
    yellowImage     = ppm6GetImage("./images/yellow.ppm");

    //create opengl texture elements
    glGenTextures(1, &UFOTexture);
    glGenTextures(1, &YellowTexture);


    //-------------------------------------------------------------------------
    //
    // UFO
    //
    int w3 = ufoImage->width;
    int h3 = ufoImage->height;

    glBindTexture(GL_TEXTURE_2D, UFOTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    Transparent = buildAlphaData(ufoImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w3, h3, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, Transparent);
    free(Transparent);


    //-------------------------------------------------------------------------
    //
    // Alien flash
    //
    glBindTexture(GL_TEXTURE_2D, YellowTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
            yellowImage->width, yellowImage->height,
            0, GL_RGB, GL_UNSIGNED_BYTE, yellowImage->data);
}


//at end of game, send users score

/*void fileWrite(unsigned double userScore)
  {

  ifstream fin;            // declare ifstream
  ofstream fout;

//File stuff
char iFile[56];
char oFile[60];

cout << "ENTER FILE NAME: " ;
cin >> iFile;
fin.open(iFile);

if(fin.fail()) {
//Fail silently
printf("Opening of file didnt work.\n\n");

exit(1);
}

strcpy(oFile, iFile);
strncat(oFile, ".out", 56 - strlen(oFile));//con catinate

fout.open(oFile);
if(fout.fail()) {
printf("Closing of file didnt work.\n\n");

exit(1);
}

reverseFile(fin, fout);

fout << "\n\n\t---End Program Execution-----\n";
cout << endl;
cout << "EOF REACHED! \n ";

// Close Files
fin.close();
fout.close();
}*/
