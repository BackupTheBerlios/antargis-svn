#include <ag_rtools.h>

#include <ruby.h>

#include <iostream>
#include <sstream>

std::string rubyHash(const std::string &p)
{
  // FIXME: TRY USING ruby's Digest::MD5::digest("xy")
  rb_eval_string("require 'digest/md5'");

  //  VALUE l=rb_gv_get("Digest::MD5");
  //  VALUE r=rb_funcall(l,rb_intern("digest"),1,rb_str_new2(p.c_str()));

  std::ostringstream os;
  os<<"s=''; Digest::MD5::digest('"<<p<<"').each_byte{|b|s+=sprintf('%X',b)};s";

  VALUE r=rb_eval_string(os.str().c_str());

  std::string s;

  s=STR2CSTR(r);

  std::cout<<"p:"<<p<<std::endl;
  std::cout<<"s:"<<s<<std::endl;

  return s;
}
