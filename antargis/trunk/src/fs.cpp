#include <string>
#include <physfs.h>
#include <assert.h>
#include <fs.h>
#include "ag_debug.h"

bool FSinited=false;

void initFS(const char *argv0)
{
  PHYSFS_init(argv0);
  PHYSFS_setSaneConfig("Antargis","Antargis","ZIP",false,false);
  FSinited=true;
  //  cdebug("writedir:"<<PHYSFS_getWriteDir());
}

std::string loadFile(const std::string &pName)
{
  assert(FSinited);

  if(!fileExists(pName))
    {
      std::cerr<<"File not "<<pName<<" does not exist!"<<std::endl;
      return "";
    }

  PHYSFS_file *f=PHYSFS_openRead(pName.c_str());
  std::string o;

  char buf[1001];
  PHYSFS_uint32 c=0;

  do
    {
      c=PHYSFS_read(f,buf,1,1000);
      o+=std::string(buf,c);
    }
  while(c);

  PHYSFS_close(f);
  return o;
}
void saveFile(const std::string &pName,const std::string &pContent)
{
  assert(FSinited);

  PHYSFS_file *f=PHYSFS_openWrite(pName.c_str());

  assert(f);

  PHYSFS_write(f,pContent.c_str(),1,pContent.length());

  PHYSFS_close(f);
}

bool fileExists(const std::string &pName)
{
  return PHYSFS_exists(pName.c_str());
}
