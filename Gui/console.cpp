/*
 < *one line to* give the program's name and a brief idea of what it does.>
 Copyright (C) 2011  <Patrick Heyer> <patrickhey@prodigy.net.mx>
 
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

#include "console.h"
#include "tab.h"
#include "input_singleton.h"

Console::Console(int x, int y, int widht, int height, std::string name, Tab* parent)
{
    this->SetName(name);
    this->SetPosition(x, y);
    this->SetSize(widht, height);
    this->SetActive(true);
    parent->InsertWidget(this);
}

void Console::addLine(std::string value, color4 colo, int font_type)
{
            text_line temp_line;
            temp_line.color=colo;
            if (font_type>6) font_type=0;
            temp_line.font=font_type;
            temp_line.content.append(value);
            m_text.push_back(temp_line);

          
}


void Console::Draw()
{
    int uiround=uiGetRoundBox();
    uiSetRoundBox("0000");
    if (Get_Active())
    gl_round_box_Hshade(GL_POLYGON, Get_x(), Get_y(), Get_x()+Get_widht(), Get_y()+Get_height(),10, "FFFFFF",  "FFFFFF");
    int line=Get_y()+10;
    int posx=Get_x();
    int found;
    int i=0;
    while( m_text.size()>(int)(Get_height()/11))
    {
        m_text.pop_front();
    }
    for( listItor itor = m_text.begin(); itor != m_text.end(); ++itor )
    {
        i++;

        found = (*itor).content.find_first_of ('\n');
        if( found <=0){
        switch ((*itor).font)
        {
            case 1:
                drawText(posx,line+15,(*itor).content, (*itor).color, (*itor).font);
                break;
            case 2:
                drawText(posx,line+13,(*itor).content, (*itor).color, (*itor).font);
                break;
            case 3:
                drawText(posx,line+10,(*itor).content, (*itor).color, (*itor).font);
                break;
            case 4:
                drawText(posx,line+24,(*itor).content, (*itor).color, (*itor).font);
                break;
            case 5:
                drawText(posx,line+12,(*itor).content, (*itor).color, (*itor).font);
                break;
            case 6:
                drawText(posx,line+18,(*itor).content, (*itor).color, (*itor).font);
                break;
            default:
                drawText(posx,line+10,(*itor).content, (*itor).color, (*itor).font);
                break;
                
        }
        posx=posx+((*itor).content.size()*6);
        }
        else
        {
            switch ((*itor).font)
            {
            case 1:
                drawText(posx,line+15,(*itor).content, (*itor).color, (*itor).font);
                line=line+15;
                break;
            case 2:
                drawText(posx,line+13,(*itor).content, (*itor).color, (*itor).font);
                line=line+13;
                break;
            case 3:
                drawText(posx,line+10,(*itor).content, (*itor).color, (*itor).font);
                line=line+10;
                break;
            case 4:
                drawText(posx,line+24,(*itor).content, (*itor).color, (*itor).font);
                line=line+24;
                break;
            case 5:
                drawText(posx,line+12,(*itor).content, (*itor).color, (*itor).font);
                line=line+12;
                break;
            case 6:
                drawText(posx,line+18,(*itor).content, (*itor).color, (*itor).font);
                line=line+18;
                break;
            default:
                drawText(posx,line+10,(*itor).content, (*itor).color, (*itor).font);
                line=line+10;
                break;              
            }
        posx=Get_x();
       
        }

        
    }
    uiSetRoundBox(uiround);
}

void Console::update()
{
    
}

void Console::proccesInput()
{
    if(InputSingleton::getInstance().key=='v')
        SetActive(true);
    if(InputSingleton::getInstance().key=='b')
        SetActive(false);
}