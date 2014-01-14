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


#ifndef GUI_H
#define GUI_H

#define WIDTH 1440
#define HEIGHT 900

#include <vector>
#include <string>
#include <list>

#include "tab.h"



class GUI
{

public:
    void    Initialize(void);
    static  GUI     &GetInstance( void );
    static  void    FreeInstance( void );
    void Draw();
    void Update();
    void ProccesInput();
    void Resize(int x, int y);
    void Set_Active_Tab(int tab);
    void Add_Tab(tab *value);
    int Get_Active_Tab();

protected:
    GUI();
    GUI(const GUI& other);
    virtual ~GUI();
    virtual GUI& operator=(const GUI& other);

private:
    static GUI *gui_singleton;
    int height;
    int width;
    int num_tabs;
    int selected_tab;
    tab *active_tab;
    typedef std::list<tab *>   TabList;
    typedef TabList::iterator   ListItor;
    
    TabList                     m_tablist;
   



};

#endif // GUI_H
