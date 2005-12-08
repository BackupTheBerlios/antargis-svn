#!/usr/bin/env ruby

if not $antargislibinit
	
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
	
	$antargislibinited=true
	# search for data-dir
end
