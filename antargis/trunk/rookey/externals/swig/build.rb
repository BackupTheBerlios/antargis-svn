require 'ftools'
require File.join(File.split(__FILE__)[0],"..","ext_config.rb")

alias :oldSystem :system 
def system(x)
  puts x
  oldSystem(x)
end
  

def getFilenameFromUrl(url)
  url.gsub(/.*\//,"")
end

def download(url)
  filename=getFilenameFromUrl(url)
  if File.exists?(filename)
    puts "File #{filename} already exists - so not downloading!"
    return
  end
  puts "Downloading from #{url}..."
  call="wget -O '#{filename}.tmp' '#{url}'"
  system call
  File.move(filename+".tmp",filename)
  return
end

def unzipTar(file)
  puts "Unzipping #{file}"
  if file=~/tar.gz$/
    system("tar xvfz #{file} >/dev/null")
  else
    system("unzip #{file}")
  end
  puts "Ready."
end

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
buildSwig($swigDir,$swigTarget)
