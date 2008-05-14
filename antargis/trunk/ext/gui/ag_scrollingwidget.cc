/*
 * Copyright (c) 2008 by David Kamphausen. All rights reserved.
 *
 * ag_scrollingwidget.cc
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

#include "ag_scrollingwidget.h"
#include "ag_layoutfactory.h"
#include "ag_painter.h"

AGScrollingWidget::AGScrollingWidget(AGWidget *pParent, const AGRect2& pRect):
  AGWidget(pParent,pRect),
  mClient(pRect.origin()),
  mVector(0,0),
  mDragging(false)
    {
    }

bool AGScrollingWidget::letChildProcess(AGWidget *pChild,AGEvent *event)
  {
    bool retValue;
    AGVector2 old=event->getMousePosition();
    
    event->setMousePosition(old+mVector);
    
    retValue=pChild->processEvent(event);
    
    event->setMousePosition(old);
    
    return retValue;
  }


void AGScrollingWidget::setClientRect(const AGRect2 &pRect)
  {
    mClient=pRect;
  }

void AGScrollingWidget::drawChild(AGPainter &pPainter,AGWidget *pChild)
  {
    pPainter.pushMatrix();
    pPainter.transform(mClient-mVector);
    AGWidget::drawChild(pPainter,pChild);
    pPainter.popMatrix();
  }
bool AGScrollingWidget::eventMouseButtonDown(AGEvent *pEvent)
  {
    bool result=AGWidget::eventMouseButtonDown(pEvent);
    cdebug(getScreenRect());
    cdebug(pEvent->getMousePosition());
    if(hovered())
      {
        mDragging=true;
        cdebug("dragging!");
        //return true;
      }
    return result;
  }
bool AGScrollingWidget::eventMouseButtonUp(AGEvent *pEvent)
  {
    if(mDragging)
      {
        cdebug("No longer dragging!");

        mDragging=false;
      }
    return AGWidget::eventMouseButtonUp(pEvent);
  }
bool AGScrollingWidget::eventDragBy(AGEvent *pEvent,const AGVector2 &pVector)
  {
    if(mDragging)
      {
        mVector-=pVector;
        cdebug(mVector);
        mVector=clip(mVector);
        cdebug(mVector);
      }
    return AGWidget::eventDragBy(pEvent,pVector);
  }

AGRect2 AGScrollingWidget::getClientRect() const
{
  return mClient;
}

AGRect2 AGScrollingWidget::getScreenRect() const
{
  return AGWidget::getScreenRect()-mVector;
}


void AGScrollingWidget::setVector(const AGVector2 &pVector)
  {
   mVector=clip(pVector); 
  }

AGVector2 AGScrollingWidget::clip(const AGVector2 &pVector)
  {
    AGRect2 client=AGRect2(0,0,mClient.width()-width(),mClient.height()-height());
    return client.clip(pVector);
  }
