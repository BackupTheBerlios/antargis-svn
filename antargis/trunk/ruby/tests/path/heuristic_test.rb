#!/usr/bin/env ruby

$:.push("ext/")

require 'antargisgame'

include Antargisgame

f=StoredHeuristicFunction.new

a=AGVector2.new(0,0)
b=AGVector2.new(1,1)
c=AGVector2.new(2,2)

values=[
  [a,b,3],
  [b,c,4],
  [c,a,5]
]

values.each{|a|
  f.store(a[0],a[1],a[2])
}

stream=BinaryStringOut.new
f.printTo(stream)

str=stream.getString

stream=BinaryStringIn.new(str)

f2=StoredHeuristicFunction.new(stream)

values.each{|a|
  raise "error within #{a}" unless f2.get(a[0],a[1])==a[2] 
}

puts "ok"
