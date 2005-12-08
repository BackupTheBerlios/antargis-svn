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

#include "ag_mixer.h"
#include "ag_kill.h"
#include "ag_debug.h"

#include <SDL_mixer.h>

Mix_Music *mMusic=0;
bool mMusicFinished=false;
bool mMusicInited=false;

void musicFinished()
{
  mMusicFinished=true;
}

void initSoundEngine()
{
  if(!mMusicInited)
    {
      if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
	printf("Mix_OpenAudio: %s\n", Mix_GetError());
	exit(2);
      }
      mMusic=0;
      
      Mix_HookMusicFinished(musicFinished);
      mMusicInited=true;
    }
}

void closeSoundEngine()
{
  if(mMusicInited)
    {
      if(mMusic)
	{
	  // free music
	  Mix_FreeMusic(mMusic);
	  mMusic=0;
	}
      Mix_CloseAudio();
    }
}

AGSound::~AGSound()
{ 
  CTRACE;
  closeSoundEngine();
  cdebug("ruby-object:"<<mRubyObject);
}

bool AGSound::playMp3(const std::string &pFilename)
{
  if(mMusic)
    {
      return false;
      // free
      //      Mix_FreeMusic(mMusic);
      //      mMusic=0;
    }
  initSoundEngine();
  // load the MP3 file "music.mp3" to play as music
  mMusic=Mix_LoadMUS(pFilename.c_str());
  if(!mMusic) {
    printf("Mix_LoadMUS(\"%s\"): %s\n",pFilename.c_str(), Mix_GetError());
    return false;
  }
    
  // this might be a critical error...
  
  
  
  // play music forever
  // Mix_Music *music; // I assume this has been loaded already
  // -1 is forever
  // 0 is never
  // 1 is once
  if(Mix_PlayMusic(mMusic, 1)==-1) {
    printf("Mix_PlayMusic: %s\n", Mix_GetError());
    // well, there's no music, but most games don't break without music...
  }
  
  
    
  mMusicFinished=false;

  // set hook
    
  return true;

}

void AGSound::stopMp3()
{
  if(mMusic)
    {
      Mix_HaltMusic();
      Mix_FreeMusic(mMusic);
      mMusic=0;
    }      
}
AGSound::AGSound():AGWidget(0,AGRect(0,0,0,0)),sigMp3Finished(this,"sigMp3Finished")
{
  REGISTER_SINGLETON(this);
}

void AGSound::checkFinished()
{
  if(mMusicInited)
    if(mMusicFinished)
      {
	Mix_FreeMusic(mMusic);
	mMusic=0;
	
	sigMp3Finished(0);
	mMusicFinished=false;
      }
}

void AGSound::fadeOutMusic(int ms)
{
  assert(ms>0);
  Mix_FadeOutMusic(ms);
}


AGSound *mSoundManager=0;
AGSound *getSoundManager()
{
  if(!mSoundManager)
    {
      mSoundManager=new AGSound;
    }
  return mSoundManager;
}


