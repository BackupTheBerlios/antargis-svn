#include "ag_color.h"
#include "ag_geometry.h"
#include "ag_debug.h"
#include <SDL.h>

#include "ag_draw.h"
#include "sge.h"

void AGDraw::drawGradient(AGSurface &psurface, const AGRect& prect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr) 
  {
    AGRect rect=prect;
    Sint32 v00,v01,v02;
    Sint32 v10,v11,v12;
    
    Sint32 w = rect.w;
    Sint32 h = rect.h;

    SDL_Surface*surface=psurface.s;//surface();
    
    if (!surface)
      return;
    
    if(w == 0 || h == 0)
      return;
    
    if (w > surface->w || h > surface->h)
      return;
    
    Uint32 c1 = ul.mapRGB(surface->format);
    Uint32 c2 = ur.mapRGB(surface->format);
    Uint32 c3 = dl.mapRGB(surface->format);
    Uint32 c4 = dr.mapRGB(surface->format);
    
    // solid color gradient ?
    if((c1 == c2) && (c2 == c3) && (c3 == c4)) {
      //	  SDL_Rect r=rect.rect();
      SDL_FillRect(surface, &rect, c1);
      return;
    }
    
    AGRect clip;
    SDL_GetClipRect(surface, &clip);
    AGRect drawrect = rect.intersect(clip);

    if(drawrect.isNull()) {
      return;
    }
    
    int ox = drawrect.x - rect.x;
    int oy = drawrect.y - rect.y;
    
    if(SDL_MUSTLOCK(surface)) {
      SDL_LockSurface(surface);
    }		
    
    // color space vectors
    v00 = ((ur.r - ul.r) * 256) / w;
    v01 = ((ur.g - ul.g) * 256) / w;
    v02 = ((ur.b - ul.b) * 256) / w;
    
    v10 = ((dr.r - dl.r) * 256) / w;
    v11 = ((dr.g - dl.g) * 256) / w;
    v12 = ((dr.b - dl.b) * 256) / w;
    
    Sint32 r1, g1, b1;
    Sint32 r2, g2, b2;
    Sint32 yr, yg, yb;
    Sint32 r,g,b;
    
    r1 = ul.r * 256;
    g1 = ul.g * 256;
    b1 = ul.b * 256;
    
    r2 = dl.r * 256;
    g2 = dl.g * 256;
    b2 = dl.b * 256;
    
    // set colors with offset (ox)
    r1 += v00 * ox;
    g1 += v01 * ox;
    b1 += v02 * ox;
    r2 += v10 * ox;
    g2 += v11 * ox;
    b2 += v12 * ox;
    
    SDL_PixelFormat* format = surface->format;
    Uint8 Rloss = 8+format->Rloss;
    Uint8 Gloss = 8+format->Gloss;
    Uint8 Bloss = 8+format->Bloss;
    Uint8 Rshift = format->Rshift;
    Uint8 Gshift = format->Gshift;
    Uint8 Bshift = format->Bshift;
    //Uint8 Amask = format->Amask;
    
    Uint8 bpp = format->BytesPerPixel;
    Uint32 pitch = surface->pitch;
    Uint8* bits = ((Uint8 *) surface->pixels) + (rect.y + oy)* pitch + (rect.x + ox)* bpp;
    Uint32 y_pitch = pitch*drawrect.h - bpp;
    register Uint32 pixel = 0;
    
    for (register Sint32 x = 0; x < drawrect.w; x++) {
      
      yr = (r2 - r1) / h;
      yg = (g2 - g1) / h;
      yb = (b2 - b1) / h;
      
      r = r1;
      g = g1;
      b = b1;
      r += yr * oy;
      g += yg * oy;
      b += yb * oy;
      
      for (register Sint32 y = 0; y < drawrect.h; y++) {
	
	/* Set the pixel */
	switch (bpp) {
	case 1:
	  pixel = SDL_MapRGB ( surface->format, r>>8, g>>8, b>>8 );
	  *((Uint8 *) (bits)) = (Uint8) pixel;
	  break;
	  
	case 2:
	  pixel =  (r>>Rloss) << Rshift
	    | (g>>Gloss) << Gshift
	    | (b>>Bloss) << Bshift;
	  
	  *((Uint16 *) (bits)) = (Uint16) pixel;
	  break;
	  
	case 3: {
	  pixel =  (r>>Rloss) << Rshift
	    | (g>>Gloss) << Gshift
	    | (b>>Bloss) << Bshift;
	  
	  Uint8 ri = (pixel >> surface->format->Rshift) & 0xFF;
	  Uint8 gi = (pixel >> surface->format->Gshift) & 0xFF;
	  Uint8 bi = (pixel >> surface->format->Bshift) & 0xFF;
	  *((bits) + surface->format->Rshift / 8) = ri;
	  *((bits) + surface->format->Gshift / 8) = gi;
	  *((bits) + surface->format->Bshift / 8) = bi;
	}
	  break;
	  
	case 4:
	  pixel =  (r>>Rloss) << Rshift
	    | (g>>Gloss) << Gshift
	    | (b>>Bloss) << Bshift;
	  
	  *((Uint32 *) (bits)) = (Uint32) pixel;
	  break;
	}
	
	r += yr;
	g += yg;
	b += yb;
	
	// next pixel
	bits += pitch;
      }
      
      r1 += v00;
      g1 += v01;
      b1 += v02;
      r2 += v10;
      g2 += v11;
      b2 += v12;
      
      bits -= y_pitch;
    }
    
    if(SDL_MUSTLOCK(surface)) {
      SDL_UnlockSurface(surface);
    }		
  }

