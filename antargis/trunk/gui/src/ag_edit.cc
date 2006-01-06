/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_edit.cc
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

#include "ag_edit.h"
#include "ag_fontengine.h"
#include "ag_theme.h"
#include "ag_debug.h"
#include "ag_menu.h"
#include "sge.h"


AGEditLine::AGEditLine(const std::string &pText,AGFont pFont,bool pHardEnd):
  mText(pText),mFont(pFont),mHardEnd(pHardEnd)
{
  mAlign=EDIT_LEFT;
  mVAlign=EDIT_TOP;
}

AGEditLine::~AGEditLine()
{
}

void AGEditLine::setAlign(AGAlign pAlign)
{
  mAlign=pAlign;
}

void AGEditLine::setVAlign(AGVAlign pVAlign)
{
  mVAlign=pVAlign;
}

void AGEditLine::setFont(const AGFont &pFont)
{
  mFont=pFont;
}


AGFont AGEditLine::getFont() const
{
  return mFont;
}

void AGEditLine::draw(AGPainter &p,const AGPoint &pPoint,const AGRect &pClip)
{
  int x=0;
  if(mAlign==EDIT_CENTER)
    x=(pClip.w-mFont.getWidth(mText))/2;

  p.renderText(mText+(mHardEnd?"":""),AGPoint(pPoint.x+x,pPoint.y),mFont);
}

void AGEditLine::drawCursor(AGPainter &p,int cx,const AGPoint &pPoint,const AGRect &pClip,const AGColor &c)
{
  int x1=AGFontEngine::getWidth(mFont,mText.substr(0,cx));
  int x2=AGFontEngine::getWidth(mFont,mText.substr(0,cx+1));

  int w=x2-x1;
  if(w==0)
    w=8;

  p.drawRect(AGRect(pPoint.x+x1,pPoint.y,w,height()),c);
}


int AGEditLine::height() const
{
  return AGFontEngine::getHeight(mFont,mText);
}
int AGEditLine::width() const
{
  return AGFontEngine::getWidth(mFont,mText);
}

void AGEditLine::insert(char c,int cx)
{
  mText=mText.substr(0,cx)+std::string(&c,1)+mText.substr(cx,std::string::npos);
}

void AGEditLine::doDelete(int cx)
{
  if(mText.length()>0)
    if(cx>=0&&cx<int(mText.length()))
      mText=mText.substr(0,cx)+mText.substr(cx+1,std::string::npos);

}

AGEditLine AGEditLine::split(int cx)
{
  std::string n=mText.substr(cx,mText.npos);
  mText=mText.substr(0,cx);
  bool hard=mHardEnd;
  mHardEnd=true;
  return AGEditLine(n,mFont,hard);
}

int AGEditLine::length() const
{
  return mText.length();
}

void AGEditLine::append(std::string s)
{
  mText+=s;
}

void AGEditLine::prepend(std::string s)
{
  mText=s+mText;
}
std::string AGEditLine::getText() const
{
  return mText;
}

// returns the remaing string after wrapping
std::pair<std::string,bool> AGEditLine::checkWrap(int pW)
{
  // first check, if line is too long

  if(AGFontEngine::getWidth(mFont,mText)<pW)
    return std::make_pair("",false);

  // ok, line is too long
  // so search for a good split (between words), but not before half of width
  std::vector<std::string> words=::split(" ",mText);
  
  std::vector<std::string>::iterator i=words.begin();
  int w=0,ow=0;
  std::string s,os;

  for(;i!=words.end();i++)
    {
      s+=*i;
      w=AGFontEngine::getWidth(mFont,s);
      if(w>pW)
	break;

      ow=w;
      os=s;
      s+=" ";
    }
  if(ow>pW/4 && ow<width())
    {
      // check if width will be at least a 1/4 of whole width
      std::string n=mText.substr(os.length()+1,n.npos);
      mText=mText.substr(0,os.length()+1);

      bool hard=mHardEnd;
      mHardEnd=false;
      return std::make_pair(n,hard);
    }


  // find first character, which stands over
  size_t k=0;
  w=ow=0;
  s=os="";
  for(;k<mText.length();k++)
    {
      s+=mText.substr(k,1);
      w=AGFontEngine::getWidth(mFont,s);
      if(w>pW)
	break;

      ow=w;
      os=s;
    }
  if(k>=mText.length())
    return std::make_pair("",false); // some error
  std::string n=mText.substr(k,mText.length());
  mText=mText.substr(0,k);
  bool hard=mHardEnd;
  mHardEnd=false;
  return std::make_pair(n,hard);
}

