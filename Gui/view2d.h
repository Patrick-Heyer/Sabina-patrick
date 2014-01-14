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

#ifndef VIEW2D_H
#define VIEW2D_H
#include <GL/glut.h>
#include <string>

struct color4
{
    float r;
    float g;
    float b;
    float a;
};

struct color3
{
    float r;
    float g;
    float b;
};

struct colorhsv
{
    float h;
    float s;
    float v;

};

struct text_line
{
    std::string content;
    color4 color;
    int font;
};

color3 Hex_to_RGB ( const char * hex );
color4 Hex_to_RGBA ( const char * hex );

color3 Hex_to_RGB ( unsigned long hex );
color4 Hex_to_RGBA ( unsigned long hex );

std::string RGB_to_Hex (color3 color);
std::string RGB_to_Hex (color4 color);

color3 RGB ( float r, float g, float b );
color4 RGBA ( float r, float g, float b, float a );

color3 RGB ( float rgb[3] );
color4 RGBA ( float rgba[4] );

color3 hsv2rgb ( double h, double s, double l );
colorhsv rgb2hsv ( int r, int g, int b);

static int roundboxtype= 15;

void setOrthographicProjection ( int X, int Y );
void resetPerspectiveProjection();

bool IsOver(int x, int y, int x1, int y1, int x2, int y2);

void drawText ( int x, int y, std::string text, color4 text_color, int font);
void drawText ( int x, int y, std::string text, color3 text_color, int font );
void drawText ( int x, int y, std::string text, std::string text_color, int font );

void drawText ( int x, int y, std::string text, color4 text_color);
void drawText ( int x, int y, std::string text, color3 text_color);
void drawText ( int x, int y, std::string text, std::string text_color);

void uiSetRoundBox ( int type );
void uiSetRoundBox ( int up_l, int up_r, int down_r, int down_l);
void uiSetRoundBox ( char * bin);
int uiGetRoundBox ( void );

void round_box_shade_col ( float *col1, float *col2, float fac );

void gl_round_box ( int mode, float minx, float miny, float maxx, float maxy, float rad );


/* linear horizontal shade within button or in outline */
/* view2d scrollers use it */
void gl_round_box_Hshade ( int mode, float minx, float miny, float maxx, float maxy, float rad, color3 shadedown, color3 shadetop );
void gl_round_box_Hshade ( int mode, float minx, float miny, float maxx, float maxy, float rad, color4 shadedown, color4 shadetop );
void gl_round_box_MHshade ( int mode, float minx, float miny, float maxx, float maxy, float rad, color3 shadedown, color3 shadetop );
void gl_round_box_MHshade ( int mode, float minx, float miny, float maxx, float maxy, float rad, color4 shadedown, color4 shadetop );

void gl_round_box_Hshade ( int mode, float minx, float miny, float maxx, float maxy, float rad, std::string shadedown, std::string shadetop );
void gl_round_box_MHshade ( int mode, float minx, float miny, float maxx, float maxy, float rad, std::string shadedown, std::string shadetop );

/* linear vertical shade within button or in outline */
/* view2d scrollers use it */
void gl_round_box_Vshade ( int mode, float minx, float miny, float maxx, float maxy, float rad, color3 shadeLeft, color3 shadeRight );
void gl_round_box_Vshade ( int mode, float minx, float miny, float maxx, float maxy, float rad, color4 shadeLeft, color4 shadeRight );
void gl_round_box_MVshade ( int mode, float minx, float miny, float maxx, float maxy, float rad, color3 shadeLeft, color3 shadeRight );
void gl_round_box_MVshade ( int mode, float minx, float miny, float maxx, float maxy, float rad, color4 shadeLeft, color4 shadeRight );

void gl_round_box_Vshade ( int mode, float minx, float miny, float maxx, float maxy, float rad, std::string shadedown, std::string shadetop );
void gl_round_box_MVshade ( int mode, float minx, float miny, float maxx, float maxy, float rad, std::string shadedown, std::string shadetop );

#endif // VIEW2D_H
