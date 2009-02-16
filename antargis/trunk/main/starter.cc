#include <SDL.h>

#include <ruby.h>
#ifdef WIN32
#undef write
#undef read
#endif
#include <iostream>

#include <rk_tools.h>
#include <rk_string.h>




void plainLog(const std::string &p)
{
  static FILE *f=0;
  if(f==0)
    f=fopen("starter.log","w");

  fwrite(p.c_str(),sizeof(char),p.length(),f);
  fwrite("\n",sizeof(char),1,f);

  fflush(f);
}

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
    plainLog("startup");

    ruby_init();

    plainLog("ruby inited");

    int argc=_argc;
    char**argv=_argv;

    if(argc==1)
      {
        plainLog("no args given");
        argv=new char*[2];
        argv[0]=new char[strlen(_argv[0])+10];
        strcpy(argv[0],_argv[0]);
        //argv[0]=_argv[0];

        std::string s=argv[0],sep;

        plainLog("my name:"+s);

        if(isWindows(s))
          sep="\\";
        else
          sep="/";

        plainLog("dir sep:"+sep);


        s=truncReverseUntil(s,sep);
        plainLog("s:"+s);

        if(s.length()>0)
          s+=sep;
        plainLog("s:"+s);


        argv[1]=new char[s.length()+100];
        strcpy(argv[1],(s+"antargisStarter.rb").c_str());
        plainLog(std::string("argv[1]:")+argv[1]);

        argc=2;
      }

    plainLog("setting ruby ops...");

    ruby_options(argc,argv);
    plainLog("rups ops set");
    rb_gv_set("antargisStarterLoaded",Qtrue);
    plainLog("global set");
    plainLog("running ruby...");

    ruby_run();
    plainLog("ended");

    std::cout<<"CCC"<<std::endl;
    return 0;
  }
