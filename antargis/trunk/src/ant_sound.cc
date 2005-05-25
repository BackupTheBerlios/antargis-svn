/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ant_sound.cc
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

#include "ant_sound.h"

Sound::Sound(int argc,char *argv[])
    {
      int i;
      for(i=0;i<3;i++)
	SourcePos[i]=SourceVel[i]=ListenerPos[i]=ListenerVel[i]=0;

      ListenerPos[0]=1;

      ListenerOri[0]=0;
      ListenerOri[1]=0;
      ListenerOri[2]=-1;
      ListenerOri[3]=0;
      ListenerOri[4]=1;
      ListenerOri[5]=0;

      //      myinit(argc,argv);

      for(i=0;i<MAX_BUFFERS;i++)
	freeBuffers.insert(i);

      for(i=0;i<MAX_SOURCES;i++)
	freeSources.insert(i);

      myinit(argc,argv);
    }



void Sound::load(std::string name,std::string file,float gain)
  {

    if(freeBuffers.size()==0)
      return;

    //    cout<<"loading Sound:"<<name<<":"<<file<<endl;
    std::set<int>::iterator i=freeBuffers.begin();

    int j=*i;
    freeBuffers.erase(i);

    mSound[name]=j;
    mGain[name]=gain;

    ALenum format;
    ALsizei size;
    ALvoid* data;
    ALsizei freq;
    ALboolean loop;

    alutLoadWAVFile((ALbyte*)file.c_str(),&format,&data,&size,&freq,&loop);
    alBufferData(Buffers[j],format,data,size,freq);
    alutUnloadWAV(format,data,size,freq);

  }

void Sound::play(Pos3D pos,std::string name,bool pLoop)
  {
    int j=mSound[name];
    //    cout<<"playing Sound:"<<name<<":"<<j<<endl;
    //    cout<<"pos:"<<pos<<endl;
    
    ALfloat SourcePos[3];
    ALfloat SourceVel[3];

    pos=pos/500;
    SourcePos[0]=pos[0];
    SourcePos[1]=pos[1];
    SourcePos[2]=pos[2];

    SourceVel[0]=0;
    SourceVel[1]=0;
    SourceVel[2]=0;
    ALboolean loop=pLoop;

    alSourcei (Sources[j], AL_BUFFER,   Buffers[j]   );
    alSourcef (Sources[j], AL_PITCH,    1.0f     );
    alSourcef (Sources[j], AL_GAIN,     mGain[name]    );
    alSourcefv(Sources[j], AL_POSITION, SourcePos);
    alSourcefv(Sources[j], AL_VELOCITY, SourceVel);
    alSourcei (Sources[j], AL_LOOPING,  loop     );

    alSourcePlay(Sources[j]);
    
  }

void Sound::engine()
  {
    play(Pos3D(ListenerPos[0],ListenerPos[1],ListenerPos[2]),"engine",true);
  }

void Sound::beep()
{
    play(Pos3D(ListenerPos[0],ListenerPos[1],ListenerPos[2]),"beep");
  }

#ifdef USE_OGRE
void Sound::updateCamera(const Camera &pCamera,float pSpeed)
  {
    Pos3D v=pCamera.getPosition()/500;
    ListenerPos[0]=v[0];
    ListenerPos[1]=v[1];
    ListenerPos[2]=v[2];

    v=pCamera.getDirection()*pSpeed/500;
    ListenerVel[0]=v[0];
    ListenerVel[1]=v[1];
    ListenerVel[2]=v[2];

    v=pCamera.getUp();
    ListenerOri[0]=v[0];
    ListenerOri[1]=v[1];
    ListenerOri[2]=v[2];

    SetListenerValues();
  }
#endif
void Sound::SetListenerValues()
  {
    alListenerfv(AL_POSITION,    ListenerPos);
    alListenerfv(AL_VELOCITY,    ListenerVel);
    alListenerfv(AL_ORIENTATION, ListenerOri);
  }
    
Sound::~Sound()
  {
    alDeleteBuffers(MAX_BUFFERS, Buffers);
    alDeleteSources(MAX_SOURCES, Sources);
    alutExit();
  }
    
  void Sound::myinit(int argc,char *argv[])
  {
    // Initialize OpenAL and clear the error bit.
    
    alutInit(&argc, argv);
    alGetError();
    
    
    // Load the wav data.
    
    alGenBuffers(MAX_BUFFERS, Buffers);
    alGenSources(MAX_SOURCES, Sources);

    //    LoadALData();

    load("shot","../media/sounds/missile_3.wav",0.5);
    load("shield","../media/sounds/shieldhit2.wav",3.5);
    load("hull","../media/sounds/armorhit.wav",5.0);
    load("explosion","../media/sounds/explosion2.wav",6.0);
    load("beep","../media/sounds/lockingbak.wav",1.0);
    load("engine","../media/sounds/afterburner.wav",7.0);
    
    SetListenerValues();
    
  }


  void Sound::play()
  {
    alSourcePlay(Sources[0]);
  }
  void Sound::stop()
  {
    alSourceStop(Sources[0]);
  }
  void Sound::pause()
  {
    alSourcePause(Sources[0]);
  }
