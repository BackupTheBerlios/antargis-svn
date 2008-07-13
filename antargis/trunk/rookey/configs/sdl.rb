module Rookey
  class SDLConfig<Configurator
    provides :sdl
    needs :compiler
    
    def run(config)
      # FIXME
      sdlconfig="sdl-config"
      
      pp config
      if config["host_os"]!="win32"
	      cflags=`#{sdlconfig} --cflags`.chomp
	      config.add("CFLAGS",cflags.split(" ").select{|f|not f=~/^-I/}.join(" "))
	      config.add("INCLUDEDIRS",cflags.split(" ").select{|f|f=~/^-I/}.map{|f|f[2..-1]}.join(" "))
	      config.add("LDFLAGS",`sdl-config --libs`.chomp)
      else
        ["sdl","sdl_image","sdl_ttf","sdl_mixer"].each{|lib|install(lib)}
        
      end      
      
      checkLibrary(config,"SDL_image","IMG_Load")
      checkLibrary(config,"SDL_ttf","TTF_Quit")
      checkLibrary(config,"SDL_mixer","Mix_LoadMus")
    end
  end
end