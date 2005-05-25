/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * jobs.h
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

#ifndef ANT_JOBS_H
#define ANT_JOBS_H
#include "ant_geometry.h"
#include "ruby.h"

class AntEntity;
class AntPlayer;
class AntHero;

class Job
{
  int priority;
 public:
  Job(int p);
  virtual ~Job();
  virtual void move(AntEntity *,float ptime);

  virtual void jobFinished(AntEntity *e);
  bool operator<=(const Job &j) const;

  virtual bool needsMorale() const;
};

class RestJob:public Job
{
  float mTime;
 public:
  RestJob(float pTime);
  virtual ~RestJob();
  void move(AntEntity *e,float ptime);
};

class MoveJob:public Job
{
  Pos2D mTarget;
  int near;
  bool mRun;
 public:
  MoveJob(int p,const Pos2D &pTarget,int pnear=0,bool pRun=false);
  virtual ~MoveJob();
  void move(AntEntity *e,float ptime);
  Pos2D getDirection(const AntEntity *e) const;
  
  
 private:
  void moveBy(AntEntity *e,float ptime,float aspeed);
};

// FIXME: implement near and far fighting (arrows)
class FightJob:public Job
  {
    AntEntity *mTarget;
    int fightDistance;
    float strength;
    float speed;

  public:
    FightJob(int p,AntEntity *pTarget);
    
    virtual ~FightJob();
    void move(AntEntity *e,float ptime);
    virtual bool needsMorale() const;
    AntEntity *getTarget()
    {
      return mTarget;
    }
  };

class FetchJob:public MoveJob
{
  std::string mWhat;
 public:
  FetchJob(int p,const Pos2D &pTarget,std::string what);
  virtual ~FetchJob();
  void move(AntEntity *e,float ptime);
  virtual void jobFinished(AntEntity *e);
  
};

#endif
