#!/usr/bin/env ruby

require 'libantargis.so'

include Libantargis
s=Scene2D.new(32,32)

m=Mesh2D.new(s)

puts m,m.object_id
l=s.getCurrentNodes
puts "C:"
puts l,l.object_id,l.class
l.each{|i|
  puts i,i.object_id,i.class
}

l=s.getCurrentNodes

l.each{|i|
  puts i,i.object_id
}

l.each{|i|
  puts i,i.object_id
}

l.each{|i|
  puts i,i.object_id
}

l.each{|i|
  puts i,i.object_id
}

l=s.getCurrentNodes

l.each{|i|
  puts i,i.object_id
}
