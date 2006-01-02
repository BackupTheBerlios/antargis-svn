#ifndef ANT_SERIAL_H
#define ANT_SERIAL_H

#include <ag_fs.h>
#include <SDL.h>
#include <sstream>
#include <physfs.h>

class SerialException
{
};

class BinaryIn
{
 public:
  BinaryIn();
  virtual ~BinaryIn();
  
  virtual bool eof();
  virtual char read();

  BinaryIn &operator>>(Sint32 &i);
  BinaryIn &operator>>(Uint32 &i);
  BinaryIn &operator>>(float &f);
};

class BinaryOut
{
 public:
  BinaryOut();
  virtual ~BinaryOut();

  virtual void write(char c);

  BinaryOut &operator<<(const Sint32 &i);
  BinaryOut &operator<<(const Uint32 &i);
  BinaryOut &operator<<(const float &f);
};

class BinaryFileIn:public BinaryIn
{
 public:
  BinaryFileIn(const std::string &pName);
  virtual ~BinaryFileIn();

  virtual bool eof();
  virtual char read();
 private:
  PHYSFS_file *f;
  std::istringstream is;
  bool mEof;
};

class BinaryFileOut:public BinaryOut
{
 public:
  BinaryFileOut(const std::string &pName);
  virtual ~BinaryFileOut();

  virtual void write(char c);
  void flush();
 private:
  PHYSFS_file *f;
  std::ostringstream buffer;
  size_t size;
};

class BinaryStringIn:public BinaryIn
{
 public:
  BinaryStringIn(const std::string &pText);

  virtual bool eof();
  virtual char read();
 private:
  std::istringstream is;
};

class BinaryStringOut:public BinaryOut
{
 public:
  BinaryStringOut();
  virtual ~BinaryStringOut();

  virtual void write(char c);

  std::string getString() const;
 private:
  std::ostringstream os;
};



#endif
