#include "ag_glsurface.h"
#include "ag_color.h"
#include "ag_debug.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <assert.h>
#include <set>
#include <map>
#include <vector>
#include <sge.h>
#include "SDL_image.h"

#include <SDL.h>

#include "ag_vector.h"


std::set<SDL_Surface *> glTestSurfaces;


void initDraw()
{
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  glDisable(GL_DEPTH_TEST); // enable depth test
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // clear screen and depth buffer
  glLoadIdentity();
  glDepthMask(false);
  //  glTranslatef(0.375, 0.375, 0.0);

}

void initGUIView(int w,int h)
{
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );

  GLfloat ratio;

  ratio = ( float )w / ( float )h;

  //  gluPerspective( 45.0f, ratio, 1.0f, 100.0f );

  gluOrtho2D(0,w,0,h);

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );

}

void myFlip()
{
  //  TRACE;
  SDL_GL_SwapBuffers();
  SDL_Delay(10);
}

AGGLScreen::AGGLScreen(int W,int H):
  w(W),h(H)
{
  // init GL
  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);
  glClearColor(0.0f,0.0f,0.0f,0.0f); // clear bgcolor
  glClearDepth(1.0f);      // clear depth buffer
  glEnable(GL_DEPTH_TEST); // enable depth test
  glDepthFunc(GL_LEQUAL); // set type depth test
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // GL_NICEST // best perspective correction
  glEnable(GL_BLEND);

  glViewport( 0, 0, w, h );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );

  GLfloat ratio;

  ratio = ( float )w / ( float )h;

  //  gluPerspective( 45.0f, ratio, 1.0f, 100.0f );

  gluOrtho2D(0,w,0,h);

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );
  //  glTranslatef(0.375, 0.375, 0.0);

}




AGGLScreen::~AGGLScreen()
{ 
}

void AGGLScreen::checkUnusedTextures()
{
  std::set<TextureID> toDel;
  std::map<TextureID,SDL_Surface*>::iterator i=mTextures.begin();
  for(;i!=mTextures.end();i++)
    {
      if(mUsedTextures[i->first]==false)
	toDel.insert(i->first);
    }

  std::set<TextureID>::iterator j=toDel.begin();
  for(;j!=toDel.end();j++)
    {
      SDL_Surface *s=mTextures[*j];
      mTexturesInv.erase(s);
      mTextures.erase(*j);
      
      // find gl-surface
      /*      std::map<SDL_Surface*,SDL_Surface*>::iterator k= mGLSurfaces.begin();

      for(;k!=mGLSurfaces.end();k++)
	{
	  if(k->second==s)
	    {
	      mGLSurfaces.erase(k);
	      break;
	    }
	}
	SDL_FreeSurface(s);*/
      //      cdebug("Texture "<<*j<<" deleted");
    }

}


