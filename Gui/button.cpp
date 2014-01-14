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

#include "button.h"
#include "tab.h"
#include "input_singleton.h"
#include "view2d.h"

Button::Button(int x, int y, int widht, int height, std::string name, Tab* parent, bool value)
{
    this->state=value ;
    this->SetName(name);
    this->SetPosition(x, y);
    this->SetSize(widht, height);
    this->SetActive(false);
    parent->InsertWidget(this);
}




void Button::Draw()
{
    int uiround=uiGetRoundBox();
    uiSetRoundBox("0000");
    if (text.content.length()*4>Get_widht())
	SetSize(Get_widht()+5,Get_height());
    if (Get_Active())
    {
	if(IsOver(InputSingleton::getInstance().left_click_x,InputSingleton::getInstance().left_click_y, this->Get_x(), this->Get_y(), this->Get_x()+this->Get_widht(), this->Get_y()+this->Get_height()))
	{
	    
	    this->state=!state;
	    InputSingleton::getInstance().left_click_x=-10;
	    InputSingleton::getInstance().left_click_y=-10;
	}
	gl_round_box_Hshade(GL_POLYGON, Get_x(), Get_y(), Get_x()+Get_widht(), Get_y()+Get_height(),10, "FFFFFF",  "FFFFFF");
	if(state)
	drawText(Get_x(),Get_y()+20, GetName(), "FF0000",4 );
	else
	    drawText(Get_x(),Get_y()+20, GetName(), "0000FF",4 );
    }
    
    uiSetRoundBox(uiround);
}

void Button::update()
{
    
    
    
}

void Button::set_text(std::string value)
{
    text.content=value;
}


void Button::proccesInput()
{
    int key=InputSingleton::getInstance().key;
    
    if (key>32 && key<=127 )
    {
	text.content +=key;
    }
    if (key==8)
    {
	text.content = text.content.substr(0, text.content.length() - 1);        
    }
    if (key==9)
    {
	text.content+="    ";
    }
    
    if (key==32)
    {
	text.content+=" ";
    }
    InputSingleton::getInstance().key=NULL;
    
}