require 'ftools'
require File.join(File.split(__FILE__)[0],"..","ext_config.rb")
require File.join(File.split(__FILE__)[0],"..","tools.rb")


def buildSwig(dir,swigTarget)
  curDir=Dir.pwd
  Dir.chdir(dir)
  a=["python","ocaml","php4","chicken","csharp","csharp","guilescm","java","mzscheme","perl5","pike","tcl","lua","allegrocl","clisp","r"]
  system "./configure "+a.map{|f|"--without-#{f}"}.join(" ")+" --prefix=#{swigTarget}"
  system "make"
  system "make install"
end


download($swigUrl)
unzipTar($swigTarGz)
buildSwig($swigDir,getBuildTarget("swig"))