void AGEditLine::setHardEnd(bool pHardEnd)
{
  mHardEnd=pHardEnd;
}

bool AGEditLine::hardEnd() const
{
  return mHardEnd;
}

std::pair<std::string,bool> AGEditLine::checkUnwrap(int pW,std::string s)
{
  std::string oldtext=mText;
  //  mText+=" ";
  mText+=s;
  std::string testtext=mText;
  std::pair<std::string,bool> res=checkWrap(pW);
  if(mText!=oldtext)
    return std::make_pair(testtext.substr(mText.length(),std::string::npos),true);
  else
    return std::make_pair("",false);
  
}

void AGEditLine::setText(const std::string &s)
{
  mText=s;
}






  
AGEdit::AGEdit(AGWidget *pParent,const AGRect &pRect):
  AGWidget(pParent,pRect),mCursorTime(0),mCursorLast(SDL_GetTicks()),
  mLShift(false),mRShift(false),mLCtrl(false),mRCtrl(false),mLAlt(false),mRAlt(false),
  mMultiLine(true),mWrapLines(true)
{
  mMaxLength=-1;
  mMutable=true;
  mShowCursor=true;
  AGFont font1("Arial.ttf",14);
  AGFont font2("Arial.ttf",13);
  font1.setColor(AGColor(0,0,0));
  font2.setColor(AGColor(0,0,0xFF));
  AGEditLine l("",font1,true);
  mAlign=EDIT_LEFT;
  mVAlign=EDIT_TOP;
  l.setAlign(mAlign);
  l.setVAlign(mVAlign);
  mLines.push_back(l);

  mCx=mCy=0;
  mViewCy=0;

  std::string t=mTheme;
  if(t.length())
    t+=".";
  mBackground=AGBackground(t+"edit.background");
  mDrawBackground=true;
  setTheme("edit");
}

void AGEdit::setMaxLength(int i)
{
  mMaxLength=i;
}


void AGEdit::draw(AGPainter &p)
{
  
  int x,y,cy;
  int completeHeight=0;
  x=y=cy=0;
  
  drawBackground(p);//pRect);

  std::list<AGEditLine>::iterator i=mLines.begin();

  //  cdebug(pRect);
  //  cdebug("getRect:"<<getRect());
  AGRect mr(getRect());//pRect.project(getRect()));

  AGColor cursorC;
  if(mShowCursor)
    cursorC=AGColor(0,0,0,0x7F);
  else
    cursorC=AGColor(0x7f,0x7f,0x7f,0x7f);
  
  if(mCy<mViewCy)
    mViewCy=mCy;

  // get complete height - for hor. centering
  for(i=mLines.begin();i!=mLines.end();i++)
    {
      completeHeight+=i->height();
    }

  //  cdebug("completeHeight:"<<completeHeight);
  //  cdebug("height:"<<getRect().h);
  if(mVAlign==EDIT_VCENTER)
    y=getRect().h/2-completeHeight/2;
  //  cdebug("y:"<<y);

  i=mLines.begin();


  //  cdebug("mRect:"<<getRect());
  //  cdebug("mViewCy:"<<mViewCy);
  for(int k=0;k<mViewCy;k++)
    i++;

  //  cdebug("mLines:"<<mLines.size());
  for(;i!=mLines.end();i++)
    {
      //      cdebug((*i).getText());
      //      cdebug("("<<x<<";"<<y<<")");
      //      cdebug(mr);
      i->draw(p,AGPoint(x,y),getRect().origin());//pRect.project(getRect()));
      if(cy+mViewCy==mCy && mMutable && hasFocus()) // FIXME: Change show cursor only if widget has focus
	i->drawCursor(p,mCx,AGPoint(x,y),getRect(),cursorC);
      y+=i->height();
      if(y>getRect().h)
	{
	  //	  cdebug("break - too far y:"<<y<<" h:"<<getRect().h);
	  break;
	}
      cy++;
      if(!mMultiLine)
	{
	  //	  cdebug("break - no multiline");
	  break;
	}
    }

  // do it next time
  if(cy+mViewCy-1<mCy)
    mViewCy=mCy-cy+1;

  mCursorTime+=SDL_GetTicks()-mCursorLast;
  mCursorLast=SDL_GetTicks();
  if(mCursorTime>300)
    {
      mCursorTime=0;
      mShowCursor=!mShowCursor;
    }
}

