#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <fstream>

int main(int argc,char *argv[])
{
	std::ofstream os("output.txt");

  // init SDL
  bool gl=true;  // opengl
  bool fs=false; // fullscreen

  if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE|SDL_INIT_AUDIO)<0)
    {
      os<<"SDL could not be initialized!"<<std::endl;
      exit(1);
    }

  const SDL_VideoInfo *videoInfo=0;

  if(!videoInfo)
    {
      videoInfo = SDL_GetVideoInfo();
      if(!videoInfo)
	{
	  os<<"SDL could not get video-info"<<std::endl;
	  exit(1);
	}
    }
  int videoFlags=0;

  if(gl)
    {
      videoFlags|=SDL_OPENGL;
      os<<"initing opengl"<<std::endl;
    }
  else
    videoFlags |= SDL_SWSURFACE;

  if(fs)
    videoFlags|=SDL_FULLSCREEN;

  int w=1024;
  int h=768;

  os<<"bits per pixel:"<<(int)videoInfo->vfmt->BitsPerPixel<<std::endl;

  // set video mode
  //  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface *ms=SDL_SetVideoMode(w,h,videoInfo->vfmt->BitsPerPixel,videoFlags);

  if(!ms)
    {
      os<<"Initing video mode failed!"<<std::endl;
      exit(1);
    }


  os<<"Trying to load a surface from file... (data/blue_cursor.png)"<<std::endl;
  SDL_Surface *s=IMG_Load("data/blue_cursor.png");

  os<<"loading from file: ok."<<std::endl;


  os<<"try loading indirectly..."<<std::endl;
  {
  FILE *f=fopen("data/blue_cursor.png","rb");
  fseek(f,0,SEEK_END);
  long len;
  len=ftell(f);
  fseek(f,0,SEEK_SET);

  char *s=new char[len+10];
  fread(s,1,len,f);
  fclose(f);




  SDL_RWops* rw=SDL_RWFromMem(s,len);//const_cast<char*>(file.c_str()),file.length());

  
  SDL_Surface *ms=IMG_Load_RW(rw,0);

  
  SDL_FreeRW(rw);;

  os<<"everything's alright"<<std::endl;
  }
  


  return 0;
}
/*
extern "C"
{
	int main()
	{
		return test();
	}
}*/