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

#ifdef USE_NEWEDITOR

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

  virtual int width()
  {
    return 0;
  }
  virtual int height()
  {
    return 0;
  }
  virtual void render(AGPainter &p,int x,int y)
  {
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
  int width()
  {
    //    cdebug(s);
    return font.getWidth(s);
  }
  int height()
  {
    return font.getHeight(s);
  }
  int widthTil(size_t i)
  {
    assert(i<s.length());
    return font.getWidth(s.substr(0,i));
  }
  virtual void render(AGPainter &p,int x,int y)
  {
    p.renderText(s,AGVector2(x,y),font);
  }
};

class AGEditWhiteSpace:public AGEditElement
{
public:
  AGFont font;
  AGEditWhiteSpace(const AGFont &pFont):font(pFont)
  {
  }
  size_t size()
  {
    return 1;
  }
  int width()
  {
    //    cdebug(font.getWidth(" "));
    return font.getWidth(" ");
  }
  int height()
  {
    return font.getHeight(" ");
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
  AGTexture texture;

  AGEditSurface(const AGSurface &pSurface):surface(pSurface)
  {
  }
  size_t size()
  {
    return 1;
  }
  int width()
  {
    return surface.width();
  }
  int height()
  {
    return surface.height();
  }
  void render(AGPainter &p,int x,int y)
  {
    texture=AGTexture(surface);
    p.blit(texture,AGRect2(x,y,texture.width(),texture.height()));
  }
};

class AGEdit2Content
{
public:

  typedef std::list<AGEditElement*> Words;
  typedef std::pair<Words::iterator,size_t> Position;
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

