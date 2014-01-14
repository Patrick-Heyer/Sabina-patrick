/*
 *    <one line to give the program's name and a brief idea of what it does.>
 *    Copyright (C) 2011  <copyright holder> <email>
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANtabILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef HUMAN_H
#define HUMAN_H

#pragma once

#include <string>
#include <map>

#include "joint.h"

class Human
{

public:
    void   Initialize(void);
    void   Initialize(std::string value);
    static Human     & getInstance();
    static  void    freeInstance( void );
    std::map<std::string, skeletonJoint> *Skeleton;
    bool tracking;
    bool write;
    std::string path;

protected:
    Human();
    Human(const Human& other);
    virtual ~Human();
    virtual Human& operator=(const Human& other);

private:
    static Human *Singleton;


};

#endif // GUI_H
