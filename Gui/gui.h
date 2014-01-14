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

#pragma once

#define WIDTH 1024
#define HEIGHT 768

#include <vector>
#include <string>
#include <list>
#include <map>

#include "tab.h"

class Gui
{

public:
    void    initialize(void);
    ///*! \brief Returns a pointer to the Gui object
    // *
    // *  Returns a pointer to the only copy in memory of the Gui object. If no object exist it
    // *  will initialize the instance befor returning the pointer \return Gui
    // */
    static Gui     & getInstance();
    static  void    freeInstance( void );

    /*! \brief Draws the Gui and its sub elements on screen
     *
     *  Draws the Gui the tabs acces button and the activeTab
     */
    void Draw();

    /*! \brief Updates the Gui
     *
     * Updates the Gui generaly not needed as Draw updates it
    */
    void update();
    void proccesInput();
    void resize(int x, int y);

    /*! \brief Sets the tab to be displayed
     *
     * Sets the tab to be displayed by the gui
     * @param value is the name of the tab to display
    */
    void setActiveTab(std::string value);

    /*! \brief Sets the tab to be displayed
    *
    * Sets the tab to be displayed by the gui
    * @param value is the numerical ID of the tab to display
    */
    void setActiveTab(int tab);
    void addTab(Tab *value);
    int getActiveTab();

protected:
    Gui();
    Gui(const Gui& other);
    virtual ~Gui();
    virtual Gui& operator=(const Gui& other);

private:
    static Gui *guiSingleton;
    int height;
    int width;
    int numTabs;
    int selectedTab;
    Tab *activeTab;
    typedef std::list<Tab *>   TabList;
    typedef TabList::iterator   ListItor;

    TabList                     tabList;


    typedef std::map<std::string, int> Var_type;
    Var_type pestana;

};

#endif // GUI_H
