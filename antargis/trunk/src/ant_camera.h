/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ant_camera.h
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

#ifndef ANT_CAMERA_H
#define ANT_CAMERA_H

#include <GLee.h>

#include <ant_frustum.h>
#include <ag_rubyobj.h>

struct Viewport
{
  GLint viewport[4];
#ifndef SWIG
  (operator GLint *)()
  {
    return viewport;
  }
#endif
};

class AntCamera:public AGRubyObject
{
 public:
  AntCamera(int w,int h);

  AntFrustum getFrustum() const;
  Viewport getViewport() const;
  AGMatrix4 getModelview() const;
  AGMatrix4 getProjection() const;

  /// affects scenePosition
  void setPosition(const AGVector3 &p);

  AGVector3 project(const AGVector3 &p) const;
  AGVector3 unProject(const AGVector3 &p) const;

 private:

  void updateMatrices();


  /// this position is changed when moving around
  AGVector4 scenePosition; 
  /// camera and light position are fixed - to be understand as relative positions to scenePosition
  AGVector4 cameraPosition;
  AGVector4 lightPosition;

  AGMatrix4 lightView,lightProjection;
  AGMatrix4 cameraView,cameraProjection;

  AGMatrix4 cameraPick;

  /// use perspective shadow maps ?
  bool mPSM; 

  int mWidth,mHeight;
};

#endif