#ifndef __ANTARGISGUI_EXCEPTION_H__
#define __ANTARGISGUI_EXCEPTION_H__


class AGException:public std::exception
{
 public:
  AGException(const std::string &e):s(e)
    {
    }

    virtual ~AGException() throw ()
      {
      }

    const char*what() const throw()
      {
	return s.c_str();
      }

 private:
  std::string s;
};

#endif