/*
void drawPixel(SDL_Surface *s,int x,int y,Uint32 mColor)
{
}

void AGDraw::drawGradientAlpha(AGSurface &psurface, const AGRect& prect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
{
  SDL_Surface*surface=psurface.surface();
  if(SDL_MUSTLOCK(surface))
    SDL_LockSurface(surface);

  SDL_PixelFormat* format = surface->format;
  Uint8 Rloss = 8+format->Rloss;
  Uint8 Gloss = 8+format->Gloss;
  Uint8 Bloss = 8+format->Bloss;
  Uint8 Rshift = format->Rshift;
  Uint8 Gshift = format->Gshift;
  Uint8 Bshift = format->Bshift;
    //Uint8 Amask = format->Amask;
    
  Uint8 bpp = format->BytesPerPixel;
  Uint32 pitch = surface->pitch;
  Uint8* bits = ((Uint8 *) surface->pixels) + (rect.y + oy)* pitch + (rect.x + ox)* bpp;
  Uint32 y_pitch = pitch*drawrect.h - bpp;
  register Uint32 pixel = 0;

  Sint32 r1, g1, b1;
  Sint32 r2, g2, b2;
  Sint32 yr, yg, yb;
  Sint32 r,g,b;
  
  for (register Sint32 x = 0; x < drawrect.w; x++) {
    
    yr = (r2 - r1) / h;
    yg = (g2 - g1) / h;
    yb = (b2 - b1) / h;
      
    r = r1;
    g = g1;
    b = b1;
    r += yr * oy;
    g += yg * oy;
    b += yb * oy;
    
    for (register Sint32 y = 0; y < drawrect.h; y++) {
	
	//////// Set the pixel 
	switch (bpp) {
	case 1:
	  pixel = SDL_MapRGB ( surface->format, r>>8, g>>8, b>>8 );
	  *((Uint8 *) (bits)) = (Uint8) pixel;
	  break;
	  
	case 2:
	  pixel =  (r>>Rloss) << Rshift
	    | (g>>Gloss) << Gshift
	    | (b>>Bloss) << Bshift;
	  
	  *((Uint16 *) (bits)) = (Uint16) pixel;
	  break;
	  
	case 3: {
	  pixel =  (r>>Rloss) << Rshift
	    | (g>>Gloss) << Gshift
	    | (b>>Bloss) << Bshift;
	  
	  Uint8 ri = (pixel >> surface->format->Rshift) & 0xFF;
	  Uint8 gi = (pixel >> surface->format->Gshift) & 0xFF;
	  Uint8 bi = (pixel >> surface->format->Bshift) & 0xFF;
	  *((bits) + surface->format->Rshift / 8) = ri;
	  *((bits) + surface->format->Gshift / 8) = gi;
	  *((bits) + surface->format->Bshift / 8) = bi;
	}
	  break;
	  
	case 4:
	  pixel =  (r>>Rloss) << Rshift
	    | (g>>Gloss) << Gshift
	    | (b>>Bloss) << Bshift;
	  
	  *((Uint32 *) (bits)) = (Uint32) pixel;
	  break;
	}
	
	r += yr;
	g += yg;
	b += yb;
	
	// next pixel
	bits += pitch;
      }
      
      r1 += v00;
      g1 += v01;
      b1 += v02;
      r2 += v10;
      g2 += v11;
      b2 += v12;
      
      bits -= y_pitch;
    }
    


  if(SDL_MUSTLOCK(surface)) 
    SDL_UnlockSurface(surface);
  		
}
*/

