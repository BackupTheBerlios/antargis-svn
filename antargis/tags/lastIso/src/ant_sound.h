/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ant_sound.h
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

#ifndef __SOUND_H
#define __SOUND_H
 
#include <stdlib.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alu.h>
#include <AL/alut.h>
#include <iostream>
#include <set>
#include <map>

#include "ant_geometry.h"

using namespace std;

#define MAX_BUFFERS 10
#define MAX_SOURCES 10

class Sound
{
  // Buffers hold sound data.
  ALuint Buffers[MAX_BUFFERS];
  
  // Sources are points emitting sound.
  ALuint Sources[MAX_SOURCES];
  
  // Position of the source sound.
  ALfloat SourcePos[3];// = { 0.0, 0.0, 0.0 };
  
  // Velocity of the source sound.
  ALfloat SourceVel[3];// = { 0.0, 0.0, 0.0 };
  
  // Position of the listener.
  ALfloat ListenerPos[3];// = { 0.0, 0.0, 0.0 };
  
  // Velocity of the listener.
  ALfloat ListenerVel[3];// = { 0.0, 0.0, 0.0 };
  
  // Orientation of the listener. (first 3 elements are "at", second 3 are "up")
  ALfloat ListenerOri[6];//] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };


  std::set<int> freeBuffers;
  std::set<int> freeSources;

  std::map<std::string,ALfloat> mGain;
  std::map<std::string,int> mSound;

 public:
  Sound(int argc,char *argv[]);

  void load(std::string name,std::string file,float gain);

  void play(Pos3D pos,std::string name,bool pLoop=false);

  void engine();

  void beep();

#ifdef USE_OGRE
  void updateCamera(const Camera &pCamera,float pSpeed);
#endif

  void SetListenerValues();
    
  ~Sound();
    
  void myinit(int argc,char *argv[]);

  void play();

  void stop();
  void pause();
};


#endif
