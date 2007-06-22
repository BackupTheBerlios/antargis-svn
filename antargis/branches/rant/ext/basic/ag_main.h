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

// INCLUDE_SWIG - used to filter, which files are included in swig-interfacing

#ifndef __AG_MAIN
#define __AG_MAIN

#include <string>
#include <ag_rubyobj.h>
#include <ag_collector.h>
#include <ag_rand_base.h>
#include <ag_video_base.h>


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

// will be called repeatedely (by AGApplication - if available)
// simply instantiate a subclass of this type - and it'll be registered within AGMain
class AGRepeatedCall
{
 public:
  AGRepeatedCall();
  virtual ~AGRepeatedCall();
  virtual void call();
};

class AGMain:public AGRubyObject
{
 public:
  AGMain();
  ~AGMain();

  long getTicks() const;

  void setRand(AGRandomizerBase *pRand);
  AGRandomizerBase *getRand();

  AGCollector *getCollector();

  AGVideoBase *getVideo();
  void setVideo(AGVideoBase *p);

  void repeatedCalls();

 protected:
  virtual void mark();

 private:
  AGVideoBase *mVideo;
  AGCollector *mCollector;

  const SDL_VideoInfo *videoInfo;

  AGRandomizerBase *mRand;

  std::set<AGRepeatedCall*> mCalls;
  friend class AGRepeatedCall;
};

bool hasMain();
AGMain *getMain();

bool hasQuit();

void setAppName(const std::string &pName);
std::string getAppName();
//std::string AGEXPORT myHash(const std::string &p);

// from ag_debug
size_t getDebugLevel();
void setDebugLevel(size_t t);
void setRubyRaising(bool flag);

bool videoInited();


#endif