AGSurface *s[6];
TextureID tids[6];
bool inited=false;
/*
void AGGLScreen::paintTerrain()
{
  if(!inited)
    {
      int x,y,z;

      for(z=0;z<6;z++)
	{
	  s[z]=new AGSurface(32,32);
	}

      std::vector<int> hs(32*32);

      for(x=0;x<32;x++)
	for(y=0;y<32;y++)
	  hs[x+y*32]=rand()%4;

      for(x=0;x<32;x++)
	for(y=0;y<32;y++)
	  for(z=0;z<2+hs[x+y*32];z++)
	    {
	      AGColor c;

	      if(z<1)
		c=AGColor(0xAA+rand()%20,0x77+rand()%20,0);
	      else
		c=AGColor(0,0xFF-rand()%20,0);

	      sge_PutPixel(s[z]->s,x,y,c.mapRGB(s[z]->surface()->format));
	    }

      std::map<std::pair<int,int>,bool> shadow;
      // shadow
      for(z=5;z>=0;z--)
	for(x=0;x<32;x++)
	for(y=0;y<32;y++)
	  //for(z=5;z>=0;z--)
	    {
	      Uint8 R,G,B,A;
	      Uint32 c=sge_GetPixel(s[z]->surface(),x,y);

	      SDL_GetRGBA(c,s[z]->surface()->format,&R,&G,&B,&A);

	      //  cdebug(x<<"//"<<z);
	      
	      if(A>0)
		{
		  int sx=x+z,sy=y;

		  sx%=32;
		  sy%=32;
		  if(shadow[std::make_pair(sx,sy)])
		    {
		      
		      R>>=1;
		      G>>=1;
		      B>>=1;
		      sge_PutPixel(s[z]->surface(),x,y,SDL_MapRGBA(s[z]->surface()->format,R,G,B,A));
		    }
		  shadow[std::make_pair(sx,sy)]=true;
		}
	    }



      for(z=0;z<6;z++)
	{
	  tids[z]=getID(s[z]->surface());
	}
      inited=true;
    }


  //  glViewport( 0, 0, w, h );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );

  GLfloat ratio;

  ratio = ( float )w / ( float )h;

  gluPerspective( 45.0f, ratio, 1.0f, 100.0f );
  //glOrtho(0,w,0,h,1,100);

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );



  //  glScalef(3,3,1);
  //  glScalef(0.5,0.5,0.5);

  glColor4f(1,1,1,1);//1,1,1);//0,0);
  
  glBindTexture(GL_TEXTURE_2D,getID(s[0]->surface()));

  
  Vec3f v[4];
  float W=4.0/w;

  W*=32;


  v[0]=Vec3f(-W,-0.4,-4); //lf
  v[1]=Vec3f(-W,-0.4,-6); //lb
  v[2]=Vec3f( W,-0.4,-6); //rb
  v[3]=Vec3f( W,-0.4,-4); //rf

  Vec2f tv[4];
  tv[0]=Vec2f(0,0);
  tv[1]=Vec2f(0,1);//1
  tv[2]=Vec2f(1,1);//1
  tv[3]=Vec2f(1,0);

  for(int j=0;j<4;j++)
    {
      v[j][0]*=4;
      v[j][1]*=4;
      tv[j]*=4;

      //      v[j][2]+=1;
    }

  for(int j=0;j<6;j++)
    {
      float alpha=1.0;//0.5/(j+1)+0.5;
      //      alpha*=alpha;
      glBindTexture(GL_TEXTURE_2D,getID(s[j]->surface()));
      glColor4f(1,1,1,alpha);

      glBegin(GL_QUADS);
      
      for(int i=0;i<4;i++)
	{
	  glTexCoord2fv(tv[i]);
	  glVertex3fv(v[i]);
	  v[i]+=Vec3f(0,5.0/h,0);
	}
      glEnd();
    }
    
  
}
*/

void AGGLScreen::flip()
{
  //  paintTerrain();
  //  TRACE;
  myFlip();
  checkUnusedTextures();
  initDraw();

  mUsedTextures.clear();

  initGUIView(w,h);
}

size_t next2pow(size_t i)
{
  size_t j=1;
  while(j<i)
    j<<=1;

  return j;
}

SDL_Surface *AGGLScreen::newSurface(int x,int y)
{
  int bytes=4;
  
  size_t nw=next2pow(std::max(x,y));

  return  SDL_CreateRGBSurface(    SDL_SWSURFACE, nw,nw, bytes*8,0xff, 0xff<<8, 0xff<<16, 0xff<<24 );
  
}


