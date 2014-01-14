#ifndef VIEW2D_H
#define VIEW2D_H
#include <GL/glut.h>

#define UI_RB_ALPHA 16
#define UI_DISABLED_ALPHA_OFFS  -160

static int roundboxtype= 15;

bool isOver(int x, int y, int x1, int y1, int x2, int y2);
void setOrthographicProjection(int X, int Y);

void resetPerspectiveProjection();
void drawText(int x, int y, std::string text);

void uiSetRoundBox(int type);


int uiGetRoundBox(void);


void gl_round_box(int mode, float minx, float miny, float maxx, float maxy, float rad);


 void round_box_shade_col(float *col1, float *col2, float fac);


/* linear horizontal shade within button or in outline */
/* view2d scrollers use it */
void gl_round_box_shade(int mode, float minx, float miny, float maxx, float maxy, float rad, float shadetop, float shadedown);

/* linear vertical shade within button or in outline */
/* view2d scrollers use it */
void gl_round_box_vertical_shade(int mode, float minx, float miny, float maxx, float maxy, float rad, float shadeLeft, float shadeRight);

/* plain antialiased unfilled rectangle */
void uiRoundRect(float minx, float miny, float maxx, float maxy, float rad);

/* plain fake antialiased unfilled round rectangle */
void uiRoundRectFakeAA(float minx, float miny, float maxx, float maxy, float rad, float asp);


/* (old, used in outliner); plain antialiased filled box */
void uiRoundBox(float minx, float miny, float maxx, float maxy, float rad);

#endif // VIEW2D_H
