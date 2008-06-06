#include <SDL.h>

#include <ruby.h>
#include <iostream>

#include <ag_rtools.h>
#include <ag_string.h>

std::string toLower(const std::string &s)
  {
    std::string a;
    for(size_t i=0;i<s.length();i++)
      {
        if(s[i]>='A' && s[i]<='Z')
          a+=s[i]-'A'+'a';
        else
          a+=s[i];
      }
    return a;
  }

bool isWindows(const std::string &str)
  {
    int s=std::max((int)str.length()-5,0);
    int l=std::max((int)str.length()-s,0);
    std::string e=str.substr(s,l);

    return(toLower(e)==".exe");
  }

std::string truncReverseUntil(const std::string &object,const std::string &search)
  {
    size_t r=object.rfind(search);
    if(r==object.npos)
      return "";

    return object.substr(0,r);
  }

int main(int _argc,char*_argv[])
  {
    ruby_init();


    int argc=_argc;
    char**argv=_argv;

    if(argc==1)
      {
        argv=new char*[2];
        argv[0]=_argv[0];

        std::string s=argv[0],sep;

        if(isWindows(s))
          sep="\\";
        else
          sep="/";
        s=truncReverseUntil(s,sep);

        argv[1]=new char[s.length()+100];
        strcpy(argv[1],(s+sep+"antargis").c_str());
        argc=2;
      }


    ruby_options(argc,argv);
    rb_gv_set("antargisStarterLoaded",Qtrue);

    ruby_run();
    std::cout<<"CCC"<<std::endl;
    return 0;
  }