	if(p<std::string::npos)
	  {
	    if(s[p]!='\n')
	      mWords.push_back(new AGEditWhiteSpace(mFont));
	    else
	      mWords.push_back(new AGEditNewLine);
	  }

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
    cdebug(c<<"  "<<at);
    Position i=getChecked(at);
    if(i.second>0)
      {
	AGEditString *s=dynamic_cast<AGEditString*>(*i.first);
	cdebug(typeid(**i.first).name());
	assert(s);
	cdebug(i.second);
	s->s=s->s.substr(0,i.second)+std::string(&c,1)+s->s.substr(i.second,std::string::npos);
	return;
      }
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
	char ms[2];
	ms[0]=c;
	ms[1]=0;
	s->s=std::string(ms)+s->s;
	return;
      }
    // else generate new block
    mWords.insert(i.first,new AGEditString(std::string(c,1),mFont));
  }
  void insertWhiteSpace(size_t at)
  {
    Position i=getChecked(at);
    // split only if at is in middle of block
    cdebug(i.second);

    if(i.second>0)
      {
	AGEditString *s=dynamic_cast<AGEditString*>(*i.first);
	if(s)
	  {
	    cdebug(s->s);
	    //mWords.insert(i.first,new AGEditWhiteSpace(s->font));
	    mWords.insert(i.first,new AGEditString(s->s.substr(0,i.second),s->font));
	    mWords.insert(i.first,new AGEditWhiteSpace(s->font));
	    s->s=s->s.substr(i.second,std::string::npos);
	  }
	else
	  {
	    throw std::runtime_error("unknown state");
	  }	
      }
    else
      {
	AGEditString *s=dynamic_cast<AGEditString*>(*i.first);
	AGEditWhiteSpace *w=dynamic_cast<AGEditWhiteSpace*>(*i.first);
	if(s)
	  mWords.insert(i.first,new AGEditWhiteSpace(s->font));
	else if(w)
	  mWords.insert(i.first,new AGEditWhiteSpace(w->font));
	else
	  mWords.insert(i.first,new AGEditWhiteSpace(mFont));
      }
  }
  void removeChar(size_t at)
  {
    Position i=getChecked(at);
    AGEditString *s=dynamic_cast<AGEditString*>(*i.first);
    if(s)
      {
	assert(s);
	s->s=s->s.substr(0,i.second)+s->s.substr(i.second+1,std::string::npos);
	if(s->s.length()==0)
	  mWords.erase(i.first);
      }
    else
      mWords.erase(i.first);
  }
  void insertNewLine(size_t at)
  {
    Position i=getChecked(at);
    // split only if at is in middle of block
    cdebug(i.second);

    if(i.second>0)
      {
	AGEditString *s=dynamic_cast<AGEditString*>(*i.first);
	if(s)
	  {
	    cdebug(s->s);
	    mWords.insert(i.first,new AGEditNewLine);
	    mWords.insert(i.first,new AGEditString(s->s.substr(0,i.second),s->font));

	    s->s=s->s.substr(i.second,std::string::npos);
	  }
	else
	  {
	    throw std::runtime_error("unknown state");
	  }	
      }
    else
      {
	AGEditString *s=dynamic_cast<AGEditString*>(*i.first);
	AGEditWhiteSpace *w=dynamic_cast<AGEditWhiteSpace*>(*i.first);
	if(s)
	  mWords.insert(i.first,new AGEditNewLine);
	else if(w)
	  mWords.insert(i.first,new AGEditNewLine);
	else
	  mWords.insert(i.first,new AGEditNewLine);
      }
  }

  size_t size()
  {
    size_t l=0;
    for(Words::iterator i=mWords.begin();i!=mWords.end();i++)
      l+=(*i)->size();
    return l;
  }

  Words getLine(size_t start,int width)
  {
    Position p=getChecked(start);
    Words::iterator i=p.first;
    int w=0;
    Words o;
    for(;i!=mWords.end();i++)
      {
	//	cdebug((*i)->size()<<"  "<<(*i)->width()<<"  "<<w<<"  "<<width);
	if((*i)->width()+w>width && w>0)
	  break;
	w+=(*i)->width();
	o.push_back(*i);
	if(dynamic_cast<AGEditNewLine*>(*i))
	  break;
      }
    return o;
  }

  #warning "add width check here - for overlong single words!"
  size_t getCursorWidth(const Words &line)
  {
    size_t w=0;
    for(Words::const_iterator i=line.begin();i!=line.end();++i)
      w+=(*i)->size();
    return w;
  }
  int getLineHeight(const Words &line)
  {
    int h=0;
    for(Words::const_iterator i=line.begin();i!=line.end();++i)
      h=std::max(h,(*i)->height());
    return h;
  }

  int getCursorHeight(size_t at)
  {
    Position i=getChecked(at);

    Words::iterator j=i.first;
    while(j!=mWords.begin() && dynamic_cast<AGEditNewLine*>(*j))
      {
	j--;
      }
    if(dynamic_cast<AGEditNewLine*>(*j))
      return mFont.getHeight(" ");
    else
      return (*j)->height();
  }

  int getCursorX(const Words &line,size_t at)
  {
    //    CTRACE;
    size_t i=0;
    int x=0;
    Words::const_iterator j=line.begin();
    while(at>i && j!=line.end())
      {
	//	cdebug(at<<" "<<i);
	AGEditString *s=dynamic_cast<AGEditString*>(*j);
	if(s)
	  {
	    //	    cdebug(s->size());
	    if(s->size()+i>at)
	      {
		//		cdebug("til");
		x+=s->widthTil(at-i);
	      }
	    else
	      x+=s->width();
	    
	  }
	else
	  {
	    //	    cdebug("other");
	    x+=(*j)->width();
	  }
	//	cdebug("x:"<<x);
	i+=(*j)->size();
	j++;
      }
    return x;
  }
  std::pair<int,int> getCursorCharPos(int width,size_t cursor)
  {
    int cx,cy,cw;
    int cp=0;
    cx=cy=0;
    cw=0;
    for(Words::iterator i=mWords.begin();i!=mWords.end() && cp<cursor;i++)
      {
	cdebug(cp<<"   "<<cursor);
	if(dynamic_cast<AGEditNewLine*>(*i))
	  {
	    cy++;
	    cx=0;
	  }
	else
	  {
	    AGEditString *s=dynamic_cast<AGEditString*>(*i);
	    if((*i)->size()<=int(cursor)-cp || s==0)
	      {
		cx+=(*i)->size();
		cw+=(*i)->width();
		if(cw>width)
		  {
		    cx=(*i)->size();
		    cy++;
		    cw=(*i)->width();
		  }
	      }
	    else
	      {
		int diff=int(cursor)-cp;
		cx+=diff;
		cw+=s->widthTil(diff);
		if(cw>width)
		  {
		    cx=diff;
		    cy++;
		    cw=s->widthTil(diff);
		  }
	      }
	  }
	cp+=(*i)->size();
      }
    return std::make_pair(cx,cy);
  }
  
  int getCursorCharX(int width,size_t cursor)
  {
    return getCursorCharPos(width,cursor).first;
  }
  int getCursorCharY(int width,size_t cursor)
  {
    return getCursorCharPos(width,cursor).second;
  }

  size_t getCursorPos(int width,int x,int y)
  {
    Words::iterator i=mWords.begin();
    // search line
    int cw=0;
    int cx=0;
    int cursor=0;
    for(;i!=mWords.end() && y>0;i++)
      {
	cursor+=(*i)->size();
	if(dynamic_cast<AGEditNewLine*>(*i))
	  {
	    cw=0;
	    cx=0;
	    y--;
	  }
	else
	  {
	    cw+=(*i)->width();
	    cx+=(*i)->size();
	    if(cw>width && (*i)->width()<width)
	      {
		y--;
		cw=(*i)->width();
		cx=(*i)->size();
	      }
	  }
      }
    // search x-pos
    for(;i!=mWords.end() && cx<x;i++)
      {
	if(dynamic_cast<AGEditNewLine*>(*i))
	  break;
	
      }
   return cursor;
    
  }

  std::string getText() const
  {
    std::ostringstream os;
    for(Words::const_iterator i=mWords.begin();i!=mWords.end();++i)
      {
	AGEditString *s=dynamic_cast<AGEditString*>(*i);
	if(s)
	  os<<s->s;
	else if(dynamic_cast<AGEditNewLine*>(*i))
	  os<<std::endl;
	else if(dynamic_cast<AGEditWhiteSpace*>(*i))
	  os<<" ";
	else
	  cdebug("Problem: Image getting not supporteed ATM");
      }
    return os.str();
  }

