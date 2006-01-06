#!/usr/bin/env ruby

$swap=false

if File.exists?("antargislib.rb")
	require "antargislib.rb"
else
	require "ruby/antargislib.rb"
end

$vs=[]
$ts=[]
$ns=[]
$fs=[]

if ARGV.length<2
	puts "ARGS: ./obj_import.rb aaa.obj bbb.ant2"
	exit
end
infile=ARGV[0]
outfile=ARGV[1]

if File.exists?(outfile) and not (ARGV.length>2 and ARGV[2]=="-f")
	throw "File already exists"
end

File.open(infile).each_line{|line|
	#puts "line:"+line
	if line=~/#.*/ then
		# ignore - it's a comment
	elsif line=~/^v .*/ then
		x,y,z=line.gsub("v ","").split(" ").collect{|a|a.to_f}
		if $swap
			z,y=y,z
		end
		$vs.push([x,y,z])
	elsif line=~/^vt .*/ then
		x,y=line.gsub("vt ","").split(" ").collect{|a|a.to_f}
		$ts.push([x,y])
	elsif line=~/^vn .*/ then
		x,y,z=line.gsub("vt ","").split(" ").collect{|a|a.to_f}
		if $swap
			z,y=y,z
		end
		$ns.push([-x,y,-z])
	elsif line=~/^f .*/ then
		vs=line.gsub("f ","").split(" ")
		faces=[]
		vs.each{|v|
			v,t,n=v.split("/").collect{|a|a.to_i-1}
			faces.push([v,t,n])
		}
		$fs.push(faces)
	else	
		puts "ignored:"+line
	end
}

file=File.new(outfile,"wb")
file.print([1].pack("S"))
file.print([$fs.length].pack("S"))
$fs.each{|face|
	file.print([face.length].pack("S"))
	face.each{|mv|
		file.print($vs[mv[0]].pack("fff"))
		file.print($ns[mv[2]].pack("fff"))
		file.print([1,1,1].pack("fff"))
		if $ts[mv[1]]
			file.print($ts[mv[1]].pack("ff"))
		else
			file.print([0,0].pack("ff"))
		end
	}
}


file.close