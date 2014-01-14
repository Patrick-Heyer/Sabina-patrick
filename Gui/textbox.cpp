/*
 * < *one line to* give the program's name and a brief idea of what it does.>
 * Copyright (C) 2011  <Patrick Heyer> <patrickhey@prodigy.net.mx>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "textbox.h"
#include "tab.h"
#include "input_singleton.h"
#include "view2d.h"

TextBox::TextBox(int x, int y, int widht, int height, std::string name, Tab* parent)
{
   ;
    this->SetName(name);
    this->SetPosition(x, y);
    this->SetSize(widht, height);
    this->SetActive(true);
    parent->InsertWidget(this);
}




void TextBox::Draw()
{
    int uiround=uiGetRoundBox();
    uiSetRoundBox("0000");
    if (text.content.length()*4>Get_widht())
        SetSize(Get_widht()+5,Get_height());
    if (Get_Active())
        gl_round_box_Hshade(GL_POLYGON, Get_x(), Get_y(), Get_x()+Get_widht(), Get_y()+Get_height(),10, "FFFFFF",  "FFFFFF");
    drawText(Get_x(),Get_y()+8, GetName(), "330000",0 );
    drawText(Get_x(),Get_y()+17,text.content, "FF0000", 0);
        

    uiSetRoundBox(uiround);
}

void TextBox::update()
{
    
}

void TextBox::set_text(std::string value)
{
    text.content=value;
}


void TextBox::proccesInput()
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