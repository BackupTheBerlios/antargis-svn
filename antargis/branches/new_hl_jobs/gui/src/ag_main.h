/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_main.h
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

#ifndef __AG_MAIN
#define __AG_MAIN

#include <string>
#include <ag_rubyobj.h>
#include <ag_collector.h>
#include <ag_rand.h>


/**
 * \defgroup AntargisGUI AntargisGUI
 * \brief the GUI-library of the game Battles of Antargis
 * 
 * This library consists of different mechanisms to make your life easier
 * with graphical user interfaces:
 * \see widgets
 * \see application
 * \see events
*/

class AGScreen;
struct SDL_VideoInfo;
class AGMain:public AGRubyObject
{
 public:
  AGMain();
  ~AGMain();

  void flip();
  /** change screen resolution to w*h*d with
      fullscreen when fs is true and in opengl mode if gl is true
      vw and vh describe virtual screen resolutions which implies scaling
      This is supported in opengl-mode only.
   */
  void initVideo(int w,int h,int d,bool fs,bool gl,int vw=-1,int vh=-1);
  void toggleFull();

  int width() const;
  int height() const;

  int realWidth() const;
  int realHeight() const;

  void setIcon(const std::string &pFile);
  void setCaption(const std::string &pCaption);

  bool fullscreen() const;
  long getTicks() const;

  void setRand(AGRandomizer *pRand);
  AGRandomizer *getRand();

  AGCollector *getCollector();
 protected:
  virtual void mark();

 private:
  AGScreen *mScreen;
  AGCollector *mCollector;

  const SDL_VideoInfo *videoInfo;

  AGRandomizer *mRand;
};

bool hasMain();
AGMain *getMain();

bool hasQuit();
bool glMode();

void setAppName(const std::string &pName);
std::string getAppName();
bool videoInited();
std::string AGEXPORT myHash(const std::string &p);

// from ag_debug
size_t getDebugLevel();
void setDebugLevel(size_t t);
void setRubyRaising(bool flag);


#endif
