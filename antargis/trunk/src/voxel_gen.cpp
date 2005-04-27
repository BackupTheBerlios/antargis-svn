/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * voxel_gen.cpp
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

#include "voxel_gen.h"
#include "voxel.h"
#include "texture_cache.h"

#include <ag_color.h>
#include <ag_fontengine.h>
#include <ag_painter.h>


///////////////////////////////////////////////////////////////////////////////
// VoxelImageData
///////////////////////////////////////////////////////////////////////////////

VoxelImageData::VoxelImageData()
{
  std::string pFilename="voxelimagedata.xml";
  Document d;
  if(fileExists(pFilename))
  {
    std::string c=loadFile(pFilename);
    if(c.length())
      {
        d.parse_memory(c);
        loadXML(d.root());
      }
  }
}

VoxelImageData::~VoxelImageData()
{
  std::string pFilename="voxelimagedata.xml";
  xmlpp::Document d;
  Node *root=createRootNode(d,"antargisLevel");
  saveXML(*root);
  std::string c=toString(d);
  cdebug(c);
  saveFile(pFilename,c);
}

void VoxelImageData::loadXML(const xmlpp::Node &n)
{
  mPositions.clear();
  xmlpp::Node::const_iterator i=n.begin();
  for(;i!=n.end();i++)
  {
    mPositions.insert(std::make_pair((*i)->get("name"),Pos2D((*i)->get("pos"))));
  }
}
void VoxelImageData::saveXML(xmlpp::Node &n) const
{
  std::map<std::string,Pos2D>::const_iterator i=mPositions.begin();
  for(;i!=mPositions.end();i++)
  {
    xmlpp::Node &c=n.newChild("position");
    c.set("name",i->first);
    c.set("pos",i->second.toString());
  }

}
Pos2D VoxelImageData::getCenter(const std::string &pFilename)
{
  std::map<std::string,Pos2D>::const_iterator i=mPositions.find(pFilename);
  if(i==mPositions.end())
    return Pos2D(0,0);
  return i->second;
}
void VoxelImageData::setCenter(const std::string &pFilename,const Pos2D &pPos)
{
  mPositions.insert(std::make_pair(pFilename,pPos));
}

VoxelImageData *mVoxelImageData=0;
VoxelImageData *getVoxelID()
{
  if(!mVoxelImageData)
    mVoxelImageData=new VoxelImageData;
  return mVoxelImageData;
}


///////////////////////////////////////////////////////////////////////////////
// AVItem
///////////////////////////////////////////////////////////////////////////////


AVItem::AVItem(const Pos3D &p):inited(false),mPos(p),mCenter(0,0),virtualY(0)
{}

void AVItem::setPosition(const Pos3D &pPos)
{
  mPos=pPos;
}

Uint8 AVItem::getAlpha(const AGPoint &p) const
{
  AGColor c=mTexture.getPixel(p.x,p.y);
  return c.a;
}



AGTexture &AVItem::getTexture()
{
  if(!inited)
    init();
  return mTexture;
}

void AVItem::setCenter(const Pos2D &pCenter)
{
  mCenter=pCenter;
}

const Pos2D &AVItem::getCenter() const
{
  return mCenter;
}



AGSurface &AVItem::getSurface()
{
  CTRACE;
  if(!inited)
    init();
  inited=true;

  // filter surface

  int x,y;
  SDL_Surface *s=mSurface.surface();

  Uint32 trans=SDL_MapRGB(s->format,255,255,255);
  Uint32 shadow=SDL_MapRGB(s->format,191,191,191);
  Uint32 ntrans=SDL_MapRGBA(s->format,0,0,0,0);
  Uint32 nshadow=SDL_MapRGBA(s->format,0,0,0,0x7f);

  for(x=0;x<mSurface.width();x++)
    for(y=0;y<mSurface.height();y++)
      {
        Uint32 c=sge_GetPixel(s,x,y);
        if(c==trans)
          sge_PutPixel(s,x,y,ntrans);
        else if(c==shadow)
          sge_PutPixel(s,x,y,nshadow);

      }




  return mSurface;
}

