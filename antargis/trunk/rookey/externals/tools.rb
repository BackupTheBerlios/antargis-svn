require File.join(File.split(__FILE__)[0],"ext_config.rb")
require 'ftools'

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
  elsif file=~/tar.bz2$/
    system("tar xvfj #{file} >/dev/null")
  else
    system("unzip -o #{file}")
  end
  puts "Ready."
end

def setEnv
  pp $tempConfig
end

def msh(a)
  puts a
  system a
end


def buildExternal(name)
  Dir.chdir(File.split(__FILE__)[0])
  puts name
  pkg=externalPackages[name]
  pkg.install
end

buildExternal(ARGV[0])