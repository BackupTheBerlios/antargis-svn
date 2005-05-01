/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ant_boss.cc
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

#include "ant_boss.h"
#include "entities.h"
#include "jobs.h"

AntBoss::AntBoss()
{
}
AntBoss::AntBoss(const Pos2D &p):
  AntEntity(p)
{
}

AntBoss::~AntBoss()
{
}

Job *AntBoss::getJob(AntEntity *pEntity)
{
  return 0;
}

void AntBoss::insert(AntMan *pMan)
{
  mMen.insert(pMan);
}
void AntBoss::remove(AntMan *pMan)
{
  mMen.erase(pMan);
}