SDL_Surface *toGLTexture(SDL_Surface *image)
{
  assert(image);

  int bytes = 4;
  //  if ( image->format->BitsPerPixel == 32 )
  //    bytes = 4;

  size_t nw=next2pow(std::max(image->w,image->h));

  SDL_Surface* openGLSurface = SDL_CreateRGBSurface(    SDL_SWSURFACE, nw,nw, bytes*8,
                                                        0xff, 0xff<<8, 0xff<<16, 0xff<<24 );

  //  cdebug((int)(image->format->BitsPerPixel));
  if (false)// image->format->BitsPerPixel != 32 )
    {
      //      TRACE;
      SDL_BlitSurface( image, 0, openGLSurface, 0 );
    }
  else
    {
      //      TRACE;
      Uint8 r, g, b, a;
      Uint32 ckey=image->format->colorkey;

      // Grr! Screws up alpha channel. Fix the colors.
      for( int j=0; j<image->h; ++j )
        {
          for( int i=0; i<image->w; ++i )
            {
              Uint32 c = sge_GetPixel( image, i, j );
		{
		  SDL_GetRGBA( c, image->format, &r, &g, &b, &a );
		  if(c==ckey)
		    a=0;//r=255,b=0;
		  //		  a=255;
		  Uint32 cprime = SDL_MapRGBA( openGLSurface->format, r, g, b, a );
		  sge_PutPixel( openGLSurface, i, j, cprime);
		  //		  sge_PutPixelAlpha( openGLSurface, i, j, cprime,a );
		}
            }
        }
    }


  glTestSurfaces.insert(openGLSurface);
  //  SDL_FreeSurface( image );
  return openGLSurface;
}

AGSurface AGGLScreen::loadSurface(const std::string &pFilename)
{
  //  CTRACE;
  SDL_Surface *s=IMG_Load(pFilename.c_str());
  if(!s)
   {
	std::cout<<"Error loading file:"<<pFilename<<std::endl;
	return AGSurface();
   }
  int w,h;
  w=s->w;
  h=s->h;
  //  SDL_SaveBMP(s,"load1.bmp");
  //  SDL_Surface *n=toGLTexture(s);
  //  SDL_FreeSurface(s);
  //  SDL_SaveBMP(n,"load2.bmp");
  //  cdebug("load:"<<n);
  return AGSurface(s,w,h);
}


GLuint assignTexture(SDL_Surface *pSurface)
{
  SDL_Surface* surface = pSurface;//toGLTexture(pSurface);
  //  assert(surface);

  assert(glTestSurfaces.find(pSurface)!=glTestSurfaces.end());

  int size = surface->w;

  assert( surface->w == surface->h );

  // Used to invert the surface. There must be a better way.
  SDL_Surface* texSurface = SDL_CreateRGBSurface(       SDL_SWSURFACE,
                                                        size, size,
                                                        surface->format->BitsPerPixel,
                                                        0xff, 0xff<<8, 0xff<<16, 0xff<<24 );

  for( int j=0; j<size; ++j )
    memcpy(     (Uint8*) texSurface->pixels + texSurface->pitch * ( size - 1 - j),
                (Uint8*) surface->pixels + surface->pitch * j,
                size * texSurface->format->BytesPerPixel );

  GLuint id;
  glGenTextures( 1, &id);
  //  cdebug("newid:"<<id);
  //  textureIDs[textureName]=id;
  glBindTexture( GL_TEXTURE_2D,id);// textureIDs[textureName] );

  //  cout<<"textureID:"<<textureIDs[textureName]<<endl;

  int format = ( texSurface->format->BytesPerPixel == 4 ) ? GL_RGBA : GL_RGB;

  glTexImage2D( GL_TEXTURE_2D,
                0,                                      // no mip mapping
                format,
                texSurface->w,
                texSurface->h,
                0,                                      // no border
                format,                 // format
                GL_UNSIGNED_BYTE,
                texSurface->pixels );

  assert( glGetError() == GL_NO_ERROR );

  SDL_FreeSurface( texSurface );
  //  SDL_FreeSurface( surface );

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//NEAREST);//LINEAR);

  assert( glGetError() == GL_NO_ERROR );

  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  
  glBindTexture( GL_TEXTURE_2D,0);

  return id;
}

void deleteTexture(GLuint id)
{
  glDeleteTextures(1,&id);
}




TextureID AGGLScreen::getID(SDL_Surface *s)
{
  //  if(mGLSurfaces[s]==0)
  //    mGLSurfaces[s]=toGLTexture(s);

  SDL_Surface *p=s;//mGLSurfaces[s];
  GLuint id;

  if(mTexturesInv.find(p)==mTexturesInv.end())
    {
      mTexturesInv[p]=assignTexture(p);
      mSurfaceRect[s]=AGRect(0,0,s->w,s->h);
    }
  id=mTexturesInv[p];
  mTextures[mTexturesInv[p]]=p;

  mUsedTextures[id]=true;

  return id;
}

