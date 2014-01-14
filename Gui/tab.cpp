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
Tab::Tab(std::string name)
{
    this->name=name;
    Gui::getInstance().addTab(this);
    
}

void Tab::InsertWidget(Widget * value)
{
    m_widgetslist.push_back(value);
}



void Tab::Draw()
{
    for( ListItor itor = m_widgetslist.begin(); itor != m_widgetslist.end(); ++itor )
    {
        (*itor)->Draw();
    }   

}

std::string Tab::get_Name()
{
    return name;
}


void Tab::Update()
{
    for( ListItor itor = m_widgetslist.begin(); itor != m_widgetslist.end(); ++itor )
    {
			(*itor)->update();
    }  
}

void Tab::ProccesInput()
{
    for( ListItor itor = m_widgetslist.begin(); itor != m_widgetslist.end(); ++itor )
    {
        (*itor)->proccesInput();
    }   
}

void Tab::set_active(bool value)
{
    active=value;
}

bool Tab::get_active()
{
    return active;
}


Widget* Tab::get_active_widget()
{
    return active_widget;
}

void Tab::set_active_widget(Widget* value)
{
    active_widget=value;
}




