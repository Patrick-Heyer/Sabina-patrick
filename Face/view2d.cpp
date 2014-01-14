/**
 * $Id: interface_draw.c 26841 2010-02-12 13:34:04Z campbellbarton $
 *
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2001-2002 by NaN Holding BV.
 * All rights reserved.
 *
 * Contributor(s): Blender Foundation
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#include <math.h>
#include <string>

#include "view2d.h"

#include <GL/glut.h>

void setOrthographicProjection(int X, int Y) {

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, X, 0, Y);
    glScalef(1, -1, 1);
    glTranslatef(0, -Y, 0);
    glMatrixMode(GL_MODELVIEW);
}

void resetPerspectiveProjection() {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
void drawText(int x, int y, std::string text)
{
    char c;
    float color[4];
    glEnable(GL_FOG);
    glGetFloatv(GL_CURRENT_COLOR, color);



    glDisable ( GL_TEXTURE_2D );
    glColor4f (0,0,0,1);
    glRasterPos2f (x,y );

    for ( c=0; c<text.length(); c++ )
    {
        if(text[c]=='\n')
        {
            glRasterPos2f (x,y+=12 );
        }
        else
        {
            glutBitmapCharacter ( GLUT_BITMAP_HELVETICA_12, text[c] );
        }
    }
    glDisable(GL_FOG);
    glColor4fv(color);

}

bool isOver ( int x, int y, int x1, int y1, int x2, int y2 )
{
    if (x>=x1 && x<=x2+20 && y>=y1 && y<=y2)
        return true;
    else return false;

}


void uiSetRoundBox(int type)
{
    /* Not sure the roundbox function is the best place to change this
     * if this is undone, its not that big a deal, only makes curves edges
     * square for the  */
    roundboxtype= type;

    /* flags to set which corners will become rounded:

    1------2
    |      |
    8------4
    */

}

int uiGetRoundBox(void)
{
    return roundboxtype;
}

void gl_round_box(int mode, float minx, float miny, float maxx, float maxy, float rad)
{
    float vec[7][2]= {{0.195, 0.02}, {0.383, 0.067}, {0.55, 0.169}, {0.707, 0.293},
        {0.831, 0.45}, {0.924, 0.617}, {0.98, 0.805}
    };
    int a;

    /* mult */
    for(a=0; a<7; a++) {
        vec[a][0]*= rad;
        vec[a][1]*= rad;
    }

    glBegin(mode);

    /* start with corner right-bottom */
    if(roundboxtype & 4) {
        glVertex2f(maxx-rad, miny);
        for(a=0; a<7; a++) {
            glVertex2f(maxx-rad+vec[a][0], miny+vec[a][1]);
        }
        glVertex2f(maxx, miny+rad);
    }
    else glVertex2f(maxx, miny);

    /* corner right-top */
    if(roundboxtype & 2) {
        glVertex2f(maxx, maxy-rad);
        for(a=0; a<7; a++) {
            glVertex2f(maxx-vec[a][1], maxy-rad+vec[a][0]);
        }
        glVertex2f(maxx-rad, maxy);
    }
    else glVertex2f(maxx, maxy);

    /* corner left-top */
    if(roundboxtype & 1) {
        glVertex2f(minx+rad, maxy);
        for(a=0; a<7; a++) {
            glVertex2f(minx+rad-vec[a][0], maxy-vec[a][1]);
        }
        glVertex2f(minx, maxy-rad);
    }
    else glVertex2f(minx, maxy);

    /* corner left-bottom */
    if(roundboxtype & 8) {
        glVertex2f(minx, miny+rad);
        for(a=0; a<7; a++) {
            glVertex2f(minx+vec[a][1], miny+rad-vec[a][0]);
        }
        glVertex2f(minx+rad, miny);
    }
    else glVertex2f(minx, miny);

    glEnd();
}

void round_box_shade_col(float *col1, float *col2, float fac)
{
    float col[3];

    col[0]= (fac*col1[0] + (1.0-fac)*col2[0]);
    col[1]= (fac*col1[1] + (1.0-fac)*col2[1]);
    col[2]= (fac*col1[2] + (1.0-fac)*col2[2]);

    glColor3fv(col);
}


