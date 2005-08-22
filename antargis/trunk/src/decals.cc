#include "decals.h"

Decals::Decals(Heightmap *map)
{

  AGVector4 v,d;
  Color4f c;
  float px,py,angle;
  
  long anzahl=0;
  AGVector4 sun(2.0, 0.0, 3.1);
  
  for(int i=0;i<anzahl;i++)
    {
      px=(rand()%(mW*32))/32.0;
      py=mW-float(i)*mW/anzahl;
      //	  py=(rand()%(mW*32))/32.0;
      angle=(rand()%1000)/1000.0*2*M_PI;
      
      
      angle = M_PI/2 + (((rand()%1000)/1000.0)-0.5)*M_PI/4;
      
      v=AGVector4(getX(px),getX(py),getHeight(px,py));
      d=AGVector4(1,0,0);//sin(angle),cos(angle),0);
      d=AGVector4(sin(angle),cos(angle),0);
      
      float l=getNewNormal(px,py)|sun;//x(rand()%100)/100.0;
      
      l=0.5*l+0.5;
      
      c=Color4f(l,l,l,1);
      
      mDecals.push_back(Decal(v,d,c));
      //	  cout<<px<<","<<py<<endl;
      //	  cout<<v<<"//"<<d<<endl;
    }

}
