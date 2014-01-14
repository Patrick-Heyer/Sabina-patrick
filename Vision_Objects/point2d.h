#ifndef POINT2D_H
#define POINT2D_H

class Point2D
{

public:
    int x;
    int y;
    double value;
    double escala;
    Point2D(int x_, int y_, double v_, double scal=1.0): x(x_), y(y_), value(v_), escala(scal) {

    }

    const bool operator<( const Point2D& p2 )const {
        return this->value <= p2.value;
    }
};

#endif // POINT2D_H
