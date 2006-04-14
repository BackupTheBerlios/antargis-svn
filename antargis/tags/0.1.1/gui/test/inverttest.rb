#!/usr/bin/env ruby
require 'antargisgui.rb'

a=AGMatrix3.new
a.set(1,0,2)
a.set(2,0,3)
a.set(0,1,4)
a.set(2,2,100)

puts a
b=a.inverted
puts a*b
