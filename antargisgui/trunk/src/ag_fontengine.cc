#include "ag_fontengine.h"
#include "ag_debug.h"
#include "sge.h"

#include <SDL_ttf.h>

#include <map>
#include <iostream>

struct FontInfo
{
  std::string pName;
  int size;

  bool operator<(const FontInfo &i) const
  {
    return pName<i.pName || (pName==i.pName && size<i.size);
  }
};

std::map<FontInfo,TTF_Font*> fontEngineFonts;
bool fontEngineInited=false;

TTF_Font *getFont(std::string s,int i)
{
  //  TRACE;
  FontInfo info;
  info.pName=s;
  info.size=i;

  if(fontEngineFonts.find(info)!=fontEngineFonts.end())
    return fontEngineFonts[info];

  if(!fontEngineFonts[info])
    {
      TTF_Font *mFont=0;
      // init library
      if(!fontEngineInited)
	{
	  fontEngineInited=true;
	  if(TTF_Init()<0)
	    {
	      std::cerr<<"Some error occured during initialization of libsdl-TTF"<<std::endl;
	      return 0;
	    }
	  atexit(TTF_Quit);

	}

      // load
      mFont=TTF_OpenFont(s.c_str(),i);
      if(!mFont)
	{
	  // Try another dir
	  //  mFont=TTF_OpenFont((GRAPHICS_DIR + pName).c_str(),pSize);
	  if(!mFont)
	    mFont=TTF_OpenFont(("c:\\Windows\\Fonts\\"+ s).c_str(),i);
	}
      //      std::cerr<<SDL_GetError()<<std::endl;

      int renderstyle=TTF_STYLE_NORMAL;
      
      if(mFont)
	TTF_SetFontStyle(mFont, renderstyle);
      
      fontEngineFonts[info]=mFont;
    }
  return fontEngineFonts[info];
}

AGFontEngine::AGFontEngine()
{
}

AGFontEngine::~AGFontEngine()
{
}

/*
bool AGFontEngine::renderText (AGSurface &pSurface, const AGRect &pClipRect, int BaseLineX, int BaseLineY, const std::string &pText, const AGFont &pFont)
{
  TTF_Font *f=getFont(pFont.getName(),pFont.getSize());

  if(!f)
    return false;

  if(pText.length()==0)
    return false;

  SDL_Surface *s=TTF_RenderText_Solid(f,pText.c_str(),pFont.getColor());//.sdlColor());

  SDL_Rect sr,dr;
  sr.x=sr.y=0;
  dr.x=BaseLineX;
  dr.y=BaseLineY;
  dr.w=sr.w=s->w;
  dr.h=sr.h=s->h;

  //  pSurface
  SDL_SetClipRect(pSurface.surface(),const_cast<AGRect*>(&pClipRect));
  //  cdebug(pClipRect.toString());
  SDL_BlitSurface(s,&sr,pSurface.surface(),&dr);
  SDL_SetClipRect(pSurface.surface(),0);
  //    SDL_SetClipRect(s,const_cast<AGRect*>(&pClipRect));
    //  sge_BlitSurface(s,&sr,pSurface.surface(),&dr);

  return true;
}
*/

std::map<std::pair<AGFont,std::string>,AGTexture> fontCache;

bool AGFontEngine::renderText (AGGScreen *pSurface, const AGRect &pClipRect, int BaseLineX, int BaseLineY, const std::string &pText, const AGFont &pFont)
{
  if(fontCache.find(make_pair(pFont,pText))==fontCache.end())
    {
      SDL_Surface *ns;
      TTF_Font *f=getFont(pFont.getName(),pFont.getSize());
      
      if(!f)
	return false;
      
      if(pText.length()==0)
	return false;
      
      ns=TTF_RenderText_Solid(f,pText.c_str(),pFont.getColor());//.sdlColor());
      
      SDL_Rect sr,dr;
      sr.x=sr.y=0;
      dr.x=BaseLineX;
      dr.y=BaseLineY;
      dr.w=sr.w=ns->w;
      dr.h=sr.h=ns->h;
      
      SDL_SetColorKey(ns,SDL_SRCCOLORKEY,sge_GetPixel(ns,0,0));
      
      AGTexture ms=getScreen().displayFormat(ns);
      SDL_FreeSurface(ns);
	
      fontCache[make_pair(pFont,pText)]=ms;
      
      //      SDL_SetColorKey(s,SDL_SRCCOLORKEY,sge_GetPixel(s,0,0));
    }
  AGTexture s=fontCache[make_pair(pFont,pText)];

  AGTexture t(s);
  
  pSurface->blit(t,AGRect(BaseLineX,BaseLineY,s.width(),s.height()));
    

  if(fontCache.size()>100)
    {
      // clear font cache
      fontCache.clear();
    }

  // SDL_FreeSurface(s);
  /*

  //  pSurface
  SDL_SetClipRect(pSurface.surface(),const_cast<AGRect*>(&pClipRect));
  //  cdebug(pClipRect.toString());
  SDL_BlitSurface(s,&sr,pSurface.surface(),&dr);
  SDL_SetClipRect(pSurface.surface(),0);
  //    SDL_SetClipRect(s,const_cast<AGRect*>(&pClipRect));
    //  sge_BlitSurface(s,&sr,pSurface.surface(),&dr);
    */
  return true;
}



int AGFontEngine::getWidth(const AGFont &pFont,const std::string &pText)
{
  TTF_Font *f=getFont(pFont.getName(),pFont.getSize());

  if(!f)
    return 0;
  
  int w,h;
  int ret=TTF_SizeText(f,pText.c_str(),&w,&h);
  if(ret)
    return 0;
  return w;
}

int AGFontEngine::getHeight(const AGFont &pFont,const std::string &pText)
{
  TTF_Font *f=getFont(pFont.getName(),pFont.getSize());

  if(!f)
    return 0;
  
  int w,h;
  int ret=TTF_SizeText(f,pText.c_str(),&w,&h);
  if(ret)
    return 0;
  //  cdebug("fontsize: w:"<<w<<" h:"<<h<<endl);
  return h;
}
