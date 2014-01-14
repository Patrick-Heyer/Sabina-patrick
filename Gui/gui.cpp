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


#include "input_singleton.h"
#include "gui.h"
#include "view2d.h"

GUI	*GUI::gui_singleton = 0;

GUI::GUI()
{
	Initialize();
}

GUI::GUI(const GUI& other)
{

}

GUI::~GUI()
{

}

GUI& GUI::operator=(const GUI& other)
{
    return *this;
}

void GUI::Initialize()
{
	height=HEIGHT;
	width=WIDTH;
	selected_tab=0;
	num_tabs=0;
	active_tab=0;
    Input_Singleton::GetInstance().Initialize();
}

GUI& GUI::GetInstance(void )
{
	if ( gui_singleton == 0 )
		gui_singleton = new GUI;
	
	return *gui_singleton;
}

void GUI::FreeInstance(void )
{
	if ( gui_singleton != 0 )
	{
		delete gui_singleton;
		gui_singleton = 0;
	}
}

void GUI::Draw()
{
	float color[4];
	glLoadIdentity();
	glDisable ( GL_TEXTURE_2D );
	setOrthographicProjection ( width, height);
    int w=width/num_tabs;
    int i=0;
    for( ListItor itor = m_tablist.begin(); itor != m_tablist.end(); ++itor )
    {
        uiSetRoundBox("1100");
        
        if (i==Get_Active_Tab ())
        {
            gl_round_box_Hshade(GL_POLYGON,i*w+1,1,(i+1)*w,height*.02,height*.02*.5,"FFFFFF", "008800");
            (*itor)->Draw();
        }
        else
        {
            gl_round_box_Hshade(GL_POLYGON,i*w+1,1,(i+1)*w,height*.02,height*.02*.5,"FFFFFF", "444488");
        }
        DrawText((w*i)+(w/2), 15,(*itor)->get_Name(), "FFFFFF", 1 );
        i++;
    }
	resetPerspectiveProjection();
}

void GUI::Update()
{
    for( ListItor itor = m_tablist.begin(); itor != m_tablist.end(); ++itor )
    {
        (*itor)->Update();
    }
}

void GUI::ProccesInput()
{
    int w=width/num_tabs;
    int i=0;
    for( ListItor itor = m_tablist.begin(); itor != m_tablist.end(); ++itor )
    {
        if (Input_Singleton::GetInstance().left_button)
        {
        if (IsOver(Input_Singleton::GetInstance().left_click_x, Input_Singleton::GetInstance().left_click_y, i*w+1,1,(i+1)*w,height))
        {
            Set_Active_Tab(i);
        }
        }
        i++;
    }
}

void GUI::Resize(int x, int y)
{
	width=x;
	height=y;
}

void GUI::Set_Active_Tab(int tab)
{
    if(m_tablist.size()>tab)
    {
        selected_tab=tab;
    }
}

int GUI::Get_Active_Tab()
{
    return selected_tab;
}

void GUI::Add_Tab(tab* value)
{
    m_tablist.push_back(value);
    num_tabs++;
}