AGRect AGGLScreen::getRect(SDL_Surface *s)
{
  return mSurfaceRect[s];
}

bool AGGLScreen::inScreen(const AGRect &r) const
{
  return !(r.x>w || r.y>h || (r.x+r.w-1<0) || (r.y+r.h-1<0));
}



void AGGLScreen::blit(const AGTexture &pSource,const AGRect &pRect)
{
  if(!inScreen(pRect))
    return;


  //  TRACE;
  int w2=std::min((int)pRect.w,pSource.width());
  int h2=std::min((int)pRect.h,pSource.height());

  float x0=pRect.x;
  float y0=h-1-pRect.y;
  float x1=pRect.x+w2;
  float y1=h-1-(pRect.y+h2);

  //  cdebug(x0<<";"<<y0<<";"<<x1<<";"<<y1);
  //  cdebug(pSource.width()<<";"<<pSource.height());
  
  //  cdebug(tw<<";"<<th);
  
  
  SDL_Surface *surface=const_cast<AGTexture&>(pSource).s;
  
  //cdebug(surface);
  //  SDL_SaveBMP(surface,"test.bmp");
  
  TextureID id=getID(surface);

  glBindTexture( GL_TEXTURE_2D,id);
  assert( glGetError() == GL_NO_ERROR );

  AGRect sRect=getRect(surface);

  float tw=float(pSource.width())/surface->w;
  float th=float(pSource.height())/surface->h;

  //  cdebug(tw<<";"<<th);

  glColor4f(1,1,1,1);
  glBegin(GL_TRIANGLES);

  glTexCoord2f(0,1);
  glVertex2f(x0,y0);
  glTexCoord2f(tw,1);
  glVertex2f(x1,y0);
  glTexCoord2f(0,1-th);
  glVertex2f(x0,y1);

  glTexCoord2f(tw,1);
  glVertex2f(x1,y0);
  glTexCoord2f(tw,1-th);
  glVertex2f(x1,y1);
  glTexCoord2f(0,1-th);
  glVertex2f(x0,y1);

  glEnd();
  glBindTexture( GL_TEXTURE_2D,0);
}

