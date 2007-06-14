#!/usr/bin/env ruby

require 'libantargis'
include Libantargis

a=AGRandomizer.new("")

puts a.randInt(20)
puts a.randInt(20)
puts a.randInt(20)
puts a.randInt(20)

main=AGMain.new
main.setRand(AGRandomizer.new(""))
randSpeed
