#!/usr/bin/env ruby

if not $antargislibinited

	# try suspending arts
	if File.exists?("/usr/bin/artsshell")
		File.popen("/usr/bin/artsshell suspend 2>&1").close
	end
	
	$programDir=Dir.pwd+"/ruby"
	# add programdir to path
	$:.push($programDir)
	
	if File.exists?("libantargis.so")
		require 'libantargis'
	elsif File.exists?("../src/.libs/libantargis.so")
		require '../src/.libs/libantargis'
	else
		require 'libantargis'
	end
	include Libantargis
	
	require 'gui/data/ag_tools.rb'
	require 'ant_tools'
	require 'ant_debug.rb'
	
	$antargislibinited=true

	$main=AGMain.new(1024,768,32,false,true)
	$main.setCaption("Antargis")
	$main.setIcon("data/gui/editor/tower1.png")

	# check options
	ARGV.each{|arg|
		if arg=~/--.*/ or arg=~/-.*/
			a=arg[1..1000]
			if arg[1..1]=="-"
				a=arg[2..1000]
			end
			case a
				when "no-sound"
					getConfig.set("soundEnabled","false")
				when "sound"
					getConfig.set("soundEnabled","true")
				when "help","h"
					STDERR.puts "Possible options:
--help     show this help message

--sound    enables sound for now and future uses
--no-sound disables sound for now and future uses"
					exit
			end
		end
	}


end
