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


#include "joint.h"

bool writable=true;

skeletonJoint::skeletonJoint()
{


}

void skeletonJoint::Set_name ( std::string value )
{
    do {
    } while(writable==false);
    writable=false;
    this->Name=value;
    writable=true;
}

void skeletonJoint::Set_pos ( float value1, float value2, float value3 )
{
    do {
    } while(writable==false);
    writable=false;
    this->Pos_X=value1;
    this->Pos_Y=value2;
    this->Pos_Z=value3;
    writable=true;

}

void skeletonJoint::Set_x ( float value )
{
    do {
    } while(writable==false);
    writable=false;
    this->Pos_X=value;
    writable=true;
}

void skeletonJoint::Set_y ( float value )
{
    do {
    } while(writable==false);
    writable=false;
    this->Pos_Y=value;
    writable=true;
}

void skeletonJoint::Set_z ( float value )
{
    do {
    } while(writable==false);
    writable=false;
    this->Pos_Z=value;
    writable=true;
}

void skeletonJoint::Set_ID(int value)
{
    do {
    } while(writable==false);
    writable=false;
    this->ID=value;
    writable=true;
}


std::string skeletonJoint::Get_name()
{
    return this->Name;
}

float skeletonJoint::Get_x()
{
    float temp;
    do {
    } while(writable==false);
    writable=false;
    temp=this->Pos_X;
    writable=true;
    return temp;
}

float skeletonJoint::Get_y()
{
    float temp;
    do {
    } while(writable==false);
    writable=false;
    temp=this->Pos_Y;
    writable=true;
    return temp;
}

float skeletonJoint::Get_z()
{
    float temp;
    do {
    } while(writable==false);
    writable=false;
    temp=this->Pos_Z;
    writable=true;
    return temp;
}

int skeletonJoint::Get_ID()
{
    int temp;
    do {
    } while(writable==false);
    writable=false;
    temp=this->ID;
    writable=true;
    return temp;
}




