#!/usr/bin/env ruby

require 'build/testing.rb'

Cmd.setQuiet(true)

testFiles=Dir[File.join("ext/**/tests/*.cc")]

testFiles.each{|file|
	Testing.runCTest(file)
}

