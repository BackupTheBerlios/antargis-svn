#include "ant_serial.h"
#include <iostream>
#include <physfs.h>

// INTEL is little endian

int mLittleEndian=-1;

bool littleEndian()
{
  if(mLittleEndian<0)
    {
      Uint16 i=0x100;
      Uint8 *p=(Uint8*)&i;
      mLittleEndian=(p[0]!=0);
    }
  return mLittleEndian;
}


/////////////////////////////////////////////////////////////
// BinaryIn
/////////////////////////////////////////////////////////////

BinaryIn::BinaryIn()
{
}
BinaryIn::~BinaryIn()
{
}
  
bool BinaryIn::eof()
{
  return true;
}
char BinaryIn::read()
{
  return 0;
}

BinaryIn &BinaryIn::operator>>(Sint32 &i)
{
  return operator>>((Uint32&)i);
}
BinaryIn &BinaryIn::operator>>(Uint32 &u)
{
  if(eof())
    throw SerialException();
  u=((Uint32(read())&0xFF)<<24)|((Uint32(read()&0xFF))<<16)|((Uint32(read())&0xFF)<<8)|(Uint32(read())&0xFF);

  return *this;
}

BinaryIn &BinaryIn::operator>>(float &f)
{
  if(eof())
    throw SerialException();

  if(sizeof(float)!=4)
    {
      std::cerr<<"sizeof(float) should be 4 :-|"<<std::endl;
      throw SerialException();
    }
  if(littleEndian())
    {
      // ok, load directly
      char *s=(char*)&f;
      s[0]=read();
      s[1]=read();
      s[2]=read();
      s[3]=read();
    }
  else
    {
      // ok, load swapped
      char *s=(char*)&f;
      s[3]=read();
      s[2]=read();
      s[1]=read();
      s[0]=read();
    }
  

  return *this;
}



/////////////////////////////////////////////////////////////
// BinaryOut
/////////////////////////////////////////////////////////////

BinaryOut::BinaryOut()
{
}
BinaryOut::~BinaryOut()
{
}
void BinaryOut::write(char c)
{
}

BinaryOut &BinaryOut::operator<<(const Sint32 &i)
{
  return operator<<((const Uint32&)i);
}
BinaryOut &BinaryOut::operator<<(const Uint32 &u)
{
  write((u>>24)&0xFF);
  write((u>>16)&0xFF);
  write((u>>8 )&0xFF);
  write((u>>0 )&0xFF);
  return *this;
}
BinaryOut &BinaryOut::operator<<(const float &f)
{
  if(sizeof(float)!=4)
    {
      std::cerr<<"sizeof(float) should be 4 :-|"<<std::endl;
      throw SerialException();
    }
  if(littleEndian())
    {
      const char *s=(const char*)&f;
      write(s[0]);
      write(s[1]);
      write(s[2]);
      write(s[3]);
    }
  else
    {
      const char *s=(const char*)&f;
      write(s[3]);
      write(s[2]);
      write(s[1]);
      write(s[0]);
    }

  return *this;
}

/////////////////////////////////////////////////////////////
// BinaryFileIn
/////////////////////////////////////////////////////////////

BinaryFileIn::BinaryFileIn(const std::string &pName)
{
  mEof=false;
  f=PHYSFS_openRead(pName.c_str());
}

BinaryFileIn::~BinaryFileIn()
{
  PHYSFS_close(f);
}


bool BinaryFileIn::eof()
{
  return mEof && is.eof();
}

char BinaryFileIn::read()
{
  if(is.str().length()==0)
    {
      if(mEof)
	return 0;
      char buf[1001];
      PHYSFS_uint32 c=0;
      c=PHYSFS_read(f,buf,1,1000);
      is.str(std::string(buf,c));
      if(c==0)
	mEof=true;
    }
  char c;
  is>>c;
  return c;
}

/////////////////////////////////////////////////////////////
// BinaryFileOut
/////////////////////////////////////////////////////////////
BinaryFileOut::BinaryFileOut(const std::string &pName)
{
  f=PHYSFS_openWrite(pName.c_str());
  size=0;
}
BinaryFileOut::~BinaryFileOut()
{
  flush();
  PHYSFS_close(f);
}

void BinaryFileOut::flush()
{
  std::string s=buffer.str();
  PHYSFS_write(f,s.c_str(),1,s.length());
  buffer.str("");
  size=0;
}

void BinaryFileOut::write(char c)
{
  // non buffered for a start
  buffer<<c;
  size++;
  if(size>1024)
    flush();
}

/////////////////////////////////////////////////////////////
// BinaryStringIn
/////////////////////////////////////////////////////////////
BinaryStringIn::BinaryStringIn(const std::string &pText)
{
  is.str(pText);
}

bool BinaryStringIn::eof()
{
  return is.eof();
}
char BinaryStringIn::read()
{
  char c;
  is>>c;
  return c;
}


/////////////////////////////////////////////////////////////
// BinaryStringOut
/////////////////////////////////////////////////////////////
BinaryStringOut::BinaryStringOut()
{
}

BinaryStringOut::~BinaryStringOut()
{
}

void BinaryStringOut::write(char c)
{
  os<<c;
}


std::string BinaryStringOut::getString() const
{
  return os.str();
}
