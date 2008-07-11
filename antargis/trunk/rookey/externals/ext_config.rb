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
  attr_accessor :configureOptions
  def initialize(url)
    @url=url
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
      download(url)
      file=urlToFile(url)
      unzipTar(file)
      dir=fileToDir(file)
      puts dir
    }
    Dir.chdir(odir)
  end
end

def externalPackages
	pkgs={
	  "swig"=>Package.new("http://downloads.sourceforge.net/swig/swig-1.3.36.tar.gz"),
	  #"zlib"=>Package.new("http://downloads.sourceforge.net/swig/swig-1.3.34.tar.gz"),
    "zlib"=>BinPackage.new("http://prdownloads.sourceforge.net/gnuwin32/zlib-1.2.3-bin.zip","http://prdownloads.sourceforge.net/gnuwin32/zlib-1.2.3-lib.zip")
	}
  # exclude other scripting langs for swig
  pkgs["swig"].configureOptions=["python","ocaml","php4","chicken","csharp","csharp",
    "guilescm","java","mzscheme","perl5","pike","tcl","lua","allegrocl","clisp","r"].map{|f|"--without-#{f}"}.join(" ")
  pkgs
end


