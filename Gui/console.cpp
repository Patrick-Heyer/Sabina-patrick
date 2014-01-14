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

console::console(int x, int y, int widht, int height, std::string name, tab* parent)
{
    this->SetName(name);
    this->SetPosition(x, y);
    this->SetSize(widht, height);
    parent->InsertWidget(this);
}

void console::AddLine(std::string value, color4 colo, int font_type)
{
            text_line polo;
            polo.color=colo;
            if (font_type>6) font_type=0;
            polo.font=font_type;
            polo.content.append(value);
            m_text.push_back(polo);
          
}


void console::Draw()
{
    int uiround=uiGetRoundBox();
    uiSetRoundBox("0000");
    gl_round_box_Hshade(GL_POLYGON, Get_x(), Get_y(), Get_x()+Get_widht(), Get_y()+Get_height(),10, "000000",  "FFFFFF");
    int line=Get_y()+10;
    int posx=Get_x();
    int found;
    int i=0;
    for( ListItor itor = m_text.begin(); itor != m_text.end(); ++itor )
    {
        i++;
        if(i> (m_text.size()+1)-(int)(Get_height()/10))
        {
        found = (*itor).content.find_first_of ('\n');
        if( found <=0){
        switch ((*itor).font)
        {
            case 1:
                DrawText(posx,line+15,(*itor).content, (*itor).color, (*itor).font);
                break;
            case 2:
                DrawText(posx,line+13,(*itor).content, (*itor).color, (*itor).font);
                break;
            case 3:
                DrawText(posx,line+10,(*itor).content, (*itor).color, (*itor).font);
                break;
            case 4:
                DrawText(posx,line+24,(*itor).content, (*itor).color, (*itor).font);
                break;
            case 5:
                DrawText(posx,line+12,(*itor).content, (*itor).color, (*itor).font);
                break;
            case 6:
                DrawText(posx,line+18,(*itor).content, (*itor).color, (*itor).font);
                break;
            default:
                DrawText(posx,line+10,(*itor).content, (*itor).color, (*itor).font);
                break;
                
        }
        posx=posx+((*itor).content.size()*6);
        }
        else
        {
            switch ((*itor).font)
            {
            case 1:
                DrawText(posx,line+15,(*itor).content, (*itor).color, (*itor).font);
                line=line+15;
                break;
            case 2:
                DrawText(posx,line+13,(*itor).content, (*itor).color, (*itor).font);
                line=line+13;
                break;
            case 3:
                DrawText(posx,line+10,(*itor).content, (*itor).color, (*itor).font);
                line=line+10;
                break;
            case 4:
                DrawText(posx,line+24,(*itor).content, (*itor).color, (*itor).font);
                line=line+24;
                break;
            case 5:
                DrawText(posx,line+12,(*itor).content, (*itor).color, (*itor).font);
                line=line+12;
                break;
            case 6:
                DrawText(posx,line+18,(*itor).content, (*itor).color, (*itor).font);
                line=line+18;
                break;
            default:
                DrawText(posx,line+10,(*itor).content, (*itor).color, (*itor).font);
                line=line+10;
                break;              
            }
        posx=Get_x();
       
        }

        }
        
    }
    uiSetRoundBox(uiround);
}

void console::Update()
{
    
}


