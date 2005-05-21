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
#include <physfs.h>
#include <assert.h>
#include <ag_fs.h>
#include "ag_debug.h"

bool FSinited=false;

void initFS(const char *argv0)
{
  PHYSFS_init(argv0);
  PHYSFS_setSaneConfig("Antargis","Antargis","ZIP",false,false);
  FSinited=true;
  const char *wp=PHYSFS_getWriteDir();
  cdebug("writedir:"<<wp);

  cdebug("searchpath:");

  PHYSFS_addToSearchPath("/usr/share/antargisgui/",1);
  PHYSFS_addToSearchPath("/usr/share/antargisgui/pics",1);
  PHYSFS_addToSearchPath("/usr/local/share/antargisgui/",1);
  PHYSFS_addToSearchPath("/usr/local/share/antargisgui/pics",1);
  PHYSFS_addToSearchPath("./",1);

  char **p=PHYSFS_getSearchPath();
  for(;*p;p++)
    {
      cdebug(*p);

    }

  cdebug("--");
}

std::string directLoad(const std::string &pName)
{
  FILE *f=fopen(pName.c_str(),"rb");
  if(!f)
    return "";
  fseek(f,0,SEEK_END);
  long len=ftell(f);
  fseek(f,0,SEEK_SET);
  char *buffer=new char[len+2];
  fread(buffer,1,len,f);

  fclose(f);
  return std::string(buffer,len);
}

std::string loadFile(const std::string &pName)
{
  assert(FSinited);

  if(!fileExists(pName))
    {
      std::string r=directLoad(pName);
      if(r.length()==0)
	std::cerr<<"File '"<<pName<<"' does not exist!"<<std::endl;
      return r;
    }

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
}
void saveFile(const std::string &pName,const std::string &pContent)
{
  assert(FSinited);

  PHYSFS_file *f=PHYSFS_openWrite(pName.c_str());

  assert(f);

  PHYSFS_write(f,pContent.c_str(),1,pContent.length());

  PHYSFS_close(f);
}

bool fileExists(const std::string &pName)
{
  return PHYSFS_exists(pName.c_str());
}

std::vector<std::string> getDirectory(const std::string &pDir)
{
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
}
