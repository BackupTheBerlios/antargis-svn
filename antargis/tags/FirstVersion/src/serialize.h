#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <SDL.h>
#include <vector>
#include <zlib.h>
#include <assert.h>
#include <ag_debug.h>

class Serial
{
  std::string s;
 protected:
  size_t p;
 public:
  Serial()
    {
      p=0;
    }
  Serial(const std::string &ps):s(ps)
    {
      p=0;
    }
    virtual ~Serial()
      {
      }
    virtual void put(Uint8 c)
  {
    s+=c;
  }
    virtual Uint8 get()
  {
    assert(p<s.length());
    return s[p++];
  }
  std::string str() const
    {
      return s;
    }
};

class ConstSerial:public Serial
{
  const std::string &ms;
 public:
  ConstSerial(const std::string &ps):Serial(),ms(ps)
    {
    }
    virtual Uint8 get()
    {
      return ms[p++];
    }
    
};



inline Serial &operator<<(Serial&s,const Uint8 &c)
{
  s.put(c);
  return s;
}

inline Serial &operator>>(Serial&s,Uint8 &c)
{
  c=s.get();
  return s;
}

// FIXME: what a waste!
inline Serial &operator<<(Serial&s,const bool &c)
{
  s.put(c);
  return s;
}

inline Serial &operator>>(Serial&s,bool &c)
{
  c=s.get();
  return s;
}

inline Serial &operator<<(Serial&s,const Sint16 &i)
{
  s<<Uint8((i>>8)&0xFF)<<Uint8(i&0xFF);
  return s;
}

inline Serial &operator<<(Serial&s,const Sint32 &i)
{
  s.put((i>>24)&0xFF);
  s.put((i>>16)&0xFF);
  s.put((i>>8)&0xFF);
  s.put(i&0xFF);
  return s;
}

inline Serial &operator<<(Serial&s,const Uint32 &i)
{
  s.put((i>>24)&0xFF);
  s.put((i>>16)&0xFF);
  s.put((i>>8)&0xFF);
  s.put(i&0xFF);
  return s;
}


inline Serial &operator>>(Serial&s,Sint16 &i)
{
  i|=((Uint16)s.get())<<8;
  i|=((Uint16)s.get());
  return s;
}

inline Serial &operator>>(Serial&s,Sint32 &i)
{
  i=((Uint32)s.get())<<24;
  i|=((Uint32)s.get())<<16;
  i|=((Uint32)s.get())<<8;
  i|=((Uint32)s.get());
  return s;
}
inline Serial &operator>>(Serial&s,Uint32 &i)
{
  i=((Uint32)s.get())<<24;
  i|=((Uint32)s.get())<<16;
  i|=((Uint32)s.get())<<8;
  i|=((Uint32)s.get());
  return s;
}

template<class T>
Serial &operator<<(Serial &s,const std::vector<T> &v)
{
  s<<Uint32(v.size());

  cdebug("saved size:"<<v.size());
  typename std::vector<T>::const_iterator i=v.begin();

  for(;i!=v.end();i++)
    {
      s<<*i;
    }
  return s;
}

template<class T>
Serial &operator>>(Serial &s,std::vector<T> &v)
{
  size_t ms;
  s>>ms;
  T c;

  cdebug("loaded size:"<<ms);
  for(size_t i=0;i<ms;i++)
    {
      s>>c;
      v.push_back(c);
    }
  return s;
}

inline Serial &operator<<(Serial&s,float f)
{
  // do not convert
  Uint8 *c=(Uint8*)(&f);
  for(size_t i=0;i<sizeof(f);i++,c++)
    s<<(*c);
  return s;
}

inline Serial &operator>>(Serial&s,float &f)
{
  // do not convert
  Uint8 *c=(Uint8*)(&f);
  for(size_t i=0;i<sizeof(f);i++,c++)
    s>>(*c);
  return s;
}


inline std::string zip(const std::string &s)
{
  Serial serial;
  serial<<s.length();


  uLongf len=uLongf(s.length()*1.2+12);

  unsigned char *dest=new unsigned char[len];

  int ret=compress2(dest,&len,(const unsigned char*)s.c_str(),s.length(),9);

  if(ret==Z_MEM_ERROR)
    {
      delete [] dest;
      return "";
    }
  else if(ret==Z_BUF_ERROR)
    {
      delete [] dest;
      return "";
    }

  std::string n=serial.str()+std::string((char*)dest,size_t(len));
  delete [] dest;
  return n;

}

inline std::string unzip(const std::string &is)
{
  uLongf len;
  ConstSerial cs(is);
  size_t l;
  cs>>l;
  len=l;
  const char *ptr=is.c_str()+sizeof(size_t);

  unsigned char *dest=new unsigned char[len+10];

  int ret=uncompress (dest,&len,(const unsigned char*)ptr,is.length()-sizeof(size_t));

  if(ret==Z_MEM_ERROR)
    {
      std::cerr<<"Memory error during decompress"<<std::endl;
      delete [] dest;
      return "";
      //      throw SHexception("Memory error during decompress");
    }
  else if(ret==Z_BUF_ERROR)
    {
      std::cerr<<"Buffer error during decompress"<<std::endl;
      delete [] dest;
      return "";
      //      throw SHexception("Buffer error during decompress");
    }
  else if(ret==Z_DATA_ERROR)
    {
      std::cerr<<"data error dring decompress"<<std::endl;
      delete [] dest;
      return "";
      //      throw SHexception("data error dring decompress");
    }

  std::string n((char*)dest,size_t(len));
  delete [] dest;

  return n;
}




#endif
