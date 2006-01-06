/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_tools.cc
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

#include "ag_tools.h"
#include "ag_debug.h"

int toInt(const std::string &s)
{
  return atoi(s.c_str());
}
float toFloat(const std::string &s)
{
  float f=atof(s.c_str());
  cdebug(s<<":"<<f);
  return f;
}

int fromHex(const std::string &s)
{
  int i=0;
  for(size_t j=0;j<s.length();j++)
    {
      int k=0;
      i<<=4;
      char c=s[j];
      if(c>='A'&&c<='F')
	k=c-'A'+10;
      else if(c>='a' && c<='f')
	k=c-'a'+10;
      else if(c>='0' && c<='9')
	k=c-'0';
      i+=k;
    }
  return i;
}
std::string toHex(int i)
{
  std::string s;
  std::string c="0";
  while(i!=0)
    {
      int k=i&0xF;

      if(k>9)
	c[0]='A'+k-10;
      else
	c[0]='0'+k;

      s=c+s;
      i>>=4;
    }
    
  
  while(s.length()<2)
    s=std::string("0")+s;
  return s;
}

std::string getFile(const std::string &pFilename)
{
  FILE *f;
  std::string s;

  f=fopen(pFilename.c_str(),"rb");
  if(f)
    {
      char buf[1002];
      size_t l;
      while(!feof(f))
	{
	  
	  if((l=fread(buf,sizeof(char),1000,f))>0)
	    s+=std::string(buf,l);
	}
    }

  return s;
}
