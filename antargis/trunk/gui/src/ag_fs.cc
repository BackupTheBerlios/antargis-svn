/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * fs.cpp
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

#include <string>
#include <list>
#include <assert.h>
#include <ag_fs.h>
#include <dirent.h>
#include "ag_debug.h"

bool FSinited=false;

std::list<std::string> mFsPaths;

void addPath(const std::string &pName)
{
  mFsPaths.push_back(pName);
#ifdef USE_PHYSFS
  TRACE;
  PHYSFS_addToSearchPath(pName.c_str(),1);
  char **p=PHYSFS_getSearchPath();
  for(;*p;p++)
    {
      cdebug(*p);

    }
#endif
}


void initFS(const char *argv0)
{
#ifdef USE_PHYSFS
  TRACE;
  PHYSFS_init(argv0);
  PHYSFS_setSaneConfig("Antargis","Antargis","ZIP",false,false);
  FSinited=true;
  const char *wp=PHYSFS_getWriteDir();
  cdebug("writedir:"<<wp);

  cdebug("searchpath:");

  PHYSFS_addToSearchPath("./data/",1);
  PHYSFS_addToSearchPath("/usr/share/antargisgui/",1);
  PHYSFS_addToSearchPath("/usr/share/antargisgui/pics",1);
  PHYSFS_addToSearchPath("/usr/local/share/antargisgui/",1);
  PHYSFS_addToSearchPath("/usr/local/share/antargisgui/pics",1);
  PHYSFS_addToSearchPath("./",1);
  PHYSFS_addToSearchPath("../",1);

  char **p=PHYSFS_getSearchPath();
  for(;*p;p++)
    {
      cdebug(*p);

    }

  cdebug("--");

#endif
  addPath("data");
  addPath("data/fonts");
#ifndef WIN32
  addPath("/usr/local/share/antargisgui/pics");
  addPath("/usr/local/share/antargisgui");
#endif
  addPath(getWriteDir());
#ifdef WIN32
  addPath("c:/Windows/Fonts/");
#endif
}


std::string checkFileName(std::string s)
{
#ifdef WIN32
  return replace(s,"/","\\");
#else
  #warning "check if directories exist"

  return s;
#endif
}

std::string directLoad(const std::string &pName)
{
  FILE *f=fopen(checkFileName(pName).c_str(),"rb");
  if(!f)
    return "";
  fseek(f,0,SEEK_END);
  long len=ftell(f);
  fseek(f,0,SEEK_SET);
  char *buffer=new char[len+2];
  fread(buffer,1,len,f);

  fclose(f);

  std::string r(buffer,len);
  delete [] buffer;
  return r;
}

std::string findFile(const std::string &pName)
{
  if(fileExists(pName))
    return pName;
  for(std::list<std::string>::iterator i=mFsPaths.begin();i!=mFsPaths.end();i++)
    {
      std::string n=*i+"/"+pName;
      n=checkFileName(n);
      if(fileExists(n))
	return n;
    }
  cdebug(pName);
  throw std::runtime_error("File not found!");
  return "";
}

std::string loadFromPath(const std::string &pName)
{
  std::string r;
  r=directLoad(pName);
  if(r.length())
    return r;

  for(std::list<std::string>::iterator i=mFsPaths.begin();i!=mFsPaths.end();i++)
    {
      r=directLoad(*i+"/"+pName);
      if(r.length())
	return r;
    }
  cdebug("LOAD FAILED:"<<pName);

  return r;
}

std::string loadFile(const std::string &pName)
{
  return loadFromPath(pName);
#ifdef USE_PHYSFS
  TRACE;
  assert(FSinited);

  std::string r=directLoad(pName);
  if(r.length()!=0)
	return r;

  if(!fileExists(pName))
    {
      std::string r=directLoad(pName);
      if(r.length()==0)
	    std::cerr<<"File '"<<pName<<"' does not exist!"<<std::endl;
      return r;
    }

  std::cerr<<"File probably doesn't exist:"<<pName<<std::endl;
  PHYSFS_file *f=PHYSFS_openRead(pName.c_str());
  std::string o;

  char buf[1001];
  PHYSFS_uint32 c=0;

  do
    {
      c=PHYSFS_read(f,buf,1,1000);
      o+=std::string(buf,c);
    }
  while(c);

  PHYSFS_close(f);
  return o;
#endif
}

std::string getWriteDir()
{
#ifdef WIN32
  return "";
#else
  return std::string(getenv("HOME"))+"/.Antargis";
#endif
}

void saveFile(const std::string &pName,const std::string &pContent)
{
#ifdef USE_PHYSFS
  TRACE;
  assert(FSinited);

  PHYSFS_file *f=PHYSFS_openWrite(pName.c_str());

  if(!f)
    {
      cdebug("Error saving file:"<<pName);
    }
  assert(f);

  PHYSFS_write(f,pContent.c_str(),1,pContent.length());

  PHYSFS_close(f);
#else
  FILE *f=fopen(checkFileName(getWriteDir()+"/"+pName).c_str(),"wb");
  fwrite(pContent.c_str(),pContent.length(),1,f);
  fclose(f);

#endif
}

bool fileExists(const std::string &pName)
{
  FILE *f=fopen(pName.c_str(),"r");
  if(f)
    {
      fclose(f);
      return true;
    }
  return false;
#ifdef USE_PHYSFS
  TRACE;
  return PHYSFS_exists(pName.c_str());
#endif
}

std::vector<std::string> getDirectory(const std::string &pDir)
{
#ifdef USE_PHYSFS
  TRACE;
  char **files= PHYSFS_enumerateFiles(pDir.c_str());
  std::vector<std::string> v;

  char **p=files;

  while(*p)
    {
      v.push_back(*p);
      p++;
    }

  PHYSFS_freeList(files);
  return v;
#else

  std::vector<std::string> v;

#ifdef WIN32
  WIN32_FIND_DATA ent;

  std::string dir=pDir+"\\*";

  dir=replace(dir,"\\\\","\\"); // remove doubles

  char path[dir.length()+20];
  strcpy(path,dir.c_str());

  HANDLE d=FindFirstFile(path,&ent);

  if(d)
    {
      do
	{
	  v.push_back(ent.cFileName);
	}
      while(FindNextFile(d,&ent)!=0);
      
      FindClose(d);
    }

#else
  struct dirent *ent;
  DIR *dir;
  std::string dirname=pDir+"/*";
  dir=opendir(dirname.c_str());
  if(dir)
    {
      while((ent=readdir(dir)))
	{
	  v.push_back(ent->d_name);
	}
    }


#endif

  return v;
#endif
}
