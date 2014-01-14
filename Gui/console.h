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


/*! \file 
 * 
 */

/**
 * @brief ...
 **/
class Console : public Widget
{
public:
	/*! \brief Creates a new Console
     *	
	 *	@param x The upper left corner x cordinate of the console 
	 *  @param y The upper left corner y cordinate of the console 
     *  @param widht Widht of the console
	 *  @param height Height of the console
	 *  @param name The name that will identify this Widget in the list of its parent Tab
	 *  @param paretn Pointer to the Tab that will contain the widget 
     */
	Console(int x, int y, int widht, int height, std::string name, Tab* parent);
	void addLine(std::string value, color4 col, int font_type);
	void Draw();
	void update ();
	void proccesInput();
	
private:
	typedef std::list <text_line> text;
	typedef text::iterator   listItor;
	
	text                     m_text;
};

/*! \fn inline void print( Console *terminal , char *format, ...)
 * \brief Prints something on the console
 *
 *  Prints to a virtual console using printf standard
 *  @param terminal Console to print to
 *  @param format Text to print  "%d" int, "%f" float, "%s" char, "%w" color in hex, "%q" font
 */
inline void print( Console *terminal , char *format, ...)
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
	
	terminal->addLine(out.str(), col, font_type);
}

#endif // CONSOLE_H