void AGGLScreen::tile(const AGTexture &pSource)
{
  TextureID id=getID(const_cast<AGTexture&>(pSource).s);//urface());

  float x0=0;
  float y0=h-1;
  float x1=w-1;
  float y1=0;

  float w2=x1-x0;
  float h2=y1-y0;

  float mw=float(w2)/pSource.width();
  float mh=float(h2)/pSource.height();

  glBindTexture( GL_TEXTURE_2D,id);
  glColor4f(1,1,1,1);
  assert( glGetError() == GL_NO_ERROR );

  glBegin(GL_TRIANGLES);
  glTexCoord2f(0,mh);
  glVertex2f(x0,y0);

  glTexCoord2f(mw,mh);
  glVertex2f(x1,y0);

  glTexCoord2f(0,0);
  glVertex2f(x0,y1);
  
  glTexCoord2f(mw,mh);
  glVertex2f(x1,y0);

  glTexCoord2f(mw,0);
  glVertex2f(x1,y1);

  glTexCoord2f(0,0);
  glVertex2f(x0,y1);

  glEnd();
  glBindTexture( GL_TEXTURE_2D,0);
}
void AGGLScreen::tile(const AGTexture &pSource,const AGRect &pDest)
{
  if(!inScreen(pDest))
    return;
  tile(pSource,pDest,AGRect(0,0,pSource.width(),pSource.height()));
  return;
  TextureID id=getID(const_cast<AGTexture&>(pSource).s);//urface());

  float x0=pDest.x;
  float y0=h-1-(pDest.y);
  float x1=pDest.x+pDest.w;
  float y1=h-1-(pDest.y+pDest.h);

  float w2=x1-x0;
  float h2=y0-y1;

  float mw=float(w2)/pSource.width();
  float mh=float(h2)/pSource.height();

  glBindTexture( GL_TEXTURE_2D,id);
  glColor4f(1,1,1,1);
  assert( glGetError() == GL_NO_ERROR );

  glBegin(GL_TRIANGLES);
  glTexCoord2f(0,mh);
  glVertex2f(x0,y0);

  glTexCoord2f(mw,mh);
  glVertex2f(x1,y0);

  glTexCoord2f(0,0);
  glVertex2f(x0,y1);
  
  glTexCoord2f(mw,mh);
  glVertex2f(x1,y0);

  glTexCoord2f(mw,0);
  glVertex2f(x1,y1);

  glTexCoord2f(0,0);
  glVertex2f(x0,y1);

  glEnd();
  glBindTexture( GL_TEXTURE_2D,0);
}
void AGGLScreen::tile(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc)
{
  if(!inScreen(pDest))
    return;
  //  CTRACE;
  //  cdebug(pSource.width()<<"/"<<pSource.height());
  //  cdebug(pDest<<";"<<pSrc);
  
  TextureID id=getID(const_cast<AGTexture&>(pSource).s);//urface());

  float x0=pDest.x;
  float y0=h-1-(pDest.y);
  float x1=pDest.x+pSrc.w;
  float y1=h-1-(pDest.y+pSrc.h);

  SDL_Surface *surface=const_cast<AGTexture&>(pSource).s;

  float tw,th;
  float sx0,sx1,sy0,sy1;

  sx0=float(pSrc.x)/surface->w;
  sy0=1-float(pSrc.y)/surface->h;


  //  float tw=float(pSource.width())/surface->w;
  //  float th=float(pSource.height())/surface->h;


  glBindTexture( GL_TEXTURE_2D,id);
  glColor4f(1,1,1,1);
  glBegin(GL_TRIANGLES);
  for(int px=0;px<pDest.w;px+=pSrc.w)
    for(int py=0;py<pDest.h;py+=pSrc.h)
      {
	float px0=x0+px;
	float py0=y0-py;
	float px1=px0+std::min((int)pSrc.w,pDest.w-px);
	float py1=py0-std::min((int)pSrc.h,pDest.h-py);
	//	float px1=x1+std::min(px;
	//	float py1=y1-py;

	tw=float(std::min((int)pSrc.w,pDest.w-px))/surface->w;
	th=float(std::min((int)pSrc.h,pDest.h-py))/surface->h;

	sx1=sx0+tw;
	sy1=sy0-th;

	glTexCoord2f(sx0,sy0);
	glVertex2f(px0,py0);

	glTexCoord2f(sx1,sy0);
	glVertex2f(px1,py0);

	glTexCoord2f(sx0,sy1);
	glVertex2f(px0,py1);

	
	glTexCoord2f(sx1,sy0);
	glVertex2f(px1,py0);

	glTexCoord2f(sx1,sy1);
	glVertex2f(px1,py1);

	glTexCoord2f(sx0,sy1);
	glVertex2f(px0,py1);
      }
  glEnd();
	
  glBindTexture( GL_TEXTURE_2D,0);



  /*

  float sx0=float(pSrc.x)/pSource.width();
  float sy0=1-float(pSrc.y)/pSource.height();
  float sx1=float(pSrc.x+pSrc.w-1)/pSource.width();
  float sy1=1-float(pSrc.y+pSrc.h-1)/pSource.height();

  glBindTexture( GL_TEXTURE_2D,id);
  glColor4f(1,1,1,1);
  assert( glGetError() == GL_NO_ERROR );

  glBegin(GL_TRIANGLES);
  glTexCoord2f(sx0,sy0);
  glVertex2f(x0,y0);

  glTexCoord2f(sx1,sy0);
  glVertex2f(x1,y0);

  glTexCoord2f(sx0,sy1);
  glVertex2f(x0,y1);
  
  glTexCoord2f(sx1,sy0);
  glVertex2f(x1,y0);

  glTexCoord2f(sx1,sy1);
  glVertex2f(x1,y1);

  glTexCoord2f(sx0,sy1);
  glVertex2f(x0,y1);

  glEnd();
  glBindTexture( GL_TEXTURE_2D,0);*/
}

