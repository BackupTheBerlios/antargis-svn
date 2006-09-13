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
#include <string>

class AGConfig
{
 public:
  AGConfig();

  std::string get(const std::string &pValue,const std::string &pDefault,const std::string &pComment);

  std::string get(const std::string &pValue) const;
  void set(const std::string &pName,const std::string &pValue);

 private:
  void writeToDisc();

  std::map<std::string,std::string> singleValue;
  std::map<std::string,std::string> comments;
};

AGConfig *getConfig();

#endif