void AGDraw::drawGradientAlpha(AGSurface &psurface, const AGRect& prect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr) 
  {
    AGRect rect=prect;
    Sint32 v00,v01,v02,v03;
    Sint32 v10,v11,v12,v13;
    
    Sint32 w = rect.w;
    Sint32 h = rect.h;

    SDL_Surface*surface=psurface.s;//surface();
    
    if (!surface)
      return;
    
    if(w == 0 || h == 0)
      return;
    
    if (w > surface->w || h > surface->h)
      return;
    
    Uint32 c1 = ul.mapRGB(surface->format);
    Uint32 c2 = ur.mapRGB(surface->format);
    Uint32 c3 = dl.mapRGB(surface->format);
    Uint32 c4 = dr.mapRGB(surface->format);
    
    // solid color gradient ?
    if((c1 == c2) && (c2 == c3) && (c3 == c4)) {
      //	  SDL_Rect r=rect.rect();
      SDL_FillRect(surface, &rect, c1);
      return;
    }
    
    AGRect clip;
    SDL_GetClipRect(surface, &clip);
    AGRect drawrect = rect.intersect(clip);

    if(drawrect.isNull()) {
      return;
    }
    
    int ox = drawrect.x - rect.x;
    int oy = drawrect.y - rect.y;
    
    if(SDL_MUSTLOCK(surface)) {
      SDL_LockSurface(surface);
    }		
    
    // color space vectors
    v00 = ((ur.r - ul.r) * 256) / w;
    v01 = ((ur.g - ul.g) * 256) / w;
    v02 = ((ur.b - ul.b) * 256) / w;
    v03 = ((ur.a - ul.a) * 256) / w;
    
    v10 = ((dr.r - dl.r) * 256) / w;
    v11 = ((dr.g - dl.g) * 256) / w;
    v12 = ((dr.b - dl.b) * 256) / w;
    v13 = ((dr.a - dl.a) * 256) / w;
    
    Sint32 r1, g1, b1,a1;
    Sint32 r2, g2, b2,a2;
    Sint32 yr, yg, yb,ya;
    Sint32 r,g,b,a;
    
    r1 = ul.r * 256;
    g1 = ul.g * 256;
    b1 = ul.b * 256;
    a1 = ul.a * 256;
    
    r2 = dl.r * 256;
    g2 = dl.g * 256;
    b2 = dl.b * 256;
    a2 = dl.a * 256;
    
    // set colors with offset (ox)
    r1 += v00 * ox;
    g1 += v01 * ox;
    b1 += v02 * ox;
    a1 += v03 * ox;

    r2 += v10 * ox;
    g2 += v11 * ox;
    b2 += v12 * ox;
    a2 += v13 * ox;
    
    SDL_PixelFormat* format = surface->format;
    Uint8 Rloss = 8+format->Rloss;
    Uint8 Gloss = 8+format->Gloss;
    Uint8 Bloss = 8+format->Bloss;
    Uint8 Rshift = format->Rshift;
    Uint8 Gshift = format->Gshift;
    Uint8 Bshift = format->Bshift;
    //Uint8 Amask = format->Amask;
    
    Uint8 bpp = format->BytesPerPixel;
    Uint32 pitch = surface->pitch;
    Uint8* bits = ((Uint8 *) surface->pixels) + (rect.y + oy)* pitch + (rect.x + ox)* bpp;
    Uint32 y_pitch = pitch*drawrect.h - bpp;
    register Uint32 pixel = 0;

    /*
    cdebug("Rloss:"<<int(Rloss)<<" Rshift:"<<int(Rshift));
    cdebug("Gloss:"<<int(Gloss)<<" Gshift:"<<int(Gshift));
    cdebug("Bloss:"<<int(Bloss)<<" Bshift:"<<int(Bshift));
    cdebug("v03:"<<v03<<" v13:"<<v13<<" a1:"<<a1<<" a2:"<<a2);
    cdebug(int(ul.a)<<" "<<int(dl.a)<<" "<<int(ur.a)<<" "<<int(dr.a));
    */
    for (register Sint32 x = 0; x < drawrect.w; x++) {
      
      yr = (r2 - r1) / h;
      yg = (g2 - g1) / h;
      yb = (b2 - b1) / h;
      ya = (a2 - a1) / h;
      
      r = r1;
      g = g1;
      b = b1;
      a = a1;
      r += yr * oy;
      g += yg * oy;
      b += yb * oy;
      a += ya * oy;
      //      cdebug(ya<<" "<<a);
      
      for (register Sint32 y = 0; y < drawrect.h; y++) {
	
	/* Set the pixel */
	//	cdebug(int(bpp)<<" "<<a);
	switch (bpp) {
	case 1:
	  pixel = SDL_MapRGB ( surface->format, r>>8, g>>8, b>>8 );
	  *((Uint8 *) (bits)) = (Uint8) pixel;
	  break;
	  
	case 2:
	  pixel =  (r>>Rloss) << Rshift
	    | (g>>Gloss) << Gshift
	    | (b>>Bloss) << Bshift;
	  
	  *((Uint16 *) (bits)) = (Uint16) pixel;
	  break;
	  
	case 3: {
	  pixel =  (r>>Rloss) << Rshift
	    | (g>>Gloss) << Gshift
	    | (b>>Bloss) << Bshift;
	  
	  Uint8 ri = (pixel >> surface->format->Rshift) & 0xFF;
	  Uint8 gi = (pixel >> surface->format->Gshift) & 0xFF;
	  Uint8 bi = (pixel >> surface->format->Bshift) & 0xFF;

	  Uint8* br=(bits) + surface->format->Rshift / 8;
	  Uint8* bg=(bits) + surface->format->Gshift / 8;
	  Uint8* bb=(bits) + surface->format->Bshift / 8;
	  
	  *br=(*br *(255-a)+ri*a)>>8;
	  *bg=(*bg *(255-a)+gi*a)>>8;
	  *bb=(*bb *(255-a)+bi*a)>>8;
	  /*
	  *((bits) + surface->format->Rshift / 8) = *((bits) + surface->format->Rshift / 8)ri;
	  *((bits) + surface->format->Gshift / 8) = gi;
	  *((bits) + surface->format->Bshift / 8) = bi;

	  *((bits) + surface->format->Rshift / 8) = ri;
	  *((bits) + surface->format->Gshift / 8) = gi;
	  *((bits) + surface->format->Bshift / 8) = bi;*/
	}
	  break;
	  
	case 4:
	  Uint32 op=*((Uint32*)bits);
	  
	  Uint32 ar=(op>>Rshift)&0xFF;
	  Uint32 ag=(op>>Gshift)&0xFF;
	  Uint32 ab=(op>>Bshift)&0xFF;
	  Uint32 aa=a>>8;

	  ar<<=8;
	  ag<<=8;
	  ab<<=8;

	  ar=r*aa+ar*(255-aa);
	  ag=g*aa+ag*(255-aa);
	  ab=b*aa+ab*(255-aa);

	  ar>>=8;
	  ag>>=8;
	  ab>>=8;

	  //	  cdebug(ar<<" "<<ag<<" "<<ab);

	  pixel =  (ar>>Rloss) << Rshift
	    | (ag>>Gloss) << Gshift
	    | (ab>>Bloss) << Bshift;
	  /*	  pixel =  (r>>Rloss) << Rshift
	    | (g>>Gloss) << Gshift
	    | (b>>Bloss) << Bshift;*/
	  
	  *((Uint32 *) (bits)) = (Uint32) pixel;
	  break;
	}
	
	r += yr;
	g += yg;
	b += yb;
	a += ya;
	//	cdebug(a);
	// next pixel
	bits += pitch;
      }
      
      r1 += v00;
      g1 += v01;
      b1 += v02;
      a1 += v03;
      r2 += v10;
      g2 += v11;
      b2 += v12;
      a2 += v13;
      
      bits -= y_pitch;
    }
    
    if(SDL_MUSTLOCK(surface)) {
      SDL_UnlockSurface(surface);
    }		
  }


