#!/usr/bin/env ruby

require 'open-uri'
require 'pp'
require 'ftools.rb'

load 'antconfig.rb'

def downloadTo(url,filename)
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

def unzip(file,dir)
	olddir=Dir.pwd
	Dir.mkdir(dir) unless File.exists?(dir)
	Dir.chdir(dir)
	puts "Unzipping #{file} at #{dir}"
	system("tar xvfz #{file} >/dev/null")
	puts "Ready."
	Dir.chdir(olddir)
end

mydir=File.split(File.expand_path(__FILE__))[0]
basedir=File.split(mydir)[0]
Dir.chdir(basedir)

libdir="build/win32/libs"

sdlLibZip="ag_sdl_libs.tar.gz"
rubyZip="ag_ruby_bin.tar.gz"

downloadTo("http://svn.berlios.de/wsvn/antargis/contrib/binary/ag_sdl_libs.tar.gz?op=file&rev=0&sc=0",File.join(libdir,sdlLibZip))
downloadTo("http://svn.berlios.de/wsvn/antargis/contrib/binary/ag_ruby_libs.tar.gz?op=file&rev=0&sc=0",File.join(libdir,rubyZip))
#downloadTo("ftp://ftp.ruby-lang.org/pub/ruby/binaries/mswin32/ruby-1.8.6-i386-mswin32.zip",File.join(libdir,rubyZip))

unzip(sdlLibZip,libdir)
unzip(rubyZip,libdir)

File.cp("build/configs/cross.rb","antconfig.rb")

puts "building extension..."
system("rake package")

puts "getting allinoneruby.exe..."

downloadTo("http://svn.berlios.de/wsvn/antargis/contrib/binary/allinoneruby.exe?op=file&rev=0&sc=0","allinoneruby.exe")

destdir="pkg/antargis-win32-"+$CONFIG['version']
system("cp allinoneruby.exe "+destdir)
system("cp build/win32/libs/usr/bin/*.dll "+destdir)

puts "strip antargis.so"

system("i586-mingw32msvc-strip #{destdir}/ext/antargis.so")

puts "getting position of Innosetup..."

path=`locate ISCC.exe`.chomp
path.gsub!(" ","\\ ")
pp path

call=path+" antargis.iss"
system(call)