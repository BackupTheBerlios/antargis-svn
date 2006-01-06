#!/usr/bin/env ruby

def execute(str)
	IO.popen(str,"r").each_line{|l|puts l}
end

# install library
system("cd src && ./extconf.rb && make && make install")

# install data
system("cd data && ./extconf.rb && make && make install")