void AGDraw::drawBorder(AGSurface &surface, const AGRect& rect,int width, const AGColor& c1, const AGColor& c2)
{
  AGRect r=rect;

  Uint32 uc1=surface.color(c1);
  Uint32 uc2=surface.color(c2);

  for(int i=0;i<width;i++)
    {
      sge_HLine(surface.s,r.x,r.x+r.w-1,r.y,uc1);
      sge_HLine(surface.s,r.x,r.x+r.w-1,r.y+r.h-1,uc2);
      sge_VLine(surface.s,r.x,r.y,r.y+r.h-1,uc1);
      sge_VLine(surface.s,r.x+r.w-1,r.y,r.y+r.h-1,uc2);
      r=r.shrink(1);
    }
}

void AGDraw::drawGradient(AGGScreen *surface, const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
{
  surface->drawGradientAlpha(rect,ul,ur,dl,dr);
}
void AGDraw::drawGradientAlpha(AGGScreen *surface, const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
{
  surface->drawGradientAlpha(rect,ul,ur,dl,dr);
}
void AGDraw::drawGradientAlpha2(AGGScreen *surface, const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
{
  surface->drawGradientAlpha(rect,ul,ur,dl,dr);
}
void AGDraw::drawBorder(AGGScreen *surface, const AGRect& rect,int width, const AGColor& c1, const AGColor& c2)
{
  surface->drawBorder(rect,width,c1,c2);
}
