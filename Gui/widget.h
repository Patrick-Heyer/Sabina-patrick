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


#ifndef WIDGET_H
#define WIDGET_H

#include "view2d.h"

class Tab;

class Widget
{

public:
    /*! \brief Draws the widget
     *	
	 * Draws the widget called by Tab::Draw
	*/
    virtual void Draw ()=0;
	
	 /*! \brief Updates the widget 
     *	
	 * Updates the widget called by Tab::update
	*/
    virtual void update ()=0;
	
    virtual void proccesInput()=0;
    Tab *get_parent();
    void SetName(std::string value);
    void SetSize(int value[2]);
    void SetSize(int width, int height);
    void SetPosition(int value[2]);
    void SetPosition(int x, int y);
    void SetEnabled(bool value);
    void SetActive(bool value);
    std::string GetName();
    int Get_x();
    int Get_y();
    int Get_widht();
    int Get_height();
    bool Get_Active();

private:
    std::string name;
    int size[2];
    int position[2];
    bool enabled;
    bool active;
    
    Tab *parent;
    
};

#endif // WIDGET_H
