/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <math.h>

class Vector3D
{
public:

    double X;
    double Y;
    double Z;

    Vector3D()
    {
        X=0;
        Y=0;
        Z=0;
    };
    Vector3D(double x, double y, double z)
    {
        X=x;
        Y=y;
        Z=z;
    };


    Vector3D Cross(Vector3D A, Vector3D B)
    {

        double x = A.Y * B.Z - B.Y * A.Z;
        double y = A.Z * B.X - B.Z * A.X;
        double z = A.X * B.Y - B.X * A.Y;
        Vector3D C;
        C.X=x;
        C.Y=y;
        C.Z=z;

        return C;
    }

    double Prod(Vector3D A, Vector3D B)
    {
        double ret;
        ret=(A.X * B.X+A.Y * B.Y+ A.Z * B.Z);

        return ret;
    }

    void Normalize()
    {
        double lenght = sqrt(X*X+Y*Y+Z*Z);
        X=X/lenght;
        Y=Y/lenght;
        Z=Z/lenght;
    }


};

#endif // VECTOR3D_H
