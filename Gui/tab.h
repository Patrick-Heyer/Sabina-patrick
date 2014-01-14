/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  <copyright holder> <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANtabILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <string>
#include <list>

#include "widget.h"

#ifndef TAB_H
#define TAB_H

class Tab
{

public:
	
	/*! \brief Creates a new tab
     *
     *  @param name The name that will identify this tab in the list of tabs from Gui
     */
    Tab(std::string name);
    void ProccesInput();
    void InsertWidget(Widget * value);
	
	/*! \brief Draws the Tab on screen
     *
     *  Draws the Tab and its widgets on screen called by Gui::Draw
     */
	void Draw();
	
	/*! \brief Updates the Tab 
     *	
	 * Updates the Tab called by Gui::update
	*/
    void Update();
    std::string get_Name();
    void set_active_widget(Widget *value);
    Widget *get_active_widget();
    void set_active(bool value);
    bool get_active();

private:
    std::string name;
    Widget *active_widget;
    bool active;
    
    typedef std::list<Widget *>   WidgetList;
    typedef WidgetList::iterator   ListItor;
    
    WidgetList                     m_widgetslist;
};

#endif // tab_H
