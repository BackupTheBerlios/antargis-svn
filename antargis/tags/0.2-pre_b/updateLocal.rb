#!/usr/bin/env ruby

if ARGV.length!=1
  puts "usage:"
  puts "./updateLocal.rb <lang>"
  puts "where lang is de or es"
  puts
  puts "example:"
  puts "./updateLocal.rb es"
  exit
end

ilines=File.open("local_new.txt").read.split("\n")
llines=[]
begin
  llines=File.open("data/local/local_#{ARGV[0]}.txt").read.split("\n")
rescue
end

inter=ilines.collect{|x|x.split(";;")[0]}
local={}
inter.each{|i|
	if (not i.nil?)
		local[i]=""
	end
}
llines.each{|l|
	k,v=l.split(";;")
	v||=""
	if (not k.nil?)
		local[k]=v
	end
}

name="local_#{ARGV[0]}_new.txt"
f=File.open(name,"w")

local.keys.sort.each{|k|
	f.puts "#{k};;#{local[k]}"
}
f.close
puts "Written to '#{name}'"