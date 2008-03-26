#include <SDL.h>

#include <ruby.h>
#include <iostream>

//extern "C" void Init_antargis();

int main(int argc,char*argv[])
  {
    ruby_init();
    //rb_load_file("antargis");
    std::cout<<"Init_antargis out of starter..."<<std::endl;
    //Init_antargis();
    std::cout<<"Init_antargis out of starter-ready"<<std::endl;
    std::cout<<"Init_antargis out of starter-ready"<<std::endl;
    ruby_options(argc,argv);
    std::cout<<"AAA"<<std::endl;
    ruby_script("sdl_starter");
    std::cout<<"BBB"<<std::endl;
    ruby_run();
    std::cout<<"CCC"<<std::endl;
    return 0;
  }