AGPoint AVItem::getPosition(const Pos3D &pPos) const
  {
    // FIXME
    return AGPoint((int)(mPos.x-pPos.x),(int)(-(mPos.y-pPos.y)-(mPos.z-pPos.z)/2));
  }

AGRect AVItem::getRect(const Pos3D &pPos) const
  {
    return AGRect(0,0,mTexture.width(),mTexture.height())+getPosition(pPos);
  }

int AVItem::getZ(const Pos3D &pPos) const
  {
    return (int)(mPos.z-pPos.z-virtualY);//-(mTexture.height()-mCenter.y);
  }

void AVItem::draw(AntargisView *view,AGPainter &p)//const AGRect &r)
{
  AGRect ar=view->getRect(this);
  //  ar=r.project(ar);
  p.blit(getTexture(),ar);
}


///////////////////////////////////////////////////////////////////////////////
// AntargisView
///////////////////////////////////////////////////////////////////////////////


AntargisView::AntargisView(AGWidget *parent,const AGRect &pRect,const Pos3D &pPos,bool pOwnsItems):
    AGWidget(parent,pRect),mPos(pPos),ownsItems(pOwnsItems)
{}

class CompareAVItem
  {
    Pos3D mPos;
  public:
    CompareAVItem(Pos3D pPos):mPos(pPos)
    {}
    bool operator()(const AVItem *i1,const AVItem *i2) const
      {
        float z1=i1->getZ(mPos);
        float z2=i2->getZ(mPos);
      
        return z1>z2 ||(z1==z2 && i2->getPosition()<i1->getPosition());//i1->getZ(mPos)>i2->getZ(mPos);
      }
  };

void AntargisView::draw(AGPainter &p)//const AGRect &r)
{
  //  AGRect mr=r.project(AGWidget::getRect());
  std::vector<AVItem*>::iterator i;

  std::sort(mItems.begin(),mItems.end(),CompareAVItem(mPos));

  for(i=mItems.begin();i!=mItems.end();i++)
    {
      (*i)->draw(this,p);
    }
}

AGRect AntargisView::getRect(AVItem *item) const
{
      AGRect ar;
      Pos2D p(item->getCenter());
      ar=item->getRect(mPos)+AGPoint((int)-p.x,(int)(-p.y+height()+32));
  return ar;
}

void AntargisView::insert(AVItem *v)
{
  mItems.push_back(v);
}

void AntargisView::clear()
{
  if(ownsItems)
  {
    std::vector<AVItem*>::iterator i=mItems.begin();
    for(;i!=mItems.end();i++)
      delete *i;
  }
  mItems.clear();
}

///////////////////////////////////////////////////////////////////////////////
// VoxelImage
///////////////////////////////////////////////////////////////////////////////

VoxelImage::VoxelImage(AGSurface pSurface,Pos3D pPos):
    AVItem(pPos)
{
  mSurface=pSurface;
  mTexture=AGTexture(mSurface);
  //  mSurface=SDL_DisplayFormatAlpha(pSurface.surface());
  setCenter(Pos2D(mSurface.width()/2,mSurface.height()-mSurface.width()/4));
}

VoxelImage::VoxelImage(const std::string &pFilename):
    AVItem(Pos3D(0,0,0))
{
  mTexture=getTextureCache()->get(TILEDIR+pFilename+".png");
  // FIXME: Check this
  
  Pos2D c=getVoxelID()->getCenter(pFilename);
  if(c==Pos2D(0,0))
  {
    c=Pos2D(mTexture.width()/2,mTexture.height()-mTexture.width()/4);
  }
  
  setCenter(c);//Pos2D(mTexture.width()/2,mTexture.height()-mTexture.width()/4)+c);
}

void VoxelImage::setTexture(const std::string &pFilename)
{
  mTexture=getTextureCache()->get(pFilename+".png");
  // FIXME: Check this
  setCenter(Pos2D(mTexture.width()/2,mTexture.height()-mTexture.width()/4));
}

void VoxelImage::setName(const std::string &pName)
  {
    mName=pName;
  }
void VoxelImage::draw(AntargisView *view,AGPainter &p)//const AGRect &r)
{
  AVItem::draw(view,p);
  if(mName.length())
  {
    AGRect ar=view->getRect(this);
    //    ar=r.project(ar);
  
    ar.y-=14;
    p.renderText(mName,AGPoint(ar.x,ar.y),AGFont("data/Arial.ttf",13));
  }
}


