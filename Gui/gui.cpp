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

Gui	*Gui::guiSingleton = 0;

Gui::Gui()
{
	initialize();
}

Gui::Gui(const Gui& other)
{

}

Gui::~Gui()
{

}

Gui& Gui::operator=(const Gui& other)
{
    return *this;
}

void Gui::initialize()
{
	height=HEIGHT;
	width=WIDTH;
	selectedTab=0;
	numTabs=0;
	activeTab=0;
    InputSingleton::getInstance().Initialize();
}

Gui& Gui::getInstance(void )
{
	if ( guiSingleton == 0 )
		guiSingleton = new Gui;
	
	return *guiSingleton;
}

void Gui::freeInstance(void )
{
	if ( guiSingleton != 0 )
	{
		delete guiSingleton;
		guiSingleton = 0;
	}
}

void Gui::Draw()
{
	float color[4];
	glLoadIdentity();
	glDisable ( GL_TEXTURE_2D );
	setOrthographicProjection ( width, height);
    int w=width/numTabs;
    int i=0;
    for( ListItor itor = tabList.begin(); itor != tabList.end(); ++itor )
    {
        uiSetRoundBox("1100");
        
        if (i==getActiveTab ())
        {
            gl_round_box_Hshade(GL_POLYGON,i*w+1,1,(i+1)*w,height*.02,height*.02*.5,"FFFFFF", "008800");
            (*itor)->Draw();
        }
        else
        {
            gl_round_box_Hshade(GL_POLYGON,i*w+1,1,(i+1)*w,height*.02,height*.02*.5,"FFFFFF", "444488");
        }
        drawText((w*i)+5, 15,(*itor)->get_Name(), "FFFFFF", 1 );
        i++;
    }
	resetPerspectiveProjection();
}

void Gui::update()
{
    for( ListItor itor = tabList.begin(); itor != tabList.end(); ++itor )
    {
        (*itor)->Update();
    }
}

void Gui::proccesInput()
{
    int w=width/numTabs;
    int i=0;
    unsigned char key=InputSingleton::getInstance().special_key;
    switch ( key )
    {
        case GLUT_KEY_F1:
            Gui::getInstance().setActiveTab(0);
            break;
        case GLUT_KEY_F2:
            Gui::getInstance().setActiveTab(1);
            break;
        case GLUT_KEY_F3:
            Gui::getInstance().setActiveTab(2);
            break;
        case GLUT_KEY_F4:
            Gui::getInstance().setActiveTab(3);
            break;
        case GLUT_KEY_F5:
            Gui::getInstance().setActiveTab(4);
            break;
        case GLUT_KEY_F6:
            Gui::getInstance().setActiveTab(5);
            break;
        case GLUT_KEY_F7:
            Gui::getInstance().setActiveTab(6);
            break;
        case GLUT_KEY_F8:
            Gui::getInstance().setActiveTab(7);
            break;
        case GLUT_KEY_F9:
            Gui::getInstance().setActiveTab(8);
            break;
        case GLUT_KEY_F10:
            Gui::getInstance().setActiveTab(9);
            break;
        case GLUT_KEY_F11:
            Gui::getInstance().setActiveTab(10);
            break;
        case GLUT_KEY_F12:
            Gui::getInstance().setActiveTab(11);
            break;
        case GLUT_KEY_PAGE_UP:
            break;
        case GLUT_KEY_PAGE_DOWN:
            break;
        case GLUT_KEY_UP:
            
            break;
        case GLUT_KEY_DOWN:
            
            break;
        case GLUT_KEY_LEFT:
            
            break;
        case GLUT_KEY_RIGHT:
            
            break;
        default:
            break;
    }
    for( ListItor itor = tabList.begin(); itor != tabList.end(); ++itor )
    {
        if (InputSingleton::getInstance().left_button)
        {
        if (IsOver(InputSingleton::getInstance().left_click_x, InputSingleton::getInstance().left_click_y, i*w+1,1,(i+1)*w,height*0.02))
        {
            Gui::getInstance().setActiveTab(i);
        }
        }
        if((*itor)->get_active())
        (*itor)->ProccesInput();
        i++;
    }
}

void Gui::resize(int x, int y)
{
	width=x;
	height=y;
}

void Gui::setActiveTab(int tab)
{
    if(tabList.size()>tab)
    {
        selectedTab=tab;
    }
    int i=0;
    for( ListItor itor = tabList.begin(); itor != tabList.end(); ++itor )
    {
     
        if (i==getActiveTab ())
        {
            
            (*itor)->set_active(true);
        }
        else
        {
            (*itor)->set_active(false);
        }
        i++;
    }
}
void Gui::setActiveTab(std::string value)
{
    Var_type::iterator iter = pestana.begin();

    iter = pestana.find(value);
    if (iter != pestana.end() )
    {
      setActiveTab(iter->second);
    }
}


int Gui::getActiveTab()
{
    return selectedTab;
}

void Gui::addTab(Tab* value)
{
    tabList.push_back(value);
    pestana.insert(std::make_pair(value->get_Name(),numTabs) );
    numTabs++;
}




