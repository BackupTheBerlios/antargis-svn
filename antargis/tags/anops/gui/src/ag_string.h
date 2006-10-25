#ifndef AG_STRING
#define AG_STRING

#include <string>
#include <vector>
#include <SDL.h>

typedef std::string AGFilename;
typedef std::string AGData;

/** A string class that natively supports UTF-8 */
class AGString:public std::string
{
 public:
  static const size_t npos = static_cast<size_t>(-1);

  AGString();
  AGString(const std::string &p);
  AGString(const std::string &p,size_t many);
  AGString(const AGString &p);
  explicit AGString(int i);
  explicit AGString(size_t i);
  explicit AGString(long i);
  explicit AGString(float f);
  AGString(const char c,size_t many=1);
  AGString(const char*c);
  AGString(const char*c,size_t size);
  
  AGString &operator=(const AGString &s);

  AGString &operator+=(const AGString &p);
  // size in UTF-8 characters
  //  size_t length() const;

  // size in bytes
  size_t memory() const;
  
  AGString substr(size_t from,size_t len) const;
  /*
  char operator[](size_t i) const;
  char &operator[](size_t i);*/

  std::vector<AGString> split(const AGString &p) const;

  //  size_t find(const AGString &p,size_t pos=0) const;

  AGString replace(const AGString &what, const AGString &by) const;
  
  AGString operator+(const AGString &s) const;
  AGString operator+(const char *s) const;
  AGString operator+(const std::string &s) const;


  /*  bool operator==(const AGString &p) const;
  bool operator!=(const AGString &p) const;
  bool operator<(const AGString &p) const;*/
  /*
#ifdef SWIG
  %rename(to_s) toString() const;
#endif
  std::string toString() const;
  std::string ascii() const;
  */
  int toInt() const;
  long toLong() const;
  float toFloat() const;
  bool toBool() const;

  Uint8 toUint8() const;
  Sint16 toSint16() const;

  int parseHex() const;

  static AGString toHex(int i);

 private:

  //  std::string get(size_t pos) const;
  //  void set(size_t pos,const std::string &s);

  //  std::string s;
  //  mutable size_t mLength;

  //  friend class AGChar;

};

//std::ostream &operator<<(std::ostream &o,const AGString &s);


#endif