private:
  Position get(size_t at)
  {
    size_t k=at;
    Words::iterator j=mWords.begin();
    for(size_t i=0;j!=mWords.end();j++)
      {
	if(i+(*j)->size()>at)
	  return std::make_pair(j,k);
	i+=(*j)->size();
	k-=(*j)->size();
      }
    return std::make_pair(j,0); // return end()
  }
  Position getChecked(size_t at)
  {
    Position i=get(at);
    if(i.first==mWords.end() && mWords.size()!=0)
      throw std::runtime_error("wrong index in getChecked");
    return i;
  }
  


  Words mWords;
  AGFont mFont;
};


AGEdit2::AGEdit2(AGWidget *pParent,const AGRect2 &pRect):
  AGWidget(pParent,pRect),
  mLShift(false),mRShift(false),mLCtrl(false),mRCtrl(false),mLAlt(false),mRAlt(false)
{
  mContent=new AGEdit2Content;
  mCursor=0;
  mViewCursor=0;
  mSelBegin=mSelSize=0;
  mInserting=true;

  mCursorTime=0;
  mCursorLast=0;
  mShowCursor=true;
  mMaxLen=0;

  mMutable=true;
  mMultiLine=true;

  mBackground=AGBackground("edit.background"); // FIXME: add themeing
  mDrawBackground=true;

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

void AGEdit2::insertNewLine()
{
  if(mInserting)
    {
      if(mMaxLen>0)
	if(mMaxLen==mContent->size())
	  {
	    cancelSound();
	    return; 
	  }
      mContent->insertNewLine(mCursor++);
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
	  mContent->insertNewLine(mCursor++);
	}
      else
	{
	  mContent->removeChar(mCursor);
	  mContent->insertNewLine(mCursor);
	}
    }
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
  size_t start=mViewCursor;

  AGEdit2Content::Words line;
  int x,y;
  x=y=0;

  // draw text

  for(;y<height() && start<mContent->size();)
    {
      line=mContent->getLine(start,width());
      //      cdebug(line.size());
      int lineHeight=mContent->getLineHeight(line);
      x=0;
      for(AGEdit2Content::Words::iterator i=line.begin();i!=line.end();i++)
	{
	  (*i)->render(p,x,y);
	  x+=(*i)->width();
	}
      
      //draw cursor
      size_t cWidth=mContent->getCursorWidth(line);
      //      cdebug(start<<"  "<<mCursor<<" "<<start+cWidth);
      if(mCursor<start+cWidth && mCursor>=start)
	{
	  int cx=mContent->getCursorX(line,mCursor-start);
	  int cHeight=mContent->getCursorHeight(mCursor);
	  //	  cdebug("cx:"<<cx<<"  "<<y);
	  AGColor c(0xFF,0,0);
	  p.fillRect(AGRect2(cx-4,y,10,2),c);
	  p.fillRect(AGRect2(cx,y+2,2,cHeight-4),c);
	  p.fillRect(AGRect2(cx-4,y+cHeight-2,10,2),c);
	}

      start+=cWidth;
      y+=lineHeight;
    }
  
  
}

