#include "ag_edit.h"
#include "ag_fontengine.h"
#include "ag_theme.h"
#include "ag_debug.h"
#include "ag_menu.h"
#include "sge.h"


AGEditLine::AGEditLine(const std::string &pText,AGFont pFont,bool pHardEnd):
  mText(pText),mFont(pFont),mHardEnd(pHardEnd)
{
}

AGEditLine::~AGEditLine()
{
}

AGFont AGEditLine::getFont() const
{
  return mFont;
}


void AGEditLine::draw(const AGPoint &pPoint,const AGRect &pClip)
{
  //  AGSurface s(getScreen());
  //  AGSurface s(
  AGFontEngine::renderText(&getScreen(),pClip,pPoint.x,pPoint.y,mText+(mHardEnd?"":"/"),mFont);
}

void AGEditLine::drawCursor(int cx,const AGPoint &pPoint,const AGRect &pClip,const AGColor &c)
{
  int x1=AGFontEngine::getWidth(mFont,mText.substr(0,cx));
  int x2=AGFontEngine::getWidth(mFont,mText.substr(0,cx+1));

  //  SDL_SetClipRect(getScreen().surface(),const_cast<AGRect*>(&pClip));
  getScreen().drawRect(AGRect(pPoint.x+x1,pPoint.y,x2,height()),c);
  //  sge_FilledRectAlpha(getScreen().surface(),pPoint.x+x1,pPoint.y,pPoint.x+x2,pPoint.y+height(),c.mapRGB(getScreen().surface()->format),c.a);
  //  SDL_SetClipRect(getScreen().surface(),0);
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
  
  //  cdebug(mText);
  std::vector<std::string>::iterator i=words.begin();
  int w=0,ow=0;
  std::string s,os;

  for(;i!=words.end();i++)
    {
      //      cdebug(*i);
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
      //      cdebug("mText:"<<mText<<":");
      //      cdebug("os:"<<os<<":");
      std::string n=mText.substr(os.length()+1,n.npos);
      mText=mText.substr(0,os.length()+1);
      //      cdebug("mText:"<<mText<<":");
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
      //      cdebug(*i);
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
  //  return std::make_pair(n,false);
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

void AGEditLine::setText(const std::string s)
{
  mText=s;
}






  
AGEdit::AGEdit(AGWidget *pParent,const AGRect &pRect):
  AGWidget(pParent,pRect),mCursorTime(0),mCursorLast(SDL_GetTicks()),
  mLShift(false),mRShift(false),mLCtrl(false),mRCtrl(false),mLAlt(false),mRAlt(false),
  mMultiLine(true),mWrapLines(true)
{
  mShowCursor=true;
  AGFont font1("Arial.ttf",14);
  AGFont font2("Arial.ttf",13);
  font1.setColor(AGColor(0,0,0));
  font2.setColor(AGColor(0,0,0xFF));
  mLines.push_back(AGEditLine("hallo1",font1,true));
  if(mMultiLine)
    {
      mLines.push_back(AGEditLine("hallo2",font2,true));
      mLines.push_back(AGEditLine("hallo3",font2,true));
    }
  mCx=mCy=0;
  mViewCy=0;


  AGMenu *menu=new AGMenu(this,AGPoint(0,0),"ContextMenu");
  menu->addItem("Copy");
  menu->addItem("Paste");
  setMenu(menu);

  mBackground=AGTexture(getTheme()->getSurface("backgroundSurface.menu"));
}

void AGEdit::draw(const AGRect &pRect)
{
  
  int x,y,cy;
  x=y=cy=0;
  
  drawBackground(pRect);

  std::list<AGEditLine>::iterator i=mLines.begin();

  AGRect mr(pRect.project(getRect()));

  AGColor cursorC;
  if(mShowCursor)
    cursorC=AGColor(0,0,0,0x7F);
  else
    cursorC=AGColor(0x7f,0x7f,0x7f,0x7f);
  
  if(mCy<mViewCy)
    mViewCy=mCy;

  for(int k=0;k<mViewCy;k++)
    i++;

  for(;i!=mLines.end();i++)
    {
      i->draw(mr.project(AGPoint(x,y)),pRect.project(getRect()));
      if(cy+mViewCy==mCy)
	
	i->drawCursor(mCx,mr.project(AGPoint(x,y)),pRect.project(getRect()),cursorC);
      y+=i->height();
      if(y>getRect().h)
	break;
      cy++;
      if(!mMultiLine)
	break;
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

void AGEdit::drawBackground(const AGRect &pRect)
{
  getScreen().tile(mBackground,pRect.project(getRect()));
}

bool AGEdit::eventKeyUp(const AGEvent *m2)
{
  const AGSDLEvent *m=reinterpret_cast<const AGSDLEvent*>(m2);
  if(m)
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
bool AGEdit::eventKeyDown(const AGEvent *m2)
{
  const AGSDLEvent *m=reinterpret_cast<const AGSDLEvent*>(m2);
  if(m)
    {
      SDLKey k=m->getKey();
      char ins=0;
      bool doInsert=false;
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
	}
      else if(k==SDLK_DELETE)
	{
	  doDelete(mCx);
	  checkWrap();
	}
      else if(k==SDLK_RETURN && mMultiLine)
	{
	  getActLine();
	  AGEditLine l=actLine->split(mCx);
	  mCx=0;
	  mCy++;
	  insertLine(l);
	  checkWrap();
	}
      else if(k==SDLK_END)
	{
	  getActLine();
	  mCx=actLine->length();
	}
      else if(k==SDLK_HOME)
	{
	  mCx=0;
	}

      else if(k==SDLK_LSHIFT)
	mLShift=true;
      else if(k==SDLK_RSHIFT)
	mRShift=true;
      else if(k==SDLK_LCTRL)
	mLCtrl=true;
      else if(k==SDLK_RCTRL)
	mRCtrl=true;
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

      if(doInsert)
	{
	  insert(ins);
	  mCx++;
	  checkWrap();
	  return true;
	}
    }
  return false;
}

void AGEdit::insert(char c)
{
  getActLine(); // FIXME:try to cache
  actLine->insert(c,mCx);
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
		i=mLines.insert(i,AGEditLine(n.first,actLine->getFont(),n.second));
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
