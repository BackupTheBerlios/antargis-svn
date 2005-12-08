/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_sound.h
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

#ifndef AG_SOUND_H
#define AG_SOUND_H

#include <string>
#include <ag_widget.h>

class AGSound:public AGWidget // use widget, though this isn't one, because it provides useable functions
{
 public:
  AGSound(); // NEVER use this! - it's only provided due to swig
  ~AGSound();
  bool playMp3(const std::string &pFilename);
  void stopMp3();
  void checkFinished();
  void fadeOutMusic(int ms);

  AGSignal sigMp3Finished;
 private:
  friend AGSound *getSoundManager();
};

AGSound *getSoundManager();

#endif

