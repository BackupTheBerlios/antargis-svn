#ifndef __AG_MAIN
#define __AG_MAIN

class AGMain
{
 public:
  AGMain();
  ~AGMain();

  void flip();
  void changeRes(int w,int h,int d,bool fs,bool gl);
  void toggleFull();
};

AGMain *getMain();

#endif