void VoxelImage::init()
{}


// returns diff from uppper left corner
void VoxelImage::cutBorders()
{
  AGPainter p(mSurface);
  int minx,miny,maxx,maxy;
  bool found=false;
  
  minx=maxx=miny=maxy=0;
  
  cdebug(mSurface.width()<<"///"<<mSurface.height());
  for(int x=0;x<mSurface.width();x++)
    for(int y=0;y<mSurface.height();y++)
    {
      AGColor c=p.getPixel(x,y);
      if(c.a>0)
      {
        if(found)
        {
          minx=std::min(minx,x);
          miny=std::min(miny,y);
          maxx=std::max(maxx,x);
          maxy=std::max(maxy,y);
          //cdebug("x:"<<x<<" y:"<<y);
          //cdebug(minx<<"/"<<maxx<<","<<miny<<",m:"<<maxy);
        }
        else
        {
          minx=maxx=x;
          miny=maxy=y;
          found=true;
        }
      }
      
    }
    
  cdebug(minx<<";"<<maxx<<";"<<miny<<";"<<maxy);
  cdebug(mSurface.width()<<"///"<<mSurface.height());
    
  if(!found)
  {
    maxx=maxy=5;
    minx=miny=0;
   }
  int w=maxx-minx+1;
  int h=maxy-miny+1;
  
  w=std::max(10,w); // workaround for bad libpng calls
  h=std::max(10,h);
  
  if(w<=0 || h<=0)
    std::cerr<<"Error cutting! "<<w<<","<<h<<std::endl;
    
  AGSurface ns(w,h);
  AGPainter p2(ns);
  p2.blit(mSurface,AGRect(0,0,w,h),AGRect(minx,miny,w,h));
  mSurface=ns;
  mCenter=mCenter-Pos2D(minx,miny);
  
  if(w==0 || h==0)
    mSurface=AGSurface(1,1); // FIXME: this is some workaround
}

void VoxelImage::save(const std::string &pFilename)
{
  cutBorders();
  getVoxelID()->setCenter(pFilename,mCenter);  
  
  std::string c=toPNG(mSurface.surface());

  saveFile(TILEDIR+pFilename+".png",c);
}


/***************************************************************************************
 * makeTileFunctions
 ***************************************************************************************/



