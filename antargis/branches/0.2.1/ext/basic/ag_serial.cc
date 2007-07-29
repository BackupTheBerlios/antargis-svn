#include "ag_serial.h"
#include <iostream>
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

  std::string n=findFile(pName);

  f=fopen(n.c_str(),"rb");
  p=0;
  is.unsetf(std::ios_base::skipws);
  size=0;
}

BinaryFileIn::~BinaryFileIn()
{
  fclose(f);
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
      size_t c=0;
      c=fread(buf,1,1000,f);
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

void checkParentDirs(const std::string &s); // imported from ag_fs.cc

/////////////////////////////////////////////////////////////
// BinaryFileOut
/////////////////////////////////////////////////////////////
BinaryFileOut::BinaryFileOut(const std::string &pName)
{
  std::string n=checkFileName(getWriteDir()+"/"+pName);
  checkParentDirs(n);

  f=fopen(n.c_str(),"wb");
  //  f=fopen(PHYSFS_openWrite(pName.c_str());
  assert(f);
  size=0;
  p=0;
}
BinaryFileOut::~BinaryFileOut()
{
  flush();
  fclose(f);
}

size_t BinaryFileOut::pos() const
{
  return p;
}

void BinaryFileOut::flush()
{
  std::string s=buffer.str();
  //  PHYSFS_write(f,s.c_str(),1,s.length());
  fwrite(s.c_str(),1,s.length(),f);
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

BinaryStringIn::~BinaryStringIn()
{
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


unsigned char fromHexBS(unsigned char c)
{
  if(c>='a' && c<='f')
    return c-'a'+0xA;
  if(c>='A' && c<='F')
    return c-'A'+0xA;

  if(c>='0' && c<='9')
    return c-'0';
  throw std::runtime_error("wrong input in fromHex()");
}

void toHexBS(char c,std::ostream &os)
{
  unsigned char c0=(c&0xF);
  unsigned char c1=((c>>4)&0xF);

  char a,b;
  //  std::ostringstream os;
  if(c0<10)
    a=(c0+'0');
  else
    a=((c0-10)+'A');

  if(c1<10)
    b=(c1+'0');
  else
    b=((c1-10)+'A');
    
  //  cdebug("a:"<<(int)a);
  //  cdebug("b:"<<(int)b);
  os<<b<<a;

  //  return os.str();
}


std::string binaryToHex(const std::string &s)
{
  std::ostringstream os;
  
  std::cout<<s.length()<<std::endl;
  for(size_t i=0;i<s.length();i++)
    {
      //      cdebug(i<<":"<<(int)s[i]);
      toHexBS(s[i],os);
      if(i>0)
	{
	  if((i%32)==0)
	    os<<std::endl;
	  else if((i%4)==0)
	    os<<" ";
	}
    }
  //  cdebug(os.getString());

  return os.str();//getString();
}


std::string hexToBinary(const std::string &s)
{
  std::ostringstream os;
  bool complete=false;
  char current=0;

  for(size_t i=0;i<s.length();i++)
    {
      char c=s[i];
      if((c>='a' && c<='f') || (c>='A' && c<='F') || (c>='0' && c<='9'))
	{
	  //	  cdebug("scan:"<<(int)fromHexBS(c));
	  if(complete)
	    {
	      //	      cdebug("current (2):"<<(int)current);
	      current<<=4;
	      //	      cdebug("current (2):"<<(int)current);
	      current|=fromHexBS(c);
	      //	      cdebug("current (2):"<<(int)current);
	      os<<current;
	      complete=false;
	    }
	  else
	    {
	      current=fromHexBS(c);
	      complete=true;
	      //	      cdebug("current (1st):"<<(int)current);
	    }
	}

    }


  return os.str();
}