/* linear horizontal shade within button or in outline */
/* view2d scrollers use it */
void gl_round_box_shade(int mode, float minx, float miny, float maxx, float maxy, float rad, float shadetop, float shadedown)
{
    float vec[7][2]= {{0.195, 0.02}, {0.383, 0.067}, {0.55, 0.169}, {0.707, 0.293},
        {0.831, 0.45}, {0.924, 0.617}, {0.98, 0.805}
    };
    float div= maxy-miny;
    float coltop[3], coldown[3], color[4];
    int a;

    /* mult */
    for(a=0; a<7; a++) {
        vec[a][0]*= rad;
        vec[a][1]*= rad;
    }
    /* get current color, needs to be outside of glBegin/End */
    glGetFloatv(GL_CURRENT_COLOR, color);

    /* 'shade' defines strength of shading */
    coltop[0]= color[0]+shadetop;
    if(coltop[0]>1.0) coltop[0]= 1.0;
    coltop[1]= color[1]+shadetop;
    if(coltop[1]>1.0) coltop[1]= 1.0;
    coltop[2]= color[2]+shadetop;
    if(coltop[2]>1.0) coltop[2]= 1.0;
    coldown[0]= color[0]+shadedown;
    if(coldown[0]<0.0) coldown[0]= 0.0;
    coldown[1]= color[1]+shadedown;
    if(coldown[1]<0.0) coldown[1]= 0.0;
    coldown[2]= color[2]+shadedown;
    if(coldown[2]<0.0) coldown[2]= 0.0;

    glShadeModel(GL_SMOOTH);
    glBegin(mode);

    /* start with corner right-bottom */
    if(roundboxtype & 4) {

        round_box_shade_col(coltop, coldown, 0.0);
        glVertex2f(maxx-rad, miny);

        for(a=0; a<7; a++) {
            round_box_shade_col(coltop, coldown, vec[a][1]/div);
            glVertex2f(maxx-rad+vec[a][0], miny+vec[a][1]);
        }

        round_box_shade_col(coltop, coldown, rad/div);
        glVertex2f(maxx, miny+rad);
    }
    else {
        round_box_shade_col(coltop, coldown, 0.0);
        glVertex2f(maxx, miny);
    }

    /* corner right-top */
    if(roundboxtype & 2) {

        round_box_shade_col(coltop, coldown, (div-rad)/div);
        glVertex2f(maxx, maxy-rad);

        for(a=0; a<7; a++) {
            round_box_shade_col(coltop, coldown, (div-rad+vec[a][1])/div);
            glVertex2f(maxx-vec[a][1], maxy-rad+vec[a][0]);
        }
        round_box_shade_col(coltop, coldown, 1.0);
        glVertex2f(maxx-rad, maxy);
    }
    else {
        round_box_shade_col(coltop, coldown, 1.0);
        glVertex2f(maxx, maxy);
    }

    /* corner left-top */
    if(roundboxtype & 1) {

        round_box_shade_col(coltop, coldown, 1.0);
        glVertex2f(minx+rad, maxy);

        for(a=0; a<7; a++) {
            round_box_shade_col(coltop, coldown, (div-vec[a][1])/div);
            glVertex2f(minx+rad-vec[a][0], maxy-vec[a][1]);
        }

        round_box_shade_col(coltop, coldown, (div-rad)/div);
        glVertex2f(minx, maxy-rad);
    }
    else {
        round_box_shade_col(coltop, coldown, 1.0);
        glVertex2f(minx, maxy);
    }

    /* corner left-bottom */
    if(roundboxtype & 8) {

        round_box_shade_col(coltop, coldown, rad/div);
        glVertex2f(minx, miny+rad);

        for(a=0; a<7; a++) {
            round_box_shade_col(coltop, coldown, (rad-vec[a][1])/div);
            glVertex2f(minx+vec[a][1], miny+rad-vec[a][0]);
        }

        round_box_shade_col(coltop, coldown, 0.0);
        glVertex2f(minx+rad, miny);
    }
    else {
        round_box_shade_col(coltop, coldown, 0.0);
        glVertex2f(minx, miny);
    }

    glEnd();
    glShadeModel(GL_FLAT);
}

