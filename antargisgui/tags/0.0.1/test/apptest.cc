/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * apptest.cc
 * by David Kamphausen (david.kamphausen@web.de)
 *
 * The "Antargis" project, including all files needed to compile it,
 * is free software; you can redistribute it and/or use it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program.
 */

#include "ag_application.h"
#include "ag_main.h"
#include "ag_button.h"
#include "ag_debug.h"
#include "SDL.h"
#include <iostream>
#include <exception>

class MyApp:public AGApplication
{
public:
  bool eventIdle()
  {
    //    tryQuit();
    return true;
  }
};

int main(int argc,char *argv[])
{
    TRACE;
 try
 {
  AGMain main;
  MyApp app;

  AGButton b(0,AGRect(0,0,50,20),"hallo");
  AGButton b1(&b,AGRect(0,20,50,20),"hallo");
  AGButton b2(&b,AGRect(0,40,50,20),"hallo");
  AGButton b3(&b,AGRect(0,60,50,20),"hallo");
  AGButton b4(&b,AGRect(0,80,50,20),"hallo");
{
                                             TRACE;
  app.setMainWidget(&b);
}
                                             {
                                                   TRACE;
 
  app.run();
}
}
catch(std::exception &x)
{
std::cerr<<x.what()<<std::endl;
}
}
