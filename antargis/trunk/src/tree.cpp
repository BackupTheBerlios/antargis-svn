#include "tree.h"

void drawBall(FastVoxelView &vv,Pos3D mp,float s,Color c)
{
  int x,y,z;
  int x0=(int)(mp.x-s);
  int y0=(int)(mp.y-s);
  int z0=(int)(mp.z-s);
  int x1=(int)(mp.x+s);
  int y1=(int)(mp.y+s);
  int z1=(int)(mp.z+s);

  float x5=-s;//+(int(-s)+s);
  float y5=-s;
  float z5=-s;

  float xd,yd,zd;

//  float s2=s*s;
  for(x=x0,xd=x5;x<=x1;x++,xd++)
    for(y=y0,yd=y5;y<=y1;y++,yd++)
      for(z=z0,zd=z5;z<=z1;z++,zd++)
	{
	  float diff=sqrt(xd*xd+yd*yd+zd*zd);
	  if(diff<s+3)
	    {
	      float a=s-diff;
	      if(a<0)
		a=0;

	      c.a=a;

	      if(a>0)
		vv.set(Pos3D(x,y,z),c);
	    }
	}
}

void draw3Line(FastVoxelView &vv,Pos3D p1,Pos3D p2,Pos3D p3,Color c,int a,float s,float s2)
{
  float fp=0,fa=1.0/a;
  int ip;
  Pos3D mp(0,0,0);
  //  cdebug(p1<<p2<<p3);
  for(ip=0;ip<=a;ip++)
    {
      float ms=(1-fp)*s+fp*s2;
      mp=bezier(fp,p1,p2,p3);
      //      cdebug(mp<<"//"<<fp<<"//"<<ms);
      drawBall(vv,mp,ms,c);
      fp+=fa;
    }
}

struct Trunk
{
  Pos3D p1,p2,p3;
  int w1,w2;
  Trunk(Pos3D P1,Pos3D P2,Pos3D P3,int W1,int W2):p1(P1),p2(P2),p3(P3),w1(W1),w2(W2)
  {
  }
};

Pos3D getRandPos(int s)
{
  int s2=s*2;
  return Pos3D((rand()%s2)/float(s)-1,
	       (rand()%s2)/float(s)-1,
	       (rand()%s2)/float(s)-1);
}

Pos3D getRandDirOrtho(Pos3D dir)
{
  return (getRandPos(250)%dir).normalized();
}

Pos3D randDir(Pos3D dir,float angle)
{
  return getRandDirOrtho(dir);
  cdebug("dir:"<<dir);
  dir=dir.normalized();
  cdebug("dir:"<<dir);
  Pos3D normal=getRandDirOrtho(dir).normalized();
  cdebug("normal:"<<normal);

  cdebug("*:"<<normal*dir);

  Pos3D res=rotVector(dir,normal,angle);

  cdebug("res:"<<res);

  return res;
}

float getRand01()
{
  return (rand()%1001)/1000.0;
}

// -1 to 1
float getRandEq()
{
  return getRand01()*2.0-1.0;
}

std::list<Pos3D> splitDir(Pos3D dir,int count,float angle,float sp_noise,float tu_noise)
{
  std::list<Pos3D> l;

  Pos3D myNorm=getRandDirOrtho(dir).normalized();
  Pos3D myBinorm=(dir%myNorm).normalized();
  
  for(int i=0;i<count;i++)
    {
      float ma=i*2*M_PI/count + getRandEq()*sp_noise;
      Pos3D norm=myNorm*sin(ma)+myBinorm*cos(ma);

      Pos3D r=rotVector(dir,norm,angle+getRandEq()*tu_noise).normalized();
      l.push_back(r);
    }
  return l;
}

void drawTree(FastVoxelView &vv,Pos3D base,float h)
{
  Pos3D p1(0,0,0),p2(0,0,0),p3(0,0,0);
  Color c(0x99,0x69,0);
  Color bc(0,0xAA,0);

  p1=base;
  p2=base+Pos3D(0,1,0)*h*0.25;
  p3=base+Pos3D(0,1,0)*h*0.5;

  cdebug(p1<<p2<<p3);

  Trunk t(p1,p2,p3,7,5);

  std::list<Trunk> tlist,nlist;

  tlist.push_back(t);

  int rw=(int)(h/4);

  float angle=M_PI/1.5; // 30 degrees

  int last=5;

  for(int i=0;i<=last;i++)
    {
      std::cout<<"_______________"<<std::endl;
      std::list<Trunk>::iterator k=tlist.begin();

      for(;k!=tlist.end();k++)
	{
	  Pos3D dir=k->p3 - k->p2;

	  int count=4;

	  std::list<Pos3D> l=splitDir(dir,count,angle,M_PI/count/8.0,0);//M_PI/count,angle/3);

	  std::list<Pos3D>::iterator j=l.begin();

	  for(;j!=l.end();j++)
	    {

	      Pos3D r=*j;


	      r=r*rw;

	      /*	      drawBall(vv,k->p1,4,c);
	      drawBall(vv,k->p2,3,c);
	      drawBall(vv,k->p3,2,c);*/

	      	  
	      draw3Line(vv,k->p1,k->p2,k->p3,c,40,k->w1,k->w2);
	      if(i==last)
		draw3Line(vv,k->p1,k->p2,k->p2,bc,4,2,1);


	      
	      float start=0.6;

	      start+=getRandEq()*0.3;

	      Pos3D sp=bezier(start,
			       k->p1,k->p2,k->p3);
	      //	      Pos3D np(k->p3*2-k->p2+Pos3D(rx,ry,rz));
	      Pos3D np(k->p3*2-k->p2+r);
	      




	      Trunk nt(sp,
		       sp*0.5+np*0.5+getRandPos(rw),
		       np,
		       k->w2,k->w2-1);
	      nlist.push_back(nt);


	    }

	}
      rw=(int)(rw*0.9);
      tlist.clear();
      cdebug(nlist.size());
      tlist=nlist;
      nlist.clear();
    }


  // roots
  std::list<Pos3D> l=splitDir(Pos3D(0,-1,0),4,M_PI/4,0,0);
  
  std::list<Pos3D>::iterator j=l.begin();
  for(;j!=l.end();j++)
    {
      Pos3D n=*j * 10 + base;
      draw3Line(vv,base,base*0.5+n*0.5,n,c,40,t.w1,t.w2);
    }
  

}

void drawPlane(FastVoxelView &vv,int vw)
{
  int x,y;
  for(x=0;x<vw;x++)
    for(y=0;y<vw;y++)
      vv.set(Pos3D(x,0,y),Color(1,1,1,1));
}

VoxelImage *makeTree()
{
  int vw=256;

  FastVoxelView vv(vw,vw,Pos3D(256,512,0),false,1);
  drawPlane(vv,vw);
  drawTree(vv,Pos3D(128,0,128),140);
  return new VoxelImage(vv.getSurface(),Pos3D(0,0,0));
}
