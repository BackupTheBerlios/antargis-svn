#!/usr/bin/env ruby

require 'mkmf'

$prefix="/usr/local"


$bindir=$prefix+"/bin"
$datadir=$prefix+"/share"
$includedir=$prefix+"/include"


create_makefile("")
f=File.open("Makefile","a")
f.puts "install:"
Dir["*.xml"].each{|d|
	f.puts "	$(INSTALL_DATA) -D #{d} #{$datadir}/antargisgui/#{d}"
}
Dir["*.png"].each{|d|
	f.puts "	$(INSTALL_DATA) -D #{d} #{$datadir}/antargisgui/pics/#{d}"
}
["ag_tools.rb","antargisgui.rb"].each{|d|
	f.puts "	$(INSTALL_PROG) -D #{d} #{$rubylibdir}/#{d}"
}
f.close