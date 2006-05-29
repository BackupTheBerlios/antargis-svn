/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * fs.h
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

#ifndef __FS_H
#define __FS_H

#include <string>
#include <vector>

void initFS(const char *argv0);
std::string loadFile(const std::string &pName);
void saveFile(const std::string &pName,const std::string &pContent);

bool fileExists(const std::string &pName);

std::vector<std::string> getDirectory(const std::string &pDir);

std::string checkFileName(std::string s);
std::string getWriteDir();
std::string findFile(const std::string &pName);

void addPath(const std::string &pName);

std::string compress(const std::string &pString);
std::string uncompress(const std::string &pString);

#endif
