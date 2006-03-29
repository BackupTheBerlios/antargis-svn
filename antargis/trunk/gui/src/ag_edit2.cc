/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_edit2.cc
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

#include "ag_edit2.h"
#include "ag_debug.h"

#include <vector>

class AGEditElement
{
public:
  virtual ~AGEditElement(){}
  virtual size_t size()
  {
    return 0;
  }
};
class AGEditString:public AGEditElement
{
public:
  std::string s;
  AGFont font;
  
  AGEditString(const std::string &p,const AGFont &pFont):s(p),font(pFont)
  {
  }

  size_t size()
  {
    return s.size();
  }
};
class AGEditNewLine:public AGEditElement
{
public:
  size_t size()
  {
    return 1;
  }
};
class AGEditSurface:public AGEditElement
{
public:
  AGSurface surface;

  AGEditSurface(const AGSurface &pSurface):surface(pSurface)
  {
  }
  size_t size()
  {
    return 1;
  }
};

class AGEdit2Content
{
  typedef std::vector<AGEditElement*> Words;
  typedef std::pair<Words::iterator,size_t> Position;
public:
  void clear()
  {
    mWords.clear();
  }
  void setFont(const AGFont &pFont)
  {
    mFont=pFont;
  }
  void setText(const std::string &s)
  {
    mWords.clear();

    size_t p=0,old=0;
    while(p!=std::string::npos)
      {
	p=std::min(s.find(" ",old+1),s.find("\n",old+1));

	AGEditElement *ns=new AGEditString(s.substr(old,p-old),mFont);
	mWords.push_back(ns);
	if(s[p]=='\n')
	  mWords.push_back(new AGEditNewLine);

	old=p+1;
      }
  }

  void insertSurface(const AGSurface &pSurface,size_t at)
  {
    Position i=getChecked(at);
    
    if(i.second==0)
      {
	// simply prepend
	mWords.insert(i.first,new AGEditSurface(pSurface));
      }
    else
      {
	// split
	AGEditString *s=dynamic_cast<AGEditString*>(*i.first);
	if(s)
	  {
	    AGEditString *n=new AGEditString(s->s.substr(i.second,std::string::npos),s->font);
	    s->s=s->s.substr(0,i.second);
	    Words::iterator j=i.first;
	    j++;
	    mWords.insert(j,n);
	    mWords.insert(j,new AGEditSurface(pSurface));
	  }
	else
	  throw std::runtime_error("unknown EditElement!");
      }
	
  }
  void insertChar(char c,size_t at)
  {
    Position i=getChecked(at);
    // first try push_backing at previous block (if exists)
    Words::iterator j=i.first;
    if(j!=mWords.begin())
      {
	j--;
	AGEditString *s=dynamic_cast<AGEditString*>(*j);
	if(s)
	  {
	    s->s+=c;
	    return;
	  }
      }
    // try to push_front to current block (if is editstring)
    AGEditString *s=dynamic_cast<AGEditString*>(*i.first);
    if(s)
      {
	s->s=std::string(c,1)+s->s;
	return;
      }
    // else generate new block
    mWords.insert(i.first,new AGEditString(std::string(c,1),mFont));
  }
  void insertWhiteSpace(size_t at)
  {
    Position i=getChecked(at);
    // split only if at is in middle of block
    if(i.second>0)
      {
	AGEditString *s=dynamic_cast<AGEditString*>(*i.first);
	if(s)
	  {
	    mWords.insert(i.first,new AGEditString(s->s.substr(0,i.second),s->font));
	    s->s=s->s.substr(i.second,std::string::npos);
	  }
	else
	  {
	    throw std::runtime_error("unknown state");
	  }	
      }
  }
  void removeChar(size_t at)
  {
    Position i=getChecked(at);
    AGEditString *s=dynamic_cast<AGEditString*>(*i.first);
    assert(s);
    s->s=s->s.substr(0,i.second)+s->s.substr(i.second+1,std::string::npos);
  }
  void insertNewLine(size_t at)
  {
    Position i=getChecked(at);
    
  }

