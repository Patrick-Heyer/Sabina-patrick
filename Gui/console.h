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


#ifndef CONSOLE_H
#define CONSOLE_H

#pragma once

#include <list>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <sstream>



#include "view2d.h"
#include "widget.h"



struct text_line
{
    std::string content;
    color4 color;
    int font;
};

class console : public widget
{
public:
    console(int x, int y, int widht, int height, std::string name, tab* parent);
    void AddLine(std::string value, color4 col, int font_type);
    void Draw();
    void Update ();
    
private:
    typedef std::list <text_line> text;
    typedef text::iterator   ListItor;
    
    text                     m_text;
};


inline void print( console *terminal , char *format, ...)
{
    
    std::stringstream out;
    const char *p;
    va_list argp;
    int d;
    double f;
    char *s;
    char fmtbuf[256];
    std::string output="";
    color4 col={0,0,0,1};
    int font_type=0;
    va_start(argp, format);
    out.str("");
    
    for(p = format; *p != '\0'; p++)
    {
        if(*p != '%')
        {
            out << *p;
            continue;
        }
        switch(*++p)
        {
            case 'd':
                d = va_arg(argp, int);
                out << d;
                break;
                
            case 'f':
                f = va_arg(argp, double);
                out << f;
                break;
                
            case 's':
                s = va_arg(argp, char *);
                out << s;
                break;

            case 'w':
                s = va_arg(argp, char *);
                col=Hex_to_RGBA(s);
                break;
                
            case 'q':
                d = va_arg(argp, int);
                font_type=d;
                break;
                
            case '%':
                out <<'%';
                break;
                
        }

    
    }
    
    
    va_end(argp);
    
    terminal->AddLine(out.str(), col, font_type);
}

#endif // CONSOLE_H
