#ifndef COMPAREPOINT_H
#define COMPAREPOINT_H
#include  "imagelocation.h"
class ComparePoint {
public:
    bool operator()(ImageLocation* t1, ImageLocation* t2)
    {
        return t1->value < t2->value;
    }
};
#endif // COMPAREPOINT_H
