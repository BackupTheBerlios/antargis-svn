#!/usr/bin/env ruby

require 'build/testing.rb'


testFiles=Dir[File.join("ext/**/tests/*.cc")]


testFiles.each{|file|
	puts "TEST: #{file}"
	Testing.runCTest(file)
}

