/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ant_hero.h
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

#ifndef ANT_HERO_H
#define ANT_HERO_H

#include "ant_boss.h"

#include <set>
#include <vector>

class AntMan;

class AntHero:public AntBoss
  {
    int typeID;

    std::string mName;

  public:
    AntHero();
    AntHero(const Pos2D &p,int ID,const std::string &pName);
    virtual ~AntHero();
    void updateSurface();

    int getTypeID() const;
    void fightHero(AntHero *h);
    void goTo(int prio,const Pos2D &pos);
    //    void discard(AntMan *man);

    float calcTroupStrength() const;

    AntHero *fights(); // returns 0 if is not fighting otherwise pointer to other hero
    void gotFight(AntEntity *e);

    Pos2D getFormation(AntMan *m) const;

    virtual AntHero *getHero();
    virtual float getHealSpeed() const;
    virtual std::string xmlName() const;

    virtual void saveXML(xmlpp::Node &node) const;
    virtual void loadXML(const xmlpp::Node &node);

  };
#endif
