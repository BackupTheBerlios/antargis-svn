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
#include "ag_mutex.h"
#include "ag_fs.h"
#include "ag_config.h"

#include <SDL_mixer.h>
#include <map>
#include <set>

Mix_Music *mMusic=0;
bool mMusicFinished=false;
bool mMusicInited=false;

bool mNoSound=false;

const int cSoundChannels=16;

std::map<std::string,Mix_Chunk*> mSounds;

std::set<int> mFreeChannels;
AGMutex *mSoundMutex;

void musicFinished()
{
  mMusicFinished=true;
}
void channelDone(int channel)
{
  mSoundMutex->enter();
  mFreeChannels.insert(channel);
  mSoundMutex->leave();
}
int getFreeChannel()
{
  mSoundMutex->enter();
  // FIXME: lock
  int c=-1;
  if(mFreeChannels.size())
    {
      c=*mFreeChannels.begin();
      mFreeChannels.erase(c);
    }
  // FIXME: unlock
  mSoundMutex->leave();
  return c;
}

void initSoundEngine()
{
  if(!mMusicInited)
    {
      TRACE;
      mSoundMutex=new AGMutex;
      if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
	printf("Mix_OpenAudio: %s\n", Mix_GetError());
	mNoSound=true;
	return;
      }
      mMusic=0;
      
      Mix_HookMusicFinished(musicFinished);

      // enable mixing


      Mix_AllocateChannels(cSoundChannels);
      for(int i=0;i<cSoundChannels;i++)
	mFreeChannels.insert(i);


      Mix_ChannelFinished(channelDone);
      mMusicInited=true;
    }
}

void closeSoundEngine()
{
  if(mNoSound)
    return;
  if(mMusicInited)
    {
      if(mMusic)
	{
	  // free music
	  Mix_FreeMusic(mMusic);
	  mMusic=0;
	}
      //FIXME: readd this
      Mix_CloseAudio();
      delete mSoundMutex;
    }
}


AGSound::~AGSound()
{ 
  closeSoundEngine();
}

bool AGSound::playMp3(const std::string &pFilename)
{
  if(mNoSound)
    return false;
  if(mMusic)
    {
      return false;
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

bool AGSound::isMusicPlaying() const
{
  return mMusicFinished;
}


bool AGSound::playMp3DRM(const std::string &pFilename,AGDecryptor &pDec)
{
  if(mNoSound)
    return false;
  // must decrypt and write to disc :-(((
  std::string file=loadFile(pFilename);

  file=pDec.decrypt(file,pFilename);
  saveFile("drm.dat",file);
  return playMp3(findFile("drm.dat"));
}



void AGSound::stopMp3()
{
  if(mNoSound)
    return;
  if(mMusic)
    {
      Mix_HaltMusic();
      Mix_FreeMusic(mMusic);
      mMusic=0;
    }      
}
AGSound::AGSound():AGWidget(0,AGRect2(0,0,0,0)),sigMp3Finished(this,"sigMp3Finished")
{
  REGISTER_SINGLETON(this);
  soundVol=1.0f;

  mNoSound=getConfig()->get("soundEnabled")=="false";
  getConfig()->set("soundEnabled",mNoSound?"false":"true");
}

void AGSound::checkFinished()
{
  if(mNoSound)
    return;
  if(mMusicInited)
    if(mMusicFinished)
      {
	Mix_FreeMusic(mMusic);
	mMusic=0;
	
	
	sigMp3Finished(new AGEvent(this,"musicFinished"));
	mMusicFinished=false;
      }
}

void AGSound::fadeOutMusic(int ms)
{
  if(mNoSound)
    return;
  assert(ms>0);
  Mix_FadeOutMusic(ms);
}

void AGSound::volumeSound(int i,float v)
{
  int mv=((int)(v*MIX_MAX_VOLUME));
  mv=std::min(std::max(0,mv),MIX_MAX_VOLUME);
  if(i>=0 && i<cSoundChannels)
    Mix_Volume(i,mv);
}


void AGSound::volumeSound(float v)
{
  if(mNoSound)
    return;
  initSoundEngine();
  if(mNoSound)
    return;
  int mv=((int)(v*MIX_MAX_VOLUME));
  mv=std::min(std::max(0,mv),MIX_MAX_VOLUME);
  for(int i=0;i<cSoundChannels;++i)
    Mix_Volume(i,mv);
  soundVol=v;
}
void AGSound::volumeMusic(float v)
{
  if(mNoSound)
    return;
  initSoundEngine();
  Mix_VolumeMusic(((int)(v*MIX_MAX_VOLUME)));
}



void AGSound::playWave(const std::string &pFilename,float volume)
{
  if(mNoSound)
    return;
  initSoundEngine();
  if(volume<0)
    volume=soundVol;
  else
    volume*=soundVol;
  if(mFreeChannels.size()>0)
    {
      loadWave(pFilename);
      int channel=getFreeChannel();
      if(channel>=0)
	{
	  Mix_Chunk *c=mSounds[pFilename];
	  Mix_Volume(channel,(int)(std::min(1.0f,volume)*MIX_MAX_VOLUME));
	  Mix_PlayChannel(channel,c,0);
	}
    }

}


int AGSound::loopPlay(const std::string &pFilename,float volume)
{
  if(mNoSound)
    return false;
  initSoundEngine();
  if(volume<0)
    volume=soundVol;
  if(mFreeChannels.size()>0)
    {
      loadWave(pFilename);
      int channel=getFreeChannel();
      if(channel>=0)
	{
	  Mix_Chunk *c=mSounds[pFilename];
	  Mix_Volume(channel,(int)(std::min(1.0f,volume)*MIX_MAX_VOLUME));
	  Mix_PlayChannel(channel,c,-1);
	}
      return channel;
    }
  return -1;
}
void AGSound::stopChannel(int i,int ms)
{
  if(mNoSound)
    return;
  if(i>=0 && i<cSoundChannels)
    Mix_FadeOutChannel(i,ms);
  channelDone(i);
}

void AGSound::stopAllChannels(int ms)
{
  if(mNoSound)
    return;
  for(int i=0;i<cSoundChannels;i++)
    if(mFreeChannels.find(i)==mFreeChannels.end())
      stopChannel(i,ms);
}



void AGSound::loadWave(const std::string &pFilename)
{
  if(mNoSound)
    return;
  std::map<std::string,Mix_Chunk*>::iterator i=mSounds.find(pFilename);
  if(i!=mSounds.end())
    return;
  
  Mix_Chunk *sample;
  std::string file=loadFile(pFilename);
  sample=Mix_LoadWAV_RW(SDL_RWFromMem(const_cast<char*>(file.c_str()),file.length()),1);

  mSounds[pFilename]=sample;
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


