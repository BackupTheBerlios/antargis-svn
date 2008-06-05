#!/usr/bin/env ruby

#require 'ruby/antargislib.rb'
require 'libantargis.so'
include Libantargis

r=AGRect2.new(0,0,20,20)

def checkPair(a,b)
  l=a.difference(b)
  l.each{|r|
    puts "ERROR: #{r} not in #{a}!" unless a.contains(r)
    puts "ERROR: #{r} is in #{b}!" if b.contains(r)
  }
  puts "size:#{l.length}"
  #puts l
end

[5,10].each{|w|
  [5,10].each{|h|
    [-20,-10,-5,0,5,10,20,30].each{|x|
      [-20,-10,-5,0,5,10,20,30].each{|y|
        a=AGRect2.new(x,y,w,h)
        checkPair(r,a)
      }
    }
  }
}

puts "seems ok"
