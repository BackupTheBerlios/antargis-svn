#ifndef ANT_SERIAL_H
#define ANT_SERIAL_H

// INCLUDE_SWIG - used to filter, which files are included in swig-interfacing

#include <ag_fs.h>
#include <SDL.h>
#include <sstream>

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
  BinaryIn &operator>>(Uint16 &i);
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

  size_t pos() const;
 private:
  FILE *f;
  std::istringstream is;
  bool mEof;
  size_t p;
  size_t size;
};

class BinaryFileOut:public BinaryOut
{
 public:
  BinaryFileOut(const std::string &pName);
  virtual ~BinaryFileOut();

  virtual void write(char c);
  void flush();

  size_t pos() const;
 private:
  FILE *f;
  std::ostringstream buffer;
  size_t size;
  size_t p;
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

std::string binaryToHex(const std::string &s);
std::string hexToBinary(const std::string &s);

#endif