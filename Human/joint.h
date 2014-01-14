/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  <copyright holder> <email>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef JOINT_H
#define JOINT_H

#include <string>

class skeletonJoint
{
private:
    float Pos_X;
    float Pos_Y;
    float Pos_Z;
    int ID;
    std::string Name;

public:
    float heading, attitude, bank;
    skeletonJoint();
    float Get_x();
    float Get_y();
    float Get_z();
    int Get_ID();
    std::string Get_name();
    void Set_pos(float value1, float value2, float value3);
    void Set_x(float value);
    void Set_y(float value);
    void Set_z(float value);
    void Set_ID(int value);
    void Set_name(std::string value);

};

#endif // JOINT_H
