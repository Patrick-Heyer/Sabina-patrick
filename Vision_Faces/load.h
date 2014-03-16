#include "cv.h"
#include "highgui.h"

int InitDB();
int InitClasifiers();
void AjustaProbabilidades(int count);
void IniciaProbabilidades();
int ComparaBD(IplImage * compa, int f);
int AprendeRostro(IplImage* compa);

int learn (IplImage *imagen);
int recognize (IplImage *imagen);