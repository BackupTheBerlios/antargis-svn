#!/usr/bin/env ruby

if not $antargislibinited

	# try suspending arts
	if File.exists?("/usr/bin/artsshell")
		File.popen("/usr/bin/artsshell suspend").close
	end
	
	$programDir=Dir.pwd+"/ruby"
	# add programdir to path
	$:.push($programDir)
	
	if File.exists?("src/.libs/libantargis.so")
		require 'src/.libs/libantargis'
	elsif File.exists?("../src/.libs/libantargis.so")
		require '../src/.libs/libantargis'
	else
		require 'libantargis'
	end
	include Libantargis
	
	require 'ag_tools'
	require 'ant_tools'
	require 'ant_debug.rb'
	
	$antargislibinited=true

	$main=AGMain.new(1024,768,32,false,true)
	$main.setCaption("Antargis")
	$main.setIcon("data/gui/editor/tower1.png")

end
