/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  <copyright holder> <email>

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

#include <GL/gl.h>
#include <GL/glut.h>

#include "gui.h"
#include "view2d.h"
tab::tab(std::string name)
{
    this->name=name;
    GUI::GetInstance().Add_Tab(this);
    
}

void tab::InsertWidget(widget * value)
{
    m_widgetslist.push_back(value);
}



void tab::Draw()
{
    for( ListItor itor = m_widgetslist.begin(); itor != m_widgetslist.end(); ++itor )
    {
        (*itor)->Draw();
    }   

}

std::string tab::get_Name()
{
    return name;
}


void tab::Update()
{
    for( ListItor itor = m_widgetslist.begin(); itor != m_widgetslist.end(); ++itor )
    {
        (*itor)->Update();
    }  
}


