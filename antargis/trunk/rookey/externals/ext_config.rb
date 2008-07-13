def getInstallTarget(name)
  File.expand_path(File.join(File.split(__FILE__)[0],"built",name))
end

module PackageBase
  def urlToFile(url)
    url.gsub(/.*\//,"")
  end
  def fileToDir(file)
    f=file
    case f
      when /zip$/i, /\.tgz$/i 
        f[0..-5]
      when /\.tar.gz$/i
        f[0..-8]
      when /\.tar.bz2$/i
        f[0..-9]
    end
  end
end

$zliburl="http://somezliburl"
class Package
  attr_reader :url
  attr_accessor :configureOptions, :local
  def initialize(url)
    @url=url
    @local=false
  end
  def file
    @url.gsub(/.*\//,"")
  end
  def dir
    f=file
    case f
	    when /zip$/i, /\.tgz$/i 
	      f[0..-5]
      when /\.tar.gz$/i
        f[0..-8]
      when /\.tar.bz2$/i
        f[0..-9]
    end
  end
  def type
    case file
      when /zip$/i 
        :zip
      when /\.tar.gz$/i, /\.tgz$/i
        :tarGZ
      when /\.tar.bz2$/i
        :tarBZ2
    end
  end
  def install
    download(url)
    unzipTar(file)
    build(dir,configureOptions)
  end    
end

class BinPackage
  include PackageBase
  def initialize(binurl,devurl)
    @binurl=binurl
    @devurl=devurl
  end
  def install
    odir=Dir.pwd
    begin
      Dir.mkdir("build")
    rescue
    end
    Dir.chdir("build")
    [@binurl,@devurl].each{|url|
      if url
	      download(url)
	      file=urlToFile(url)
	      unzipTar(file)
	      dir=fileToDir(file)
	      puts dir
        
        strangers=Dir["*"].select{|f|f=~/^[A-Z].*/}.select{|f|File.directory?(f)}
        strangers.each{|d|
          ["include","lib","bin"].each{|md|
            Dir[File.join(d,md,"*")].each{|f|File.mv(f,File.join(md,File.split(f)[1]))}
          }
          #Dir.delete!(d)
        }
        
        
      end
    }
    Dir["*.dll"].each{|f|File.move(f,File.join("lib",f))}
    Dir.chdir(odir)
  end
end

def externalPackages
	pkgs={
	  "swig"=>Package.new("http://downloads.sourceforge.net/swig/swig-1.3.36.tar.gz"),
    "zlib"=>BinPackage.new("http://prdownloads.sourceforge.net/gnuwin32/zlib-1.2.3-bin.zip","http://prdownloads.sourceforge.net/gnuwin32/zlib-1.2.3-lib.zip"),
    "sdl"=>BinPackage.new("http://libsdl.org/release/SDL-1.2.13-win32.zip","http://libsdl.org/release/SDL-devel-1.2.13-mingw32.tar.gz"),
    "sdl_image"=>BinPackage.new("http://www.libsdl.org/projects/SDL_image/release/SDL_image-1.2.6-win32.zip","http://www.libsdl.org/projects/SDL_image/release/SDL_image-devel-1.2.6-VC8.zip"),
    "sdl_ttf"=>BinPackage.new("http://www.libsdl.org/projects/SDL_ttf/release/SDL_ttf-2.0.9-win32.zip","http://www.libsdl.org/projects/SDL_ttf/release/SDL_ttf-devel-2.0.9-VC8.zip"),
    "sdl_mixer"=>BinPackage.new("http://www.libsdl.org/projects/SDL_mixer/release/SDL_mixer-1.2.8-win32.zip","http://www.libsdl.org/projects/SDL_mixer/release/SDL_mixer-devel-1.2.8-VC8.zip")
	}
  # exclude other scripting langs for swig
  pkgs["swig"].configureOptions=["python","ocaml","php4","chicken","csharp","csharp",
    "guilescm","java","mzscheme","perl5","pike","tcl","lua","allegrocl","clisp","r"].map{|f|"--without-#{f}"}.join(" ")
  pkgs["swig"].local=true
  pkgs
end


