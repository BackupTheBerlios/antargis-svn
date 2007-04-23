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

#include <ag_string.h>
#include <vector>

#include <ag_base.h>

void initFS(const char *argv0);
AGData AGEXPORT loadFile(const AGFilename &pName);
bool AGEXPORT saveFile(const AGFilename &pName,const AGData &pContent);

bool AGEXPORT fileExists(const AGFilename &pName);

std::vector<std::string> getDirectory(const AGFilename &pDir);

AGFilename checkFileName(AGFilename s);
AGFilename getWriteDir();
AGFilename findFile(const AGFilename &pName);

AGFilename getDocumentsDir();

void addPath(const AGFilename &pName);

AGData compress(const AGData &pString);
AGData uncompress(const AGData &pString);

AGFilename getDirSep();

#endif