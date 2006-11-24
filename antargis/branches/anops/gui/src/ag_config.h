/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_config.h
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

#ifndef AG_CONFIG_H
#define AG_CONFIG_H

#include <map>
#include <ag_string.h>

class AGConfig
{
 public:
  AGConfig();

  AGString get(const AGString &pValue,const AGString &pDefault,const AGString &pComment);

  AGString get(const AGString &pValue) const;
  void set(const AGString &pName,const AGString &pValue);

 private:
  void writeToDisc();

  std::map<AGString,AGString> singleValue;
  std::map<AGString,AGString> comments;
};

AGConfig *getConfig();


//#define EXPORT __declspec(dllexport)

#endif
