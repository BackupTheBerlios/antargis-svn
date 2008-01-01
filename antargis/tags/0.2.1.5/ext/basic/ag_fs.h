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

// INCLUDE_SWIG - used to filter, which files are included in swig-interfacing

#ifndef __FS_H
#define __FS_H

#include <ag_string.h>
#include <vector>

#include <ag_base.h>

AGEXPORT void initFS(const char *argv0);
AGEXPORT AGData loadFile(const AGFilename &pName);
AGEXPORT bool saveFile(const AGFilename &pName,const AGData &pContent);

AGEXPORT bool fileExists(const AGFilename &pName);

AGEXPORT std::vector<AGFilename> getDirectory(const AGFilename &pDir);

AGEXPORT AGFilename checkFileName(AGFilename s);
AGEXPORT AGFilename getWriteDir();
AGEXPORT AGFilename findFile(const AGFilename &pName);

AGEXPORT AGFilename getDocumentsDir();

AGEXPORT void addPath(const AGFilename &pName);
AGEXPORT void addPathFront(const AGFilename &pName);

AGEXPORT AGData compress(const AGData &pString);
AGEXPORT AGData uncompress(const AGData &pString);

AGEXPORT AGFilename getDirSep();

#endif
