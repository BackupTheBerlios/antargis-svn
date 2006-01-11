#include "ant_serial.h"
#include <iostream>
#include <physfs.h>
#include <ag_debug.h>

// INTEL is little endian

int mLittleEndian=-1;

bool littleEndian()
{
  //return true;
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
  throw SerialException();
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
  u=((Uint32(read())&0xFF))|((Uint32(read()&0xFF))<<8)|((Uint32(read())&0xFF)<<16)|((Uint32(read())&0xFF)<<24);

  return *this;
}

BinaryIn &BinaryIn::operator>>(Uint16 &i)
{
  if(eof())
    throw SerialException();
  i=((Uint16(read())&0xFF))|((Uint16(read())&0xFF)<<8);
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
  if(!littleEndian())
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

BinaryIn &BinaryIn::operator>>(AGVector2 &v)
{
  (*this)>>v[0]>>v[1];
  return *this;
}
BinaryIn &BinaryIn::operator>>(AGVector3 &v)
{
  (*this)>>v[0]>>v[1]>>v[2];
  return *this;
}
BinaryIn &BinaryIn::operator>>(AGVector4 &v)
{
  (*this)>>v[0]>>v[1]>>v[2]>>v[3];
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
  write((u>>0 )&0xFF);
  write((u>>8 )&0xFF);
  write((u>>16)&0xFF);
  write((u>>24)&0xFF);
  return *this;
}
BinaryOut &BinaryOut::operator<<(const float &f)
{
  if(sizeof(float)!=4)
    {
      std::cerr<<"sizeof(float) should be 4 :-|"<<std::endl;
      throw SerialException();
    }
  if(!littleEndian())
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
  p=0;
  is.unsetf(std::ios_base::skipws);
  size=0;
}

BinaryFileIn::~BinaryFileIn()
{
  PHYSFS_close(f);
}

size_t BinaryFileIn::pos() const
{
  return p;
}


bool BinaryFileIn::eof()
{
  return mEof && size==0;
}

char BinaryFileIn::read()
{
  if(size==0)
    {
      if(mEof)
	{
	  throw SerialException();
	  return 0;
	}
      char buf[1001];
      PHYSFS_uint32 c=0;
      c=PHYSFS_read(f,buf,1,1000);
      is.str(std::string(buf,c));
      size+=c;
      if(c==0)
	mEof=true;
    }
  unsigned char c;
  is>>c;
  p++;
  size--;
  return c;
}

/////////////////////////////////////////////////////////////
// BinaryFileOut
/////////////////////////////////////////////////////////////
BinaryFileOut::BinaryFileOut(const std::string &pName)
{
  f=PHYSFS_openWrite(pName.c_str());
  size=0;
  p=0;
}
BinaryFileOut::~BinaryFileOut()
{
  flush();
  PHYSFS_close(f);
}

size_t BinaryFileOut::pos() const
{
  return p;
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
  p++;
}

/////////////////////////////////////////////////////////////
// BinaryStringIn
/////////////////////////////////////////////////////////////
BinaryStringIn::BinaryStringIn(const std::string &pText)
{
  is.unsetf(std::ios_base::skipws);
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