  size_t size()
  {
    size_t l=0;
    for(Words::iterator i=mWords.begin();i!=mWords.end();i++)
      l+=(*i)->size();
    return l;
  }

private:
  Position get(size_t at)
  {
    size_t k=at;
    Words::iterator j=mWords.begin();
    for(size_t i=0;j!=mWords.end();j++)
      {
	if(i>=at)
	  return std::make_pair(j,k);
	i+=(*j)->size();
	k-=(*j)->size();
      }
    return std::make_pair(j,0); // return end()
  }
  Position getChecked(size_t at)
  {
    Position i=get(at);
    if(i.first==mWords.end())
      throw std::runtime_error("wrong index in getChecked");
    return i;
  }
  


  Words mWords;
  AGFont mFont;
};


AGEdit2::AGEdit2(AGWidget *pParent,const AGRect2 &pRect):
  AGWidget(pParent,pRect)
{
  mContent=new AGEdit2Content;
  mCursor=0;
  mSelBegin=mSelSize=0;
  mInserting=true;
}
AGEdit2::~AGEdit2()
{
  delete mContent;
}

void AGEdit2::setText(const std::string &pText)
{
  mContent->setText(pText);
  checkCursor();
}
void AGEdit2::setCursor(size_t i)
{
  mCursor=i;
  checkCursor();
}

void AGEdit2::checkCursor()
{
  mCursor=std::min(mContent->size()-1,mCursor);
}

void AGEdit2::checkSelection()
{
  int size=mContent->size()-1;
  int end=std::min((int)(mSelBegin+mSelSize),size);
  int begin=std::min((int)mSelBegin,size);

  mSelBegin=begin;
  mSelSize=end-begin;
}


void AGEdit2::setSelection(size_t start,size_t len)
{
  mSelBegin=start;
  mSelSize=len;
  checkSelection();
}

void AGEdit2::insertWhiteSpace()
{
  if(mInserting)
    {
      if(mMaxLen>0)
	if(mMaxLen==mContent->size())
	  {
	    cancelSound();
	    return; 
	  }
      mContent->insertWhiteSpace(mCursor++);
    }
  else
    {
      if(mContent->size()==mCursor)
	{
	  if(mMaxLen>0)
	    if(mMaxLen==mContent->size())
	      {
		cancelSound();
		return; 
	      }
	  mContent->insertWhiteSpace(mCursor++);
	}
      else
	{
	  mContent->removeChar(mCursor);
	  mContent->insertWhiteSpace(mCursor);
	}
    }

}

void AGEdit2::insertChar(Char c)
{
  if(mInserting)
    {
      if(mMaxLen>0)
	if(mMaxLen==mContent->size())
	  {
	    cancelSound();
	    return; 
	  }
      mContent->insertChar(c,mCursor++);
    }
  else
    {
      if(mContent->size()==mCursor)
	{
	  if(mMaxLen>0)
	    if(mMaxLen==mContent->size())
	      {
		cancelSound();
		return; 
	      }
	  
	  mContent->insertChar(c,mCursor++);
	}
      else
	{
	  mContent->removeChar(mCursor);
	  mContent->insertChar(c,mCursor);
	}
    }
}
void AGEdit2::cancelSound()
{
  // FIXME
}

void AGEdit2::prepareDraw()
{
  mCursorTime+=SDL_GetTicks()-mCursorLast;
  mCursorLast=SDL_GetTicks();
  if(mCursorTime>300 && mMutable && hasFocus())
    {
      mCursorTime=0;
      mShowCursor=!mShowCursor;
      queryRedraw();
    }


  AGWidget::prepareDraw();
}


bool AGEdit2::eventGotFocus()
{
  queryRedraw();
  return AGWidget::eventGotFocus();
}
bool AGEdit2::eventLostFocus()
{
  queryRedraw();
  return AGWidget::eventLostFocus();
}

void AGEdit2::clear()
{
  mContent->clear();
  queryRedraw();
}


void AGEdit2::draw(AGPainter &p)
{
  drawBackground(p);

  
}
