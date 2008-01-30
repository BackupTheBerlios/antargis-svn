#include <SDL.h>

#include <ruby.h>

int main(int argc,char*argv[])
{
	ruby_init();
	ruby_options(argc,argv);
	ruby_script("sdl_starter");
	//rb_load_file("antargis");
	ruby_run();
	return 0;
}