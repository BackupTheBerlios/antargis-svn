#!/usr/bin/env ruby

$antargislibinited||=false

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
		puts "Please run 'rake' before starting this program!"
		require 'libantargis'
	end
	include Libantargis
	
	require 'gui/data/ag_tools.rb'
	require 'ant_tools'
	require 'ant_debug.rb'
	
	$antargislibinited=true

	$fullscreen=false
	$antProfiling=false

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
				when "profile"
					require 'profile'
					$antProfiling=true
				when "gc-check"
					GC.stress=true
				when "intro"
					getConfig.set("intro","true")
				when "no-intro"
					getConfig.set("intro","false")
				when "fullscreen"
					$fullscreen=true
				when "window"
					$fullscreen=false
				when "help","h"
					STDERR.puts "Possible options:
--help         show this help message

--fullscreen   enable fullscreen
--window       start in windows mode

--profile      enables profiling output of BoA's ruby-part
--gc-check     enables debug mode for gc-check (GC.stress=true)

--intro        enables the intro
--no-intro     disables the intro

--sound        enables sound for now and future uses
--no-sound     disables sound for now and future uses"
					exit
			end
		end
	}

	$main=AGMain.new(1024,768,32,$fullscreen,true)
	$main.setCaption("Antargis")
	$main.setIcon("data/gui/editor/tower1.png")

end

def startGC
	if not $antProfiling
		puts "starting GC"
		GC.enable
		GC.start
		GC.disable
		puts "GC ok"
	else
		puts "starting GC"
		GC.enable
		GC.disable
		puts "GC ok"
	end
end
