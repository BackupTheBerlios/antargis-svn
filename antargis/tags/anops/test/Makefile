sdl_image_test: sdl_image_test.cc
	g++ -c sdl_image_test.cc -g -Ic:/msys/local/include/SDL
	# -Dmain=SDL_main
	g++ -o sdl_image_test  "-Wl,-(" -Lc:/msys/local/lib -lmingw32 -lSDLmain -lSDL -lSDL_image -mwindows -g sdl_image_test.o