void AGGLScreen::drawRect(const AGRect &pRect,const AGColor &c)
{
  //  return;
  float x0=pRect.x;
  float y0=h-pRect.y;
  float x1=pRect.x+pRect.w;
  float y1=h-pRect.y-pRect.h;

  glColor4f(c.r/255.0,c.g/255.0,c.b/255.0,c.a/255.0);


  glBegin(GL_TRIANGLES);
  glVertex2f(x0,y0);
  glVertex2f(x0,y1);
  glVertex2f(x1,y0);
  glVertex2f(x0,y1);
  glVertex2f(x1,y1);
  glVertex2f(x1,y0);
  glEnd();
}

AGRect AGGLScreen::getRect() const
{
  return AGRect(0,0,w,h);
}

void glColor(const AGColor &c)
{
  //  cdebug((int)c.r<<"/"<<(int)c.g<<"/"<<(int)c.b<<"/"<<(int)c.a);
  glColor4f(c.r/255.0,c.g/255.0,c.b/255.0,c.a/255.0);
}

void AGGLScreen::drawGradientAlpha(const AGRect& pRect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
{
  //    TRACE;
  float x0=pRect.x;
  float y0=(h-1)-pRect.y;
  float x1=pRect.x+pRect.w;
  float y1=(h-1)-(pRect.y+pRect.h);

  AGColor ul2=AGColor(255,0,0);

  glBegin(GL_TRIANGLES);
  glColor(ul);
  glVertex2f(x0,y0);
  glColor(ur);
  glVertex2f(x1,y0);
  glColor(dl);
  glVertex2f(x0,y1);
  glColor(ur);
  glVertex2f(x1,y0);
  glColor(dr);
  glVertex2f(x1,y1);
  glColor(dl);
  glVertex2f(x0,y1);
  glEnd();
}

void AGGLScreen::drawBorder(const AGRect& rect,int W, const AGColor& c1, const AGColor& c2)
{
  //  TRACE;

  //  cdebug(rect);
  //  cdebug(c1<<"//"<<c2);
  float x0=rect.x;
  float y0=h-1-rect.y;
  float x1=rect.x+rect.w;
  float y1=h-1-(rect.y+rect.h);

  //  W=3;
  glBindTexture(GL_TEXTURE_2D,0);

  // left and //top
  glColor(c1);
  glBegin(GL_TRIANGLES);
  // top
  glVertex2f(x0,y0);
  glVertex2f(x1,y0);
  glVertex2f(x0+W,y0-W);

  glVertex2f(x1,y0);
  glVertex2f(x1-W,y0-W);
  glVertex2f(x0+W,y0-W);

  // left
  glVertex2f(x0,y0);
  glVertex2f(x0+W,y0-W);
  glVertex2f(x0+W,y1+W);

  glVertex2f(x0,y0);
  glVertex2f(x0+W,y1+W);
  glVertex2f(x0,y1);
  glEnd();

  // right and //bottom
  glColor(c2);
  glBegin(GL_TRIANGLES);
  // bottom
  glVertex2f(x0,y1);
  glVertex2f(x1,y1);
  glVertex2f(x0+W,y1+W);

  glVertex2f(x1,y1);
  glVertex2f(x1-W,y1+W);
  glVertex2f(x0+W,y1+W);

  // right
  glVertex2f(x1,y0);
  glVertex2f(x1-W,y0-W);
  glVertex2f(x1-W,y1+W);

  glVertex2f(x1,y0);
  glVertex2f(x1-W,y1+W);
  glVertex2f(x1,y1);
  glEnd();



}

void AGGLScreen::putPixel(int x,int y,const AGColor &pc)
{
  float X=x;
  float Y=h-1-y;

  Uint32 c;
  c=(pc.r<<24)|(pc.g<<16)|(pc.b<<8)|pc.a;

  glRasterPos2i((int)X,(int)Y);
  glDrawPixels(1,1,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,&c);
}

AGTexture AGGLScreen::displayFormat(SDL_Surface *s)
{
  //  CTRACE;
  //  cdebug(s->w<<"//"<<s->h);
  return AGTexture(toGLTexture(s),s->w,s->h);
}
