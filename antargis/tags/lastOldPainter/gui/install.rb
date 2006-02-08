#!/usr/bin/env ruby

def execute(str)
	IO.popen(str,"r").each_line{|l|puts l}
end

# install library
system("cd src && ruby extconf.rb && make && make install")

# install data
system("cd data && ruby extconf.rb && make && make install")