bool AGEdit2::eventKeyUp(AGEvent *m)
{
  if(!mMutable)
    return false;

  if(m->isSDLEvent())
    {
      SDLKey k=m->getKey();

      if(k==SDLK_LSHIFT)
	mLShift=false;
      else if(k==SDLK_RSHIFT)
	mRShift=false;
      else if(k==SDLK_LCTRL)
	mLCtrl=false;
      else if(k==SDLK_RCTRL)
	mRCtrl=false;
      else if(k==SDLK_LALT)
	mLAlt=false;
      else if(k==SDLK_RALT)
	mRAlt=false;
    }
  return false;
}
bool AGEdit2::eventKeyDown(AGEvent *m)
{
  if(!mMutable)
    return false;
  if(!hasFocus())
    return false;
  queryRedraw();
  if(m->isSDLEvent())
    {
      SDLKey k=m->getKey();
      //      char ins=0;
      //      bool doInsert=false;
      bool used=false;

      if(k==SDLK_RIGHT)
	{
	  if(mMaxLen>0)
	    {
	      if(mCursor<mContent->size() && mCursor<mMaxLen-1)
		mCursor++;
	    }
	  if(mCursor<mContent->size())
	    mCursor++;
	}
      else if(k==SDLK_LEFT)
	{
	  if(mCursor>0)
	    mCursor--;
	}
      else if(k==SDLK_SPACE)
	{
	  insertWhiteSpace();
	}
      else if(k==SDLK_BACKSPACE)
	{
	  if(mCursor>0)
	    {
	      mContent->removeChar(mCursor-1);
	      mCursor--;
	    }
	}
      else if(k==SDLK_DELETE)
	{
	  if(mCursor<mContent->size())
	    mContent->removeChar(mCursor);
	}
      else if(k==SDLK_RETURN && mMultiLine)
	{
	  insertNewLine();
	}
      else if(k>=SDLK_a && k<=SDLK_z)
	{
	  if(mRShift||mLShift)
	    insertChar('A'+(k-SDLK_a));
	  else
	    insertChar('a'+(k-SDLK_a));
	}
      else if(k==SDLK_LSHIFT)
	{
	  used=true;
	  mLShift=true;
	}
      else if(k==SDLK_RSHIFT)
	{
	  used=true;
	  mRShift=true;
	}
      else if(k==SDLK_LCTRL)
	{
	  used=true;
	  mLCtrl=true;
	}
      else if(k==SDLK_RCTRL)
	{
	  used=true;
	  mRCtrl=true;
	}
      else if(k==SDLK_LALT)
	{
	  cdebug("lalt");
	  mLAlt=true;
	}
      else if(k==SDLK_RALT)
	{
	  cdebug("ralt");
	  mRAlt=true;
	}
    }

  cdebug(mContent->getCursorCharX(width(),mCursor)<<"    "<<mContent->getCursorCharY(width(),mCursor));

  return false;
}

bool AGEdit2::canFocus() const
{
  return mMutable;
}

void AGEdit2::drawBackground(AGPainter &p)
{
  if(mDrawBackground)
    mBackground.draw(getRect().origin(),p);
}

std::string AGEdit2::getText() const
{
  
}

#endif
