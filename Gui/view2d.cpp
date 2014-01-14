/*
 < *one line to* give the program's name and a brief idea of what it does.>
 Copyright (C) 2011  <Patrick Heyer> <patrickhey@prodigy.net.mx>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <math.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sstream>


#include "view2d.h"

#include <GL/glut.h>

std::string RGB_to_Hex(color3 color)
{

              std::string result="";
        
              char r[255];
              sprintf(r, "%.2X", (int)(color.r*255));
              result.append(r );
        
              char g[255];
              sprintf(g, "%.2X", (int)(color.g*255));
              result.append(g );
        
              char b[255];
              sprintf(b, "%.2X", (int)(color.b*255));
              result.append(b );
        
              return result;

}

std::string RGB_to_Hex(color4 color)
{
    
    std::string result="";
    
    char r[255];
    sprintf(r, "%.2X", (int)(color.r*255));
    result.append(r );
    
    char g[255];
    sprintf(g, "%.2X", (int)(color.g*255));
    result.append(g );
    
    char b[255];
    sprintf(b, "%.2X", (int)(color.b*255));
    result.append(b );

    char a[255];
    sprintf(a, "%.2X", (int)(color.a*255));
    result.append(a );
    
    return result;
    
}

color3 Hex_to_RGB(const char * hex)
{
    unsigned long val;
    color3 col;
    val = strtoul(hex, NULL, 16);
    col=Hex_to_RGB(val);
    if (strlen(hex) !=6)
    {
        col.r = col.g = col.b = 0.0;
        return col;
    }

    return col;
}

color4 Hex_to_RGBA(const char* hex)
{
    unsigned long val;
    color4 col;

    val = strtoul(hex, NULL, 16);

    col=Hex_to_RGBA(val);
    if (strlen(hex) ==6)
    {
        col.a=1;
    }
    else if (strlen(hex) !=8)
    {
        col.r = col.g = col.b = 0.0;
        col.a=1;
        return col;
    }
    return col;
}

color3 Hex_to_RGB(long unsigned int hex)
{
    color3 col;
    col.r = ((float)((hex & 0xff0000) >> 16)) / 255.0;
    col.g = ((float)((hex & 0xff00) >> 8)) / 255.0;
    col.b = ((float)(hex & 0xff)) / 255.0;
    return col;
}

color4 Hex_to_RGBA(long unsigned int hex)
{
    color4 col;
    col.a = ((float)((hex & 0xff000000) >> 24)) / 255.0;
    col.r = ((float)((hex & 0xff0000) >> 16)) / 255.0;
    col.g = ((float)((hex & 0xff00) >> 8)) / 255.0;
    col.b = ((float)(hex & 0xff)) / 255.0;
    return col;
}

color3 RGB(float r, float g, float b)
{
    color3 col;
    col.r = r;
    col.g = g;
    col.b = b;
    return col;
}

color4 RGBA(float r, float g, float b, float a)
{
    color4 col;
    col.r = r;
    col.g = g;
    col.b = b;
    col.a = a;
    return col;
}

color3 RGB(float rgb[3])
{
    color3 col;
    col.r = rgb[0];
    col.g = rgb[1];
    col.b = rgb[2];
    return col;
}

color4 RGBA(float rgba[4])
{
    color4 col;
    col.r = rgba[0];
    col.g = rgba[1];
    col.b = rgba[2];
    col.a = rgba[4];
    return col;
}

colorhsv rgb2hsv(int r, int g, int b)
{
    float min, max, delta, rc, gc, bc;
    colorhsv temp;
    
    rc = (float)r / 255.0;
    gc = (float)g / 255.0;
    bc = (float)b / 255.0;
    max = std::max(rc, std::max(gc, bc));
    min = std::min(rc, std::min(gc, bc));
    delta = max - min;
    temp.v = max;
    
    if (max != 0.0)
        temp.s = delta / max;
    else
        temp.s = 0.0;
    
    if (temp.s == 0.0) {
        temp.h = 0.0;
    }
    else {
        if (rc == max)
            temp.h = (gc - bc) / delta;
        else if (gc == max)
            temp.h = 2 + (bc - rc) / delta;
        else if (bc == max)
            temp.h = 4 + (rc - gc) / delta;
        
        temp.h= 60.0;
        if (temp.h < 0)
            temp.h += 360.0;
    }
}

color3 hsv2rgb(double h, double s, double l)
{
	double r, g, b;
	double temp1, temp2, tempr, tempg, tempb;
	
	h =h / 360.0;  //Hue is represented as a range of 360 degrees

	
	if (s == 0)         //Saturation of 0 means a shade of grey
    {
		r = g = b = l;
	}
	else                //
    {
		if (l < 0.5) temp2 = l * (1.0 + s);
		else temp2 = (l + s) - (l * s);
		
		temp1 = 2.0 * l - temp2;
		tempr = h + 1.0 / 3.0;
		if (tempr > 1.0) tempr-= 1.0;
		tempg = h;
		tempb = h - 1.0 / 3.0;
		if (tempb < 0) tempb += 1.0; 
		
		// Calculate red value:     
			if (6.0 * tempr < 1.0)
			{
				r = temp1 + (temp2 - temp1) * 6.0 * tempr;
			}
			else if (2.0 * tempr < 1.0)
			{
				r = temp2;
			}
			else if (3.0 * tempr < 2.0)
			{
				r = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempr) * 6.0;
			}
			else
			{
				r = temp1;
			}
			
			// Calculate green value       
			if (6.0 * tempg < 1.0)
			{
				g = temp1 + (temp2 - temp1) * 6.0 * tempg;
			}
			else if (2.0 * tempg < 1.0)
			{
				g = temp2;
			}
			else if (3.0 * tempg < 2.0)
			{
				g = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempg) * 6.0;
			}
			else
			{
				g = temp1; 
			}
			
			// Calculate blue value    
			if (6.0 * tempb < 1.0)
			{
				b = temp1 + (temp2 - temp1) * 6.0 * tempb;
			}
			else if (2.0 * tempb < 1.0)
			{
				b = temp2;
			}
			else if (3.0 * tempr < 2.0)
			{
				b = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempb) * 6.0;
			}
			else
			{
				g = temp1; 
			}
	}
	
	color3 TempColor;
	TempColor.r =r;
	TempColor.g =g;
	TempColor.b =b;
	
	
	return TempColor;
}

void setOrthographicProjection(int X, int Y) {

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, X, 0, Y);
    glScalef(1, -1, 1);
    glTranslatef(0, -Y, 0);
    glMatrixMode(GL_MODELVIEW);
    glDisable (GL_CULL_FACE);
}

void resetPerspectiveProjection() {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable (GL_CULL_FACE);
}

bool IsOver(int x, int y, int x1, int y1, int x2, int y2)
{
    if (x > x1 && x < x2 && y > y1 && y < y2) return true;
    else return false;
}

void drawText(int x, int y, std::string text, color4 text_color, int font)
{
    char c;
    float color[4];
    void *glut_font;
    switch (font)
    {
        case 1:
            glut_font= GLUT_BITMAP_9_BY_15;
            break;
        case 2:
            glut_font= GLUT_BITMAP_8_BY_13;
            break;
        case 3:
            glut_font= GLUT_BITMAP_TIMES_ROMAN_10;
            break;
        case 4:
            glut_font= GLUT_BITMAP_TIMES_ROMAN_24;
            break;
        case 5:
            glut_font= GLUT_BITMAP_HELVETICA_12;
            break;
        case 6:            
            glut_font= GLUT_BITMAP_HELVETICA_18;
            break;
        default:
            glut_font= GLUT_BITMAP_HELVETICA_10;
            break;            
    }
    glEnable(GL_FOG);
    glGetFloatv(GL_CURRENT_COLOR, color);

    glDisable ( GL_TEXTURE_2D );
    glColor4f (text_color.r, text_color.g, text_color.b, text_color.a);
    glRasterPos2f (x,y );

    for ( c=0; c<text.length(); c++ )
    {
        if (text[c]=='\n')
        {
            glRasterPos2f (x,y+=12 );
        }
        else
        {
            glutBitmapCharacter ( glut_font, text[c] );
        }
    }
    glDisable(GL_FOG);
    glColor4fv(color);
}

void drawText ( int x, int y, std::string text, color3 text_color, int font)
{
    color4 color;
    color.r=text_color.r;
    color.g=text_color.g;
    color.b=text_color.b;
    color.a=1;
    drawText(x, y, text, color, font);
	
}

void drawText(int x, int y, std::string text, std::string text_color, int font)
{
    if (text_color.length()==8)
        drawText(x, y, text, Hex_to_RGBA(text_color.c_str()), font);

    if (text_color.length()==6)
        drawText(x, y, text, Hex_to_RGB(text_color.c_str()), font);

}


void drawText(int x, int y, std::string text, color4 text_color)
{
    drawText(x, y, text, text_color, 0);
}

void drawText ( int x, int y, std::string text, color3 text_color )
{
    color4 color;
    color.r=text_color.r;
    color.g=text_color.g;
    color.b=text_color.b;
    color.a=1;
    drawText(x, y, text, color, 0);
    
}

void drawText(int x, int y, std::string text, std::string text_color)
{
    if (text_color.length()==8)
        drawText(x, y, text, Hex_to_RGBA(text_color.c_str()), 0);
    
    if (text_color.length()==6)
        drawText(x, y, text, Hex_to_RGB(text_color.c_str()), 0);
    
}


int bin2dec(char *bin)
{
    int b, k, m, n;
    int len, sum = 0;
    
    len = strlen(bin) - 1;
    
    for(k = 0; k <= len; k++)
    {
        n = (bin[k] - '0'); // char to numeric value
        if ((n > 1) || (n < 0))
        {
            puts("\n\n ERROR! BINARY has only 1 and 0!\n");
        }
        for(b = 1, m = len; m > k; m--)
        {
            // 1 2 4 8 16 32 64 ... place-values, reversed here
            b *= 2;
        }
        // sum it up
        sum = sum + n * b;
        //printf("%d*%d + ",n,b); // uncomment to show the way this works
    }
    return(sum);
}

void uiSetRoundBox(int type)
{
    roundboxtype= type;
}

void uiSetRoundBox ( char* bin )
{
    roundboxtype=(bin2dec(bin));
}

void uiSetRoundBox ( int up_l, int up_r, int down_r, int down_l )
{
    roundboxtype=((up_l*8)+(up_r*4)+(down_r*2)+(down_l*1));
}


int uiGetRoundBox(void)
{
    return roundboxtype;
}


void round_box_shade_col(float *col1, float *col2, float fac)
{
    float col[4];

    col[0]= (fac*col1[0] + (1.0-fac)*col2[0]);
    col[1]= (fac*col1[1] + (1.0-fac)*col2[1]);
    col[2]= (fac*col1[2] + (1.0-fac)*col2[2]);
    col[3]= (fac*col1[3] + (1.0-fac)*col2[3]);
    glColor4fv(col);
}


/* linear horizontal shade within button or in outline */
/* view2d scrollers use it */
void gl_round_box_Hshade(int mode, float minx, float miny, float maxx, float maxy, float rad, color4 shadedown, color4 shadetop)
{
    float vec[7][2]= {{0.195, 0.02}, {0.383, 0.067}, {0.55, 0.169}, {0.707, 0.293},
        {0.831, 0.45}, {0.924, 0.617}, {0.98, 0.805}
    };
    if (minx>maxx)
    {
        float temp=minx;
        minx=maxx;
        maxx=temp;
    }
    if (miny>maxy)
    {
        float temp=miny;
        miny=maxy;
        maxy=temp;
    }
    float div= maxy-miny;
    float coltop[4], coldown[4], color[4];
    if (rad>((maxx-minx)/2)) rad=(maxx-minx)/2;
    if (rad>((maxy-miny)/2)) rad=(maxy-miny)/2;
    int a;

    /* mult */
    for (a=0; a<7; a++) {
        vec[a][0]*= rad;
        vec[a][1]*= rad;
    }
    /* get current color, needs to be outside of glBegin/End */
    glGetFloatv(GL_CURRENT_COLOR, color);

    /* 'shade' defines strength of shading */
    coltop[0]= shadetop.r;
    if (coltop[0]>1.0) coltop[0]= 1.0;
    coltop[1]= shadetop.g;
    if (coltop[1]>1.0) coltop[1]= 1.0;
    coltop[2]= shadetop.b;
    if (coltop[2]>1.0) coltop[2]= 1.0;
    coltop[3]= shadetop.a;
    if (coltop[3]>1.0) coltop[3]= 1.0;
    coldown[0]= shadedown.r;
    if (coldown[0]<0.0) coldown[0]= 0.0;
    coldown[1]= shadedown.g;
    if (coldown[1]<0.0) coldown[1]= 0.0;
    coldown[2]= shadedown.b;
    if (coldown[2]<0.0) coldown[2]= 0.0;
    coldown[3]= shadedown.a;
    if (coldown[3]<0.0) coldown[3]= 0.0;

    glShadeModel(GL_SMOOTH);
    glEnable( GL_BLEND );
    glDisable ( GL_TEXTURE_2D );
    glBegin(mode);

    /* start with corner right-bottom */
    if (roundboxtype & 4) {

        round_box_shade_col(coltop, coldown, 0.0);
        glVertex2f(maxx-rad, miny);

        for (a=0; a<7; a++) {
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
    if (roundboxtype & 2) {

        round_box_shade_col(coltop, coldown, (div-rad)/div);
        glVertex2f(maxx, maxy-rad);

        for (a=0; a<7; a++) {
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
    if (roundboxtype & 1) {

        round_box_shade_col(coltop, coldown, 1.0);
        glVertex2f(minx+rad, maxy);

        for (a=0; a<7; a++) {
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
    if (roundboxtype & 8) {

        round_box_shade_col(coltop, coldown, rad/div);
        glVertex2f(minx, miny+rad);

        for (a=0; a<7; a++) {
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
    glColor4fv(color);
    glShadeModel(GL_FLAT);
}

void gl_round_box_Hshade(int mode, float minx, float miny, float maxx, float maxy, float rad, color3 shadedown, color3 shadetop)
{
    color4 down, top;
    down.r=shadedown.r;
    down.g=shadedown.g;
    down.b=shadedown.b;
    down.a=1;

    top.r=shadetop.r;
    top.g=shadetop.g;
    top.b=shadetop.b;
    top.a=1;
    gl_round_box_Hshade(mode, minx, miny, maxx, maxy, rad, down, top);
}

void gl_round_box_Hshade ( int mode, float minx, float miny, float maxx, float maxy, float rad, std::string shadedown, std::string shadetop )
{
	if (shadedown.length()==8 && shadetop.length()==8)
		gl_round_box_Hshade(mode, minx, miny, maxx, maxy, rad, Hex_to_RGBA(shadedown.c_str()), Hex_to_RGBA(shadetop.c_str()));
	
	if (shadedown.length()==6 && shadetop.length()==6)
		gl_round_box_Hshade(mode, minx, miny, maxx, maxy, rad, Hex_to_RGB(shadedown.c_str()), Hex_to_RGB(shadetop.c_str()));
}

void gl_round_box_MHshade(int mode, float minx, float miny, float maxx, float maxy, float rad, color4 shadedown, color4 shadetop)
{
    if (minx>maxx)
    {
        float temp=minx;
        minx=maxx;
        maxx=temp;
    }
    if (miny>maxy)
    {
        float temp=miny;
        miny=maxy;
        maxy=temp;
    }
    int type=roundboxtype;
    if (roundboxtype & 2)	uiSetRoundBox(roundboxtype ^ 2);
    if (roundboxtype & 1)	uiSetRoundBox(roundboxtype ^ 1);

    gl_round_box_Hshade(mode, minx,miny,maxx,maxy-((maxy-miny)/2),rad, shadedown, shadetop);
    uiSetRoundBox(type);
    if (roundboxtype & 8)	uiSetRoundBox(roundboxtype ^ 8);
    if (roundboxtype & 4)	uiSetRoundBox(roundboxtype ^ 4);

    gl_round_box_Hshade(mode, minx,miny+((maxy-miny)/2),maxx,maxy,rad, shadetop, shadedown);
    uiSetRoundBox(type);
}

void gl_round_box_MHshade(int mode, float minx, float miny, float maxx, float maxy, float rad, color3 shadedown, color3 shadetop)
{
    color4 down, top;
    down.r=shadedown.r;
    down.g=shadedown.g;
    down.b=shadedown.b;
    down.a=1;

    top.r=shadetop.r;
    top.g=shadetop.g;
    top.b=shadetop.b;
    top.a=1;
    gl_round_box_MHshade(mode,minx,miny,maxx,maxy,rad,down,top);

}

void gl_round_box_MHshade ( int mode, float minx, float miny, float maxx, float maxy, float rad, std::string shadedown, std::string shadetop )
{
	if (shadedown.length()==8 && shadetop.length()==8)
		gl_round_box_MHshade(mode, minx, miny, maxx, maxy, rad, Hex_to_RGBA(shadedown.c_str()), Hex_to_RGBA(shadetop.c_str()));
	
	if (shadedown.length()==6 && shadetop.length()==6)
		gl_round_box_MHshade(mode, minx, miny, maxx, maxy, rad, Hex_to_RGB(shadedown.c_str()), Hex_to_RGB(shadetop.c_str()));
}



/* linear vertical shade within button or in outline */
/* view2d scrollers use it */
void gl_round_box_Vshade(int mode, float minx, float miny, float maxx, float maxy, float rad, color4 shadeLeft, color4 shadeRight)
{
    float vec[7][2]= {{0.195, 0.02}, {0.383, 0.067}, {0.55, 0.169}, {0.707, 0.293},
        {0.831, 0.45}, {0.924, 0.617}, {0.98, 0.805}
    };
    if (minx>maxx)
    {
        int temp=minx;
        minx=maxx;
        maxx=temp;
    }
    if (miny>maxy)
    {
        int temp=miny;
        miny=maxy;
        maxy=temp;
    }
    float div= maxy-miny;
    float colLeft[4], colRight[4], color[4];
    if (rad>((maxx-minx)/2)) rad=(maxx-minx)/2;
    if (rad>((maxy-miny)/2)) rad=(maxy-miny)/2;
    int a;

    /* mult */
    for (a=0; a<7; a++) {
        vec[a][0]*= rad;
        vec[a][1]*= rad;
    }
    /* get current color, needs to be outside of glBegin/End */
    glGetFloatv(GL_CURRENT_COLOR, color);

    /* 'shade' defines strength of shading */
    colLeft[0]= shadeLeft.r;
    if (colLeft[0]>1.0) colLeft[0]= 1.0;
    colLeft[1]= shadeLeft.g;
    if (colLeft[1]>1.0) colLeft[1]= 1.0;
    colLeft[2]= shadeLeft.b;
    if (colLeft[2]>1.0) colLeft[2]= 1.0;
    colLeft[3]= shadeLeft.a;
    if (colLeft[3]>1.0) colLeft[3]= 1.0;
    colRight[0]= shadeRight.r;
    if (colRight[0]<0.0) colRight[0]= 0.0;
    colRight[1]= shadeRight.g;
    if (colRight[1]<0.0) colRight[1]= 0.0;
    colRight[2]= shadeRight.b;
    if (colRight[2]<0.0) colRight[2]= 0.0;
    colRight[3]= shadeRight.a;
    if (colRight[3]<0.0) colRight[3]= 0.0;

    glShadeModel(GL_SMOOTH);
    glEnable( GL_BLEND );
    glDisable ( GL_TEXTURE_2D );
    glBegin(mode);

    /* start with corner right-top */
    if (roundboxtype & 4) {

        round_box_shade_col(colLeft, colRight, 0.0);
        glVertex2f(maxx-rad, miny);

        for (a=0; a<7; a++) {
            round_box_shade_col(colLeft, colRight, vec[a][1]/div);
            glVertex2f(maxx-rad+vec[a][0], miny+vec[a][1]);
        }

        round_box_shade_col(colLeft, colRight, rad/div);
        glVertex2f(maxx, miny+rad);
    }
    else {
        round_box_shade_col(colLeft, colRight, 0.0);
        glVertex2f(maxx, miny);
    }

    /* corner right-bottom */
    if (roundboxtype & 2) {
        round_box_shade_col(colLeft, colRight, 0.0);
        glVertex2f(maxx, maxy-rad);

        for (a=0; a<7; a++) {

            round_box_shade_col(colRight, colLeft, (div-rad-vec[a][1])/div);
            glVertex2f(maxx-vec[a][1], maxy-rad+vec[a][0]);
        }
        round_box_shade_col(colRight, colLeft, (div-rad)/div);
        glVertex2f(maxx-rad, maxy);
    }
    else {
        round_box_shade_col(colLeft, colRight, 0.0);
        glVertex2f(maxx, maxy);
    }

    /* corner left-top */
    if (roundboxtype & 1) {
        round_box_shade_col(colLeft, colRight, (div-rad)/div);
        glVertex2f(minx+rad, maxy);

        for (a=0; a<7; a++) {
            // round_box_shade_col(colLeft, colRight, (div-rad+vec[a][0])/div);
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
    if (roundboxtype & 8) {
        round_box_shade_col(colLeft, colRight, 1.0);
        glVertex2f(minx, miny+rad);

        for (a=0; a<7; a++) {
            //round_box_shade_col(colLeft, colRight, (vec[a][0])/div);
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

void gl_round_box_Vshade(int mode, float minx, float miny, float maxx, float maxy, float rad, color3 shadeLeft, color3 shadeRight)
{
    color4 colLeft, colRight;
    colLeft.r=shadeLeft.r;
    colLeft.g=shadeLeft.g;
    colLeft.b=shadeLeft.b;
    colLeft.a=1;

    colRight.r=shadeRight.r;
    colRight.g=shadeRight.g;
    colRight.b=shadeRight.b;
    colRight.a=1;
    gl_round_box_Vshade(mode, minx, miny, maxx, maxy, rad, colLeft, colRight);
}

void gl_round_box_Vshade ( int mode, float minx, float miny, float maxx, float maxy, float rad, std::string shadedown, std::string shadetop )
{
	if (shadedown.length()==8 && shadetop.length()==8)
		gl_round_box_Vshade(mode, minx, miny, maxx, maxy, rad, Hex_to_RGBA(shadedown.c_str()), Hex_to_RGBA(shadetop.c_str()));
	
	if (shadedown.length()==6 && shadetop.length()==6)
		gl_round_box_Vshade(mode, minx, miny, maxx, maxy, rad, Hex_to_RGB(shadedown.c_str()), Hex_to_RGB(shadetop.c_str()));
}

void gl_round_box_MVshade(int mode, float minx, float miny, float maxx, float maxy, float rad, color4 shadeLeft, color4 shadeRight)
{
    if (minx>maxx)
    {
        float temp=minx;
        minx=maxx;
        maxx=temp;
    }
    if (miny>maxy)
    {
        float temp=miny;
        miny=maxy;
        maxy=temp;
    }
    int type=roundboxtype;
    if (roundboxtype & 8)	uiSetRoundBox(roundboxtype ^ 8);
    if (roundboxtype & 1)	uiSetRoundBox(roundboxtype ^ 1);

    gl_round_box_Vshade(mode, minx+((maxx-minx)/2),miny,maxx,maxy,rad, shadeLeft, shadeRight);
    uiSetRoundBox(type);
    if (roundboxtype & 4)	uiSetRoundBox(roundboxtype ^ 4);
    if (roundboxtype & 2)	uiSetRoundBox(roundboxtype ^ 2);

    gl_round_box_Vshade(mode, minx,miny,maxx-((maxx-minx)/2),maxy,rad, shadeRight, shadeLeft);
    uiSetRoundBox(type);
}

void gl_round_box_MVshade(int mode, float minx, float miny, float maxx, float maxy, float rad, color3 shadeLeft, color3 shadeRight)
{
    color4 colLeft, colRight;
    colLeft.r=shadeLeft.r;
    colLeft.g=shadeLeft.g;
    colLeft.b=shadeLeft.b;
    colLeft.a=1;

    colRight.r=shadeRight.r;
    colRight.g=shadeRight.g;
    colRight.b=shadeRight.b;
    colRight.a=1;
    gl_round_box_MVshade(mode, minx, miny, maxx, maxy, rad, colLeft, colRight);

}

void gl_round_box_MVshade ( int mode, float minx, float miny, float maxx, float maxy, float rad, std::string shadedown, std::string shadetop )
{
	if (shadedown.length()==8 && shadetop.length()==8)
		gl_round_box_MVshade(mode, minx, miny, maxx, maxy, rad, Hex_to_RGBA(shadedown.c_str()), Hex_to_RGBA(shadetop.c_str()));
	
	if (shadedown.length()==6 && shadetop.length()==6)
		gl_round_box_MVshade(mode, minx, miny, maxx, maxy, rad, Hex_to_RGB(shadedown.c_str()), Hex_to_RGB(shadetop.c_str()));
}