void AGEdit::drawBackground(AGPainter &p)
{
  //  CTRACE;
  if(mDrawBackground)
    {
      //  CTRACE;
      mBackground.draw(p);
    }
  //    p.tile(mBackground,getRect());
}

bool AGEdit::eventKeyUp(AGEvent *m)
{
  if(!mMutable)
    return false;
  //  const AGSDLEvent *m=reinterpret_cast<const AGSDLEvent*>(m2);
  if(m->isSDLEvent())
    {
      SDLKey k=m->getKey();
      //      char ins=0;
      //      bool doInsert=false;
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
bool AGEdit::eventKeyDown(AGEvent *m)
{
  if(!mMutable)
    return false;
  if(!hasFocus())
    return false;

  if(m->isSDLEvent())
    {
      SDLKey k=m->getKey();
      char ins=0;
      bool doInsert=false;
      bool used=false;
      if(k==SDLK_RIGHT)
	{
	  getActLine();
	  if(actLine->length()>mCx)
	    mCx++;
	  else if(mCy<(int)mLines.size()-1)
	    {
	      mCy++;
	      mCx=0;
	    }
	  return true;
	}
      else if(k==SDLK_LEFT)
	{
	  if(mCx>0)
	    mCx--;
	  else if(mCy>0)
	    {
	      mCy--;
	      getActLine();
	      mCx=actLine->length();
	    }
	  
	  return true;
	}
      else if(k==SDLK_UP && mMultiLine)
	{
	  if(mCy>0)
	    mCy--;
	  getActLine();
	  if(mCx>=actLine->length())
	    mCx=actLine->length();
	  return true;
	}
      else if(k==SDLK_DOWN && mMultiLine)
	{
	  if((int)mLines.size()>mCy+1)
	    mCy++;
	  getActLine();
	  if(mCx>=actLine->length())
	    mCx=actLine->length();
	  return true;
	}
      else if(k>=SDLK_0 && k<=SDLK_9)
	{
	  doInsert=true;
	  if(mRShift||mLShift)
	    {
	      std::string s="=!\"§$%&/()";
	      ins=s[k-SDLK_0];
	    }
	  else
	    ins='0'+(k-SDLK_0);
	}
      else if(k>=SDLK_a && k<=SDLK_z)
	{
	  doInsert=true;
	  if(mRShift||mLShift)
	    ins='A'+(k-SDLK_a);
	  else
	    ins='a'+(k-SDLK_a);
	}
      else if(k==SDLK_SPACE)
	{
	  doInsert=true;
	  ins=' ';
	}
      else if(k==SDLK_BACKSPACE)
	{
	  if(mCx>0)
	    {
	      doDelete(mCx-1);
	      mCx--;
	    }
	  else if(mCy>0 && mMultiLine)
	    {
	      mCy--;
	      getActLine();
	      mCx=actLine->length();
	      if(!actLine->hardEnd())
		{
		  // delete last char
		  doDelete(actLine->length()-1);
		  mCx--;
		}
	      mergeLine(mCy);
	    }
	  checkWrap();
	  used=true;
	}
      else if(k==SDLK_DELETE)
	{
	  doDelete(mCx);
	  checkWrap();
	  used=true;
	}
      else if(k==SDLK_RETURN && mMultiLine)
	{
	  getActLine();
	  AGEditLine l=actLine->split(mCx);
	  l.setAlign(mAlign);
	  l.setVAlign(mVAlign);
	  mCx=0;
	  mCy++;
	  insertLine(l);
	  checkWrap();
	  used=true;
	}
      else if(k==SDLK_END)
	{
	  getActLine();
	  mCx=actLine->length();
	  used=true;
	}
      else if(k==SDLK_HOME)
	{
	  mCx=0;
	  used=true;
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
      else if(strlen(SDL_GetKeyName(k))==1)
	{
	  cdebug(k<<":"<<SDL_GetKeyName(k));
	  ins=SDL_GetKeyName(k)[0];
	  doInsert=true;
	}
      else if(k==SDLK_WORLD_68)
	{
	  ins=(mLShift||mRShift)?'Ä':'ä';
	  doInsert=true;
	}
      else if(k==SDLK_WORLD_86)
	{
	  ins=(mLShift||mRShift)?'Ö':'ö';
	  doInsert=true;
	}
      else if(k==SDLK_WORLD_92)
	{
	  ins=(mLShift||mRShift)?'Ü':'ü';
	  doInsert=true;
	}
      else if(k==SDLK_WORLD_63)
	{
	  ins='ß';
	  doInsert=true;
	}

      cdebug("KEY:"<<SDL_GetKeyName(k)<<"  "<<k);

      if(doInsert)
	{
	  if(insert(ins))
	    mCx++;
	  checkWrap();
	  return true;
	}
      if(used)
	return true;
    }
  return false;
}

bool AGEdit::insert(char c)
{
  if(mMaxLength>0)
    {
      if((int)getText().length()==mMaxLength)
	return false; // ignore input
    }
  getActLine(); // FIXME:try to cache
  actLine->insert(c,mCx);
  return true;
}

void AGEdit::getActLine()
{
  // slow
  int i=0;
  std::list<AGEditLine>::iterator j=mLines.begin();

  for(;i!=mCy;i++)
    j++;
  actLine=&(*j);
}

void AGEdit::doDelete(int p)
{
  if(p>=0)
    {
      getActLine();
      if(p==(int)actLine->length() && mCy<(int)mLines.size()-1)
	{
	  // merge 2 lines
	  mergeLine(mCy);
	}
      else
	actLine->doDelete(p);

    }
}

void AGEdit::insertLine(AGEditLine l)
{
  std::list<AGEditLine>::iterator j=mLines.begin();
  for(;&(*j)!=actLine;j++);
  j++;
  mLines.insert(j,l);
}

void AGEdit::mergeLine(int p)
{
  if(!mMultiLine)
    return;
  std::list<AGEditLine>::iterator j=mLines.begin();
  int i=0;
  for(;i<p;i++)
    j++;
  actLine=&(*j);
  j++;
  actLine->append(j->getText());
  bool h=j->hardEnd();
  mLines.erase(j);
  actLine->setHardEnd(h);
}

void AGEdit::setMulti(bool pMultiLine)
{
  mMultiLine=pMultiLine;
}

void AGEdit::checkWrap()
{
  if(mWrapLines)
    {
      std::list<AGEditLine>::iterator i=mLines.begin();
      for(;i!=mLines.end();)
	{
	  std::pair<std::string,bool> n=i->checkWrap(width());
	  if(n.first.length())
	    {
	      // make new line
	      if(i->length()<mCx)
		{
		  mCx-=i->length();
		  mCy++;
		}
	      i++;
	      if(n.second) // is hard end
		{
		  AGEditLine l(n.first,actLine->getFont(),n.second);
		  l.setAlign(mAlign);
		  l.setVAlign(mVAlign);
		  i=mLines.insert(i,l);
		}
	      else
		{
		  // fill line after
		  i->prepend(n.first);
		}

	    }
	  else
	    i++;
	}
     
      // unwrapping -test

      int cy=0;
      i=mLines.begin();
      for(;i!=mLines.end();)
	{
	  std::list<AGEditLine>::iterator j=i;
	  j++;
	  if(j!=mLines.end() && !i->hardEnd())
	    {
	      //	      cdebug(i->getText());
	      std::pair<std::string,bool> nText=i->checkUnwrap(width(),j->getText());
	      if(nText.second) // changed
		{
		  int count=j->getText().length()-nText.first.length();
		  // do nothing
		  j->setText(nText.first);

		  // correct cursor position
		  if(cy==mCy-1)
		    {
		      // cursor is in next line
		      mCx-=count;
		      if(mCx<0)
			{
			  mCy--;
			  mCx+=i->length();
			}
		    }
		  else if(cy==mCy)
		    {
		      if(mCx>i->length())
			{
			  mCy++;
			  mCx-=i->length();
			}
		    }
		  if(nText.first=="")
		    {
		      // empty line
		      i->setHardEnd(j->hardEnd());
		      mLines.erase(j);
		    }
		}
	      else
		{
		  i++;
		  cy++;
		}
	    }
	  else
	    {
	      i++;
	      cy++;
	    }
	}
 
    }
}


void AGEdit::setFont(const AGFont &pFont)
{
  mFont=pFont;
  std::list<AGEditLine>::iterator i=mLines.begin();
  for(;i!=mLines.end();i++)
    i->setFont(mFont);

}

void AGEdit::setText(const std::string &pText)
{
  clear();

  size_t i;
  for(i=0;i<pText.length();i++)
    {
      if(pText[i]=='\n')
	{
	  getActLine();
	  AGEditLine l=actLine->split(mCx);
	  l.setAlign(mAlign);
	  l.setVAlign(mVAlign);
	  mCx=0;
	  mCy++;
	  insertLine(l);
	  checkWrap();
	}
      else
	{
	  insert(pText[i]);
	  mCx++;
	  checkWrap();
	}
    }
  setFont(mFont); // reset Font
  checkWrap();
  //  mCy=0;
  //  mCx=mT
  setFont(mFont); // reset Font
}
void AGEdit::setMutable(bool pMutable)
{
  mMutable=pMutable;
  mShowCursor=pMutable; // FIXME: show cursor only if widget has focus
}

void AGEdit::setAlign(AGAlign pAlign)
{
  mAlign=pAlign;
  std::list<AGEditLine>::iterator i=mLines.begin();
  for(;i!=mLines.end();i++)
    i->setAlign(pAlign);
}

void AGEdit::setVAlign(AGVAlign pVAlign)
{
  mVAlign=pVAlign;
  std::list<AGEditLine>::iterator i=mLines.begin();
  for(;i!=mLines.end();i++)
    i->setVAlign(pVAlign);
}

void AGEdit::setBackground(const AGBackground &bg)
{
  mDrawBackground=true;
  mBackground=bg;
}


void AGEdit::setBackground(bool pDrawBackground)
{
  mDrawBackground=pDrawBackground;
}

void AGEdit::setTheme(const std::string &s)
{
  std::string ms=s;
  if(ms=="")
    ms="edit";

  AGFont font=getTheme()->getFont(s+".font");
  setFont(font);
  mTheme=s;

  std::string t=mTheme;
  if(t.length())
    t+=".";

  mBackground=AGBackground(t+"background");
}

bool AGEdit::canFocus() const
{
  return mMutable;
}

AGEdit &toAGEdit(AGWidget &w)
{
  return dynamic_cast<AGEdit&>(w);
}

std::string AGEdit::getText() const
{
  std::ostringstream os;
  std::list<AGEditLine>::const_iterator i=mLines.begin();
  for(;i!=mLines.end();)
    {
      bool hard=i->hardEnd();
      os<<i->getText();
      i++;
      if(i!=mLines.end() && hard)
	os<<std::endl;
    }
  return os.str();
}

void AGEdit::clear()
{
  mLines.clear();
  mCx=mCy=0;
  mViewCy=0;
  AGFont font1("Arial.ttf",14);
  AGEditLine l("",font1,true);
  l.setAlign(mAlign);
  l.setVAlign(mVAlign);
  mLines.push_back(l);
}