VoxelImage *makeTerrainTile(const SplineMapD &m,const SplineMapD &gm,int px,int py)
//ComplexVoxelImage *makeTerrainTile(const SplineMap<float> &m,const SplineMap<float> &gm,int px,int py)
{
  gDrawingTerrain=true;
  Uint32 t1=SDL_GetTicks();

  int w=TILE_WIDTH;
  FastVoxelView v(w,w*4,Pos3D(0,0,0),true);

  if(true)
  {
  for(int x=0;x<w;x++)
    for(int z=0;z<w;z++)
      //    for(int z=w/2-4;z<w/2;z++)
      {
        float mx=float(x*POINTS_PER_TILE)/w+px;
        float mz=float(z*POINTS_PER_TILE)/w+py;

        //  cdebug(mx<<" "<<mz);
        float h=m.get(mx,mz);
        for(int y=std::max(0,(int)(h-3));y<h;y++)
          {
            if(y>WATER_HEIGHT)
            {
              float a=std::min(1.0f,h-y);
              int n=rand()%50;
             
              v.set(Pos3D(x,y,z),Color(0xAA-n,0xAA-n,0,a));
            }
          }

        // grass above


        int mgh=(int)gm.get(mx,mz);
        if(mgh>0)
          {
            float gh=rand()%int(mgh);
            if(gh>0)
              for(int y=0;y<gh;y++)
                {
                  float a=1.0f-(y/gh);
                  int mh=(int)(y+h);
                  //    cdebug(a);
                  if(mh>WATER_HEIGHT)
                    v.set(Pos3D(x,mh,z),Color(0,0xAA,0,a));
                }
          }

        /*
        if(rand()%60<2)
          {
            int w=10;
            int w2=w/2;
            float x1=(rand()%w)-w2;
            float z1=(rand()%w)-w2;
            float x2=(rand()%w)-w2;
            float z2=(rand()%w)-w2;

            float steps=15;

            // paint a longer leaf
            for(int i=0;i<steps;i++)
              {
          int ax=(int)bezier2(i/steps,float(x),x1+x,x2+x);
          int az=(int)bezier2(i/steps,float(z),z1+z,z2+z);
          int ay=(int)bezier2(i/steps,0.0f,steps/4,steps/2);

          v.set(Pos3D(ax,int(ay+h),az),Color(0,0xAA,0,i/steps));
              }
          }
        */
      }
  /*
      for(int x=0;x<8;x++)
  for(int y=0;y<8;y++)
   for(int z=0;z<8;z++)
     {
       int mx=x-4;
       int my=y-4;
       int mz=z-4;
       if(sqrt(mx*mx+my*my+mz*mz)<4)
  v.set(Pos3D(x+10,y+40,z+20),Color(0xAA,0,0));
     }
     
  */
  }
  
 
  /** add water **/
  
  srand(0);
  if(false) // skip water
{  
  float a=18;

  SplineMapD wh((int)a,(int)a,3,1,3,true);

  for(int x=0;x<w;x++)
    for(int z=0;z<w;z++)
      {
        float mx=x-z+w/2;
        float mz=x+z-w/2;
        
        mx/=w;
        mz/=w;
        while(mx>1)
          mx-=1;
        while(mx<0)
          mx+=1;
        while(mz>1)
          mz-=1;
        while(mz<0)
          mz+=1;

        float h=wh.get(a*float(mx),a*float(mz))+6;

        for(int y=0;y<h;y++)
          {
            float f=std::min(1.0f,h-y);
            v.set(Pos3D(x,y,z),Color(0,0,0xAA,f));
          }
      }
 } 
  
  
  /** till here */
  
  Uint32 t2=SDL_GetTicks();

  cdebug("TIME:"<<t2-t1);

  AGSurface s= v.getSurface();

  t1=SDL_GetTicks();

  cdebug("TIME:"<<t1-t2);
  
  //SDL_SaveBMP(s.surface(),"hupe.bmp");


  //return new ComplexVoxelImage(Pos3D(0,0,0),s,v.getDepthMap(),v.getShadowMap());

  gDrawingTerrain=false;
  return new VoxelImage(s,Pos3D(0,0,0));
}

VoxelImage *makeWaterTile()
{
  int w=TILE_WIDTH;
  FastVoxelView v(w,w*2,Pos3D(0,0,0),true);

 
  /** add water **/
  
  srand(0);
{  
  float a=18;

  SplineMapD wh((int)a,(int)a,3,1,3,true);

  for(int x=0;x<w;x++)
    for(int z=0;z<w;z++)
      {
        float mx=x-z+w/2;
        float mz=x+z-w/2;
        
        mx/=w;
        mz/=w;
        while(mx>1)
          mx-=1;
        while(mx<0)
          mx+=1;
        while(mz>1)
          mz-=1;
        while(mz<0)
          mz+=1;

        float h=wh.get(a*float(mx),a*float(mz))+6;

        for(int y=0;y<h;y++)
          {
            float f=std::min(1.0f,h-y);
            v.set(Pos3D(x,y,z),Color(0,0,0xAA,f));
          }
      }
 } 
  
  
  /** till here */

  AGSurface s= v.getSurface();

  return new VoxelImage(s,Pos3D(0,0,0));
}



AGSurface makeWaterTileOld()
{
  int w=TILE_WIDTH;
  VoxelView v(w,w*2,Pos3D(0,0,0),true);

  float a=12;

  SplineMapD wh((int)a,(int)a,5,1,6);

  for(int x=0;x<w;x++)
    for(int z=0;z<w;z++)
      {
        float mx=x-z+w/2;
        float mz=x+z-w/2;

        float h=wh.get(a*float(mx)/w,a*float(mz)/w)+6;

        for(int y=std::max(0,(int)(h)-3);y<h;y++)
          {
            float f=std::min(1.0f,h-y);
            v.set(Pos3D(x,y,z),Color(0,0,0xAA,f));
          }
      }
  return v.getSurface();
}
