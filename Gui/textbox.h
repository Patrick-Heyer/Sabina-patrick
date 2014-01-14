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


#ifndef TEXTBOX_H
#define TEXTBOX_H

#pragma once

#include <list>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <sstream>



#include "view2d.h"
#include "widget.h"




class TextBox : public Widget
{
public:
    TextBox(int x, int y, int widht, int height, std::string name, Tab* parent);
    void AddLine(std::string value, color4 col, int font_type);
    void set_text(std::string value);
    void set_color(std::string value);
    void Draw();
    void update ();
    
    void proccesInput();
    
private:
    text_line text;
};



#endif // CONSOLE_H
