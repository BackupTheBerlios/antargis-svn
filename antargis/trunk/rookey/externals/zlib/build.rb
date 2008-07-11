require 'ftools'
require File.join(File.split(__FILE__)[0],"..","ext_config.rb")
require File.join(File.split(__FILE__)[0],"..","tools.rb")

def buildZ(dir,swigTarget)
  curDir=Dir.pwd
  Dir.chdir(dir)
  setEnv
  system "./configure --prefix=#{target}"
  system "make"
  system "make install"
end


download($zlibUrl)
unzipTar($zlibTGZ)
buildZ($zlibDir,getBuildDir("zlib"))