/* linear vertical shade within button or in outline */
/* view2d scrollers use it */
void gl_round_box_vertical_shade(int mode, float minx, float miny, float maxx, float maxy, float rad, float shadeLeft, float shadeRight)
{
    float vec[7][2]= {{0.195, 0.02}, {0.383, 0.067}, {0.55, 0.169}, {0.707, 0.293},
        {0.831, 0.45}, {0.924, 0.617}, {0.98, 0.805}
    };
    float div= maxx-minx;
    float colLeft[3], colRight[3], color[4];
    int a;

    /* mult */
    for(a=0; a<7; a++) {
        vec[a][0]*= rad;
        vec[a][1]*= rad;
    }
    /* get current color, needs to be outside of glBegin/End */
    glGetFloatv(GL_CURRENT_COLOR, color);

    /* 'shade' defines strength of shading */
    colLeft[0]= color[0]+shadeLeft;
    if(colLeft[0]>1.0) colLeft[0]= 1.0;
    colLeft[1]= color[1]+shadeLeft;
    if(colLeft[1]>1.0) colLeft[1]= 1.0;
    colLeft[2]= color[2]+shadeLeft;
    if(colLeft[2]>1.0) colLeft[2]= 1.0;
    colRight[0]= color[0]+shadeRight;
    if(colRight[0]<0.0) colRight[0]= 0.0;
    colRight[1]= color[1]+shadeRight;
    if(colRight[1]<0.0) colRight[1]= 0.0;
    colRight[2]= color[2]+shadeRight;
    if(colRight[2]<0.0) colRight[2]= 0.0;

    glShadeModel(GL_SMOOTH);
    glBegin(mode);

    /* start with corner right-bottom */
    if(roundboxtype & 4) {
        round_box_shade_col(colLeft, colRight, 0.0);
        glVertex2f(maxx-rad, miny);

        for(a=0; a<7; a++) {
            round_box_shade_col(colLeft, colRight, vec[a][0]/div);
            glVertex2f(maxx-rad+vec[a][0], miny+vec[a][1]);
        }

        round_box_shade_col(colLeft, colRight, rad/div);
        glVertex2f(maxx, miny+rad);
    }
    else {
        round_box_shade_col(colLeft, colRight, 0.0);
        glVertex2f(maxx, miny);
    }

    /* corner right-top */
    if(roundboxtype & 2) {
        round_box_shade_col(colLeft, colRight, 0.0);
        glVertex2f(maxx, maxy-rad);

        for(a=0; a<7; a++) {

            round_box_shade_col(colLeft, colRight, (div-rad-vec[a][0])/div);
            glVertex2f(maxx-vec[a][1], maxy-rad+vec[a][0]);
        }
        round_box_shade_col(colLeft, colRight, (div-rad)/div);
        glVertex2f(maxx-rad, maxy);
    }
    else {
        round_box_shade_col(colLeft, colRight, 0.0);
        glVertex2f(maxx, maxy);
    }

    /* corner left-top */
    if(roundboxtype & 1) {
        round_box_shade_col(colLeft, colRight, (div-rad)/div);
        glVertex2f(minx+rad, maxy);

        for(a=0; a<7; a++) {
            round_box_shade_col(colLeft, colRight, (div-rad+vec[a][0])/div);
            glVertex2f(minx+rad-vec[a][0], maxy-vec[a][1]);
        }

        round_box_shade_col(colLeft, colRight, 1.0);
        glVertex2f(minx, maxy-rad);
    }
    else {
        round_box_shade_col(colLeft, colRight, 1.0);
        glVertex2f(minx, maxy);
    }

    /* corner left-bottom */
    if(roundboxtype & 8) {
        round_box_shade_col(colLeft, colRight, 1.0);
        glVertex2f(minx, miny+rad);

        for(a=0; a<7; a++) {
            round_box_shade_col(colLeft, colRight, (vec[a][0])/div);
            glVertex2f(minx+vec[a][1], miny+rad-vec[a][0]);
        }

        round_box_shade_col(colLeft, colRight, 1.0);
        glVertex2f(minx+rad, miny);
    }
    else {
        round_box_shade_col(colLeft, colRight, 1.0);
        glVertex2f(minx, miny);
    }

    glEnd();
    glShadeModel(GL_FLAT);
}

/* plain antialiased unfilled rectangle */
void uiRoundRect(float minx, float miny, float maxx, float maxy, float rad)
{
    float color[4];

    if(roundboxtype & UI_RB_ALPHA) {
        glGetFloatv(GL_CURRENT_COLOR, color);
        color[3]= 0.5;
        glColor4fv(color);
        glEnable( GL_BLEND );
    }

    /* set antialias line */
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_BLEND );

    gl_round_box(GL_LINE_LOOP, minx, miny, maxx, maxy, rad);

    glDisable( GL_BLEND );
    glDisable( GL_LINE_SMOOTH );
}


/* (old, used in outliner) plain antialiased filled box */
void uiRoundBox(float minx, float miny, float maxx, float maxy, float rad)
{
    float color[4];

    if(roundboxtype & UI_RB_ALPHA) {
        glGetFloatv(GL_CURRENT_COLOR, color);
        color[3]= 0.5;
        glColor4fv(color);
        glEnable( GL_BLEND );
    }

    /* set antialias line */
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_BLEND );

    gl_round_box(GL_LINE_LOOP, minx, miny, maxx, maxy, rad);


    glGetFloatv(GL_CURRENT_COLOR, color);
    color[0]+= 0.5;
    color[1]+= 0.5;
    color[2]+= 0.5;
    color[3]= 0.4;
    glColor4fv(color);
    glDisable( GL_LINE_SMOOTH );

    /* solid part */
    gl_round_box(GL_POLYGON, minx, miny, maxx, maxy, rad);

    glDisable( GL_BLEND );

}



