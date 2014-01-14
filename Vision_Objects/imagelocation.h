#ifndef IMAGELOCATION_H
#define IMAGELOCATION_H
#include <cmath>
class ImageLocation
{
public:
    int x;
    int y;
    int width;
    int height;
    double value;
    double escala;
    int templateNumber;
    ImageLocation(int x_, int y_, int w,int h,int temp=1,double v_=0.0, double scal=1.0): x(x_), y(y_), width(w),height(h),templateNumber(temp), value(v_), escala(scal) {

    }

    const bool operator<( const ImageLocation& p2 )const {
        return this->value <= p2.value;
    }

    bool isnear(ImageLocation& otro, double numpix) {
        double dist=sqrt((x-otro.x)*(x-otro.x)+(y-otro.y)*(y-otro.y));
        if(dist<=numpix)
            return true;
        else
            return false;

    }
};

#endif // IMAGELOCATION_H
